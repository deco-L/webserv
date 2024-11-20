/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMethod.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/20 12:28:48 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHttpMethod.hpp"
#include "webserv.hpp"
#include "Config.hpp"
#include "HttpResponse.hpp"

AHttpMethod::AHttpMethod(void) : _method("default"), _uri(""), _version(""), _autoindex(false) {
  return ;
}

AHttpMethod::AHttpMethod(std::string method, std::string uri, std::string version) : _method(method), _uri(uri), _version(version), _autoindex(false) {
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

HttpResponse* AHttpMethod::setResponseStatus(const ConfigServer& config) {
  std::string path = "";
  ConfigLocation location;

  for (std::vector<ConfigLocation>::const_iterator it = config.locations.begin(); it != config.locations.end(); it++) {
    if (!this->_uri.compare(0, it->path.length(), it->path) && path.length() < it->path.length()) {
      location = *it;
      if (!location.root.empty())
        path = location.root + this->_uri;
      this->_autoindex = location.autoindex;
    }
  }
  if (!config.root.empty())
    path = config.root + this->_uri;
  if (config.autoindex)
    this->_autoindex = true;
  if (path.empty())
    return (new HttpResponse(HTTP_NOT_FOUND));
  if (location.methods.size()) {
    std::vector<std::string>::const_iterator it = std::find(location.methods.begin(), location.methods.end(), this->_method);
    if (it == location.methods.end())
      return (new HttpResponse(HTTP_METHOD_NOT_ALLOWED));
  }
  if (!this->_method.compare("POST") && !location.upload_enable)
    return (new HttpResponse(HTTP_METHOD_NOT_ALLOWED));
  else if (!this->_method.compare("POST") && !location.upload_store.empty()) {
    path = location.upload_store + this->_uri;
    this->setUri(path);
  } else if (!this->_method.compare("POST") && location.upload_store.empty() && !location.root.empty()) {
    this->setUri(path);
  }
  if (!this->_method.compare("GET") && !this->_autoindex && path.at(path.length() - 1) == '/') {
    if (mylib::isPathValid(path))
      return (new HttpResponse(HTTP_NOT_FOUND));
    else if (mylib::isDirectory(path))
      return (new HttpResponse(HTTP_FORBIDDEN));
    if (location.index.size()) {
      for (std::vector<std::string>::const_iterator it = location.index.begin(); it != location.index.end(); it++) {
        if (mylib::isPathValid(path + *it))
          return (new HttpResponse(HTTP_NOT_FOUND));
        else if (!mylib::isModeValid(path + *it, S_IRUSR))
          return (new HttpResponse(HTTP_FORBIDDEN));
        else {
          path = path + *it;
          this->setUri(path);
          return (new HttpResponse(HTTP_OK));
        }
      }
    } else if (!location.index.size() && config.index.size()){
      for (std::vector<std::string>::const_iterator it = config.index.begin(); it != config.index.end(); it++) {
        if (mylib::isPathValid(path + *it))
          return (new HttpResponse(HTTP_NOT_FOUND));
        else if (!mylib::isModeValid(path + *it, S_IRUSR))
          return (new HttpResponse(HTTP_FORBIDDEN));
        else {
          path = path + *it;
          this->setUri(path);
          return (new HttpResponse(HTTP_OK));
        }
      }
    }
  }
  if (mylib::isPathValid(path))
    return (new HttpResponse(HTTP_NOT_FOUND));
  else if (!mylib::isModeValid(path, S_IRUSR))
    return (new HttpResponse(HTTP_FORBIDDEN));
  std::cout << path << std::endl;
  this->setUri(path);
  return (new HttpResponse(HTTP_OK));
}

// static std::pair<std::string, std::string> setRootPath(const ConfigServer& config, std::string uri) {
//   std::pair<std::string, std::string> rootAndFilePath;
//   size_t pos;

//   (void)config;
//   pos = uri.rfind("/");
//   if (pos == std::string::npos)
//     return (rootAndFilePath);
//   rootAndFilePath.first = uri.substr(0, pos + 1);
//   rootAndFilePath.second = uri.substr(pos + 1);
//   return (rootAndFilePath);
// }

// HttpResponse* AHttpMethod::setResponseStatus(const ConfigServer& config) {
//   std::pair<std::string, std::string> uri;
//   std::vector<std::string> autoindexUris;
//   ConfigLocation location;

//   uri = setRootPath(config, this->_uri);
//   if (uri.first.empty() && uri.second.empty())
//     return (new HttpResponse(HTTP_BAD_REQUEST));
//   for (std::vector<ConfigLocation>::const_iterator it = config.locations.begin(); it != config.locations.end(); it++) {
//     if (it->autoindex)
//       autoindexUris.push_back(it->path);
//     if (!uri.first.compare(it->path))
//       location = *it;
//   }
//   if (location.methods.size()) {
//     std::vector<std::string>::const_iterator it = std::find(location.methods.begin(), location.methods.end(), this->_method);
//     if (it == location.methods.end())
//       return (new HttpResponse(HTTP_METHOD_NOT_ALLOWED));
//   }
//   if (!config.root.empty())
//     uri.first = config.root;
//   if (!location.root.empty())
//     uri.first = location.root;
//   if (this->getMethod() == "POST" && location.upload_enable && !location.upload_store.empty())
//     uri.first = location.upload_store;
//   else if (this->getMethod() == "POST" && location.upload_enable && location.upload_store.empty())
//     return (new HttpResponse(HTTP_INTERNAL_SERVER_ERROR));
//   if (uri.first.at(uri.first.size() - 1) != '/')
//     uri.first.append("/");
//   if (mylib::isPathValid(uri.first) || !mylib::isDirectory(uri.first))
//     return (new HttpResponse(HTTP_NOT_FOUND));
//   if (uri.second.empty() && location.index.size()) {
//     int status = 0;
//     for (std::vector<std::string>::const_iterator it = location.index.begin(); it != location.index.end(); it++) {
//       if (mylib::isPathValid(uri.first + *it))
//         status = HTTP_NOT_FOUND;
//       else if (!mylib::isModeValid(uri.first + *it, S_IRUSR))
//         status = HTTP_FORBIDDEN;
//       if (!mylib::isPathValid(uri.first + *it) && mylib::isModeValid(uri.first + *it, S_IRUSR)) {
//         uri.second = *it;
//         status = 0;
//         break ;
//       }
//     }
//     if (status != 0 && this->getMethod() == "GET" && location.autoindex) {
//       this->_autoindex = true;
//       this->setUri(uri.first);
//       return (new HttpResponse(HTTP_OK));
//     }
//     else if (status != 0)
//       return(new HttpResponse(status));
//   }
//   if (mylib::isPathValid(uri.first + uri.second) || !mylib::isModeValid(uri.first + uri.second, S_IRUSR))
//     return (new HttpResponse(HTTP_FORBIDDEN));
//   this->setUri(uri.first.append(uri.second));
//   return (new HttpResponse(HTTP_OK));
// }


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
