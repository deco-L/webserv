/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/02/02 11:29:04 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "Socket.hpp"
#include "Event.hpp"
#include "CgiEvent.hpp"
#include "webserv.hpp"
#include "Epoll.hpp"
#include "Error.hpp"
#include "Http.hpp"
#include "MIME.hpp"

static void _initHeaderList(headerList& list) {
  list.server = std::make_pair("Server", "webserv/1.0");
  list.date = std::make_pair("Date", "");
  list.contentType = std::make_pair("Content-Type", "");
  list.contentLength = std::make_pair("Content-Length", "");
  list.lastModified = std::make_pair("Last-Modified", "");
  list.allow = std::make_pair("Allow", "");
  list.connection = std::make_pair("Connection", "close");
  list.acceptRanges = std::make_pair("Accept-Ranges", "bytes");
  return ;
}

HttpResponse::HttpResponse(void): _status(0), _returnFlag(false), _location(), _redirectPath(""), _response("") {
  _initHeaderList(this->_responseHeader);
  return ;
}

HttpResponse::HttpResponse(unsigned int status): _status(status), _returnFlag(false), _location(), _redirectPath(""), _response("") {
  _initHeaderList(this->_responseHeader);
  return ;
}

HttpResponse::HttpResponse(unsigned int status, ConfigLocation location): _status(status), _returnFlag(false), _location(location), _redirectPath(""), _response("") {
  _initHeaderList(this->_responseHeader);
  return ;
}

HttpResponse::HttpResponse(unsigned int status, std::string redirectPath, ConfigLocation location): _status(status), _returnFlag(false), _location(location), _redirectPath(redirectPath), _response("") {
  _initHeaderList(this->_responseHeader);
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
  switch(this->_status) {
    case HTTP_CONTINUE:
      this->_response.append(" Continue");
      break ;
    case HTTP_SWITCHING_PROTOCOlS:
      this->_response.append(" Switching Protocols");
      break ;
    case HTTP_OK:
      this->_response.append(" OK");
      break ;
    case HTTP_CREATED:
      this->_response.append(" Created");
      break ;
    case HTTP_ACCEPTED:
      this->_response.append(" Accepted");
      break ;
    case HTTP_NON_AUTHORITATIVE_INFO:
      this->_response.append(" Non Authoritative Info");
      break ;
    case HTTP_NO_CONTENT:
      this->_response.append(" No Content");
      break ;
    case HTTP_RESET_CONTENT:
      this->_response.append(" Reset Content");
      break ;
    case HTTP_PARTIAL_CONTENT:
      this->_response.append(" Partial Content");
      break ;
    case HTTP_SPECIAL_RESPONSE:
      this->_response.append(" Special Response");
      break ;
    case HTTP_MOVED_PERMANENTLY:
      this->_response.append(" Moved Permanently");
      break ;
    case HTTP_MOVED_TEMPORARILY:
      this->_response.append(" Moved Temporarily");
      break ;
    case HTTP_SEE_OTHER:
      this->_response.append(" See Other");
      break ;
    case HTTP_NOT_MODIFIED:
      this->_response.append(" Not Modified");
      break ;
    case HTTP_USE_PROXY:
      this->_response.append(" Use Proxy");
      break ;
    case HTTP_TEMPORARY_REDIRECT:
      this->_response.append(" Temporary Redirect");
      break ;
    case HTTP_PERMANENT_REDIRECT:
      this->_response.append(" Permanent Redirect");
      break ;
    case HTTP_BAD_REQUEST:
      this->_response.append(" Bad Request");
      break ;
    case HTTP_UNAUTHORIZED:
      this->_response.append(" Unauthorized");
      break ;
    case HTTP_PAYMENT_REQUIRED:
      this->_response.append(" Payment Requeired");
      break ;
    case HTTP_FORBIDDEN:
      this->_response.append(" Forbidden");
      break ;
    case HTTP_NOT_FOUND:
      this->_response.append(" Not Found");
      break ;
    case HTTP_METHOD_NOT_ALLOWED:
      this->_response.append(" Method Not Allowed");
      break ;
    case HTTP_NOT_ACCEPTABLE:
      this->_response.append(" Not Acceptable");
      break ;
    case HTTP_PROXY_AUTHENTICATION_REQUIRED:
      this->_response.append(" Proxy Authentication Required");
      break ;
    case HTTP_REQUEST_TIME_OUT:
      this->_response.append(" Request Time Out");
      break ;
    case HTTP_CONFLICT:
      this->_response.append(" Conflict");
      break ;
    case HTTP_GONE:
      this->_response.append(" Gone");
      break ;
    case HTTP_LENGTH_REQUIRED:
      this->_response.append(" Length Required");
      break ;
    case HTTP_PRECONDETION_FAILED:
      this->_response.append(" Precondetion Failed");
      break ;
    case HTTP_REQUEST_ENTITY_TOO_LARGE:
      this->_response.append(" Request Entity Too Large");
      break ;
    case HTTP_REQUEST_URI_TOO_LARGE:
      this->_response.append(" Request Uri Too Large");
      break ;
    case HTTP_UNSUPPORTED_MEDIA_TYPE:
      this->_response.append(" Unsupported Media Type");
      break ;
    case HTTP_RANGE_NOT_SATISFIABLE:
      this->_response.append(" Range Not Satisfiable");
      break ;
    case HTTP_EXPECTATION_FAILED:
      this->_response.append(" Expectation Failed");
      break ;
    case HTTP_MISDIRECTED_REQUEST:
      this->_response.append(" Misdirected Request");
      break ;
    case HTTP_UNPROCESSABLE_CONTENT:
      this->_response.append(" Unprocessable Content");
      break ;
    case HTTP_UPGRADE_REQUIRED:
      this->_response.append(" Upgrade Required");
      break ;
    case HTTP_INTERNAL_SERVER_ERROR:
      this->_response.append(" Internal Server Error");
      break ;
    case HTTP_NOT_IMPLEMENTED:
      this->_response.append(" Not Implemented");
      break ;
    case HTTP_BAD_GATEWAY:
      this->_response.append(" Bad Gateway");
      break ;
    case HTTP_SERVICE_UNAVAILABLE:
      this->_response.append(" Service Unavailable");
      break ;
    case HTTP_GATEWAY_TIME_OUT:
      this->_response.append(" Gateway Time Out");
      break ;
    case HTTP_VERSION_NOT_SUPPORTED:
      this->_response.append(" Version Not Supported");
      break ;
  }
  this->_response.append(CRLF);
  return (this->_response.length());
}

