/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/18 19:36:56 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "Socket.hpp"
#include "webserv.hpp"
#include <map>

HttpResponse::HttpResponse(void): _status(0), _response("") {
  return ;
}

HttpResponse::HttpResponse(unsigned int status): _status(status) {
  return ;
}

HttpResponse::HttpResponse(const HttpResponse& obj) {
  *this = obj;
  return ;
}

HttpResponse::~HttpResponse() {
  return ;
}

int HttpResponse::_createStatusLine(std::string version) {
  this->_response.append(version);
  this->_response.append(" ");
  this->_response.append(mylib::nbrToS(this->_status));
  if (this->_status == HTTP_OK)
    this->_response.append(" OK");
  else if (this->_status == HTTP_CREATED)
    this->_response.append(" Created");
  this->_response.append(CRLF);
  return (this->_response.length());
}

int HttpResponse::_createHeaderLine(HttpRequest& request, int bodySIze) {
  (void)request;
  int size = 0;

  std::string tmp = "Content-Length: ";
  tmp.append(mylib::nbrToS(bodySIze));
  tmp.append(CRLF);
  size += tmp.length();
  this->_response.append(tmp);
  return (size);
}

// int HttpResponse::_createHeaderLine(HttpRequest& request) {
//   int size = this->_response.length();
//   std::map<std::string, std::string> headers = request.getHeader();

//   for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
//     this->_response.append(it->first);
//     this->_response.append(": ");
//     this->_response.append(it->second);
//     this->_response.append(CRLF);
//   }
//   size = this->_response.length() - size;
//   return (size);
// }

unsigned int HttpResponse::getStatus(void) const {
  return (this->_status);
}

const std::string& HttpResponse::getResponse(void) const {
  return (this->_response);
}

void HttpResponse::setStatus(unsigned int status) {
  this->_status = status;
  return ;
}

int HttpResponse::createResponseMessage(const std::string& method, std::string path, HttpRequest& request, std::string version) {
  int responseSize;
  int bodySize;

  if (400 <= this->_status && this->_status <= 600)
    return (0);
  responseSize = this->_createStatusLine(version);
  if (!method.compare("GET")) {
    bodySize = mylib::countFileSize(path);
    responseSize = this->_createHeaderLine(request, bodySize);
    this->_response.append(CRLF);
    if (!mylib::readFile(path, this->_response))
      return (-1);
  } else if (!method.compare("POST")) {
    this->_createHeaderLine(request, 0);
    this->_response.append(CRLF);
  } else if (!method.compare("DELETE")) {
    this->_createHeaderLine(request, 0);
    this->_response.append(CRLF);
  }
  return (responseSize);
}

void HttpResponse::execute(Socket& socket, HttpRequest& request, std::string version) {
  if (400 <= this->_status && this->_status <= 600)
    this->createResponseMessage("NONE", "", request, version);
  socket.send(this->_response, this->_response.length());
  return ;
}

HttpResponse& HttpResponse::operator=(const HttpResponse& obj) {
  if (this != &obj) {
    this->_status = obj.getStatus();
    this->_response = obj.getResponse();
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return (*this);
}
