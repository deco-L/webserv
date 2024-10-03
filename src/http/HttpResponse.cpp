/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 16:12:41 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpHeader.hpp"
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
  this->_response.append(CRLF);
  return (this->_response.length());
}

int HttpResponse::_createHeaderLine(HttpHeader& header) {
  int size = this->_response.length();
  std::map<std::string, std::string> headers = header.getHeader();

  for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
    this->_response.append(it->first);
    this->_response.append(": ");
    this->_response.append(it->second);
    this->_response.append(CRLF);
  }
  size = this->_response.length() - size;
  return (size);
}

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

int HttpResponse::createResponseMessage(std::string path, HttpHeader& header, std::string version) {
  int responseSize;
  int readSize;

  if (400 <= this->_status && this->_status <= 600)
    return (0);
  responseSize = this->_createStatusLine(version);
  responseSize = this->_createHeaderLine(header);
  this->_response.append(CRLF);
  readSize = mylib::readFile(path, this->_response);
  if (readSize == -1)
    return (-1);
  responseSize += readSize;
  return (responseSize);
}

void HttpResponse::execute(Socket& socket, HttpHeader& header, std::string version) {
  if (400 <= this->_status && this->_status <= 600)
    this->createResponseMessage("", header, version);
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
