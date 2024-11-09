/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/09 14:49:52 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Http.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "Error.hpp"

static void showRequestMessage(Http& http) {
  std::cout << NORMA_COLOR << "request message" << COLOR_RESET << std::endl;
  http.showRequestLine();
  http.showRequestHeaders();
  std::cout << "\r\n";
  http.showRequestBody();
  return ;
}

static void showResponseMessage(Http& http) {
  std::cout << NORMA_COLOR << "response message" << COLOR_RESET << std::endl;
  http.showResponseMessage();
  return ;
}

void httpServer(Socket& cSocket, const ConfigServer& config, Epoll& epoll) {
  while (true) {
    Http http;

    try {
      epoll.epollWait(-1);
      http.recvRequestMessage(cSocket);
      if (cSocket._error == 0)
        break ;
      if (http.checkSemantics(cSocket)) {
        http.sendResponse(cSocket, http.getVersion());
        throw Http::HttpError("HTTP_BAD_REQUEST");
        break ;
      }
      http.parseRequestMessage(cSocket);
      if (config.client_max_body_size != 0 && http.getRequestBodySize() > config.client_max_body_size) {
        http.setHttpResponse(HTTP_REQUEST_ENTITY_TOO_LARGE);
        throw Http::HttpError("HTTP_REQUEST_ENTITY_TOO_LARGE");
      }
      showRequestMessage(http);
      std::memset((void* )cSocket._outBuf.c_str(), 0, cSocket._outBuf.length());
      if (!http.createMethod())
        throw Http::HttpError("HTTP_BAD_REQUEST");
      http.executeMethod(config);
      showResponseMessage(http);
      http.sendResponse(cSocket, http.getVersion());
    }
    catch(const std::exception& e) {
      std::string error(e.what());

      std::cout << error << std::endl;
      if (error.compare("accept"))
        http.sendResponse(cSocket, http.getVersion());
      break ;
    }
  }
  return ;
}
