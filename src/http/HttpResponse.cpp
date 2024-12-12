/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/08 16:21:02 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "Socket.hpp"
#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

HttpResponse::HttpResponse(void): _status(0), _redirectPath(""), _response("") {
  return ;
}

HttpResponse::HttpResponse(unsigned int status): _status(status), _redirectPath(""), _response("") {
  return ;
}

HttpResponse::HttpResponse(unsigned int status, std::string redirectPath): _status(status), _redirectPath(redirectPath), _response("") {
  return ;
}

HttpResponse::HttpResponse(const HttpResponse& obj) {
  *this = obj;
  return ;
}

HttpResponse::~HttpResponse() {
  return ;
}

HttpResponse::HttpResponseError::HttpResponseError(std::string error) : _error_message(error) {
  return ;
}

HttpResponse::HttpResponseError::~HttpResponseError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
}

const char* HttpResponse::HttpResponseError::what(void) const throw() {
  return (this->_error_message.c_str());
}

int HttpResponse::_createStatusLine(std::string version) {
  this->_response.append(version);
  this->_response.append(" ");
  this->_response.append(mylib::nbrToS(this->_status));
  this->_response.append(" ");
  switch(this->_status) {
    case HTTP_CONTINUE:
      this->_response.append("Continue");
      break ;
    case HTTP_SWITCHING_PROTOCOlS:
      this->_response.append("Switching Protocols");
      break ;
    case HTTP_OK:
      this->_response.append("OK");
      break ;
    case HTTP_CREATED:
      this->_response.append("Created");
      break ;
    case HTTP_ACCEPTED:
      this->_response.append("Accepted");
      break ;
    case HTTP_NON_AUTHORITATIVE_INFO:
      this->_response.append("Non Authoritative Info");
      break ;
    case HTTP_NO_CONTENT:
      this->_response.append("No Content");
      break ;
    case HTTP_RESET_CONTENT:
      this->_response.append("Reset Content");
      break ;
    case HTTP_PARTIAL_CONTENT:
      this->_response.append("Partial Content");
      break ;
    case HTTP_SPECIAL_RESPONSE:
      this->_response.append("Special Response");
      break ;
    case HTTP_MOVED_PERMANENTLY:
      this->_response.append("Moved Permanently");
      break ;
    case HTTP_MOVED_TEMPORARILY:
      this->_response.append("Moved Temporarily");
      break ;
    case HTTP_SEE_OTHER:
      this->_response.append("See Other");
      break ;
    case HTTP_NOT_MODIFIED:
      this->_response.append("Not Modified");
      break ;
    case HTTP_USE_PROXY:
      this->_response.append("Use Proxy");
      break ;
    case HTTP_TEMPORARY_REDIRECT:
      this->_response.append("Temporary Redirect");
      break ;
    case HTTP_PERMANENT_REDIRECT:
      this->_response.append("Permanent Redirect");
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
    case HTTP_METHOD_NOT_ALLOWED:
      this->_response.append("Method Not Allowed");
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

static std::string _traverse(std::string path) {
  std::string directories;
  std::string filePath;
  DIR *dir;
  struct dirent *entry;
  struct stat status;

  dir = opendir(path.c_str());
  if (dir == NULL) {
    return ("");
  }
  while ((entry = readdir(dir)) != NULL) {
    std::string filePath = path + "/" + entry->d_name;
    if (stat(filePath.c_str(), &status) == 0) {
      std::ostringstream oss;

      oss << "      <a href=\"" << entry->d_name;
      if (mylib::isDirectory(path + entry->d_name))
        oss << "/\">" << entry->d_name << "</a>\n";
      else
        oss <<  "\">" << entry->d_name << "</a>\n";
      oss << "       " << mylib::formatTime(status.st_mtime) << " ";
      if (S_ISDIR(status.st_mode)) {
        oss << "- \n";
      } else {
        oss << mylib::fileSizeToString(status.st_size) << " \n";
      }
      directories.append(oss.str());
    }
  }
  closedir(dir);
  return (directories);
}

std::string HttpResponse::_createAutoindexBody(std::string path) {
  std::string body;
  std::string tmp;

  body.append(
    "<!DOCTYPE html>\n"
    "<html>\n"
    "  <head>\n"
    "    <link rel=\"icon\" href=\"data:,\" />\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>Index of "
  );
  body.append(path);
  body.append(
    "</title>\n"
    "  </head>\n"
    "  <body cz-shortcut-listen=\"true\">\n"
    "    <h1>Index of "
  );
  body.append(path);
  body.append(
    "</h1>\n"
    "    <hr>\n"
    "    <pre>\n"
  );
  tmp = _traverse(path);
  if (tmp.empty())
    return ("");
  body.append(tmp);
  body.append(
    "    </pre>\n"
    "    <hr>\n"
    "  </body>\n"
    "</html>\n"
  );
  return (body);
}

unsigned int HttpResponse::getStatus(void) const {
  return (this->_status);
}

const std::string& HttpResponse::getRedirectPath(void) const {
  return (this->_redirectPath);
}

const std::string& HttpResponse::getResponse(void) const {
  return (this->_response);
}

void HttpResponse::setStatus(unsigned int status) {
  this->_status = status;
  return ;
}

static std::string responseTemplateMessage(const std::string& uri, const std::string& exec) {
  std::string str;

  str.append(
    "<!DOCTYPE html>\n"
    "<html>\n"
    "  <head>\n"
    "    <link rel=\"icon\" href=\"data:,\" />\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title> File \""
  );
  str.append(uri);
  str.append(
    "\" "
  );
  str.append(exec);
  str.append(
    ".</title>\n"
    "  </head>\n"
    "  <body cz-shortcut-listen=\"true\">\n"
    "    <center>\n"
    "      <h1> File\""
  );
  str.append(uri);
  str.append(
    "\" "
  );
  str.append(exec);
  str.append(
    ".</h1>\n"
    "    </center>\n"
    "    <hr>\n"
    "    <center>webserv/1.0</center>\n"
    "  </body>\n"
    "</html>\n"
  );
  return (str);
}

void HttpResponse::_createPostResponseMessage(const std::string& uri, const ConfigServer& config) {
  int bodySize = 0;
  std::string PostBody;

  PostBody = responseTemplateMessage(uri, "created");
  bodySize = PostBody.length();
  this->_createHeaderLine(config, bodySize);
  this->_response.append(CRLF);
  this->_response.append(PostBody);
  return ;
}

void HttpResponse::_createDeleteResponseMessage(const std::string& uri, const ConfigServer& config) {
  int bodySize = 0;
  std::string deleteBody;

  deleteBody = responseTemplateMessage(uri, "deleted");
  bodySize = deleteBody.length();
  this->_createHeaderLine(config, bodySize);
  this->_response.append(CRLF);
  this->_response.append(deleteBody);
  return ;
}

int HttpResponse::_createRedirectResponseMessage(const std::string& uri, const ConfigServer& config) {
  int responseSize;
  int bodySize = 0;
  std::string redirectBody;

  redirectBody = wsvRedirectPage(this->_status, uri);
  bodySize = redirectBody.length();
  if (bodySize == 0)
    return (-1);
  this->_response.append("Location: " + uri + '/' + CRLF);
  this->_createHeaderLine(config, bodySize);
  this->_response.append(CRLF);
  this->_response.append(redirectBody);
  responseSize = this->_response.length();
  return (responseSize);
}

int HttpResponse::_createErrorResponseMessage(const ConfigServer& config, const std::string& version) {
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
  if (300 <= this->_status && this->_status < 400)
    return (this->_createRedirectResponseMessage(path, config));
  if (400 <= this->_status && this->_status < 600)
    return (this->_createErrorResponseMessage(config, version));
  if (!method.compare("GET")) {
    bodySize = mylib::countFileSize(path);
    responseSize = this->_createHeaderLine(config, bodySize);
    this->_response.append(CRLF);
    if (!mylib::readFile(path, this->_response))
      return (-1);
  } else if (!method.compare("POST"))
    this->_createPostResponseMessage(path, config);
  else if (!method.compare("DELETE"))
    this->_createDeleteResponseMessage(path, config);
  responseSize = this->_response.length();
  return (responseSize);
}

int HttpResponse::createAutoindexMessage(std::string path, const ConfigServer& config, std::string version) {
  int responseSize;
  std::string body;

  this->_createStatusLine(version);
  body = _createAutoindexBody(path);
  if (body.empty()) {
    this->_response.clear();
    this->setStatus(HTTP_INTERNAL_SERVER_ERROR);
  }
  this->_createHeaderLine(config, body.length());
  this->_response.append(CRLF);
  this->_response.append(body);
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








static std::vector<std::string> createEnvs(const ConfigServer& config, std::string _uri, std::string method, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string version)
{
  std::vector<std::string> envs;
  
  //(void)_uri;
  (void)cgiExtension;
  
  //envs = config.getEnvp();
  //PATH_INFO	パス情報。たとえば、「cgi-bin/xxx.cgi/taro/xxx.htm」というURLでCGIスクリプトを呼び出した場合、PATH_INFOには「/taro/xxx.htm」が格納される。
  //cgiExtensionがある部分より右側の部分を取得する
  if (_uri != _uri_old)
  {
    envs.push_back("PATH_INFO=" + _uri_old.substr(_uri_old.find(cgiExtension) + cgiExtension.length()));
  }
  envs.push_back("SCRIPT_NAME=" + cgiPath);
  
  
  
  //if (_uri.find("?") == std::string::npos) //_uriに?が含まれいない場合
  //{
  //  envs.push_back("PATH_INFO=" + _uri.substr(_uri.find(cgiExtension) + cgiExtension.length()));
  //  envs.push_back("SCRIPT_NAME=" + cgiPath);
  //} else { //_uriに?が含まれる場合
  //  envs.push_back("PATH_INFO=" + _uri.substr(_uri.find(cgiExtension) + cgiExtension.length(), _uri.find("?") - _uri.find(cgiExtension) - cgiExtension.length()));
  //  envs.push_back("SCRIPT_NAME=" + cgiPath + _uri.substr(0, _uri.find("?")));
  //  envs.push_back("QUERY_STRING=" + _uri.substr(_uri.find("?") + 1));
  //  std::cout << "QUERY_STRING: " << _uri.substr(_uri.find("?") + 1) << std::endl;
  //}
  envs.push_back("SERVER_NAME=" + config.server_name.front());
  envs.push_back("SERVER_PORT=" + config.listen.front().first);
  envs.push_back("REQUEST_METHOD=" + method);
  envs.push_back("SERVER_PROTOCOL=" + version);
  envs.push_back("SERVER_SOFTWARE=webserv");
  return (envs);
}


int cgiExecGet(int &readFd, pid_t &pid, const std::vector<char*>& envs, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string _uri) {
  int pipeFd[2];
  int status;
  
  //(void)cgiRelativePath;
  (void)_uri;
  (void)_uri_old;
  //std::cout << _uri << std::endl;
  
  if (pipe(pipeFd) == -1)
  {
    std::cout << "pipe error" << std::endl;
    return (-1);
  }
  
  pid = fork();
  if (pid == -1)
  {
    close(pipeFd[0]);
    close(pipeFd[1]);
    return (-1);
  }
  if (pid == 0) {
    close(pipeFd[0]);
    if (dup2(pipeFd[1], 1) == -1)
    {
      perror("dup2");
      _exit(EXIT_FAILURE);
    }
    close(pipeFd[1]);

    if (cgiExtension == ".py") {
      char* argv[] = {
        const_cast<char*>("python3"),
        const_cast<char*>(_uri.c_str()),
        NULL};
      if (execve(cgiPath.c_str(), argv, envs.data()) == -1)
      {
        perror("execve");
        _exit(EXIT_FAILURE);
      }
    } else {
      char *argv[] = {
        (char *)_uri.c_str(),
        NULL};
      if (execve(cgiPath.c_str(), argv, envs.data()) == -1)
      {
        perror("execve");
        _exit(EXIT_FAILURE);
      }
    }
  }
  close(pipeFd[1]);
  readFd = pipeFd[0];
  if (waitpid(pid, &status, 0) == -1)
  {
    perror("waitpid");
    return (-1);
  }
  return (0);
}


//POSTのCGIは、requestのbodyをCGIのstdinに書き込む
/// この関数
int cgiExecPost(int &readFd, pid_t &pid, const std::vector<char*>& envs, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string _uri, std::string body) {
  int pipeIn[2]; // parent -> child
  int pipeOut[2]; // child -> parent
  
  int status;
  
  
  (void)_uri;
  (void)_uri_old;
  
  if (pipe(pipeIn) == -1)
  {
    std::cout << "pipe error" << std::endl;
    return (-1);
  }
  
  if (pipe(pipeOut) == -1)
  {
    close(pipeIn[0]);
    close(pipeIn[1]);
    std::cout << "pipe error" << std::endl;
    return (-1);
  }
  std::cout << "pipe done" << std::endl;
  
  pid = fork();
  if (pid == -1)
  {
    close(pipeIn[0]);
    close(pipeIn[1]);
    close(pipeOut[0]);
    close(pipeOut[1]);
    return (-1);
  }
  
  if (pid == 0)
    std::cout << "fork done (child)" << std::endl;
  else
    std::cout << "fork done (parent)" << std::endl;
  
  // child
  if (pid == 0) {
    std::cout << "child start" << std::endl;
    close(pipeIn[1]);
    if (dup2(pipeIn[0], STDIN_FILENO) == -1)
    {
      perror("dup2");
      _exit(EXIT_FAILURE);
    }
    close(pipeIn[0]);
    
    std::cout << "child dup2 pipeIn done" << std::endl;
    
    //std::cout << "pipeOut[0]: " << pipeOut[0] << std::endl;
    //std::cout << _uri << std::endl;
    
    close(pipeOut[0]);
    if (dup2(pipeOut[1], STDOUT_FILENO) == -1)
    {
      perror("dup2");
      _exit(EXIT_FAILURE);
    }
    close(pipeOut[1]);
    
    std::cerr << "child dup2 pipeOut done" << std::endl;
    
    if (cgiExtension == ".py") {
      std::cerr << "child python3" << std::endl;
      char* argv[] = {
        const_cast<char*>("python3"),
        const_cast<char*>(_uri.c_str()),
        NULL};
      if (execve(cgiPath.c_str(), argv, envs.data()) == -1)
      {
        perror("execve");
        _exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "child else" << std::endl;
      char *argv[] = {
        (char *)_uri.c_str(),
        NULL};
      if (execve(cgiPath.c_str(), argv, envs.data()) == -1)
      {
        perror("execve");
        _exit(EXIT_FAILURE);
      }
    }
  }
  else // parent
  {
    std::cout << "parent start" << std::endl;
    close(pipeIn[0]);
    
    //sleep(1);
    std::cout << body.length() << std::endl;
    // body.size() == 1048415 <- 異常

    body = body.substr(0, body.find_last_not_of('\0') + 1);
    
    write(pipeIn[1], body.data(), body.length());
    //write(stdout, body.c_str(), body.length());
    //write(1, body.c_str(), body.length());
    
    std::cout << "write done" << std::endl;
    close(pipeIn[1]);
    std::cout << "send EOF" << std::endl;
    
    close(pipeOut[1]);
    readFd = pipeOut[0];
    if (waitpid(pid, &status, 0) == -1)
    {
      perror("waitpid");
      return (-1);
    }
  }
  std::cout << "end" << std::endl;
  return (0);
}


std::string HttpResponse::_doCgi(const std::string& method, std::string _uri, const ConfigServer& config, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string version, std::string _body) {
  std::string body;
  
  std::vector<std::string> envs = createEnvs(config, _uri, method, cgiPath, cgiExtension, _uri_old, version);
  
  std::vector<char*> env_cstrs;
  for (size_t i = 0; i < envs.size(); ++i) {
      env_cstrs.push_back(const_cast<char*>(envs[i].c_str()));
  }
  env_cstrs.push_back(NULL); // NULL 終端を追加
  
  std::cout << "env done" << std::endl;
  
  int readFd;
  //int writeFd;
  pid_t pid;
  

  if (!method.compare("GET")) {
    std::cout << "cgi start" << std::endl; 
    if (cgiExecGet(readFd, pid, env_cstrs, cgiPath, cgiExtension, _uri_old, _uri) < 0) {
      if (pid != 0 && kill(pid, 0) == 0) {
        if (kill(pid, SIGTERM) == -1)
        {
          std::cout << "kill error" << std::endl;
          throw HttpResponse::HttpResponseError("kill");
        }
      }
      this->_response.clear();
      this->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    }
  } else if (!method.compare("POST")) {
    if (cgiExecPost(readFd, pid, env_cstrs, cgiPath, cgiExtension, _uri_old, _uri, _body) < 0) {
    std::cout << "cgi start" << std::endl;
      if (pid != 0 && kill(pid, 0) == 0) {
        if (kill(pid, SIGTERM) == -1)
        {
          std::cout << "kill error" << std::endl;
          throw HttpResponse::HttpResponseError("kill");
        }
      }
      this->_response.clear();
      this->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    }
    
  }
  if (readFd != -1) {
    std::cout << "start read ===" << std::endl;
    char buf[1024];
    int len;
    while ((len = read(readFd, buf, 1024)) > 0) {
      body.append(buf, len);
    }
    close(readFd);
  }

  // std::cout << "body: " << body << std::endl;
  return (body);
}


//int HttpResponse::_judgeCgiCase(std::string body) {
//  if (body.find("Location: ") != std::string::npos) {
//    this->setStatus(HTTP_MOVED_TEMPORARILY);
//    if (body.find("Content-Type: text/html") != std::string::npos)
//      return (3);
//    return (2);
//  }
//  if (body.find("Content-Type: text/html") != std::string::npos)
//  {
//    this->setStatus(HTTP_OK);
//    return (0);
//  }
//  this->setStatus(HTTP_OK);
//  return (1);
//}

std::string makeCgiHeader(std::string str) {
  std::string header;
  //std::string status;
  std::string location;
  std::string contentType;
  std::string contentLength;

  contentType = str.substr(str.find("Content-Type: ") + 14, str.find("\n", str.find("Content-Type: ")) - str.find("Content-Type: ") - 14);
  contentType = "Content-Type: " + contentType;

  //contentTypeの部分を削除
  str = str.substr(str.find("\n", str.find("Content-Type: ")) + 1);
  //先頭の改行を削除
  str = str.substr(1);

  // contentType = "Content-Type: text/html";
  contentLength = "Content-Length: " + mylib::nbrToS(str.length());
  
  
  header = contentType + CRLF + contentLength;
  if (location.length() > 0)
    header = "Location: " + location + CRLF + header;
  return (header);
}

//std::string makeCgiBody(std::string str, int cgiCase) {
//  std::string body;

//  str = str.substr(str.find("\n\n") + 2);

//  if (cgiCase == 0) {
//    body = str;
//  } else if (cgiCase == 1) {
//    body = str;
//  } else if (cgiCase == 2) {
//    body = str;
//  } else if (cgiCase == 3) {
//    body = str.substr(str.find("\n\n") + 2);
//  }
//  return (body);
//}

int HttpResponse::createCgiMessage(const std::string& method, std::string _uri, const ConfigServer& config, std::string version, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string _body) {
  int responseSize;
  std::string body;
  std::string header;
  std::string tmp;

  tmp = this->_doCgi(method, _uri, config, cgiPath, cgiExtension, _uri_old, version, _body);


  
  //int cgiCase;
  // 0: Document Response <= これのみ実装
  // 1: Local Redirect Response
  // 2: Client Redirect Response
  // 3: Client Ridirect Response with Document
  
  //Only Document Response
  //cgiCase = this->_judgeCgiCase(tmp); // ないぶで判定して、ステータスを変更する　
  header = makeCgiHeader(tmp);
  body = tmp.substr(tmp.find("\n\n") + 2);

  // this->setStatus(HTTP_NOT_FOUND);
  // std::cout << "body: " << body << std::endl;
  this->_createStatusLine(version);
  // this->_createHeaderLine(config, body.length());
  // std::cout << this->_response << std::endl;
  this->_response.append(header);
  this->_response.append(CRLF);
  this->_response.append(CRLF);
  this->_response.append(body);
  // std::cout << "response: " << this->_response << std::endl;
  responseSize = this->_response.length();
  return (responseSize);
}
