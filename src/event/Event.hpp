/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/26 01:26:00 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdlib>
#include <vector>

class Epoll;
class Socket;
struct ConfigServer;

struct Event {
  int fd;
  int event;
  const ConfigServer* config;
  Socket socket;
  void (*func)(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config);

  Event(void): fd(0), event(0), config(NULL), socket(), func(NULL) {}; 
  Event(
    int fd,
    int event,
    const ConfigServer* config,
    Socket socket,
    void (*func)(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config)
  ): fd(fd), event(event), config(config), socket(socket), func(func) {};
};

struct FindByFd {
    int target_fd;

    FindByFd(int fd) : target_fd(fd) {}

    bool operator()(const Event& event) const {
        return event.fd == target_fd;
    }
};


void execEvent(Epoll& epoll, const epoll_event& event, std::vector<Event>& events);
void connectHandler(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config);
void readHandler(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config);
void writeHandler(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config);

#endif