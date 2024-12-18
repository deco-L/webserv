/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/18 15:50:45 by csakamot         ###   ########.fr       */
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
  std::string _method;
  std::string _uri;
  std::string _version;
  std::map<std::string, std::string> _headers;
  std::string _body;
  long long _bodySize;

  HttpRequest(const HttpRequest& obj);
  HttpRequest& operator=(const HttpRequest& obj);

public:
  HttpRequest(void);
  ~HttpRequest();

  const std::string& getMethod(void) const;
  const std::string& getUri(void) const;
  const std::string& getVersion(void) const;
  long long getBodySize(void) const;
  const std::map<std::string, std::string>& getHeader(void) const;
  void setMethod(const std::string& method);
  void setUri(const std::string& uri);
  void setVersion(const std::string& version);
  void setHeaders(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end);
  void setBody(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end);
  void setChunkedBody(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& end);
  std::string getBody(void) const;
  void showHeaders(void) const;
};

#endif
