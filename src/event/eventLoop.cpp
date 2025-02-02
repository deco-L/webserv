/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/02/02 14:01:55 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include "Error.hpp"


void eventLoop(std::vector<Socket>& sockets, const std::vector<ConfigServer>& configs) {
  Epoll epoll;
  std::vector<Event> events;
  int time = TIMEOUT;

  try {
    epoll.epollCreate();
    for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++) {
      Event tmp(it->_socket, EPOLLIN, &configs[it - sockets.begin()], *it, connectHandler);

      epoll.setEvent(it->_socket, EPOLLIN);
      events.push_back(tmp);
    }
  }
  catch(const std::exception& e) {
    std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
    for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
      it->close();
    std::exit(WSV_ERROR);
  }
  while (true) {
    try {
      while (true) {
      epoll.epollWait(time);
      for (int i = 0; i < epoll.getWait(); i++)
        execEvent(epoll, events);
      }
    }
    catch(const std::exception& e) {
      std::string error = e.what();

      if (error == "Error: epoll_wait timeout." && events.size() == sockets.size())
        continue ;
      else if (error == "Error: epoll_wait timeout." && events.size() != sockets.size()) {
        for (std::vector<Event>::iterator it = events.begin(); it != events.end(); it++)
          it->timeoutFlag = true;
        execEvent(epoll, events);
      } else if (error != "Error: epoll_wait timeout.")
        std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
    }
  }
  epoll.epollCrose();
  return ;
}
