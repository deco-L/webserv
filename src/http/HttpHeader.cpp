/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/27 17:57:10 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpHeader.hpp"
#include "Error.hpp"

HttpHeader::HttpHeader(void) {
  return ;
}

HttpHeader::HttpHeader(const HttpHeader& obj) {
  *this = obj;
  return ;
}

HttpHeader::~HttpHeader() {
  return ;
}

void HttpHeader::setHeaders(std::vector<std::string>& headers) {
  std::vector<std::string>::iterator it = headers.begin();

  while (it != headers.end()) {
    std::string header;
    std::string element;
    std::istringstream stream(*it);
    std::getline(stream, header, ':');
    std::getline(stream, element);
    if (!element.length())
      break ;
    this->_headers[header] = element.substr(1, element.length());
    it++;
  }
  return ;
}

void HttpHeader::showHeaders(void) const {
  std::map<std::string, std::string>::const_iterator it = this->_headers.begin();

  while (it != this->_headers.end()) {
    std::cout << it->first << ": " << it->second << std::endl;
    it++;
  }
  return ;
}

HttpHeader& HttpHeader::operator=(const HttpHeader& obj) {
  if (this != &obj) {
    ;
  }
  else
  {
    std::cout << ERROR_COLOR
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << COLOR_RESET << std::endl;
  }
  return *this;
}
