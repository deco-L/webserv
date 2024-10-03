/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 16:57:22 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Http.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "Error.hpp"

void httpServer(Socket& cSocket, Epoll& epoll) {
  Http http;

  epoll.setEvent(cSocket, EPOLLIN | EPOLLET);
  while (true) {
    try {
      epoll.epollWait(cSocket._timeOut * 1000);
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
      std::cout << NORMA_COLOR << "request message" << COLOR_RESET << std::endl;
      http.showRequestLine();
      http.showHttpHeaders();
      std::memset((void* )cSocket._outBuf.c_str(), 0, cSocket._outBuf.length());
      if (!http.createMethod()) {
        http.sendResponse(cSocket, http.getVersion());
        break ;
      }
      http.executeMethod();
      std::cout << NORMA_COLOR << "response message" << COLOR_RESET << std::endl;
      http.showResponseMessage();
      http.sendResponse(cSocket, http.getVersion());
    }
    catch(const std::exception& e) {
      std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
      break ;
    }
  }
  return ;
}
