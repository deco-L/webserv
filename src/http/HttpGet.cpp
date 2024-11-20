/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/17 00:14:12 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpGet.hpp"
#include "webserv.hpp"
#include "Config.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

HttpGet::HttpGet(): AHttpMethod("GET", "", "") {
  return ;
}

HttpGet::HttpGet(std::string uri, std::string version): AHttpMethod("GET", uri, version) {
  return ;
}

HttpGet::HttpGet(const HttpGet& obj): AHttpMethod("GET", obj.getUri(), obj.getVersion()) {
  *this = obj;
  return ;
}

HttpGet::~HttpGet() {
  return ;
}

void HttpGet::setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const {
  int responseSize;
  (void)request;

  if (this->_autoindex)
    responseSize = response.createAutoindexMessage(this->_uri, config, this->_version);
  else
    responseSize = response.createResponseMessage(this->getMethod(), this->_uri, config, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpGet::execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response) {
  response = this->setResponseStatus(config);
  if (400 <= response->getStatus() && response->getStatus() <= 600)
    return ;
  this->setResponseMessage(config, request, *response);
  return ;
}

HttpGet& HttpGet::operator=(const HttpGet& obj) {
  if (this != &obj) {
    ;
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return *this;
}
