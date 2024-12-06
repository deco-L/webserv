/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRedirectPage.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/06 16:09:18 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

static std::string redirectPageTemplate(const std::string& status, const std::string& uri) {
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
  body.append(status);
  body.append(
    "</title>\n"
    "  </head>\n"
    "  <body cz-shortcut-listen=\"true\">\n"
    "    <center>\n"
    "      <h1>"
  );
  body.append(status);
  body.append(
    "</h1>\n"
  );
  body.append(
    "      The document has moved\n"
    "      <a href=\""
  );
  body.append(uri);
  body.append(
    "\">here</a>\n"
    "    </center>\n"
    "    <hr>\n"
    "    <center>webserv/1.0</center>\n"
    "  </body>\n"
    "</html>\n"
  );
  return (body);
}

std::string wsvRedirectPage(int status, const std::string& uri) {
  switch(status) {
    case HTTP_SPECIAL_RESPONSE:
      return (redirectPageTemplate("300 Special Response", uri[uri.length() - 1] == '/' ? uri : uri + '/'));
    case HTTP_MOVED_PERMANENTLY:
      return (redirectPageTemplate("301 Moved Permanently", uri[uri.length() - 1] == '/' ? uri : uri + '/'));
    case HTTP_MOVED_TEMPORARILY:
      return (redirectPageTemplate("302 Moved Temporarily", uri[uri.length() - 1] == '/' ? uri : uri + '/'));
    case HTTP_SEE_OTHER:
      return (redirectPageTemplate("303 See Other", uri[uri.length() - 1] == '/' ? uri : uri + '/'));
    case HTTP_NOT_MODIFIED:
      return (redirectPageTemplate("304 Not Modified", uri[uri.length() - 1] == '/' ? uri : uri + '/'));
    case HTTP_USE_PROXY:
      return (redirectPageTemplate("305 Use Proxy", uri[uri.length() - 1] == '/' ? uri : uri + '/'));
    case HTTP_TEMPORARY_REDIRECT:
      return (redirectPageTemplate("306 Temporary Redirect", uri[uri.length() - 1] == '/' ? uri : uri + '/'));
    case HTTP_PERMANENT_REDIRECT:
      return (redirectPageTemplate("307 Permanent Redirect", uri[uri.length() - 1] == '/' ? uri : uri + '/'));
  }
  return ("");
}
