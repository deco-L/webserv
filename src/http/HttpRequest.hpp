/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/17 22:39:12 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <vector>

class HttpRequest {
private:
  std::map<std::string, std::string> _headers;
  std::string _body;

  HttpRequest(const HttpRequest& obj);
  HttpRequest& operator=(const HttpRequest& obj);

public:
  HttpRequest(void);
  ~HttpRequest();

  std::map<std::string, std::string> getHeader(void) const;
  void setHeaders(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end);
  void setBody(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end);
  std::string getBody(void) const;
  void showHeaders(void) const;
};

#endif
