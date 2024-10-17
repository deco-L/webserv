/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/18 00:58:19 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Socket.hpp"
#include "Error.hpp"
#include "HttpGet.hpp"
#include "HttpPost.hpp"
#include "HttpDelete.hpp"


Http::Http(void): _method(""), _uri(""), _version(""), _requestSize(0), _httpRequest() {
  return ;
}

Http::Http(const Http& obj) {
  *this = obj;
  return ;
}

Http::~Http() {
  delete this->_httpResponse;
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

void Http::executeMethod(void) {
  this->_httpMethod->execute(this->_httpRequest, this->_httpResponse);
  return ;
}

bool Http::createMethod(void) {
  if (this->_method == "GET")
    this->_httpMethod = new HttpGet(this->_uri, this->_version);
  else if (this->_method == "DELETE")
    this->_httpMethod = new HttpDelete;
  else if (this->_method == "POST")
    this->_httpMethod = new HttpPost;
  else {
    this->_httpMethod = NULL;
    this->_httpResponse = new HttpResponse(400);
  }
  return (this->_httpMethod != NULL);
}

void Http::sendResponse(Socket& socket, const std::string& version) {
  this->_httpResponse->execute(socket, this->_httpRequest, version);
  return ;
}

std::string Http::getMethod(void) const {
  return (this->_method);
}

std::string Http::getUri(void) const {
  return (this->_uri);
}

std::string Http::getVersion(void) const {
  return (this->_version);
}

int Http::getRequestSize(void) const {
  return (this->_requestSize);
}

bool Http::checkSemantics(Socket& socket) {
  (void)socket;
  return (false);
}

void Http::_parseRequestLine(std::string line) {
  std::string element;
  std::istringstream stream(line);

  while (std::getline(stream, element, ' ')) {
    if (this->_method.empty())
      this->_method = element;
    else if (this->_uri.empty())
      this->_uri = element;
    else if (this->_version.empty())
      this->_version = element.substr(0, element.length() - 1);
  }
  return ;
}

void Http::parseRequestMessage(Socket& socket) {
  std::string line;
  std::vector<std::string> crline;
  std::istringstream stream(socket._outBuf);

  while (std::getline(stream, line)) {
    this->_requestSize -= line.length();
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

void Http::recvRequestMessage(Socket& socket) {
  this->_requestSize = socket.recv();
  if (this->_requestSize < 0)
    throw Http::HttpError("recvRequestMessage error");
  else if (this->_requestSize > MAX_SOCK_BUFFER)
    this->_httpResponse = new HttpResponse(413);
  else if (this->_requestSize == 0)
    std::cout << NORMA_COLOR << "connection end." << COLOR_RESET << std::endl;
  socket._error = this->_requestSize;
  return ;
}

void Http::showRequestLine(void) const {
  std::cout << this->_method << " " << this->_uri << " " << this->_version << std::endl;
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
  std::cout << this->_httpResponse->getResponse() << std::endl;
  return ;
}

Http& Http::operator=(const Http& obj) {
  if (this != &obj) {
    this->_method = obj.getMethod();
    this->_uri = obj.getUri();
    this->_version = obj.getVersion();
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
