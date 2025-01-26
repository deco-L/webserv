/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/26 15:13:10 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Error.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include "Http.hpp"

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


void execEvent(Epoll& epoll, const epoll_event& event, std::vector<Event>& events) {
  (void) event;
  std::vector<Event> tmp = events;

  for (std::vector<Event>::iterator it = tmp.begin(); it != tmp.end(); it++) {
    if ((it->event & EPOLLIN) == EPOLLIN)
      it->func(epoll, events, it->socket, *it->config);
    else if ((it->event & (EPOLLIN | EPOLLET)) == (EPOLLIN | EPOLLET))
      it->func(epoll, events, it->socket, *it->config);
    else if ((it->event & (EPOLLOUT)) == EPOLLOUT)
      it->func(epoll, events, it->socket, *it->config);
  }
  tmp = events;
  for (std::vector<Event>::iterator it = tmp.begin(); it != tmp.end(); it++) {
    if ((it->event & EPOLLOUT) == EPOLLOUT)
      it->func(epoll, events, it->socket, *it->config);
  }
  return ;
}

void connectHandler(Epoll& epoll, std::vector<Event>& evnents, Socket& socket, const ConfigServer& config) {
  Socket cSocket;

  socket.accept(cSocket);
  if (cSocket._socket == -1)
    return ;
  epoll.setEvent(cSocket, EPOLLIN | EPOLLET);

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
  }
  else if (size == 0 && !socket._outBuf.length()) {
    std::vector<Event>::iterator it;

    it = std::find_if(events.begin(), events.end(), FindByFd(socket._socket));
    epoll.delEvent(socket);
    events.erase(it);
    socket.close();
    std::cout << NORMA_COLOR << "connection end" << COLOR_RESET << std::endl;
  }
  return ;
}

void readCgiHandler(Epoll& epoll, std::vector<Event>& events, Cgi cgi) {
  pid_t pid = waitpid(-1, NULL, WNOHANG);
}

void writeHandler(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config) {
  Http http;

  try {
    http.parseRequestMessage(socket);
    #ifdef DEBUG
    showRequestMessage(http);
    #endif
    socket._outBuf.clear();
    http.checkRequestMessage(config);
    if (!http.createMethod())
      throw Http::HttpError("HTTP_BAD_REQUEST");
    http.executeMethod(config);
    #ifdef DEBUG
    showResponseMessage(http);
    #endif
    http.sendResponse(socket);
  }
    catch(const std::exception& e) {
      std::string error(e.what());

      std::cout << ERROR_COLOR << error << COLOR_RESET << std::endl;
      http.createResponseMessage(config);
      #ifdef DEBUG
      showResponseMessage(http);
      #endif
      http.sendResponse(socket);
    }

  std::vector<Event>::iterator it;

  it = std::find_if(events.begin(), events.end(), FindByFd(socket._socket));
  events.erase(it);
  Event tmp(socket._socket, EPOLLIN | EPOLLET, &config, socket, readHandler);
  events.push_back(tmp);
  epoll.modEvent(socket, EPOLLIN | EPOLLET);
  return ;
}
