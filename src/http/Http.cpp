/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/16 16:43:17 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Socket.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "HttpGet.hpp"
#include "HttpPost.hpp"
#include "HttpDelete.hpp"


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
    throw Http::HttpError("accept");
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
  this->_httpRequest.setBody(it, end);
  return ;
}

bool Http::checkSemantics(Socket& socket) {
  (void)socket;
  return (false);
}

void Http::executeMethod(const ConfigServer& config) {
  this->_httpMethod->execute(config, this->_httpRequest, this->_httpResponse);
  if (400 <= this->_httpResponse->getStatus() && this->_httpResponse->getStatus() <= 600) {
    if (this->_httpResponse->getStatus() == 400)
      throw Http::HttpError("HTTP_BAD_REQUEST");
    else if (this->_httpResponse->getStatus() == 401)
      throw Http::HttpError("HTTP_UNAUTHORIZED");
    else if (this->_httpResponse->getStatus() == 402)
      throw Http::HttpError("HTTP_PAYMENT_REQUIRED");
    else if (this->_httpResponse->getStatus() == 403)
      throw Http::HttpError("HTTP_FORBIDDEN");
    else if (this->_httpResponse->getStatus() == 404)
      throw Http::HttpError("HTTP_NOT_FOUND");
    else if (this->_httpResponse->getStatus() == 405)
      throw Http::HttpError("HTTP_NOT_ALLOWED");
    else if (this->_httpResponse->getStatus() == 406)
      throw Http::HttpError("HTTP_NOT_ACCEPTED");
    else if (this->_httpResponse->getStatus() == 407)
      throw Http::HttpError("HTTP_PROXY_AUTHENTICATION_REQUIRED");
    else if (this->_httpResponse->getStatus() == 408)
      throw Http::HttpError("HTTP_REQUEST_TIME_OUT");
    else if (this->_httpResponse->getStatus() == 409)
      throw Http::HttpError("HTTP_CONFLICT");
    else if (this->_httpResponse->getStatus() == 410)
      throw Http::HttpError("HTTP_GONE");
    else if (this->_httpResponse->getStatus() == 411)
      throw Http::HttpError("HTTP_LENGTH_REQUIRED");
    else if (this->_httpResponse->getStatus() == 412)
      throw Http::HttpError("HTTP_PRECONDETION_FAILED");
    else if (this->_httpResponse->getStatus() == 413)
      throw Http::HttpError("HTTP_REQUEST_ENTITY_TOO_LARGE");
    else if (this->_httpResponse->getStatus() == 414)
      throw Http::HttpError("HTTP_REQUEST_URI_TOO_LARGE");
    else if (this->_httpResponse->getStatus() == 415)
      throw Http::HttpError("HTTP_UNSUPPORTED_MEDIA_TYPE");
    else if (this->_httpResponse->getStatus() == 416)
      throw Http::HttpError("HTTP_RANGE_NOT_SATISFIABLE");
    else if (this->_httpResponse->getStatus() == 417)
      throw Http::HttpError("HTTP_EXPECTATION_FAILED");
    else if (this->_httpResponse->getStatus() == 421)
      throw Http::HttpError("HTTP_MISDIRECTED_REQUEST");
    else if (this->_httpResponse->getStatus() == 421)
      throw Http::HttpError("HTTP_UNPROCESSABLE_CONTENT");
    else if (this->_httpResponse->getStatus() == 426)
      throw Http::HttpError("HTTP_UPGRADE_REQUIRED");
    else if (this->_httpResponse->getStatus() == 500)
      throw Http::HttpError("HTTP_INTERNAL_SERVER_ERROR");
    else if (this->_httpResponse->getStatus() == 501)
      throw Http::HttpError("HTTP_NOT_IMPLEMENTED");
    else if (this->_httpResponse->getStatus() == 502)
      throw Http::HttpError("HTTP_BAD_GATEWAY");
    else if (this->_httpResponse->getStatus() == 503)
      throw Http::HttpError("HTTP_SERVICE_UNAVAILABLE");
    else if (this->_httpResponse->getStatus() == 504)
      throw Http::HttpError("HTTP_GATEWAY_TIME_OUT");
    else if (this->_httpResponse->getStatus() == 505)
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
