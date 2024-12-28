/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmiyazaw <kmiyazaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/28 11:41:35 by kmiyazaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Http.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "Error.hpp"

#ifdef DEBUG
static void showRequestMessage(Http& http) {
  std::cout << NORMA_COLOR << "request message" << COLOR_RESET << std::endl;
  http.showRequestLine();
  http.showRequestHeaders();
  std::cout << "\r\n";
  http.showRequestBody();
  return ;
}
#endif

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
        http.sendResponse(cSocket);
        throw Http::HttpError("HTTP_BAD_REQUEST");
        break ;
      }
      http.parseRequestMessage(cSocket);
      #ifdef DEBUG
        showRequestMessage(http);
      #endif
      if (config.client_max_body_size != 0 && http.getRequestBodySize() > config.client_max_body_size) {
        http.setHttpResponse(HTTP_REQUEST_ENTITY_TOO_LARGE);
        throw Http::HttpError("HTTP_REQUEST_ENTITY_TOO_LARGE");
      }
      cSocket._outBuf.clear();
      if (!http.createMethod())
        throw Http::HttpError("HTTP_BAD_REQUEST");
      http.executeMethod(config);
      #ifdef DEBUG
        showResponseMessage(http);
      #endif
      http.sendResponse(cSocket);
    }
    catch(const std::exception& e) {
      std::string error(e.what());

      std::cout << ERROR_COLOR << error << COLOR_RESET << std::endl;
      if (!error.compare("recv"))
        break ;
      http.createResponseMessage(config);
      showResponseMessage(http);
      http.sendResponse(cSocket);
    }
  }
  return ;
}
