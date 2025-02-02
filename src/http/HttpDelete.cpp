/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/02/02 16:18:08 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Event.hpp"
#include "Epoll.hpp"
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

HttpDelete::HttpDelete(const AHttpMethod& obj): AHttpMethod("DELETE", obj.getUri(), obj.getVersion()) {
  *this = obj;
  return ;
}

HttpDelete::HttpDelete(const HttpDelete& obj): AHttpMethod("DELETE", obj.getUri(), obj.getVersion()) {
  *this = obj;
  return ;
}

HttpDelete::~HttpDelete() {
  return ;
}

void HttpDelete::setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response, std::pair<class Epoll*, std::vector<Event>*>& event) const {
  int responseSize;
  (void)request;
  (void)event;

  responseSize = response.createResponseMessage(this->getMethod(), this->_uri, config, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpDelete::execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response, std::pair<Epoll*, std::vector<Event>*>& event) {
  response = this->setResponseStatus(config);
  if (400 <= response->getStatus() && response->getStatus() < 600)
    return ;
  if (std::remove(this->_uri.c_str())) {
    response->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  if (300 <= response->getStatus() && response->getStatus() < 600)
    return ;
  this->setResponseMessage(config, request, *response, event);
  return ;
}

HttpDelete& HttpDelete::operator=(const HttpDelete& obj) {
  if (this != &obj) {
    this->_uri_old = obj._uri_old;
    this->_autoindex = obj._autoindex;
    this->_cgi_extension = obj._cgi_extension;
    this->_cgi_path = obj._cgi_path;
    this->_cgi_relative_path = obj._cgi_relative_path;
  } else {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return *this;
}
