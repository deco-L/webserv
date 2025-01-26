/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/26 16:02:19 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdlib>

class Epoll;
class Socket;
struct ConfigServer;
struct CgiEvent;

struct Event {
  int fd;
  int event;
  const ConfigServer* config;
  Socket socket;
  CgiEvent* cgiEvent;
  void (*socketFunc)(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config);
  void (*cgiFunc)(Epoll& epoll, std::vector<Event>& events, CgiEvent& cgiEvent);

  Event(void): fd(0), event(0), config(NULL), socket(), cgiEvent(NULL), socketFunc(NULL), cgiFunc(NULL) {};
  Event(
    int fd,
    int event,
    const ConfigServer* config,
    Socket socket,
    void (*func)(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config)
  ): fd(fd), event(event), config(config), socket(socket), cgiEvent(NULL), socketFunc(func), cgiFunc(NULL) {};
  Event(
    int fd,
    int event,
    CgiEvent* cgiEvent,
    void (*func)(Epoll& epoll, std::vector<Event>& events, CgiEvent& cgiEvent)
  ): fd(fd), event(event), config(NULL), socket(), cgiEvent(cgiEvent), socketFunc(NULL), cgiFunc(func) {};
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
void readCgiHandler(Epoll& epoll, std::vector<Event>& events, CgiEvent& cgi);
void writeHandler(Epoll& epoll, std::vector<Event>& events, Socket& socket, const ConfigServer& config);

#endif