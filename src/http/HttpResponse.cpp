/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/16 17:39:14 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "Socket.hpp"
#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"
#include <map>

HttpResponse::HttpResponse(void): _status(0), _response("") {
  return ;
}

HttpResponse::HttpResponse(unsigned int status): _status(status) {
  return ;
}

HttpResponse::HttpResponse(const HttpResponse& obj) {
  *this = obj;
  return ;
}

HttpResponse::~HttpResponse() {
  return ;
}

int HttpResponse::_createStatusLine(std::string version) {
  this->_response.append(version);
  this->_response.append(" ");
  this->_response.append(mylib::nbrToS(this->_status));
  this->_response.append(" ");
  switch(this->_status) {
    case HTTP_OK:
      this->_response.append("OK");
      break ;
    case HTTP_CREATED:
      this->_response.append("Created");
      break ;
    case HTTP_BAD_REQUEST:
      this->_response.append("Bad Request");
      break ;
    case HTTP_UNAUTHORIZED:
      this->_response.append("Unauthorized");
      break ;
    case HTTP_PAYMENT_REQUIRED:
      this->_response.append("Payment Requeired");
      break ;
    case HTTP_FORBIDDEN:
      this->_response.append("Forbidden");
      break ;
    case HTTP_NOT_FOUND:
      this->_response.append("Not Found");
      break ;
    case HTTP_NOT_ALLOWED:
      this->_response.append("Not Allowed");
      break ;
    case HTTP_NOT_ACCEPTABLE:
      this->_response.append("Not Acceptable");
      break ;
    case HTTP_PROXY_AUTHENTICATION_REQUIRED:
      this->_response.append("Proxy Authentication Required");
      break ;
    case HTTP_REQUEST_TIME_OUT:
      this->_response.append("Request Time Out");
      break ;
    case HTTP_CONFLICT:
      this->_response.append("Conflict");
      break ;
    case HTTP_GONE:
      this->_response.append("Gone");
      break ;
    case HTTP_LENGTH_REQUIRED:
      this->_response.append("Length Required");
      break ;
    case HTTP_PRECONDETION_FAILED:
      this->_response.append("Precondetion Failed");
      break ;
    case HTTP_REQUEST_ENTITY_TOO_LARGE:
      this->_response.append("Request Entity Too Large");
      break ;
    case HTTP_REQUEST_URI_TOO_LARGE:
      this->_response.append("Request Uri Too Large");
      break ;
    case HTTP_UNSUPPORTED_MEDIA_TYPE:
      this->_response.append("Unsupported Media Type");
      break ;
    case HTTP_RANGE_NOT_SATISFIABLE:
      this->_response.append("Range Not Satisfiable");
      break ;
    case HTTP_EXPECTATION_FAILED:
      this->_response.append("Expectation Failed");
      break ;
    case HTTP_MISDIRECTED_REQUEST:
      this->_response.append("Misdirected Request");
      break ;
    case HTTP_UNPROCESSABLE_CONTENT:
      this->_response.append("Unprocessable Content");
      break ;
    case HTTP_UPGRADE_REQUIRED:
      this->_response.append("Upgrade Required");
      break ;
    case HTTP_INTERNAL_SERVER_ERROR:
      this->_response.append("Internal Server Error");
      break ;
    case HTTP_NOT_IMPLEMENTED:
      this->_response.append("Not Implemented");
      break ;
    case HTTP_BAD_GATEWAY:
      this->_response.append("Bad Gateway");
      break ;
    case HTTP_SERVICE_UNAVAILABLE:
      this->_response.append("Service Unavailable");
      break ;
    case HTTP_GATEWAY_TIME_OUT:
      this->_response.append("Gateway Time Out");
      break ;
    case HTTP_VERSION_NOT_SUPPORTED:
      this->_response.append("Version Not Supported");
      break ;
  }
  this->_response.append(CRLF);
  return (this->_response.length());
}

int HttpResponse::_createHeaderLine(const ConfigServer& config, int bodySIze) {
  (void)config;
  int size = 0;

  std::string tmp = "Content-Length: ";
  tmp.append(mylib::nbrToS(bodySIze));
  tmp.append(CRLF);
  size += tmp.length();
  this->_response.append(tmp);
  return (size);
}

void HttpResponse::_createErrorResponse(const ConfigServer& config, int status) {
  int bodySize;
  std::string errorBody;

  errorBody = wsvErrorPage(status);
  bodySize = errorBody.length();
  this->_createHeaderLine(config, bodySize);
  this->_response.append(CRLF);
  this->_response.append(errorBody);
  return ;
}

unsigned int HttpResponse::getStatus(void) const {
  return (this->_status);
}

const std::string& HttpResponse::getResponse(void) const {
  return (this->_response);
}

void HttpResponse::setStatus(unsigned int status) {
  this->_status = status;
  return ;
}

int HttpResponse::createErrorResponseMessage(ConfigServer config, std::string version) {
  int responseSize;
  int bodySize = 0;
  std::vector<std::pair<int, std::string> >::const_iterator it;

  it = std::find_if(config.error_page.begin(), config.error_page.end(), FindNbrInVector(this->_status));

  if (it != config.error_page.end()) {
    bodySize = mylib::countFileSize(config.root + it->second);
    responseSize = this->_createHeaderLine(config, bodySize);
    this->_response.append(CRLF);
    if (!mylib::readFile(config.root + it->second, this->_response))
      responseSize = -1;
  }
  else
    _createErrorResponse(config, this->_status);
  if (responseSize == -1) {
    this->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    this->_response.clear();
    this->_createStatusLine(version);
    _createErrorResponse(config, this->_status);
  }
  responseSize = this->_response.length();
  return (responseSize);
}

int HttpResponse::createResponseMessage(const std::string& method, std::string path, const ConfigServer& config, std::string version) {
  int responseSize;
  int bodySize;

  this->_createStatusLine(version);
  if (400 <= this->_status && this->_status <= 600)
    return (this->createErrorResponseMessage(config, version));
  if (!method.compare("GET")) {
    bodySize = mylib::countFileSize(path);
    responseSize = this->_createHeaderLine(config, bodySize);
    this->_response.append(CRLF);
    if (!mylib::readFile(path, this->_response))
      return (-1);
  } else if (!method.compare("POST")) {
    this->_createHeaderLine(config, 0);
    this->_response.append(CRLF);
  } else if (!method.compare("DELETE")) {
    this->_createHeaderLine(config, 0);
    this->_response.append(CRLF);
  }
  responseSize = this->_response.length();
  return (responseSize);
}

void HttpResponse::execute(Socket& socket) {
  socket.send(this->_response, this->_response.length());
  return ;
}

HttpResponse& HttpResponse::operator=(const HttpResponse& obj) {
  if (this != &obj) {
    this->_status = obj.getStatus();
    this->_response = obj.getResponse();
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return (*this);
}

bool FindNbrInVector::operator()(const std::pair<int, std::string>& p) const {
  return (p.first == target);
}
