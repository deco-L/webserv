/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/27 14:16:18 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"
#include "Error.hpp"

void eventLoop(Socket& sSocket) {
  Socket cSocket;
  int epollFd, wait;
  struct epoll_event ev, events[FD_SETSIZE];

  epollFd = epoll_create(1);
  if (epollFd < 0) {
    std::cerr << ERROR_COLOR << "epoll_create error" << COLOR_RESET << std::endl;
    sSocket.close();
    std::exit(WSV_ERROR);
  }
  ev.events = EPOLLIN;
  ev.data.fd = sSocket._socket;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, sSocket._socket, &ev) == -1) {
    std::cerr << ERROR_COLOR << "epoll_ctl error" << COLOR_RESET << std::endl;
    sSocket.close();
    std::exit(WSV_ERROR);
  }
  while (true) {
    wait = epoll_wait(epollFd, events, FD_SETSIZE, -1);
    if (wait < 0) {
      std::cerr << ERROR_COLOR << "epoll_wait error" << COLOR_RESET << std::endl;
      sSocket.close();
      std::exit(WSV_ERROR);
    }
    try {
      sSocket.accept(cSocket);
      httpServer(cSocket, epollFd, ev, events);
    }
    catch(const std::exception& e) {
      std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
      cSocket.close();
      break ;
    }
    cSocket.close();
  }
  return ;
}