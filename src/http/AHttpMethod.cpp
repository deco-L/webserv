/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMethod.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmiyazaw <kmiyazaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/07 17:14:30 by kmiyazaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHttpMethod.hpp"
#include "webserv.hpp"
#include "Config.hpp"
#include "HttpResponse.hpp"

AHttpMethod::AHttpMethod(void) : _method("default"), _uri(""), _version(""), _autoindex(false), _cgi_extension(""), _cgi_path("") {
  return ;
}

AHttpMethod::AHttpMethod(std::string method, std::string uri, std::string version) : _method(method), _uri(uri), _version(version), _autoindex(false), _cgi_extension(""), _cgi_path("") {
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

HttpResponse* AHttpMethod::_returnRedirectStatus(const ConfigLocation& location) {
  std::string tmp = location.return_.second;

  this->setUri(tmp);
  return (new HttpResponse(location.return_.first));
}

HttpResponse* AHttpMethod::_setGetResponseStatus(const ConfigServer& config, std::string& path, const ConfigLocation& location) {
  if (path.at(path.length() - 1) == '/') {
    int status = 0;

    if (location.index.size()) {
      if (!mylib::isPathValid(path))
        return (new HttpResponse(HTTP_NOT_FOUND));
      if (!this->_autoindex && mylib::isDirectory(path))
        return (new HttpResponse(HTTP_FORBIDDEN));
      for (std::vector<std::string>::const_iterator it = location.index.begin(); it != location.index.end(); it++) {
        if (!mylib::isPathValid(path + *it))
          status = HTTP_NOT_FOUND;
        else if (!mylib::isModeValid(path + *it, S_IRUSR))
          status = HTTP_FORBIDDEN;
        else {
          path = path + *it;
          this->setUri(path);
          if (location.return_.first != 0)
            return (new HttpResponse(location.return_.first));
          return (new HttpResponse(HTTP_OK));
        }
      }
    } else if (!location.index.size() && config.index.size()){
      for (std::vector<std::string>::const_iterator it = config.index.begin(); it != config.index.end(); it++) {
        if (!mylib::isPathValid(path + *it))
          status = HTTP_NOT_FOUND;
        else if (!mylib::isModeValid(path + *it, S_IRUSR))
          status = HTTP_FORBIDDEN;
        else {
          path = path + *it;
          this->setUri(path);
          if (location.return_.first != 0)
            return (new HttpResponse(location.return_.first));
          return (new HttpResponse(HTTP_OK));
        }
      }
    }
    if (this->_autoindex && mylib::isDirectory(path)) {
      this->setUri(path);
      if (location.return_.first != 0)
        return (new HttpResponse(location.return_.first));
      return (new HttpResponse(HTTP_OK));
    }
    return (new HttpResponse(status));
  }
  if (!mylib::isPathValid(path))
    return (new HttpResponse(HTTP_NOT_FOUND));
  if (mylib::isDirectory(path))
    return (new HttpResponse(HTTP_MOVED_PERMANENTLY));
  else if (!mylib::isModeValid(path, S_IRUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  this->setUri(path);
  return (new HttpResponse(HTTP_OK));
}

HttpResponse* AHttpMethod::_setPostResponseStatus(std::string& path, const ConfigLocation& location) {
  if (!location.upload_enable)
    return (new HttpResponse(HTTP_METHOD_NOT_ALLOWED));
  else if (!location.upload_store.empty()) {
    path = location.upload_store + this->_uri;
    this->setUri(path);
  } else if (location.upload_store.empty() && !location.root.empty()) {
    this->setUri(path);
  }
  if (location.return_.first != 0)
    return (new HttpResponse(location.return_.first));
  return (new HttpResponse(HTTP_CREATED));
}

HttpResponse* AHttpMethod::_setDeleteResponseStatus(const ConfigServer& config, std::string& path, const ConfigLocation& location) {
  (void)config;

  this->setUri(path);
  if (mylib::isPathValid(this->_uri))
    return (new HttpResponse(HTTP_NO_CONTENT));
  if (mylib::isDirectory(this->_uri) || mylib::isModeValid(this->_uri, S_IRUSR | S_IXUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  if (location.return_.first != 0)
    return (new HttpResponse(location.return_.first));
  return (new HttpResponse(HTTP_OK));
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

static bool checkMethodPermission(const std::string& method, const std::vector<std::string>& methods) {
  std::vector<std::string>::const_iterator it = std::find(methods.begin(), methods.end(), method);

  if (it == methods.end())
    return (true);
  return (false);
}

HttpResponse* AHttpMethod::setResponseStatus(const ConfigServer& config) {
  std::string path("");
  ConfigLocation location;

  for (std::vector<ConfigLocation>::const_iterator it = config.locations.begin(); it != config.locations.end(); it++) {
    if (!this->_uri.compare(0, it->path.length(), it->path) && path.length() < it->path.length()) {
      location = *it;
      if (!location.root.empty())
        path = location.root + this->_uri;
      this->_autoindex = location.autoindex;

      if (location.cgi_extension.size())
      {
      // cgi_extension .py /usr/bin/python3
        // std::cout << location.cgi_extension[0].second << std::endl;
        // std::cout << "location.cgi_extension.empty()" << std::endl;
        this->_cgi_extension = location.cgi_extension[0].first;
        this->_cgi_path = location.cgi_extension[0].second;
        // std::cout << "this->_cgi_extension: " << this->_cgi_extension << std::endl;
        // std::cout << "this->_cgi_path: " << this->_cgi_path << std::endl;
      }
    }
  }
  if (path.empty() && location.return_.first != 0 && !location.return_.second.empty())
    return (this->_returnRedirectStatus(location));
  if (path.empty() && !config.root.empty()) {
    path = config.root + this->_uri;
    if (config.autoindex)
      this->_autoindex = true;
  }
  if (path.empty())
    return (new HttpResponse(HTTP_NOT_FOUND));
  if (location.methods.size() && checkMethodPermission(this->_method, location.methods))
    return (new HttpResponse(HTTP_METHOD_NOT_ALLOWED));
  if (!this->_method.compare("GET"))
    return (this->_setGetResponseStatus(config, path, location));
  else if (!this->_method.compare("POST"))
    return (this->_setPostResponseStatus(path, location));
  else if (!this->_method.compare("DELETE"))
    return (this->_setDeleteResponseStatus(config, path, location));
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
