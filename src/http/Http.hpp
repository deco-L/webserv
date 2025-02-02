/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/29 01:12:19 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
#define HTTP_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include "HttpRequest.hpp"
#include "AHttpMethod.hpp"
#include "HttpResponse.hpp"

struct ConfigServer;
struct Event;
class Socket;
class Epoll;

class Http {
private:
  int _requestSize;
  HttpRequest _httpRequest;
  AHttpMethod* _httpMethod;
  HttpResponse* _httpResponse;


  void _parseRequestLine(std::string line);

public:
  Http(void);
  Http(const Http& obj);
  Http& operator=(const Http& obj);
  ~Http();

  class HttpError : public std::exception {
  private:
    std::string _error_message;

  public:
    HttpError(std::string error);
    virtual ~HttpError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
    virtual const char* what(void) const throw();
  };

  const std::string& getMethod(void) const;
  const std::string& getUri(void) const;
  const std::string& getVersion(void) const;
  int getRequestSize(void) const;
  unsigned long getRequestBodySize(void) const;
  const HttpRequest& getHttpRequest(void) const;
  AHttpMethod* getHttpMethod(void) const;
  HttpResponse* getHttpResponse(void) const;
  void setHttpResponse(unsigned int status);
  void recvRequestMessage(Socket& socket);
  void parseRequestMessage(Socket& socket);
  void checkRequestMessage(const ConfigServer& config);
  void executeMethod(const ConfigServer& config, std::pair<Epoll*, std::vector<Event>*>& event);
  bool createMethod(void);
  void createResponseMessage(const ConfigServer& config);
  void sendResponse(Socket& socket);
  void showRequestLine(void) const;
  void showRequestHeaders(void) const;
  void showRequestBody(void) const;
  void showResponseMessage(void) const;
};

#endif