/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/08 20:17:28 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"

Socket::Socket(void) :
_sSockAddr(),
_cSockAddr(),
_sPort(0),
_cPort(0),
_addrLen(sizeof(_sSockAddr)),
_bufFlag(0),
_buf1(std::string(MAX_SOCK_BUFFER / 2, 0)),
_buf2(std::string(MAX_SOCK_BUFFER / 2, 0)),
_beg1(0),
_beg2(0),
_end1(0),
_end2(0),
_socket(-1),
_error(0),
_outBuf(std::string(MAX_SOCK_BUFFER, 0)),
_ipAddressName(NULL),
_ipAddress(NULL),
_timeOut(60) {
  return ;
}

Socket::~Socket() {
  if (this->_socket > 0)
    ::close(this->_socket);
  return ;
}

Socket::SocketError::SocketError(std::string error) : _error_message(error) {
  return ;
}

const char* Socket::SocketError::what(void) const throw() {
  return (this->_error_message.c_str());
}

void Socket::create(void) {
  this->_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_socket == -1)
    throw Socket::SocketError("socket error");
  return ;
}

void Socket::passive(short int port, bool opt) {
  int optval = 1;

  if (opt) {
    setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(optval));
  }

  this->_sPort = port;
  mylib::bzero((void *)&this->_socket, this->_addrLen);
  this->_sSockAddr.sin_family = AF_INET;
  this->_sSockAddr.sin_port = htons(this->_sPort);
  this->_sSockAddr.sin_addr.s_addr = INADDR_ANY;
  this->_error = bind(this->_socket, (struct sockaddr *) &this->_sSockAddr, this->_addrLen);
  if (this->_error < 0)
    throw Socket::SocketError("bind failed");
  return ;
}
