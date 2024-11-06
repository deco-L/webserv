/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/06 17:12:21 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpDelete.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "webserv.hpp"

HttpDelete::HttpDelete(): AHttpMethod("DELETE") {
  return ;
}

HttpDelete::HttpDelete(std::string uri, std::string version): AHttpMethod("DELETE") {
  return ;
}

HttpDelete::HttpDelete(const HttpDelete& obj): AHttpMethod("DELETE") {
  *this = obj;
  return ;
}

HttpDelete::~HttpDelete() {
  return ;
}

HttpResponse* HttpDelete::setResponseStatus(void) {
  std::string root = "./wsv/html";

  if (mylib::isPathValid(root.append(this->_uri)))
    return (new HttpResponse(HTTP_NOT_FOUND));
  if (mylib::isDirectory(root.append(this->_uri)) || mylib::isModeValid(root.append(this->_uri), S_IRUSR | S_IXUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  return (new HttpResponse(HTTP_OK));
}

void HttpDelete::setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const {
  int responseSize;
  std::string root = "./wsv/html";

  responseSize = response.createResponseMessage(this->getMethod(), root.append(this->_uri), request, this->_version);
  if (responseSize < 0) {
    response.setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  return ;
}

void HttpDelete::execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response) {
  std::string root = "./wsv/html";
  response = this->setResponseStatus();
  if (std::remove(root.append(this->_uri).c_str())) {
    response->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }
  if (400 <= response->getStatus() && response->getStatus() <= 600)
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

