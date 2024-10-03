/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 16:33:08 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpGet.hpp"
#include "webserv.hpp"
#include "HttpResponse.hpp"

HttpGet::HttpGet(): AHttpMethod("GET") {
  return ;
}

HttpGet::HttpGet(std::string uri, std::string version): AHttpMethod("GET"), _uri((uri == "/") ?  "/index.html" : uri), _version(version) {
  return ;
}

HttpGet::HttpGet(const HttpGet& obj): AHttpMethod("GET") {
  *this = obj;
  return ;
}

HttpGet::~HttpGet() {
  return ;
}

const std::string& HttpGet::getUri(void) const {
  return (this->_uri);
}

const std::string& HttpGet::getVersion(void) const {
  return (this->_version);
}

HttpResponse* HttpGet::setResponseStatus(void) {
  std::string root = "./wsv/html";

  if (mylib::isPathValid(root) || mylib::isDirectory(root))
    return (new HttpResponse(HTTP_NOT_FOUND));
  if (mylib::isPathValid(root.append(this->_uri)) || mylib::isModeValid(root.append(this->_uri), S_IRUSR | S_IXUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  return (new HttpResponse(HTTP_OK));
}

void HttpGet::setResponseMessage(HttpHeader& header, HttpResponse& response) const {
  int responseSize;
  std::string root = "./wsv/html";

  responseSize = response.createResponseMessage(root.append(this->_uri), header, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpGet::execute(HttpHeader& header, HttpResponse*& response) {
  response = this->setResponseStatus();
  if (400 <= response->getStatus() && response->getStatus() <= 600)
    return ;
  this->setResponseMessage(header, *response);
  return ;
}

HttpGet& HttpGet::operator=(const HttpGet& obj) {
  if (this != &obj) {
    this->_uri = obj.getUri();
    this->_version = obj.getVersion();
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return *this;
}