int HttpResponse::_createHeaderLine(const ConfigServer& config, int bodySize) {
  (void) config;
  int size = 0;

  this->_responseHeader.date.second = mylib::getCurrentTime();
  this->_responseHeader.contentLength.second = mylib::nbrToS(bodySize);
  for (size_t i = 0; i < this->_location.methods.size(); i++) {
    if (i == 0)
      this->_responseHeader.allow.second = this->_location.methods[i];
    else
      this->_responseHeader.allow.second += ", " + this->_location.methods[i];
  }
  this->_responseHeader.connection.second = "close";
  if (!this->_responseHeader.server.second.empty()) {
    this->_response.append(this->_responseHeader.server.first);
    this->_response.append(": ");
    this->_response.append(this->_responseHeader.server.second);
    this->_response.append(CRLF);
  }
  if (!this->_responseHeader.date.second.empty()) {
    this->_response.append(this->_responseHeader.date.first);
    this->_response.append(": ");
    this->_response.append(this->_responseHeader.date.second);
    this->_response.append(CRLF);
  }
  if (!this->_responseHeader.contentType.second.empty()) {
    this->_response.append(this->_responseHeader.contentType.first);
    this->_response.append(": ");
    this->_response.append(this->_responseHeader.contentType.second);
    this->_response.append(CRLF);
  }
  if (!this->_responseHeader.contentLength.second.empty()) {
    this->_response.append(this->_responseHeader.contentLength.first);
    this->_response.append(": ");
    this->_response.append(this->_responseHeader.contentLength.second);
    this->_response.append(CRLF);
  }
  if (!this->_responseHeader.lastModified.second.empty()) {
    this->_response.append(this->_responseHeader.lastModified.first);
    this->_response.append(": ");
    this->_response.append(this->_responseHeader.lastModified.second);
    this->_response.append(CRLF);
  }
  if (!this->_responseHeader.allow.second.empty()) {
    this->_response.append(this->_responseHeader.allow.first);
    this->_response.append(": ");
    this->_response.append(this->_responseHeader.allow.second);
    this->_response.append(CRLF);
  }
  if (!this->_responseHeader.connection.second.empty()) {
    this->_response.append(this->_responseHeader.connection.first);
    this->_response.append(": ");
    this->_response.append(this->_responseHeader.connection.second);
    this->_response.append(CRLF);
  }
  if (!this->_responseHeader.acceptRanges.second.empty()) {
    this->_response.append(this->_responseHeader.acceptRanges.first);
    this->_response.append(": ");
    this->_response.append(this->_responseHeader.acceptRanges.second);
    this->_response.append(CRLF);
  }
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

bool HttpResponse::getReturnFlag(void) const {
  return (this->_returnFlag);
}

const std::string& HttpResponse::getRedirectPath(void) const {
  return (this->_redirectPath);
}

const std::string& HttpResponse::getResponse(void) const {
  return (this->_response);
}

const headerList& HttpResponse::getheader(void) const {
  return (this->_responseHeader);
}

void HttpResponse::setStatus(unsigned int status) {
  this->_status = status;
  return ;
}

void HttpResponse::setReturnFlag(bool flag) {
  this->_returnFlag = flag;
  return ;
}

void HttpResponse::setRedirectPath(const std::string& path) {
  this->_redirectPath = path;
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
  std::string path = this->_redirectPath.length() ? this->_redirectPath : uri;
  std::string redirectBody;

  if (300 < this->_status && this->_status < 400) {
    redirectBody = wsvRedirectPage(this->_status, path);
    bodySize = redirectBody.length();
    if (bodySize == 0)
      return (-1);
    this->_response.append("Location: " + path + CRLF);
    this->_createHeaderLine(config, bodySize);
    this->_response.append(CRLF);
    this->_response.append(redirectBody);
    responseSize = this->_response.length();
  } else {
    redirectBody = path;
    bodySize = redirectBody.length();
    this->_createHeaderLine(config, bodySize);
    this->_response.append(CRLF);
    this->_response.append(redirectBody);
    responseSize = this->_response.length();
  }
  return (responseSize);
}

int HttpResponse::_createErrorResponseMessage(const ConfigServer& config, const std::string& version) {
  int responseSize = 0;
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
  if (this->_returnFlag || (300 <= this->_status && this->_status < 400))
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

int HttpResponse::cgiEventProcess(int readfd, const std::string& version) {
  int len = 0;
  int responseSize = 0;
  std::string body;
  std::string tmp;
  std::string header;
  char buf[KILOBYTE];

  while ((len = read(readfd, buf, KILOBYTE)) > 0)
    tmp.append(buf, len);
  header = makeCgiHeader(tmp);
  body = tmp.substr(tmp.find("\n\n") + 2);
  this->_createStatusLine(version);
  this->_response.append(header);
  this->_response.append(CRLF);
  this->_response.append(body);
  responseSize = this->_response.length();
  return (responseSize);
}

HttpResponse& HttpResponse::operator=(const HttpResponse& obj) {
  if (this != &obj) {
    this->_status = obj.getStatus();
    this->_returnFlag = obj.getReturnFlag();
    this->_redirectPath = obj.getRedirectPath();
    this->_response = obj.getResponse();
    this->_responseHeader = obj.getheader();
  } else {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return (*this);
}

bool FindNbrInVector::operator()(const std::pair<int, std::string>& p) const {
  return (p.first == target);
}

// Authorization: Basic のデコード
std::string decodeBase64(const std::string &input)
{
    // Base64 の変換テーブルに従って値を取り出すためのマップを作成
    // ASCIIの全256文字分用意し、初期値は-1とする
    int decodeMap[256];
    std::memset(decodeMap, -1, sizeof(decodeMap));

    // Base64 で使われる64文字
    const char* base64Chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    // decodeMap に文字 -> 値 の対応をセット
    for (int i = 0; i < 64; ++i) {
        unsigned char c = (unsigned char)base64Chars[i];
        decodeMap[c] = i;
    }
    // パディング用 '=' は特別扱い（-2などにして判定できるようにしてもいい）
    // ここでは -1 のままにしておいて後で判定

    // 出力先（可変長で受けたいので、いったん std::string を用意）
    std::string output;
    output.reserve(input.size()); // 大まかなサイズ確保

    // 4文字単位で処理
    // 1ブロック = 4文字 -> 3バイト (実際はパディング'='があるかもしれない)
    size_t i = 0;
    while (i < input.size()) {
        // 4文字バッファ
        int values[4];

        // 4文字読み取る
        for (int j = 0; j < 4; ++j) {
            // インデックスが範囲外なら -1
            if (i >= input.size()) {
                values[j] = -1;
            } else {
                unsigned char c = (unsigned char)input[i++];
                // 改行や空白を飛ばす実装を入れても良い
                // ここではそのまま処理
                if (decodeMap[c] >= 0) {
                    values[j] = decodeMap[c];
                } else if (c == '=') {
                    // パディングとみなして -2 とする
                    values[j] = -2;
                } else {
                    // 不正文字（例: 改行など） -> とりあえず -1
                    values[j] = -1;
                }
            }
        }

        // パディングが無いブロックの場合
        if (values[0] < 0) break; // そもそも先頭が不正なら終わり
        if (values[1] < 0) break; // 2文字目が不正なら終わり

        // 24ビットを3バイトにまとめる
        //   1文字目(6bit) << 18  + 2文字目(6bit) << 12 + 3文字目(6bit) << 6 + 4文字目(6bit)
        int decoded = (values[0] & 0x3F) << 18
                    | (values[1] & 0x3F) << 12;

        unsigned char c1 = (decoded >> 16) & 0xFF;
        output.push_back((char)c1);

        if (values[2] >= 0) {
            decoded |= (values[2] & 0x3F) << 6;
            unsigned char c2 = (decoded >> 8) & 0xFF;
            output.push_back((char)c2);
        }
        if (values[3] >= 0) {
            decoded |= (values[3] & 0x3F);
            unsigned char c3 = decoded & 0xFF;
            output.push_back((char)c3);
        }

        // パディング(=)があればそこまでで終了
        // values[2] == -2 とか values[3] == -2 で判断
        if (values[2] == -2 || values[3] == -2) {
            break;
        }
    }

    return output;
}

// Authorization: Digest のパース
bool parseDigestAuthHeader(const std::string &authHeader,
                           std::map<std::string, std::string> &outParams)
{
    outParams.clear();

    // "Digest " で始まっているか
    const std::string prefix = "Digest ";
    if (authHeader.size() < prefix.size()) {
        return false;
    }
    if (authHeader.compare(0, prefix.size(), prefix) != 0) {
        return false;
    }

    // "Digest " の後ろ (パラメータ部) を取り出す
    std::string content = authHeader.substr(prefix.size());

    // カンマ区切りで key=value を取り出す
    std::string::size_type start = 0;
    while (start < content.size()) {
        // 次にカンマがある位置を探す
        std::string::size_type commaPos = content.find(',', start);
        std::string token;
        if (commaPos == std::string::npos) {
            // カンマが見つからなければ、末尾まで
            token = content.substr(start);
            start = content.size();
        } else {
            token = content.substr(start, commaPos - start);
            start = commaPos + 1; // カンマの次から
        }

        // 前後の空白を除去 (簡易トリム)
        while (!token.empty() && (token[0] == ' ' || token[0] == '\t')) {
            token.erase(token.begin());
        }
        while (!token.empty() &&
               (token[token.size() - 1] == ' ' || token[token.size() - 1] == '\t')) {
            token.erase(token.end() - 1);
        }

        // key=value に分割
        std::string::size_type eqPos = token.find('=');
        if (eqPos == std::string::npos) {
            // '=' が無ければ不正
            continue; // 今回は無視して進む
        }

        std::string key = token.substr(0, eqPos);
        std::string val = token.substr(eqPos + 1);

        // key の前後空白除去
        while (!key.empty() && (key[0] == ' ' || key[0] == '\t')) {
            key.erase(key.begin());
        }
        while (!key.empty() &&
               (key[key.size() - 1] == ' ' || key[key.size() - 1] == '\t')) {
            key.erase(key.end() - 1);
        }

        // val の前後空白除去
        while (!val.empty() && (val[0] == ' ' || val[0] == '\t')) {
            val.erase(val.begin());
        }
        while (!val.empty() &&
               (val[val.size() - 1] == ' ' || val[val.size() - 1] == '\t')) {
            val.erase(val.end() - 1);
        }

        // もし val が "..." で囲まれていれば外す
        if (val.size() >= 2 && val[0] == '\"' && val[val.size() - 1] == '\"') {
            val = val.substr(1, val.size() - 2);
        }

        // map に格納
        outParams[key] = val;
    }

    return true;
}

/**
 * @brief
 *  Digest認証のパラメータ文字列("xxx...")から username を抽出し、返却する。
 *
 * @param digestString  "Authorization: Digest " の後に続くパラメータ部。
 *   例:  username="Mufasa", realm="testrealm@host.com", nonce="12345", ...
 * @return std::string  抽出した username。見つからない or パース失敗なら空文字列。
 */
std::string getDigestUser(const std::string &digestString)
{
    // parseDigestAuthHeader は "Digest " を含む文字列を要求するので、
    // "Digest " をプレフィックスとして付ける
    std::string fullHeader = "Digest " + digestString;

    std::map<std::string, std::string> params;
    bool success = parseDigestAuthHeader(fullHeader, params);
    if (!success) {
        // パース失敗 -> 空文字
        return std::string();
    }

    // "username" キーが存在しなければデフォルトコンストラクタ(空文字)が返る
    return params["username"];
}

std::vector<std::string> HttpResponse::createEnvs(const ConfigServer& config, std::string _uri, std::string method, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string version, HttpRequest &request)
{
  (void)cgiExtension;
  (void)request;
  (void)version;
  (void)config;
  (void)method;
  (void)cgiPath;
  std::vector<std::string> envs;


  std::map<std::string, std::string> headers = request.getHeader();

  // AUTH_TYPE
  if (headers.find("Authorization") != headers.end())
    envs.push_back("AUTH_TYPE=" + headers["Authorization"].substr(0, headers["Authorization"].find(' ')));
  // CONTENT_LENGTH
  if (method == "POST" && headers.find("Content-Length") != headers.end())
    envs.push_back("CONTENT_LENGTH=" + headers["Content-Length"]);
  // CONTENT_TYPE
  if (method == "POST" && headers.find("Content-Type") != headers.end())
  {
    std::string tmp = headers["Content-Type"];
    envs.push_back("CONTENT_TYPE=" + tmp.substr(0, tmp.find(';')) + ';');
  }
  // GATEWAY_INTERFACE
  envs.push_back("GATEWAY_INTERFACE=CGI/1.1");
  // PATH_INFO, PATH_TRANSLATED
  if (_uri != config.root + _uri_old) // /cgi/cgi.py/usr/ -> /usr
  {
    std::string tmp = _uri_old.substr(_uri_old.find(cgiExtension) + cgiExtension.length());
    tmp = tmp.substr(0, tmp.find('?'));
    envs.push_back("PATH_INFO=" + tmp);
    envs.push_back("PATH_TRANSLATED=" + config.root + tmp);
  }
  // QUERY_STRING
  if (_uri_old.find('?') != std::string::npos)
    envs.push_back("QUERY_STRING=" + _uri_old.substr(_uri_old.find('?') + 1));

  // REMOTE_ADDR
  // requestのIPアドレス
  char* cIp = inet_ntoa(request.getIp());
  envs.push_back("REMOTE_ADDR=" + std::string(cIp));

  // REMOTE_HOST
  envs.push_back("REMOTE_HOST="); //逆引きDNSができないので空文字列
  // REMOTE_IDENT
  envs.push_back("REMOTE_IDENT="); //必要ないので空文字列
  // REMOTE_USER
  if (headers.find("Authorization") != headers.end())
  {
    std::vector<std::string> auths = mylib::split(headers["Authorization"], " ");
    if (auths.size() == 2 && auths[0] == "Basic")
    {
      std::string decoded = decodeBase64(auths[1]);
      envs.push_back("REMOTE_USER=" + decoded.substr(0, decoded.find(':')));
    }
    else if (auths.size() == 2 && auths[0] == "Digest")
    {
      std::string user = getDigestUser(auths[1]);
      envs.push_back("REMOTE_USER=" + user);
    }
    else
    {
      this->setStatus(HTTP_INTERNAL_SERVER_ERROR);
      throw Http::HttpError("HTTP_INTERNAL_SERVER_ERROR");
    }
  }
  // REQUEST_METHOD
  envs.push_back("REQUEST_METHOD=" + method);
  // SCRIPT_NAME
  std::cout << "SCRIPT_NAME=" + _uri.substr(config.root.length()) << std::endl;
  envs.push_back("SCRIPT_NAME=" + _uri.substr(config.root.length()));
  // SERVER_NAME
  // SERVER_PORT
  std::string ip = headers["Host"].substr(0, headers["Host"].find(':'));
  std::string port = headers["Host"].substr(headers["Host"].find(':') + 1);
  bool flag = false;
  for (size_t i = 0; i < config.server_name.size() && !flag; i++)
  {
    for (size_t j = 0; j < config.listen.size(); j++)
    {
      if (config.listen[j].first == ip && config.listen[j].second == port)
      {
        envs.push_back("SERVER_NAME=" + config.server_name[i]);
        envs.push_back("SERVER_PORT=" + port);
        flag = true;
        break;
      }
    }
  }
  envs.push_back("SERVER_PROTOCOL=" + version);
  envs.push_back("SERVER_SOFTWARE=webserv/1.0");
  if (headers.find("Accept") != headers.end())
    envs.push_back("HTTP_ACCEPT=" + headers["Accept"]);
  if (headers.find("Forwarded") != headers.end())
    envs.push_back("HTTP_FORWARDED=" + headers["Forwarded"]);
  if (headers.find("Referer") != headers.end())
    envs.push_back("HTTP_REFERER=" + headers["Referer"]);
  if (headers.find("User-Agent") != headers.end())
    envs.push_back("HTTP_USER_AGENT=" + headers["User-Agent"]);
  if (headers.find("X-Forwarded-For") != headers.end())
    envs.push_back("HTTP_X_FORWARDED_FOR=" + headers["X-Forwarded-For"]);
  envs.push_back("PWD=" + _uri.substr(0, _uri.find_last_of('/')));
  return (envs);
}

int cgiExecGet(const ConfigServer& config, int &readFd, pid_t &pid, const std::vector<char*>& envs, std::string cgiPath, std::string cgiExtension, std::string _uri, std::pair<class Epoll&, std::vector<Event>&>& event) {
  int pipeFd[2];
  int status;
  std::string path_chdir = _uri;

  path_chdir = path_chdir.substr(0, path_chdir.find_last_of('/'));
  if (pipe(pipeFd) == -1) {
    perror("pipe");
    return (-1);
  }
  if (mylib::nonBlocking(pipeFd[0]) == -1) {
    close(pipeFd[0]);
    close(pipeFd[1]);
    return (-1);
  }
  pid = fork();
  if (pid == -1) {
    close(pipeFd[0]);
    close(pipeFd[1]);
    return (-1);
  } else if (pid == 0) {
    close(pipeFd[0]);
    if (dup2(pipeFd[1], STDOUT_FILENO) == -1) {
      perror("dup2");
      _exit(EXIT_FAILURE);
    }
    close(pipeFd[1]);
    if (chdir(path_chdir.c_str()) == -1) {
      perror("chdir");
      _exit(EXIT_FAILURE);
    }
    if (cgiExtension == ".py") {
      char* argv[] = {
        const_cast<char*>("python3"),
        const_cast<char*>(_uri.c_str()),
        NULL};
      if (execve(cgiPath.c_str(), argv, envs.data()) == -1) {
        perror("execve");
        _exit(EXIT_FAILURE);
      }
    } else {
      char *argv[] = {
        (char *)_uri.c_str(),
        NULL};
      if (execve(cgiPath.c_str(), argv, envs.data()) == -1) {
        perror("execve");
        _exit(EXIT_FAILURE);
      }
    }
  } else {
    pid_t result = waitpid(pid, &status, WNOHANG);
    if (result == -1) {
      close(pipeFd[1]);
      return (-1);
    } else if (result == 0) {
      std::vector<int> pipeFds;

      pipeFds.push_back(pipeFd[0]);
      pipeFds.push_back(pipeFd[1]);

      CgiEvent cgiEvent(pid, pipeFds);
      Event tmp(pipeFd[0], EPOLLIN, &config, cgiEvent, readCgiHandler);

      tmp.cgiFlag = true;
      event.first.setEvent(pipeFd[0], EPOLLIN);
      event.second.push_back(tmp);
      return (1);
    }
    close(pipeFd[1]);
    readFd = pipeFd[0];
  }
  return (0);
}

int cgiExecPost(const ConfigServer& config, int &readFd, pid_t &pid, const std::vector<char*>& envs, std::string cgiPath, std::string cgiExtension, std::string _uri, std::string body, std::pair<class Epoll&, std::vector<Event>&> event) {
  (void) event;
  int pipeIn[2]; // parent -> child
  int pipeOut[2]; // child -> parent
  int status;
  std::string path_chdir = _uri;
  path_chdir = path_chdir.substr(0, path_chdir.find_last_of('/'));

  if (pipe(pipeIn) == -1) {
    perror("pipe");
    return (-1);
  }
  if (mylib::nonBlocking(pipeIn[1]) == -1) {
    close(pipeIn[0]);
    close(pipeIn[1]);
    return (-1);
  }
  if (pipe(pipeOut) == -1) {
    close(pipeIn[0]);
    close(pipeIn[1]);
    perror("pipe");
    return (-1);
  }
  if (mylib::nonBlocking(pipeOut[0]) == -1) {
    close(pipeIn[0]);
    close(pipeIn[1]);
    close(pipeOut[0]);
    close(pipeOut[1]);
    return (-1);
  }
  pid = fork();
  if (pid == -1) {
    close(pipeIn[0]);
    close(pipeIn[1]);
    close(pipeOut[0]);
    close(pipeOut[1]);
    return (-1);
  }
  else if (pid == 0) {
    close(pipeIn[1]);
    if (dup2(pipeIn[0], STDIN_FILENO) == -1) {
      perror("dup2");
      _exit(EXIT_FAILURE);
    }
    close(pipeIn[0]);
    close(pipeOut[0]);
    if (dup2(pipeOut[1], STDOUT_FILENO) == -1) {
      perror("dup2");
      _exit(EXIT_FAILURE);
    }
    close(pipeOut[1]);
    if (chdir(path_chdir.c_str()) == -1) {
      perror("chdir");
      _exit(EXIT_FAILURE);
    }
    if (cgiExtension == ".py") {
      char* argv[] = {
        const_cast<char*>("python3"),
        const_cast<char*>(_uri.c_str()),
        NULL};
      if (execve(cgiPath.c_str(), argv, envs.data()) == -1) {
        perror("execve");
        _exit(EXIT_FAILURE);
      }
    } else {
      char *argv[] = {(char *)_uri.c_str(), NULL};
      if (execve(cgiPath.c_str(), argv, envs.data()) == -1) {
        perror("execve");
        _exit(EXIT_FAILURE);
      }
    }
  } else {
    close(pipeIn[0]);
    body = body.substr(0, body.find_last_not_of('\0') + 1);

    ssize_t write_ret = write(pipeIn[1], body.data(), body.length());

    if (write_ret == -1) {
      std::vector<int>pipeInFds;
      std::vector<int>pipeOutFds;

      pipeInFds.push_back(pipeIn[0]);
      pipeInFds.push_back(pipeIn[1]);
      pipeOutFds.push_back(pipeOut[0]);
      pipeOutFds.push_back(pipeOut[1]);

      CgiEvent cgiEvent(pid, pipeInFds, pipeOutFds, body.data());
      Event tmp(pipeIn[1], EPOLLOUT, &config, cgiEvent, writeCgiHandler);

      tmp.cgiFlag = true;
      event.first.setEvent(pipeIn[1], EPOLLOUT);
      event.second.push_back(tmp);
      return (1);
    }
    close(pipeIn[1]);

    pid_t result = waitpid(pid, &status, WNOHANG);

    if (result == -1) {
      close(pipeOut[1]);
      return (-1);
    } else if (result == 0) {
      std::vector<int> pipeFds;

      pipeFds.push_back(pipeOut[0]);
      pipeFds.push_back(pipeOut[1]);

      CgiEvent cgiEvent(pid, pipeFds);
      Event tmp(pipeOut[0], EPOLLIN, &config, cgiEvent, readCgiHandler);

      tmp.cgiFlag = true;
      event.first.setEvent(pipeOut[0], EPOLLIN);
      event.second.push_back(tmp);
      return (1);
    }
    close(pipeOut[1]);
    readFd = pipeOut[0];
  }
  return (0);
}


std::string HttpResponse::_doCgi(const std::string& method, std::string _uri, const ConfigServer& config, std::string cgiPath, std::string cgiExtension, std::string _uri_old, std::string version, HttpRequest &request, std::pair<class Epoll&, std::vector<Event>&>& event) {
  std::string body;
  std::vector<std::string> envs = this->createEnvs(config, _uri, method, cgiPath, cgiExtension, _uri_old, version, request);
  std::vector<char*> env_cstrs;
  int readFd = -1;
  pid_t pid;

  for (size_t i = 0; i < envs.size(); ++i) {
      env_cstrs.push_back(const_cast<char*>(envs[i].c_str()));
  }
  env_cstrs.push_back(NULL);
  if (!method.compare("GET")) {
    int execResult = cgiExecGet(config, readFd, pid, env_cstrs, cgiPath, cgiExtension, _uri, event);

    if (execResult == 1)
      throw HttpResponse::HttpResponseError("cgi unfinished");
    if (execResult < 0) {
      if (pid != 0 && kill(pid, 0) == 0) {
        if (kill(pid, SIGTERM) == -1) {
          std::cerr << ERROR_COLOR << "kill error" << COLOR_RESET << std::endl;
          throw HttpResponse::HttpResponseError("kill");
        }
      }
      this->_response.clear();
      this->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    }
  } else if (!method.compare("POST")) {
    std::string _body = request.getBody();
    int execResult = cgiExecPost(config, readFd, pid, env_cstrs, cgiPath, cgiExtension, _uri, _body, event);

    if (execResult == 1)
      throw HttpResponse::HttpResponseError("cgi unfinished");
    if (execResult < 0) {
      if (pid != 0 && kill(pid, 0) == 0) {
        if (kill(pid, SIGTERM) == -1) {
          std::cerr << ERROR_COLOR << "kill error" << COLOR_RESET << std::endl;
          throw HttpResponse::HttpResponseError("kill");
        }
      }
      this->_response.clear();
      this->setStatus(HTTP_INTERNAL_SERVER_ERROR);
    }
  }
  if (readFd != -1) {
    char buf[1024];
    int len;
    while ((len = read(readFd, buf, 1024)) > 0)
      body.append(buf, len);
    close(readFd);
  }
  return (body);
}

std::string makeCgiHeader(std::string str) { // str: cgiの出力
  std::string header;

  std::string date;
  std::string server;
  std::string contentType;
  std::string contentLength;

  date = "Date: " + mylib::getCurrentTime();

  server = "Server: webserv/1.0";

  contentType = str.substr(str.find("Content-Type: ") + 14, str.find("\n", str.find("Content-Type: ")) - str.find("Content-Type: ") - 14);
  contentType = "Content-Type: " + contentType;
  str = str.substr(str.find("\n", str.find("Content-Type: ")) + 1);
  str = str.substr(1);
  contentLength = "Content-Length: " + mylib::nbrToS(str.length());
  header = date + CRLF + server + CRLF + contentType + CRLF + contentLength + CRLF;
  return (header);
}

int HttpResponse::createCgiMessage(const std::string& method, std::string _uri, const ConfigServer& config, std::string version, std::string cgiPath, std::string cgiExtension, std::string _uri_old, HttpRequest& request, std::pair<class Epoll&, std::vector<Event>&>& event) {
  int responseSize;
  std::string body;
  std::string header;
  std::string tmp;

  tmp = this->_doCgi(method, _uri, config, cgiPath, cgiExtension, _uri_old, version, request, event);
  header = makeCgiHeader(tmp);
  body = tmp.substr(tmp.find("\n\n") + 2);
  this->_createStatusLine(version);
  this->_response.append(header);
  this->_response.append(CRLF);
  this->_response.append(body);
  responseSize = this->_response.length();
  return (responseSize);
}
