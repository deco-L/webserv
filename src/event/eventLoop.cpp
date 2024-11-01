/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/01 22:11:41 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "Error.hpp"

void eventLoop(std::vector<Socket>& sockets, const std::vector<ConfigServer>& configs) {
  Socket cSocket;
  Epoll epoll;

  try {
    epoll.epollCreate();
    for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
      epoll.setEvent(*it, EPOLLIN);
  }
  catch(const std::exception& e) {
    std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
    for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
      it->close();
    std::exit(WSV_ERROR);
  }
  while (true) {
    try {
      epoll.epollWait(-1);
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
        it->close();
      std::exit(WSV_ERROR);
    }
    for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++) {
      try {
        it->accept(cSocket);
        std::cout << "accept" << std::endl;
      }
      catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        continue ;
      }
      try {
        epoll.setEvent(cSocket, EPOLLIN | EPOLLET);
        httpServer(cSocket, configs[it - sockets.begin()], epoll);
      }
      catch(const std::exception& e) {
        std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
        cSocket.close();
        break ;
      }
    }
    cSocket.close();
  }
  return ;
}