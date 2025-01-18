/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/18 23:55:05 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Socket.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "HttpGet.hpp"
#include "HttpPost.hpp"
#include "HttpDelete.hpp"
#include "HttpResponse.hpp"


Http::Http(void): _requestSize(0), _httpRequest(), _httpMethod(NULL), _httpResponse(NULL) {
  return ;
}

Http::Http(const Http& obj) {
  *this = obj;
  return ;
}

Http::~Http() {
  if (this->_httpResponse != NULL)
    delete this->_httpResponse;
  if (this->_httpMethod != NULL)
    delete this->_httpMethod;
  return ;
}

Http::HttpError::HttpError(std::string error) : _error_message(error) {
  return ;
}

Http::HttpError::~HttpError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
}

const char* Http::HttpError::what(void) const throw() {
  return (this->_error_message.c_str());
}

void Http::_parseRequestLine(std::string line) {
  std::string element;
  std::istringstream stream(line);

  while (std::getline(stream, element, ' ')) {
    if (this->_httpRequest.getMethod().empty())
      this->_httpRequest.setMethod(element);
    else if (this->_httpRequest.getUri().empty())
      this->_httpRequest.setUri(element);
    else if (this->_httpRequest.getVersion().empty())
      this->_httpRequest.setVersion(element.substr(0, element.length() - 1));
  }
  return ;
}

const std::string& Http::getMethod(void) const {
  return (this->_httpRequest.getMethod());
}

const std::string& Http::getUri(void) const {
  return (this->_httpRequest.getUri());
}

const std::string& Http::getVersion(void) const {
  return (this->_httpRequest.getVersion());
}

int Http::getRequestSize(void) const {
  return (this->_requestSize);
}

unsigned long Http::getRequestBodySize(void) const {
  return (this->_httpRequest.getBodySize());
}

void Http::setHttpResponse(unsigned int status) {
  this->_httpResponse = new HttpResponse(status);
  return ;
}

void Http::recvRequestMessage(Socket& socket) {
  this->_requestSize = socket.recv();
  if (this->_requestSize < 0)
    throw Http::HttpError("recv");
  else if (this->_requestSize == 0)
    std::cout << NORMA_COLOR << "connection end." << COLOR_RESET << std::endl;
  socket._error = this->_requestSize;
  return ;
}

void Http::parseRequestMessage(Socket& socket) {
  std::string line;
  std::vector<std::string> crline;
  std::istringstream stream(socket._outBuf);

  while (std::getline(stream, line)) {
    if (!line.empty()) {
      line.substr(0, line.length() - 1);
      crline.push_back(line);
    }
  }

  std::vector<std::string>::iterator it = crline.begin();
  std::vector<std::string>::iterator end = crline.end();
  this->_parseRequestLine(*it);
  it++;
  this->_httpRequest.setHeaders(it, end);
  if (it != end)
    it++;
  if (
    this->_httpRequest.getHeader().find("Transfer-Encoding") !=
    this->_httpRequest.getHeader().end() &&
    this->_httpRequest.getHeader().at("Transfer-Encoding") == "chunked"
  )
    this->_httpRequest.setChunkedBody(it, end);
  else {
    this->_httpRequest.setBody(it, end);
  }
  this->_httpRequest.setIp(socket.getCSockAddr().sin_addr);
  if (this->_httpRequest.getBodySize() == -1)
    Http::HttpError("stringstream error");
  return ;
}

void Http::checkRequestMessage(const ConfigServer& config) {
  std::map<std::string, std::string>::const_iterator it;
  std::string host;
  size_t colonPos = 0;

  if (config.client_max_body_size != 0 && this->getRequestBodySize() > config.client_max_body_size) {
    this->setHttpResponse(HTTP_REQUEST_ENTITY_TOO_LARGE);
    throw Http::HttpError("HTTP_REQUEST_ENTITY_TOO_LARGE");
  }
  it = this->_httpRequest.getHeader().find("Host");
  colonPos = it->second.find(':');
  if (colonPos != std::string::npos)
    host = it->second.substr(0, colonPos);
  else
    host = it->second;
  if (host != config.server_name.at(0) && host != config.listen.at(0).first) {
    this->setHttpResponse(HTTP_BAD_REQUEST);
    throw Http::HttpError("HTTP_BAD_REQUEST");
  }
  return ;
}

