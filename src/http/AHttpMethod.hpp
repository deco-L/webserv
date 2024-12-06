/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMethod.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/06 13:42:17 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHTTPMETHOD_HPP
#define AHTTPMETHOD_HPP

#include <iostream>
#include <vector>

class ConfigServer;
class ConfigLocation;
class Socket;
class HttpRequest;
class HttpResponse;

class AHttpMethod {
private:
  AHttpMethod(const AHttpMethod& obj);
  AHttpMethod& operator=(const AHttpMethod& obj);
  HttpResponse* _returnRedirectStatus(const ConfigLocation& location);
  HttpResponse* _setGetResponseStatus(const ConfigServer& config, std::string& path, const ConfigLocation& location);
  HttpResponse* _setPostResponseStatus(std::string& path, const ConfigLocation& location);
  HttpResponse* _setDeleteResponseStatus(const ConfigServer& config, std::string& path, const ConfigLocation& location);

protected:
  const std::string _method;
  std::string _uri;
  const std::string _version;
  bool _autoindex;

  class MethodError : public std::exception {
  private:
    std::string _error_message;
  
  public:
    MethodError(std::string error);
    virtual ~MethodError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
    virtual const char* what(void) const throw();
  };

public:
  AHttpMethod(void);
  AHttpMethod(std::string method, std::string uri, std::string version);
  virtual ~AHttpMethod();

  const std::string& getMethod(void) const;
  const std::string& getUri(void) const;
  const std::string& getVersion(void) const;
  void setUri(std::string& uri);
  HttpResponse* setResponseStatus(const ConfigServer& config);
  virtual void execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response) = 0;
  virtual void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const = 0;
};

#endif
