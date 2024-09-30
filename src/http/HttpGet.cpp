/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/29 19:46:39 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpGet.hpp"

HttpGet::HttpGet() : AHttpMethod("GET") {
  return ;
}

HttpGet::HttpGet(const HttpGet& obj) {
  *this = obj;
  return ;
}

HttpGet::~HttpGet() {
  return ;
}

void HttpGet::execute(Socket& socket, HttpHeader& headers, HttpResponse& response) {
  return ;
}

HttpGet& HttpGet::operator=(const HttpGet& obj) {
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
