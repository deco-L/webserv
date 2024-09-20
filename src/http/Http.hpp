/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/20 18:38:31 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
#define HTTP_HPP

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>

class Socket;

class Http {
private:
  std::string _method;
  std::string _uri;
  std::string _version;
  int _requestSize;

  Http(const Http& obj);
  Http& operator=(const Http& obj);

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

  std::string getMethod(void) const;
  std::string getUri(void) const;
  std::string getVersion(void) const;
  int getRequestSize(void) const;
  void parseRequestMessage(Socket& socket);
  void recvRequestMessage(Socket& socket);
};

#endif