/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMethod.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/26 01:30:59 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHTTPMETHOD_HPP
#define AHTTPMETHOD_HPP

#include "Socket.hpp"
#include "Epoll.hpp"
#include "Event.hpp"

#include <iostream>
#include <vector>

struct ConfigServer;
struct ConfigLocation;
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
  std::string _uri_old;
  const std::string _version;

  bool _autoindex;
  std::string _cgi_extension;
  std::string _cgi_path;
  std::string _cgi_relative_path;
  Epoll *_epoll;
  std::vector<Event> *_events;

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
  Epoll* getEpoll(void) const;
  std::vector<Event>* getEvents(void) const;
  
  void setUri(std::string& uri);
  void setEpoll(Epoll &epoll);
  void setEvents(std::vector<Event> &events);
  
  HttpResponse* setResponseStatus(const ConfigServer& config);
  
  virtual void execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response) = 0;
  virtual void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const = 0;
};

#endif
