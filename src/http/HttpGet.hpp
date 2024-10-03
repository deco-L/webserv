/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 16:30:05 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGET_HPP
#define HTTPGET_HPP

#include "AHttpMethod.hpp"

class HttpGet : public AHttpMethod {
private:
  std::string _uri;
  std::string _version;

  HttpGet(void);
  HttpGet(const HttpGet& obj);
  HttpGet& operator=(const HttpGet& obj);

public:
  HttpGet(std::string uri, std::string version);
  ~HttpGet();

  const std::string& getUri(void) const;
  const std::string& getVersion(void) const;
  HttpResponse* setResponseStatus(void);
  void setResponseMessage(HttpHeader& header, HttpResponse& response) const;
  void execute(HttpHeader& header, HttpResponse*& response);
};

#endif
