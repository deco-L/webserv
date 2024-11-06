/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMethod.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/06 17:23:19 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHttpMethod.hpp"
#include "webserv.hpp"
#include "Config.hpp"
#include "HttpResponse.hpp"

AHttpMethod::AHttpMethod(void) : _method("default"), _uri(""), _version("") {
  return ;
}

AHttpMethod::AHttpMethod(std::string method) : _method(method) {
  return ;
}

AHttpMethod::AHttpMethod(const AHttpMethod& obj) : _method(obj.getMethod()) {
  *this = obj;
  return ;
}

AHttpMethod::~AHttpMethod() {
  return ;
}

AHttpMethod::MethodError::MethodError(std::string error) : _error_message(error) {
  return ;
}

AHttpMethod::MethodError::~MethodError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
  return ;
}

const char* AHttpMethod::MethodError::what(void) const throw() {
  return (this->_error_message.c_str());
}

const std::string& AHttpMethod::getMethod(void) const {
  return (this->_method);
}

const std::string& AHttpMethod::getUri(void) const {
  return (this->_uri);
}

const std::string& AHttpMethod::getVersion(void) const {
  return (this->_version);
}

void AHttpMethod::setUri(std::string& uri) {
  if (!uri.empty())
    this->_uri = uri;
  return ;
}

static std::pair<std::string, std::string>& setRootPath(const ConfigServer& config, std::string uri) {
  std::pair<std::string, std::string> rootAndFilePath;
  size_t pos;

  pos = uri.rfind("/");
  if (pos == std::string::npos)
    return (rootAndFilePath);
  rootAndFilePath.first = uri.substr(0, pos + 1);
  rootAndFilePath.second = uri.substr(pos + 1);
  return (rootAndFilePath);
}

HttpResponse* AHttpMethod::setResponseStatus(const ConfigServer& config) {
  std::pair<std::string, std::string> uri;
  ConfigLocation location;

  uri = setRootPath(config, this->_uri);
  if (uri.first.empty() && uri.second.empty())
    throw (new HttpResponse(HTTP_BAD_REQUEST));
  for (std::vector<ConfigLocation>::const_iterator it = config.locations.begin(); it != config.locations.end(); it++) {
    if (!uri.first.compare(it->path))
      location = *it;
  }
  if (location.methods.size()) {
    std::vector<std::string>::const_iterator it = std::find(location.methods.begin(), location.methods.end(), this->_method);
    if (it == location.methods.end())
      throw (new HttpResponse(HTTP_NOT_ALLOWED));
  }
  if (!config.root.empty())
    uri.first = config.root;
  if (!location.root.empty())
    uri.first = location.root;
  if (location.upload_enable && !location.upload_store.empty())
    uri.first = location.upload_store;
  else if (location.upload_enable && location.upload_store.empty())
    throw (new HttpResponse(HTTP_INTERNAL_SERVER_ERROR));
  if (mylib::isPathValid(uri.first) || mylib::isDirectory(uri.first))
    return (new HttpResponse(HTTP_NOT_FOUND));
  if (uri.second.empty() && location.index.size()) {
    for (std::vector<std::string>::const_iterator it = location.index.begin(); it != location.index.end(); it++)
      if (!mylib::isPathValid(uri.first.append(*it)) && mylib::isModeValid(uri.first.append(*it), S_IRUSR | S_IXUSR))
        break ;
    throw(new HttpResponse(HTTP_FORBIDDEN));
  }
  if (mylib::isPathValid(uri.first.append(uri.second)) || mylib::isModeValid(uri.first.append(uri.second), S_IRUSR | S_IXUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  this->setUri(uri.first.append(uri.second));
  return (new HttpResponse(HTTP_OK));
}


AHttpMethod& AHttpMethod::operator=(const AHttpMethod& obj) {
  if (this != &obj) {
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return *this;
}
