/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/27 18:00:41 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Http.hpp"
#include "Socket.hpp"
#include "Error.hpp"

void httpServer(Socket& cSocket, int& epollfd, struct epoll_event& ev, struct epoll_event *events) {
  Http http;

  ev.events = EPOLLIN;
  ev.data.fd = cSocket._socket;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, cSocket._socket, &ev);
  while (true) {
    try {
      int wait;

      wait = epoll_wait(epollfd, events, FD_SETSIZE, cSocket._timeOut * 1000);
      if (wait < 0) {
        std::cerr << ERROR_COLOR << "epoll_wait error" << COLOR_RESET << std::endl;
        cSocket.close();
        break ;
      }
      http.recvRequestMessage(cSocket);
      if (cSocket._error == 0)
        break ;
      http.parseRequestMessage(cSocket);
      std::cout << "-----request line-----" << std::endl;
      http.showRequestLine();
      std::cout << "-----headers-----" << std::endl;
      http.showHttpHeaders();
      std::cout << "----------" << std::endl;
      cSocket._outBuf = std::string(MAX_SOCK_BUFFER, 0);
    }
    catch(const std::exception& e) {
      std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
      break ;
    }
  }
  return ;
}
