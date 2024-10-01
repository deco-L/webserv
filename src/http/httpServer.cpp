/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/01 21:21:46 by csakamot         ###   ########.fr       */
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
      int wait;

      epoll.epollWait(cSocket._timeOut * 1000);
      http.recvRequestMessage(cSocket);
      if (cSocket._error == 0)
        break ;
      if (http.getRequestSize() > MAX_SOCK_BUFFER) {
        http.sendResponse(cSocket);
        break ;
      }
      http.parseRequestMessage(cSocket);
      std::memset((void* )cSocket._outBuf.c_str(), 0, cSocket._outBuf.length());
      if (!http.createMethod()) {
        http.sendResponse(cSocket);
        break ;
      }
      http.executeMethod(cSocket);
      http.sendResponse(cSocket);
    }
    catch(const std::exception& e) {
      std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
      break ;
    }
  }
  return ;
}
