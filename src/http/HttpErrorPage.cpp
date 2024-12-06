/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpErrorPage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/06 15:34:53 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

static std::string errorPageTemplate(const std::string& error) {
  std::string body;

  body.append(
    "<!DOCTYPE html>\n"
    "<html>\n"
    "  <head>\n"
    "    <link rel=\"icon\" href=\"data:,\" />\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>"
  );
  body.append(error);
  body.append(
    "</title>\n"
    "  </head>\n"
    "  <body cz-shortcut-listen=\"true\">\n"
    "    <center>\n"
    "      <h1>"
  );
  body.append(error);
  body.append(
    "</h1>\n"
    "    </center>\n"
    "    <hr>\n"
    "    <center>webserv/1.0</center>\n"
    "  </body>\n"
    "</html>\n"
  );
  return (body);
}

std::string wsvErrorPage(int status) {
  switch (status) {
  case HTTP_BAD_REQUEST:
    return (errorPageTemplate("400 Bad Request"));
  case HTTP_UNAUTHORIZED:
    return (errorPageTemplate("401 Unauthorized"));
  case HTTP_PAYMENT_REQUIRED:
    return (errorPageTemplate("402 Payment Required"));
  case HTTP_FORBIDDEN:
    return (errorPageTemplate("403 Forbidden"));
  case HTTP_NOT_FOUND:
    return (errorPageTemplate("404 Not Found"));
  case HTTP_METHOD_NOT_ALLOWED:
    return (errorPageTemplate("405 Method Not Allowed"));
  case HTTP_NOT_ACCEPTABLE:
    return (errorPageTemplate("406 Not Acceptable"));
  case HTTP_PROXY_AUTHENTICATION_REQUIRED:
    return (errorPageTemplate("407 Proxy Authentication Required"));
  case HTTP_REQUEST_TIME_OUT:
    return (errorPageTemplate("408 Request Time Out"));
  case HTTP_CONFLICT:
    return (errorPageTemplate("409 Conflict"));
  case HTTP_GONE:
    return (errorPageTemplate("410 Gone"));
  case HTTP_LENGTH_REQUIRED:
    return (errorPageTemplate("411 Length Required"));
  case HTTP_PRECONDETION_FAILED:
    return (errorPageTemplate("412 Precondention Failed"));
  case HTTP_REQUEST_ENTITY_TOO_LARGE:
    return (errorPageTemplate("413 Request Entity Too Large"));
  case HTTP_REQUEST_URI_TOO_LARGE:
    return (errorPageTemplate("414 Request Uri Too Large"));
  case HTTP_UNSUPPORTED_MEDIA_TYPE:
    return (errorPageTemplate("415 Unsupported Media Type"));
  case HTTP_RANGE_NOT_SATISFIABLE:
    return (errorPageTemplate("416 Range Not Satisfiable"));
  case HTTP_EXPECTATION_FAILED:
    return (errorPageTemplate("417 Expectation Failed"));
  case HTTP_MISDIRECTED_REQUEST:
    return (errorPageTemplate("421 Misdirected Request"));
  case HTTP_UNPROCESSABLE_CONTENT:
    return (errorPageTemplate("422 Unprocessable Content"));
  case HTTP_UPGRADE_REQUIRED:
    return (errorPageTemplate("426 Upgrade Required"));
  case HTTP_INTERNAL_SERVER_ERROR:
    return (errorPageTemplate("500 Internal Server Error"));
  case HTTP_NOT_IMPLEMENTED:
    return (errorPageTemplate("501 Not Implemented"));
  case HTTP_BAD_GATEWAY:
    return (errorPageTemplate("502 Bad Gateway"));
  case HTTP_SERVICE_UNAVAILABLE:
    return (errorPageTemplate("503 Service Unavailable"));
  case HTTP_GATEWAY_TIME_OUT:
    return (errorPageTemplate("504 Gateway Time Out"));
  case HTTP_VERSION_NOT_SUPPORTED:
    return (errorPageTemplate("505 Version Not Supported"));
  }
  return (errorPageTemplate("500 Internal Server Error"));
}
