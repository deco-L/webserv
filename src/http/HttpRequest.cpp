/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/22 15:58:26 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "Error.hpp"
#include "webserv.hpp"

HttpRequest::HttpRequest(void): _method(""), _uri(""), _version(""), _body(""), _bodySize(0) {
  return ;
}

HttpRequest::HttpRequest(const HttpRequest& obj): _method(""), _uri(""), _version(""), _body(""), _bodySize(0) {
  *this = obj;
  return ;
}

HttpRequest::~HttpRequest() {
  return ;
}

const std::map<std::string, std::string>& HttpRequest::getHeader(void) const {
  return (this->_headers);
}

const std::string& HttpRequest::getMethod(void) const {
  return (this->_method);
}

const std::string& HttpRequest::getUri(void) const {
  return (this->_uri);
}

const std::string& HttpRequest::getVersion(void) const {
  return (this->_version);
}

long long HttpRequest::getBodySize(void) const {
  return (this->_bodySize);
}

void HttpRequest::setMethod(const std::string& method) {
  this->_method = method;
  return ;
}

void HttpRequest::setUri(const std::string& uri) {
  this->_uri = uri;
  return ;
}

void HttpRequest::setVersion(const std::string& version) {
  this->_version = version;
  return ;
}

void HttpRequest::setHeaders(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end) {

  while (it != end) {
    std::string header;
    std::string element;
    std::istringstream stream(*it);

    if (!stream.str().compare(CRLF)) {
      it++;
      break ;
    }
    std::getline(stream, header, ':');
    std::getline(stream, element);
    if (!element.length())
      break ;
    this->_headers[header] = element.substr(1, element.length() - 2);
    it++;
  }
  return ;
}

void HttpRequest::setChunkedBody(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end) {
  std::vector<std::string>::iterator begin = it;
  int size = 0;

  while (it != end) {
    if ((it - begin) % 2)
      this->_body.append(it->substr(0, size));
    else
      size = mylib::stringToSize(*it);
    if (size == 0)
      break ;
    else if (size == -1) {
      this->_bodySize = size;
      return ;
    }
    it++;
  }
  this->_bodySize = this->_body.length();
  return ;
}

void HttpRequest::setBody(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end) {
  while (it != end) {
    this->_body.append(*it);
    it++;
  }
  this->_bodySize = this->_body.length();
  return ;
}

std::string HttpRequest::getBody(void) const {
  return this->_body;
}

void HttpRequest::showHeaders(void) const {
  std::map<std::string, std::string>::const_iterator it = this->_headers.begin();

  while (it != this->_headers.end()) {
    std::cout << it->first << ": " << it->second << std::endl;
    it++;
  }
  return ;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& obj) {
  if (this != &obj) {
    ;
  }
  else
  {
    std::cout << ERROR_COLOR
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << COLOR_RESET << std::endl;
  }
  return *this;
}
