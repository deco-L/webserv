/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/17 17:49:02 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpDelete.hpp"

HttpDelete::HttpDelete(): AHttpMethod("DELETE") {
  return ;
}

HttpDelete::HttpDelete(const HttpDelete& obj): AHttpMethod("DELETE") {
  *this = obj;
  return ;
}

HttpDelete::~HttpDelete() {
  return ;
}

void HttpDelete::execute(HttpRequest& header, HttpResponse*& response) {
  (void)header;
  (void)response;
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

