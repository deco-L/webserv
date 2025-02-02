/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/02/02 14:28:10 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Error.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "CgiEvent.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include "Http.hpp"
#include "HttpResponse.hpp"

#ifdef DEBUG
static void showRequestMessage(Http& http) {
  std::cout << NORMA_COLOR << "request message" << COLOR_RESET << std::endl;
  http.showRequestLine();
  http.showRequestHeaders();
  std::cout << "\r\n";
  http.showRequestBody();
  return ;
}

static void showResponseMessage(Http& http) {
  std::cout << NORMA_COLOR << "response message" << COLOR_RESET << std::endl;
  http.showResponseMessage();
  return ;
}
#endif


void execEvent(Epoll& epoll, std::vector<Event>& events) {
  std::vector<Event> tmp = events;

  for (std::vector<Event>::iterator it = tmp.begin(); it != tmp.end(); it++) {
    if (!it->cgiFlag)
      it->socketFunc(epoll, events, it->socket, *it->config);
    else
      it->cgiFunc(epoll, events, *it);
  }
  return ;
}

void connectHandler(Epoll& epoll, std::vector<Event>& evnents, Socket& socket, const ConfigServer& config) {
  Socket cSocket;

  socket.accept(cSocket);
  if (cSocket._socket == -1)
    return ;
  epoll.setEvent(cSocket._socket, EPOLLIN | EPOLLET);

  Event tmp(cSocket._socket, EPOLLIN | EPOLLET, &config, cSocket, readHandler);
  evnents.push_back(tmp);
  return ;
}

void readHandler(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config) {
  ssize_t size;

  size = socket.recv();
  if (size == -1)
    return ;
  else if (size < 8 * KILOBYTE && socket._outBuf.length()) {
    std::vector<Event>::iterator it;

    it = std::find_if(events.begin(), events.end(), FindByFd(socket._socket));
    events.erase(it);

    Event tmp(socket._socket, EPOLLOUT, &config, socket, writeHandler);

    events.push_back(tmp);
    epoll.modEvent(socket, EPOLLOUT);
  } else if (size == 0 && !socket._outBuf.length()) {
    std::vector<Event>::iterator it;

    it = std::find_if(events.begin(), events.end(), FindByFd(socket._socket));
    epoll.delEvent(socket._socket);
    events.erase(it);
    socket.close();
    std::cout << NORMA_COLOR << "connection end" << COLOR_RESET << std::endl;
  } else {
    std::vector<Event>::iterator it;

    it = std::find_if(events.begin(), events.end(), FindByFd(socket._socket));
    it->socket._outBuf = socket._outBuf;
    epoll.modEvent(socket, (EPOLLIN | EPOLLET));
  }
  return ;
}

void readCgiHandler(Epoll& epoll, std::vector<Event>& events, Event& event) {
  try {
    pid_t pid = waitpid(event.cgiEvent._pid, NULL, WNOHANG);

    if (event.timeoutFlag) {
      event.http.getHttpResponse()->setStatus(HTTP_GATEWAY_TIME_OUT);
      throw std::runtime_error("HTTP_GATEWAY_TIMEOUT");
    }
    if (pid == -1) {
      event.http.getHttpResponse()->setStatus(HTTP_INTERNAL_SERVER_ERROR);
      throw std::runtime_error("HTTP_INTERNAL_SERVER_ERROR");
    } else if (pid == 0) {
      return ;
    } else {
      int responseSize = 0;

      responseSize = event.http.getHttpResponse()->cgiEventProcess(
        event.cgiEvent._readFd[0],
        event.http.getHttpMethod()->getVersion()
      );
      close(event.cgiEvent._readFd[1]);
      if (responseSize < 0) {
        event.http.getHttpResponse()->setStatus(HTTP_INTERNAL_SERVER_ERROR);
        throw std::runtime_error("HTTP_INTERNAL_SERVER_ERROR");
      }
      #ifdef DEBUG
      showResponseMessage(event.http);
      #endif
      event.http.sendResponse(event.socket);
    }
  } catch(const std::exception& e) {
    std::string error(e.what());

    std::cout << ERROR_COLOR << error << COLOR_RESET << std::endl;
    close(event.cgiEvent._readFd[1]);
    kill(event.cgiEvent._pid, SIGKILL);
    event.http.createResponseMessage(*event.config);
    #ifdef DEBUG
    showResponseMessage(event.http);
    #endif
    event.http.sendResponse(event.socket);
  }
  std::vector<Event>::iterator it;

  it = std::find_if(events.begin(), events.end(), FindByFd(event.cgiEvent._readFd[0]));
  events.erase(it);
  epoll.delEvent(event.cgiEvent._readFd[0]);
  close(event.cgiEvent._readFd[0]);

  Event tmp(event.socket._socket, EPOLLIN | EPOLLET, event.config, event.socket, readHandler);

  events.push_back(tmp);
  epoll.setEvent(event.socket._socket, EPOLLIN | EPOLLET);
  return ;
}

