/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/08 17:43:43 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpDelete.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "webserv.hpp"

HttpDelete::HttpDelete(): AHttpMethod("DELETE", "", "") {
  return ;
}

HttpDelete::HttpDelete(std::string uri, std::string version): AHttpMethod("DELETE", uri, version) {
  (void)uri;
  (void)version;
  return ;
}

HttpDelete::HttpDelete(const HttpDelete& obj): AHttpMethod("DELETE", obj.getUri(), obj.getVersion()) {
  *this = obj;
  return ;
}

HttpDelete::~HttpDelete() {
  return ;
}

void HttpDelete::setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const {
  int responseSize;
  (void)request;

  responseSize = response.createResponseMessage(this->getMethod(), this->_uri, config, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpDelete::execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response) {
  response = this->setResponseStatus(config);
  if (std::remove(this->_uri.c_str())) {
    response->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  if (300 <= response->getStatus() && response->getStatus() < 600)
    return ;
  this->setResponseMessage(config, request, *response);
  return ;
}

HttpDelete& HttpDelete::operator=(const HttpDelete& obj) {
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

