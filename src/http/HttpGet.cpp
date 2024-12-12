/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/12 16:38:34 by miyazawa.ka      ###   ########.fr       */
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
  
  //if (this->getMethod() == "POST" && request.getBody().size())
  //{
  //  std::cout << "POST yeah" << std::endl;
  //  std::cout << "body: " << request.getBody() << std::endl;
  //}

  // cgiを実行する
  if ((!this->_cgi_extension.empty() && !this->_cgi_path.empty()) || this->_uri_old.size())
  {
    std::cout << "CGI yeah" << std::endl;
    responseSize = response.createCgiMessage(this->getMethod(), this->_uri, config, this->_version, this->_cgi_path, this->_cgi_extension, this->_uri_old, request.getBody());
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
  if ((this->_cgi_extension.empty() || this->_cgi_path.empty()) || !this->_uri_old.empty())
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
