/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/01 21:10:01 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpDelete.hpp"

HttpDelete::HttpDelete() : AHttpMethod("Delete") {
  return ;
}

HttpDelete::HttpDelete(const HttpDelete& obj) {
  *this = obj;
  return ;
}

HttpDelete::~HttpDelete() {
  return ;
}

void HttpDelete::execute(Socket& socket, HttpHeader& header, HttpResponse* response) {
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

