/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/18 17:38:24 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpDelete.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "webserv.hpp"

HttpDelete::HttpDelete(): AHttpMethod("DELETE"), _uri(""), _version("") {
  return ;
}

HttpDelete::HttpDelete(std::string uri, std::string version): AHttpMethod("DELETE"), _uri(uri), _version(version) {
  return ;
}

HttpDelete::HttpDelete(const HttpDelete& obj): AHttpMethod("DELETE"), _uri(""), _version("") {
  *this = obj;
  return ;
}

HttpDelete::~HttpDelete() {
  return ;
}

const std::string& HttpDelete::getUri(void) const {
  return (this->_uri);
}

const std::string& HttpDelete::getVersion(void) const {
  return (this->_version);
}

HttpResponse* HttpDelete::setResponseStatus(void) {
  std::string root = "./wsv/html";

  if (mylib::isPathValid(root.append(this->_uri)))
    return (new HttpResponse(HTTP_NOT_FOUND));
  if (mylib::isDirectory(root.append(this->_uri)) || mylib::isModeValid(root.append(this->_uri), S_IRUSR | S_IXUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  return (new HttpResponse(HTTP_OK));
}

void HttpDelete::setResponseMessage(HttpRequest& request, HttpResponse& response) const {
  int responseSize;
  std::string root = "./wsv/html";

  responseSize = response.createResponseMessage(this->getMethod(), root.append(this->_uri), request, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpDelete::execute(HttpRequest& request, HttpResponse*& response) {
  std::string root = "./wsv/html";
  response = this->setResponseStatus();
  if (std::remove(root.append(this->_uri).c_str())) {
    response->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  if (400 <= response->getStatus() && response->getStatus() <= 600)
    return ;
  this->setResponseMessage(request, *response);
  return ;
}

HttpDelete& HttpDelete::operator=(const HttpDelete& obj) {
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

