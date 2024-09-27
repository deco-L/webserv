/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/27 17:43:57 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Socket.hpp"
#include "Error.hpp"


Http::Http(void): _method(""), _uri(""), _version(""), _requestSize(0), _httpHeader() {
  return ;
}

Http::Http(const Http& obj) {
  *this = obj;
  return ;
}

Http::~Http() {
  return ;
}

Http::HttpError::HttpError(std::string error) : _error_message(error) {
  return ;
}

Http::HttpError::~HttpError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
}

const char* Http::HttpError::what(void) const throw() {
  return (this->_error_message.c_str());
}

std::string Http::getMethod(void) const {
  return (this->_method);
}

std::string Http::getUri(void) const {
  return (this->_uri);
}

std::string Http::getVersion(void) const {
  return (this->_version);
}

int Http::getRequestSize(void) const {
  return (this->_requestSize);
}

void Http::_parseRequestLine(std::string line) {
  std::string element;
  std::istringstream stream(line);

  while (std::getline(stream, element, ' ')) {
    if (this->_method.empty())
      this->_method = element;
    else if (this->_uri.empty())
      this->_uri = element;
    else if (this->_version.empty())
      this->_version = element;
  }
  return ;
}

void Http::parseRequestMessage(Socket& socket) {
  std::string line;
  std::string tmp;
  std::vector<std::string> crline;
  std::istringstream stream(socket._outBuf);

  while (std::getline(stream, line)) {
    this->_requestSize -= line.length();
    if (this->_requestSize < 0)
      break ;
    if (!line.empty() && line[line.length() - 1] == '\r') {
      line.substr(0, line.length() - 1);
      tmp.append(line);
      crline.push_back(line);
    }
    tmp.append(line);
  }

  std::vector<std::string>::iterator it = crline.begin();
  this->_parseRequestLine(*it);
  std::vector<std::string> headers(crline.begin() + 1, crline.end());
  this->_httpHeader.setHeaders(headers);
  return ;
}

void Http::recvRequestMessage(Socket& socket) {
  this->_requestSize = socket.recv();
  if (this->_requestSize < 0)
    throw Http::HttpError("recvRequestMessage error");
  else if (this->_requestSize > MAX_SOCK_BUFFER)
    this->sendErrorMessage(socket);
  else if (this->_requestSize == 0) {
    std::cout << NORMA_COLOR << "connection end." << COLOR_RESET << std::endl;
  }
  socket._error = this->_requestSize;
  return ;
}

void Http::sendErrorMessage(Socket& socket) {
  (void) socket;
  return ;
}

void Http::showRequestLine(void) const {
  std::cout << this->_method << " " << this->_uri << " " << this->_version << std::endl;
  return ;
}

void Http::showHttpHeaders(void) const {
  this->_httpHeader.showHeaders();
  return ;
}

Http& Http::operator=(const Http& obj) {
  if (this != &obj) {
    this->_method = obj.getMethod();
    this->_uri = obj.getUri();
    this->_version = obj.getVersion();
    this->_requestSize = obj.getRequestSize();
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return (*this);
}
