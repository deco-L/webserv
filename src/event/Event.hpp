/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/26 19:19:42 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdlib>
#include <vector>
#include <sys/epoll.h>
#include "Socket.hpp"

class Epoll;
class Http;
struct ConfigServer;
struct CgiEvent;

struct Event {
  int fd;
  int event;
  bool cgiFlag;
  const ConfigServer* config;
  Socket socket;
  Http *http;
  CgiEvent* cgiEvent;
  void (*socketFunc)(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config);
  void (*cgiFunc)(Epoll& epoll, std::vector<Event>& events, Event& event);

  Event(void): fd(0), event(0), cgiFlag(false), config(NULL), socket(), cgiEvent(NULL), socketFunc(NULL), cgiFunc(NULL) {};
  Event(
    int fd,
    int event,
    const ConfigServer* config,
    Socket socket,
    void (*socketFunc)(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config)
  ): fd(fd), event(event), cgiFlag(false), config(config), socket(socket), cgiEvent(NULL), socketFunc(socketFunc), cgiFunc(NULL) {};
  Event(
    int fd,
    int event,
    CgiEvent* cgiEvent,
    void (*cgiFunc)(Epoll& epoll, std::vector<Event>& events, Event& event)
  ): fd(fd), event(event), cgiFlag(false), config(NULL), socket(), cgiEvent(cgiEvent), socketFunc(NULL), cgiFunc(cgiFunc) {};
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
void readCgiHandler(Epoll& epoll, std::vector<Event>& events, Event& event);
void writeHandler(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config);

#endif