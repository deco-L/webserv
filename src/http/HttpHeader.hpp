/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/27 17:56:42 by csakamot         ###   ########.fr       */
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

  void setHeaders(std::vector<std::string>& headers);
  void showHeaders(void) const;
};

#endif
