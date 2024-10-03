/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 16:02:42 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <iostream>
#include <fstream>

#define HTTP_CONTINUE                       100
#define HTTP_SWITCHING_PROTOCOlS            101

#define HTTP_OK                             200
#define HTTP_CREATED                        201
#define HTTP_ACCEPTED                       202
#define HTTP_NON_AUTHORITATIVE_INFO         203
#define HTTP_NO_CONTENT                     204
#define HTTP_RESET_CONTENT                  205
#define HTTP_PARTIAL_CONTENT                206

#define HTTP_SPECIAL_RESPONSE               300
#define HTTP_MOVED_PERMANENTLY              301
#define HTTP_MOVED_TEMPORARILY              302
#define HTTP_SEE_OTHER                      303
#define HTTP_NOT_MODIFIED                   304
#define HTTP_USE_PROXY                      305
#define HTTP_TEMPORARY_REDIRECT             307
#define HTTP_PERMANENT_REDIRECT             308

#define HTTP_BAD_REQUEST                    400
#define HTTP_UNAUTHORIZED                   401
#define HTTP_PAYMENT_REQUIRED               402
#define HTTP_FORBIDDEN                      403
#define HTTP_NOT_FOUND                      404
#define HTTP_NOT_ALLOWED                    405
#define HTTP_NOT_ACCEPTABLE                 406
#define HTTP_PROXY_AUTHENTICATION_REQUIRED  407
#define HTTP_REQUEST_TIME_OUT               408
#define HTTP_CONFLICT                       409
#define HTTP_GONE                           410
#define HTTP_LENGTH_REQUIRED                411
#define HTTP_PRECONDETION_FAILED            412
#define HTTP_REQUEST_ENTITY_TOO_LARGE       413
#define HTTP_REQUEST_URI_TOO_LARGE          414
#define HTTP_UNSUPPORTED_MEDIA_TYPE         415
#define HTTP_RANGE_NOT_SATISFIABLE          416
#define HTTP_EXPECTATION_FAILED             417
#define HTTP_MISDIRECTED_REQUEST            421
#define HTTP_UNPROCESSABLE_CONTENT          422
#define HTTP_UPGRADE_REQUIRED               426

#define HTTP_INTERNAL_SERVER_ERROR          500
#define HTTP_NOT_IMPLEMENTED                501
#define HTTP_BAD_GATEWAY                    502
#define HTTP_SERVIICE_UNAVAILABLE           503
#define HTTP_GATEWAY_TIME_OUT               504
#define HTTP_VERSION_NOT_SUPPORTED          505

class Socket;
class HttpHeader;
class AHttpMethod;

class HttpResponse {
private:
  unsigned int _status;
  std::string _response;

  HttpResponse(void);

  int _createStatusLine(std::string version);
  int _createHeaderLine(HttpHeader& header);

public:
  HttpResponse(unsigned int status);
  HttpResponse(const HttpResponse& obj);
  ~HttpResponse();

  unsigned int getStatus(void) const;
  const std::string& getResponse(void) const;
  void setStatus(unsigned int status);
  int createResponseMessage(std::string path, HttpHeader& header, std::string version);
  void execute(Socket& socket, HttpHeader& header, std::string version);

  HttpResponse& operator=(const HttpResponse& obj);
};

#endif
