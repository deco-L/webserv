/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMethod.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/25 20:18:55 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHttpMethod.hpp"
#include "webserv.hpp"
#include "Config.hpp"
#include "HttpResponse.hpp"

AHttpMethod::AHttpMethod(void) : _method("default"), _uri(""), _uri_old(""), _version(""), _autoindex(false), _cgi_extension(""), _cgi_path(""), _cgi_relative_path("") {
  return ;
}

AHttpMethod::AHttpMethod(std::string method, std::string uri, std::string version) : _method(method), _uri(uri), _uri_old(uri), _version(version), _autoindex(false), _cgi_extension(""), _cgi_path(""), _cgi_relative_path("") {
  std::string::size_type pos = uri.find(".py");
  std::string tmp = uri;

  this->_cgi_relative_path.clear();
  if (pos != std::string::npos) {
      if (pos + 3 <= uri.size()) {
          this->_uri = uri.substr(0, pos + 3);
          this->_uri_old = uri;
      } else
          this->_uri = uri;
  } else {
      this->_cgi_relative_path.clear();
      this->_uri = uri;
  }
  return;
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
  HttpResponse* response = new HttpResponse(location.return_.first);
  std::string redirectPath = location.return_.second;

  response->setReturnFlag(true);
  response->setRedirectPath(redirectPath);
  return (response);
}

HttpResponse* AHttpMethod::_setGetResponseStatus(const ConfigServer& config, std::string& path, const ConfigLocation& location) {
  path = path + this->_uri;
  if (path.at(path.length() - 1) == '/') {
    int status = 0;

    if (location.index.size()) {
      if (!mylib::isPathValid(path))
        return (new HttpResponse(HTTP_NOT_FOUND, location));
      for (std::vector<std::string>::const_iterator it = location.index.begin(); it != location.index.end(); it++) {
        if (!mylib::isPathValid(path + *it))
          status = HTTP_NOT_FOUND;
        else if (!mylib::isModeValid(path + *it, S_IRUSR))
          status = HTTP_FORBIDDEN;
        else {
          path = path + *it;
          this->setUri(path);
          if (location.return_.first != 0)
            return (new HttpResponse(location.return_.first, location));
          return (new HttpResponse(HTTP_OK, location));
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
            return (new HttpResponse(location.return_.first, location));
          return (new HttpResponse(HTTP_OK, location));
        }
      }
    }
    if (!this->_autoindex && mylib::isDirectory(path))
      return (new HttpResponse(HTTP_FORBIDDEN, location));
    else if (this->_autoindex && mylib::isDirectory(path)) {
      this->setUri(path);
      if (location.return_.first != 0)
        return (new HttpResponse(location.return_.first, location));
      return (new HttpResponse(HTTP_OK, location));
    }
    return (new HttpResponse(status, location));
  }
  if (!mylib::isPathValid(path))
    return (new HttpResponse(HTTP_NOT_FOUND, location));
  if (mylib::isDirectory(path) && path[path.size() - 1] != '/')
    return (new HttpResponse(HTTP_MOVED_PERMANENTLY, path, location));
  else if (!mylib::isModeValid(path, S_IRUSR))
    return (new HttpResponse(HTTP_FORBIDDEN, location));
  this->setUri(path);
  return (new HttpResponse(HTTP_OK, location));
}

HttpResponse* AHttpMethod::_setPostResponseStatus(std::string& path, const ConfigLocation& location) {
  if (!location.upload_enable)
    return (new HttpResponse(HTTP_METHOD_NOT_ALLOWED, location));
  else if (!location.upload_store.empty())
    path = path + location.upload_store + this->_uri;
  else
    path = path + this->_uri;
  if (mylib::isDirectory(path) && path[path.size() - 1] != '/')
    return (new HttpResponse(HTTP_MOVED_PERMANENTLY, path, location));
  if (mylib::isDirectory(path) && !mylib::isPathValid(path))
    return (new HttpResponse(HTTP_NOT_FOUND, location));
  this->setUri(path);
  if (location.return_.first != 0)
    return (new HttpResponse(location.return_.first, location));
  return (new HttpResponse(HTTP_CREATED, location));
}

HttpResponse* AHttpMethod::_setDeleteResponseStatus(std::string& path, const ConfigLocation& location) {
  path = path + this->_uri;
  if (!mylib::isPathValid(path))
    return (new HttpResponse(HTTP_NO_CONTENT, location));
  if (mylib::isDirectory(path) || !mylib::isModeValid(path, S_IRUSR))
    return (new HttpResponse(HTTP_FORBIDDEN, location));
  if (mylib::isDirectory(path) && path[path.size() - 1] != '/')
    return (new HttpResponse(HTTP_MOVED_PERMANENTLY, path, location));
  this->setUri(path);
  if (location.return_.first != 0)
    return (new HttpResponse(location.return_.first, location));
  return (new HttpResponse(HTTP_OK, location));
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
      path = it->path;
      location = *it;
      this->_autoindex = location.autoindex;
      if (location.cgi_extension.size()) {
        this->_cgi_extension = location.cgi_extension[0].first;
        this->_cgi_path = location.cgi_extension[0].second;
      }
    }
  }
  if ((location.path.empty() && config.root.empty()) || (!location.path.empty() && config.root.empty()))
    return (new HttpResponse(HTTP_NOT_FOUND, location));
  else if (!location.path.empty() && !location.root.empty())
    path = location.root;
  else if (!location.path.empty() && location.root.empty() && !config.root.empty())
    path = config.root;
  if (location.return_.first != 0 && !location.return_.second.empty())
    return (this->_returnRedirectStatus(location));
  if (location.path.empty() && !config.root.empty()) {
    path = config.root;
    if (config.autoindex)
      this->_autoindex = true;
  }
  if (location.methods.size() && checkMethodPermission(this->_method, location.methods))
    return (new HttpResponse(HTTP_METHOD_NOT_ALLOWED, location));
  if (!this->_method.compare("GET"))
    return (this->_setGetResponseStatus(config, path, location));
  else if (!this->_method.compare("POST"))
    return (this->_setPostResponseStatus(path, location));
  else if (!this->_method.compare("DELETE"))
    return (this->_setDeleteResponseStatus(path, location));
  return (new HttpResponse(HTTP_OK, location));
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
