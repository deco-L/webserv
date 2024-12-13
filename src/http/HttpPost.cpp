/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/13 12:43:56 by csakamot         ###   ########.fr       */
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
  //(void)request;
  
  if (this->getMethod() == "POST" && request.getBody().size())
  {
    std::cout << "POST yeah" << std::endl;
    std::cout << "body: " << request.getBody() << std::endl;
  }
  
  // cgiを実行する
  if ((!this->_cgi_extension.empty() && !this->_cgi_path.empty()) || this->_cgi_relative_path.size())
  {
    responseSize = response.createCgiMessage(this->getMethod(), this->_uri, config, this->_version, this->_cgi_path, this->_cgi_extension, this->_uri_old, request.getBody());
  }
  else {
    responseSize = response.createResponseMessage(this->getMethod(), this->_uri, config, this->_version);
  }
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpPost::execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response) {
  if ((this->_cgi_extension.empty() || this->_cgi_path.empty()) || !this->_uri_old.size())
    response = this->setResponseStatus(config);
  if (400 <= response->getStatus() && response->getStatus() <= 600)
    return ;
  if ((this->_cgi_extension.empty() || this->_cgi_path.empty()) || !this->_uri_old.size())
  {
    std::cout << this->_cgi_extension << std::endl;
    std::cout << this->_cgi_path << std::endl;
    std::cout << this->_cgi_relative_path << std::endl;
    std::cout << "in execute" << std::endl;
    if (!this->_uploadFile(request)) {
      response->setStatus(HTTP_INTERNAL_SERVER_ERROR);
      return ;
    }
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
