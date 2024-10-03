/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 13:27:31 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADER_HPP
#define HTTPHEADER_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <vector>

class HttpHeader {
private:
  std::map<std::string, std::string> _headers;

  HttpHeader(const HttpHeader& obj);
  HttpHeader& operator=(const HttpHeader& obj);

public:
  HttpHeader(void);
  ~HttpHeader();

  std::map<std::string, std::string> getHeader(void) const;
  void setHeaders(std::vector<std::string>& headers);
  void showHeaders(void) const;
};

#endif
