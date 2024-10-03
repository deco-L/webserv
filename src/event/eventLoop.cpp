/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 16:54:28 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "Error.hpp"

void eventLoop(Socket& sSocket) {
  Socket cSocket;
  Epoll epoll;

  try {
    epoll.epollCreate();
    epoll.setEvent(sSocket, EPOLLIN);
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
    sSocket.close();
    std::exit(WSV_ERROR);
  }
  while (true) {
    try {
      epoll.epollWait(-1);
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      sSocket.close();
      std::exit(WSV_ERROR);
    }
    try {
      sSocket.accept(cSocket);
      std::cout << "accept" << std::endl;
      httpServer(cSocket, epoll);
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