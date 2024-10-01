/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/01 21:53:57 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Socket.hpp"

HttpResponse::HttpResponse(void): _status(0), _response("") {
  return ;
}

HttpResponse::HttpResponse(unsigned int status): _status(status) {
  return ;
}

HttpResponse::HttpResponse(const HttpResponse& obj) {
  *this = obj;
  return ;
}

HttpResponse::~HttpResponse() {
  return ;
}

unsigned int HttpResponse::getStatus(void) const {
  return (this->_status);
}

const std::string& HttpResponse::getResponse(void) const {
  return (this->_response);
}

void HttpResponse::setStatus(unsigned int status) {
  this->_status = status;
  return ;
}

int HttpResponse::createResponseMessage(void) {
  int responseSize;
  return (responseSize);
}

void HttpResponse::execute(Socket& socket) {
  return ;
}

HttpResponse& HttpResponse::operator=(const HttpResponse& obj) {
  if (this != &obj) {
    this->_status = obj.getStatus();
    this->_response = obj.getResponse();
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
}