void writeHandler(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config) {
  Http http;
  std::pair<Epoll*, std::vector<Event>*> event(&epoll, &events);
  std::vector<Event>::iterator it;


  try {
    it = std::find_if(events.begin(), events.end(), FindByFd(socket._socket));
    if (it->timeoutFlag) {
      http.getHttpResponse()->setStatus(HTTP_REQUEST_TIME_OUT);
      throw std::runtime_error("HTTP_REQUEST_TIME_OUT");
    }
    http.parseRequestMessage(socket);
    #ifdef DEBUG
    showRequestMessage(http);
    #endif
    socket._outBuf.clear();
    http.checkRequestMessage(config);
    if (!http.createMethod())
      throw Http::HttpError("HTTP_BAD_REQUEST");
    http.executeMethod(config, event);
    #ifdef DEBUG
    showResponseMessage(http);
    #endif
    http.sendResponse(socket);
  }
  catch(const std::exception& e) {
    std::string error(e.what());

    if (error == "cgi unfinished") {
      events.back().http = http;
      events.back().socket = socket;

      std::vector<Event>::iterator it;

      it = std::find_if(events.begin(), events.end(), FindByFd(socket._socket));
      events.erase(it);
      epoll.delEvent(socket._socket);
      return ;
    } else {
      std::cout << ERROR_COLOR << error << COLOR_RESET << std::endl;
      http.createResponseMessage(config);
      #ifdef DEBUG
      showResponseMessage(http);
      #endif
      http.sendResponse(socket);
    }
  }
  events.erase(it);

  Event tmp(socket._socket, EPOLLIN | EPOLLET, &config, socket, readHandler);

  events.push_back(tmp);
  epoll.modEvent(socket, EPOLLIN | EPOLLET);
  return ;
}

void writeCgiHandler(Epoll& epoll, std::vector<Event>& events, Event& event) {
  try {
    ssize_t size = 0;

    if (event.timeoutFlag) {
      event.http.getHttpResponse()->setStatus(HTTP_GATEWAY_TIME_OUT);
      throw std::runtime_error("HTTP_GATEWAY_TIME_OUT");
    }
    size = write(event.cgiEvent._writeFd[1], event.cgiEvent._writeBuf.c_str(), event.cgiEvent._writeBuf.length());
    if (size == -1)
      return ;
    std::vector<Event>::iterator it;

    it = std::find_if(events.begin(), events.end(), FindByFd(event.cgiEvent._writeFd[1]));
    events.erase(it);
    epoll.delEvent(event.cgiEvent._writeFd[1]);
    close(event.cgiEvent._writeFd[1]);

    Event tmp(event.cgiEvent._readFd[0], EPOLLIN, event.config, event.cgiEvent, readCgiHandler);

    events.push_back(tmp);
    epoll.setEvent(event.cgiEvent._readFd[0], EPOLLIN);
  } catch(const std::exception& e) {
    std::string error(e.what());

    kill(event.cgiEvent._pid, SIGKILL);
    std::cout << ERROR_COLOR << error << COLOR_RESET << std::endl;
    event.http.createResponseMessage(*event.config);
    #ifdef DEBUG
    showResponseMessage(event.http);
    #endif
    event.http.sendResponse(event.socket);
    std::vector<Event>::iterator it;

    it = std::find_if(events.begin(), events.end(), FindByFd(event.cgiEvent._writeFd[1]));
    events.erase(it);
    epoll.delEvent(event.cgiEvent._writeFd[1]);
    close(event.cgiEvent._writeFd[1]);
  }
  return ;
}
