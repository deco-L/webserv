/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/20 18:48:27 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Socket.hpp"

Http::Http(void): _method(""), _uri(""), _version(""), _requestSize(0) {
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

void Http::parseRequestMessage(Socket& socket) {
  std::istringstream stream(socket._outBuf);

  stream >> this->_method >> this->_uri >> this->_version;
  return ;
}

void Http::recvRequestMessage(Socket& socket) {
  // std::cout << "==========request message==========" << std::endl;
  this->_requestSize = socket.recvTeol(false);
  std::cout << socket._outBuf;
  if (this->_requestSize < 0)
    throw Http::HttpError("recvRequestMessage error");
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
