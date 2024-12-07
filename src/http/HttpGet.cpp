/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmiyazaw <kmiyazaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/07 15:51:29 by kmiyazaw         ###   ########.fr       */
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

  // cgiを実行する
  if (!this->_cgi_extension.empty() && !this->_cgi_path.empty())
  {
    std::cout << "CGI yeah" << std::endl;
    responseSize = response.createCgiMessage(this->getMethod(), this->_uri, config, this->_version, this->_cgi_extension, this->_cgi_path);
  }
  else if (this->_autoindex && mylib::isDirectory(this->_uri))
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
  if (this->_cgi_extension.empty() || this->_cgi_path.empty())
    response = this->setResponseStatus(config);
  if (300 <= response->getStatus() && response->getStatus() < 600)
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
