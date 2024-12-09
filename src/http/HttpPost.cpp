/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/09 22:36:27 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpPost.hpp"
#include "webserv.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

HttpPost::HttpPost(void): AHttpMethod("POST", "", "") {
  return ;
}

HttpPost::HttpPost(std::string uri, std::string version): AHttpMethod("POST", uri, version) {
  return ;
}

HttpPost::HttpPost(const HttpPost& obj): AHttpMethod("POST", obj.getUri(), obj.getVersion()) {
  *this = obj;
  return ;
}

HttpPost::~HttpPost() {
  return ;
}

bool HttpPost::_uploadFile(HttpRequest& request) {
  if (mylib::isDirectory(this->_uri) && this->_uri[this->_uri.length() - 1] == '/')
    this->_uri.append("tmp.txt");

  std::ofstream file(this->_uri.c_str());

  if (!file.is_open())
    return (false);
  file << request.getBody().substr(0, request.getBody().find_last_not_of('\0') + 1);
  file.close();
  return (true);
}

void HttpPost::setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const {
  int responseSize;
  (void)request;

  responseSize = response.createResponseMessage(this->getMethod(), this->_uri, config, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpPost::execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response) {
  response = this->setResponseStatus(config);
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
