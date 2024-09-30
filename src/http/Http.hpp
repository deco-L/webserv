/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/29 19:48:09 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
#define HTTP_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include "HttpHeader.hpp"
#include "AHttpMethod.hpp"
#include "HttpResponse.hpp"

class Socket;

class Http {
private:
  std::string _method;
  std::string _uri;
  std::string _version;
  int _requestSize;
  HttpHeader _httpHeader;
  AHttpMethod* _httpMethod;
  HttpResponse* _httpResponse;

  Http(const Http& obj);
  Http& operator=(const Http& obj);

  void _parseRequestLine(std::string line);

public:
  Http(void);
  ~Http();

  class HttpError : public std::exception {
  private:
    std::string _error_message;
  
  public:
    HttpError(std::string error);
    virtual ~HttpError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
    virtual const char* what(void) const throw();
  };

  bool createMethod(void);
  void sendResponse(Socket& socket);
  void executeMethod(Socket& socket);
  std::string getMethod(void) const;
  std::string getUri(void) const;
  std::string getVersion(void) const;
  int getRequestSize(void) const;
  void parseRequestMessage(Socket& socket);
  void recvRequestMessage(Socket& socket);
  void showRequestLine(void) const;
  void showHttpHeaders(void) const;
};

#endif