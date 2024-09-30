/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/29 19:50:35 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Socket.hpp"

HttpResponse::HttpResponse(void): _status(0) {
  return ;
}

HttpResponse::HttpResponse(unsigned int status): _status(status) {
  return ;
}

HttpResponse::HttpResponse(const HttpResponse& obj): _status(obj.getStatus()) {
  *this = obj;
  return ;
}

HttpResponse::~HttpResponse() {
  return ;
}

unsigned int HttpResponse::getStatus(void) const {
  return (this->_status);
}

void HttpResponse::execute(Socket& socket) {
  return ;
}

HttpResponse& HttpResponse::operator=(const HttpResponse& obj) {
  if (this != &obj) {
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
}
