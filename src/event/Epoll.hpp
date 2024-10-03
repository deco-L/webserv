/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 13:48:45 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <iostream>
#include <sys/epoll.h>

class Socket;

class Epoll {
private:
  int _epollFd;
  int _wait;
  struct epoll_event _ev;
  struct epoll_event _events[FD_SETSIZE];

  Epoll(const Epoll& ovj);
  Epoll& operator=(const Epoll& obj);

public:
  Epoll(void);
  ~Epoll();

  class EpollError : public std::exception {
  private:
    std::string _error_message;
  
  public:
    EpollError(std::string error);
    virtual ~EpollError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
    virtual const char* what(void) const throw();
  };


  int getEpollFd(void) const;
  int getWait(void) const;
  const struct epoll_event getEv(void) const;
  const struct epoll_event* getEvents(void) const;
  void epollCreate(void);
  void setEvent(const Socket& socket, unsigned int flag);
  void epollWait(int time);
};

#endif
