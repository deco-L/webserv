/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/26 19:02:20 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <sys/stat.h>
#include "Config.hpp"

#define HTTP_CONTINUE                       100
#define HTTP_SWITCHING_PROTOCOlS            101

#define HTTP_OK                             200
#define HTTP_CREATED                        201
#define HTTP_ACCEPTED                       202
#define HTTP_NON_AUTHORITATIVE_INFO         203
#define HTTP_NO_CONTENT                     204
#define HTTP_RESET_CONTENT                  205
#define HTTP_PARTIAL_CONTENT                206

#define HTTP_SPECIAL_RESPONSE               300
#define HTTP_MOVED_PERMANENTLY              301
#define HTTP_MOVED_TEMPORARILY              302
#define HTTP_SEE_OTHER                      303
#define HTTP_NOT_MODIFIED                   304
#define HTTP_USE_PROXY                      305
#define HTTP_TEMPORARY_REDIRECT             307
#define HTTP_PERMANENT_REDIRECT             308

#define HTTP_BAD_REQUEST                    400
#define HTTP_UNAUTHORIZED                   401
#define HTTP_PAYMENT_REQUIRED               402
#define HTTP_FORBIDDEN                      403
#define HTTP_NOT_FOUND                      404
#define HTTP_METHOD_NOT_ALLOWED             405
#define HTTP_NOT_ACCEPTABLE                 406
#define HTTP_PROXY_AUTHENTICATION_REQUIRED  407
#define HTTP_REQUEST_TIME_OUT               408
#define HTTP_CONFLICT                       409
#define HTTP_GONE                           410
#define HTTP_LENGTH_REQUIRED                411
#define HTTP_PRECONDETION_FAILED            412
#define HTTP_REQUEST_ENTITY_TOO_LARGE       413
#define HTTP_REQUEST_URI_TOO_LARGE          414
#define HTTP_UNSUPPORTED_MEDIA_TYPE         415
#define HTTP_RANGE_NOT_SATISFIABLE          416
#define HTTP_EXPECTATION_FAILED             417
#define HTTP_MISDIRECTED_REQUEST            421
#define HTTP_UNPROCESSABLE_CONTENT          422
#define HTTP_UPGRADE_REQUIRED               426

#define HTTP_INTERNAL_SERVER_ERROR          500
#define HTTP_NOT_IMPLEMENTED                501
#define HTTP_BAD_GATEWAY                    502
#define HTTP_SERVICE_UNAVAILABLE            503
#define HTTP_GATEWAY_TIME_OUT               504
#define HTTP_VERSION_NOT_SUPPORTED          505

#define CGI_TIMEOUT_ITERATION 10000000

struct Event;
class Socket;
class Epoll;
class HttpRequest;
class AHttpMethod;

struct headerList {
  std::pair<std::string, std::string> server;
  std::pair<std::string, std::string> date;
  std::pair<std::string, std::string> contentType;
  std::pair<std::string, std::string> contentLength;
  std::pair<std::string, std::string> lastModified;
  std::pair<std::string, std::string> allow;
  std::pair<std::string, std::string> connection;
  std::pair<std::string, std::string> acceptRanges;
};

class HttpResponse {
private:
  unsigned int _status;
  bool _returnFlag;
  ConfigLocation _location;
  std::string _redirectPath;
  std::string _response;
  headerList _responseHeader;

  HttpResponse(void);

  int _createStatusLine(std::string version);
  int _createHeaderLine(const ConfigServer& request, int bodySize);
  void _createPostResponseMessage(const std::string& uri, const ConfigServer& config);
  void _createDeleteResponseMessage(const std::string& uri, const ConfigServer& config);
  int _createRedirectResponseMessage(const std::string& uri, const ConfigServer& config);
  int _createErrorResponseMessage(const ConfigServer& config, const std::string& version);
  void _createErrorResponse(const ConfigServer& config, int status);
  std::string _createAutoindexBody(std::string path);


  std::vector<std::string> createEnvs(const ConfigServer& config, std::string _uri, std::string method, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string version, HttpRequest &request);
  std::string _doCgi(const std::string& method, std::string _uri, const ConfigServer& config, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string version, HttpRequest &request, std::pair<class Epoll&, std::vector<Event>&>& event);

public:
  HttpResponse(unsigned int status);
  HttpResponse(unsigned int status, ConfigLocation location);
  HttpResponse(unsigned int status, std::string redirectPath, ConfigLocation location);
  HttpResponse(const HttpResponse& obj);
  ~HttpResponse();

  class HttpResponseError : public std::exception {
  private:
    std::string _error_message;

  public:
    HttpResponseError(std::string error);
    virtual ~HttpResponseError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
    virtual const char* what(void) const throw();
  };

  unsigned int getStatus(void) const;
  bool getReturnFlag(void) const;
  const std::string& getRedirectPath(void) const;
  const std::string& getResponse(void) const;
  const headerList& getheader(void) const;
  void setStatus(unsigned int status);
  void setReturnFlag(bool flag);
  void setRedirectPath(const std::string& path);
  int createResponseMessage(const std::string& method, std::string path, const ConfigServer& config, std::string version);
  int createAutoindexMessage(std::string path, const ConfigServer& config, std::string version);
  int createCgiMessage(const std::string& method, std::string _uri, const ConfigServer& config, std::string version, std::string cgiPath, std::string cgiExtension, std::string _uri_old, HttpRequest& request, std::pair<class Epoll&, std::vector<Event>&>& event);
  void execute(Socket& socket);
  int cgiEventProcess(int readfd);

  HttpResponse& operator=(const HttpResponse& obj);
};

struct FindNbrInVector {
    int target;
    FindNbrInVector (int code) : target(code) {}

    bool operator()(const std::pair<int, std::string>& p) const;
};

std::string wsvErrorPage(int status);
std::string wsvRedirectPage(int status, const std::string& uri);
std::string makeCgiHeader(std::string str);

#endif
