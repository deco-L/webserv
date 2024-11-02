/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/02 15:48:09 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Http.hpp"
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
  (void)config;
  while (true) {
    try {
      Http http;

      epoll.epollWait(-1);
      http.recvRequestMessage(cSocket);
      if (cSocket._error == 0)
        break ;
      if (http.checkSemantics(cSocket)) {
        http.sendResponse(cSocket, http.getVersion());
        break ;
      }
      http.parseRequestMessage(cSocket);
      if (http.getRequestSize() > MAX_SOCK_BUFFER) {
        http.sendResponse(cSocket, http.getVersion());
        break ;
      }
      showRequestMessage(http);
      std::memset((void* )cSocket._outBuf.c_str(), 0, cSocket._outBuf.length());
      if (!http.createMethod()) {
        http.sendResponse(cSocket, http.getVersion());
        break ;
      }
      http.executeMethod();
      showResponseMessage(http);
      http.sendResponse(cSocket, http.getVersion());
    }
    catch(const std::exception& e) {
      if (mylib::strlen(e.what()))
        std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
      break ;
    }
  }
  return ;
}
