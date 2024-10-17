/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/18 00:27:51 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "Error.hpp"
#include "webserv.hpp"

HttpRequest::HttpRequest(void): _body("") {
  return ;
}

HttpRequest::HttpRequest(const HttpRequest& obj): _body("") {
  *this = obj;
  return ;
}

HttpRequest::~HttpRequest() {
  return ;
}

std::map<std::string, std::string> HttpRequest::getHeader(void) const {
  return (this->_headers);
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
    this->_headers[header] = element.substr(1, element.length());
    it++;
  }
  return ;
}

void HttpRequest::setBody(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end) {
  while (it != end) {
    std::istringstream stream(*it);
    this->_body.append(stream.str());
    it++;
  }
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
