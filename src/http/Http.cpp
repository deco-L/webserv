/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/06 15:25:01 by csakamot         ###   ########.fr       */
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

void Http::sendResponse(Socket& socket, const std::string& version) {
  this->_httpResponse->execute(socket, this->_httpRequest, version);
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