void Http::executeMethod(const ConfigServer& config) {
  this->_httpMethod->execute(config, this->_httpRequest, this->_httpResponse);
  if (300 <= this->_httpResponse->getStatus() && this->_httpResponse->getStatus() < 600) {
    if (this->_httpResponse->getStatus() == HTTP_SPECIAL_RESPONSE)
      throw Http::HttpError("HTTP_SPECIAL_RESPONSE");
    else if (this->_httpResponse->getStatus() == HTTP_MOVED_PERMANENTLY)
      throw Http::HttpError("HTTP_MOVED_PERMANENTLY");
    else if (this->_httpResponse->getStatus() == HTTP_MOVED_TEMPORARILY)
      throw Http::HttpError("HTTP_MOVED_TEMPORARILY");
    else if (this->_httpResponse->getStatus() == HTTP_SEE_OTHER)
      throw Http::HttpError("HTTP_SEE_OTHER");
    else if (this->_httpResponse->getStatus() == HTTP_NOT_MODIFIED)
      throw Http::HttpError("HTTP_NOT_MODIFIED");
    else if (this->_httpResponse->getStatus() == HTTP_USE_PROXY)
      throw Http::HttpError("HTTP_USE_PROXY");
    else if (this->_httpResponse->getStatus() == HTTP_TEMPORARY_REDIRECT)
      throw Http::HttpError("HTTP_TEMPORARY_REDIRECT");
    else if (this->_httpResponse->getStatus() == HTTP_PERMANENT_REDIRECT)
      throw Http::HttpError("HTTP_PERMANENT_REDIRECT");
    else if (this->_httpResponse->getStatus() == HTTP_BAD_REQUEST)
      throw Http::HttpError("HTTP_BAD_REQUEST");
    else if (this->_httpResponse->getStatus() == HTTP_UNAUTHORIZED)
      throw Http::HttpError("HTTP_UNAUTHORIZED");
    else if (this->_httpResponse->getStatus() == HTTP_PAYMENT_REQUIRED)
      throw Http::HttpError("HTTP_PAYMENT_REQUIRED");
    else if (this->_httpResponse->getStatus() == HTTP_FORBIDDEN)
      throw Http::HttpError("HTTP_FORBIDDEN");
    else if (this->_httpResponse->getStatus() == HTTP_NOT_FOUND)
      throw Http::HttpError("HTTP_NOT_FOUND");
    else if (this->_httpResponse->getStatus() == HTTP_METHOD_NOT_ALLOWED)
      throw Http::HttpError("HTTP_NOT_ALLOWED");
    else if (this->_httpResponse->getStatus() == HTTP_NOT_ACCEPTABLE)
      throw Http::HttpError("HTTP_NOT_ACCEPTED");
    else if (this->_httpResponse->getStatus() == HTTP_PROXY_AUTHENTICATION_REQUIRED)
      throw Http::HttpError("HTTP_PROXY_AUTHENTICATION_REQUIRED");
    else if (this->_httpResponse->getStatus() == HTTP_REQUEST_TIME_OUT)
      throw Http::HttpError("HTTP_REQUEST_TIME_OUT");
    else if (this->_httpResponse->getStatus() == HTTP_CONFLICT)
      throw Http::HttpError("HTTP_CONFLICT");
    else if (this->_httpResponse->getStatus() == HTTP_GONE)
      throw Http::HttpError("HTTP_GONE");
    else if (this->_httpResponse->getStatus() == HTTP_LENGTH_REQUIRED)
      throw Http::HttpError("HTTP_LENGTH_REQUIRED");
    else if (this->_httpResponse->getStatus() == HTTP_PRECONDETION_FAILED)
      throw Http::HttpError("HTTP_PRECONDETION_FAILED");
    else if (this->_httpResponse->getStatus() == HTTP_REQUEST_ENTITY_TOO_LARGE)
      throw Http::HttpError("HTTP_REQUEST_ENTITY_TOO_LARGE");
    else if (this->_httpResponse->getStatus() == HTTP_REQUEST_URI_TOO_LARGE)
      throw Http::HttpError("HTTP_REQUEST_URI_TOO_LARGE");
    else if (this->_httpResponse->getStatus() == HTTP_UNSUPPORTED_MEDIA_TYPE)
      throw Http::HttpError("HTTP_UNSUPPORTED_MEDIA_TYPE");
    else if (this->_httpResponse->getStatus() == HTTP_RANGE_NOT_SATISFIABLE)
      throw Http::HttpError("HTTP_RANGE_NOT_SATISFIABLE");
    else if (this->_httpResponse->getStatus() == HTTP_EXPECTATION_FAILED)
      throw Http::HttpError("HTTP_EXPECTATION_FAILED");
    else if (this->_httpResponse->getStatus() == HTTP_MISDIRECTED_REQUEST)
      throw Http::HttpError("HTTP_MISDIRECTED_REQUEST");
    else if (this->_httpResponse->getStatus() == HTTP_UNPROCESSABLE_CONTENT)
      throw Http::HttpError("HTTP_UNPROCESSABLE_CONTENT");
    else if (this->_httpResponse->getStatus() == HTTP_UPGRADE_REQUIRED)
      throw Http::HttpError("HTTP_UPGRADE_REQUIRED");
    else if (this->_httpResponse->getStatus() == HTTP_INTERNAL_SERVER_ERROR)
      throw Http::HttpError("HTTP_INTERNAL_SERVER_ERROR");
    else if (this->_httpResponse->getStatus() == HTTP_NOT_IMPLEMENTED)
      throw Http::HttpError("HTTP_NOT_IMPLEMENTED");
    else if (this->_httpResponse->getStatus() == HTTP_BAD_GATEWAY)
      throw Http::HttpError("HTTP_BAD_GATEWAY");
    else if (this->_httpResponse->getStatus() == HTTP_SERVICE_UNAVAILABLE)
      throw Http::HttpError("HTTP_SERVICE_UNAVAILABLE");
    else if (this->_httpResponse->getStatus() == HTTP_GATEWAY_TIME_OUT)
      throw Http::HttpError("HTTP_GATEWAY_TIME_OUT");
    else if (this->_httpResponse->getStatus() == HTTP_VERSION_NOT_SUPPORTED)
      throw Http::HttpError("HTTP_VERSION_NOT_SUPPORTED");
  }
  return ;
}

