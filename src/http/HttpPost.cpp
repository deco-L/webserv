/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/06 17:13:48 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpPost.hpp"
#include "webserv.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

HttpPost::HttpPost(void): AHttpMethod("POST") {
  return ;
}

HttpPost::HttpPost(std::string uri, std::string version): AHttpMethod("POST") {
  return ;
}

HttpPost::HttpPost(const HttpPost& obj): AHttpMethod("POST") {
  *this = obj;
  return ;
}

HttpPost::~HttpPost() {
  return ;
}

bool HttpPost::_uploadFile(HttpRequest& request) {
  std::string root = "./wsv";
  std::ofstream file(root.append(this->_uri).c_str());

  if (!file.is_open())
    return (false);
  file << request.getBody();
  file.close();
  return (true);
}

HttpResponse* HttpPost::setResponseStatus(void) {
  std::string root = "./wsv";

  if (mylib::isDirectory(root.append(this->_uri)) || mylib::isModeValid(root.append(this->_uri), S_IRUSR | S_IXUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  return (new HttpResponse(HTTP_CREATED));
}

void HttpPost::setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const {
  int responseSize;
  std::string root = "./wsv";

  responseSize = response.createResponseMessage(this->getMethod(), root.append(this->_uri), request, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpPost::execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response) {
  response = this->setResponseStatus();
  if (400 <= response->getStatus() && response->getStatus() <= 600)
    return ;
  if (!this->_uploadFile(request)) {
    response->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  this->setResponseMessage(config, request, *response);
  return ;
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
