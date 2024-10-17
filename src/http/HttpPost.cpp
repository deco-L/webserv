/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/17 17:49:48 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpPost.hpp"

HttpPost::HttpPost(void): AHttpMethod("POST") {
  return ;
}

HttpPost::HttpPost(const HttpPost& obj): AHttpMethod("POST") {
  *this = obj;
  return ;
}

HttpPost::~HttpPost() {
  return ;
}

void HttpPost::execute(HttpRequest& header, HttpResponse*& response) {
  (void)header;
  (void)response;
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
