/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/18 16:48:06 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpPost.hpp"
#include "webserv.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

HttpPost::HttpPost(void): AHttpMethod("POST"), _uri(""), _version("") {
  return ;
}

HttpPost::HttpPost(std::string uri, std::string version): AHttpMethod("POST"), _uri(uri), _version(version) {
  return ;
}

HttpPost::HttpPost(const HttpPost& obj): AHttpMethod("POST"), _uri(""), _version("") {
  *this = obj;
  return ;
}

HttpPost::~HttpPost() {
  return ;
}

HttpResponse* HttpPost::setResponseStatus(void) {
  std::string root = "./wsv";

  if (mylib::isPathValid(root.append(this->_uri)))
    return (new HttpResponse(HTTP_NOT_FOUND));
  if (mylib::isModeValid(root.append(this->_uri), S_IRUSR | S_IXUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  return (new HttpResponse(HTTP_CREATED));
}

void HttpPost::setResponseMessage(HttpRequest& request, HttpResponse& response) const {
  int responseSize;
  std::string root = "./wsv/html";

  responseSize = response.createResponseMessage(this->getMethod(), root.append(this->_uri), request, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpPost::execute(HttpRequest& request, HttpResponse*& response) {
  response = this->setResponseStatus();
  if (400 <= response->getStatus() && response->getStatus() <= 600)
    return ;
  this->setResponseMessage(request, *response);
  return ;
}

const std::string& HttpPost::getUri(void) const {
  return (this->_uri);
}

const std::string& HttpPost::getVersion(void) const {
  return (this->_version);
}

HttpPost& HttpPost::operator=(const HttpPost& obj) {
  if (this != &obj) {
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return *this;
}