bool Http::createMethod(void) {
  if (this->_httpRequest.getMethod() == "GET")
    this->_httpMethod = new HttpGet(this->_httpRequest.getUri(), this->_httpRequest.getVersion());
  else if (this->_httpRequest.getMethod() == "DELETE")
    this->_httpMethod = new HttpDelete(this->_httpRequest.getUri(), this->_httpRequest.getVersion());
  else if (this->_httpRequest.getMethod() == "POST")
    this->_httpMethod = new HttpPost(this->_httpRequest.getUri(), this->_httpRequest.getVersion());
  else {
    this->_httpMethod = NULL;
    this->_httpResponse = new HttpResponse(HTTP_BAD_REQUEST);
  }
  return (this->_httpMethod != NULL);
}

void Http::createResponseMessage(const ConfigServer& config) {
  this->_httpResponse->createResponseMessage(
    this->getMethod(),
    this->getUri(),
    config,
    this->getVersion()
  );
  return ;
}

void Http::sendResponse(Socket& socket) {
  this->_httpResponse->execute(socket);
  return ;
}

void Http::showRequestLine(void) const {
  std::cout << this->_httpRequest.getMethod() << " " << this->_httpRequest.getUri() << " " << this->_httpRequest.getVersion() << std::endl;
  return ;
}

void Http::showRequestHeaders(void) const {
  this->_httpRequest.showHeaders();
  return ;
}

void Http::showRequestBody(void) const {
  std::cout << this->_httpRequest.getBody() << std::endl;
  return ;
}

void Http::showResponseMessage(void) const {
  std::cout << this->_httpResponse->getResponse();
  return ;
}

Http& Http::operator=(const Http& obj) {
  if (this != &obj) {
    this->_requestSize = obj.getRequestSize();
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return (*this);
}
