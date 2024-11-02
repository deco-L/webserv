/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/02 15:11:23 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"
#include "webserv.hpp"
#include "Socket.hpp"

Epoll::Epoll(void): _epollFd(0), _wait(0), _ev(), _events() {
  return ;
}

Epoll::Epoll(const Epoll& obj) {
  *this = obj;
  return ;
}

Epoll::~Epoll() {
  if (mylib::ifFdValid(this->_epollFd))
    close(this->_epollFd);
  return ;
}

Epoll::EpollError::EpollError(std::string error) : _error_message(error) {
  return ;
}

Epoll::EpollError::~EpollError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
  return ;
}

const char* Epoll::EpollError::what(void) const throw() {
  return (this->_error_message.c_str());
}

int Epoll::getEpollFd(void) const {
  return (this->_epollFd);
}

int Epoll::getWait(void) const {
  return (this->_wait);
}

const struct epoll_event Epoll::getEv(void) const {
  return (this->_ev);
}

const struct epoll_event* Epoll::getEvents(void) const {
  return (this->_events);
}

void Epoll::epollCreate(void) {
  this->_epollFd = epoll_create(FD_SETSIZE);
  if (this->_epollFd < 0)
    throw Epoll::EpollError("Error: epoll_create error.");
  return ;
}

void Epoll::setEvent(const Socket& socket, unsigned int flag) {
  struct epoll_event event;
  event.events = flag;
  event.data.fd = socket._socket;
  if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, socket._socket, &event) == -1) {
    std::cout << strerror(errno) << std::endl;
    throw Epoll::EpollError("Error: epoll_ctl error.");
  }
  return ;
}

void Epoll::epollWait(int time) {
  this->_wait = epoll_wait(this->_epollFd, this->_events, FD_SETSIZE, time);
  if (this->_wait < 0)
    throw Epoll::EpollError("Error: epoll_wait error");
  return ;
}

void Epoll::epollCrose(void) {
  if (mylib::ifFdValid(this->_epollFd))
    close(this->_epollFd);
  return ;
}

Epoll& Epoll::operator=(const Epoll& obj) {
  if (this != &obj) {
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return (*this);
}