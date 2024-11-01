/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/01 23:41:37 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"
#include "Error.hpp"

Socket::Socket(void) :
_sSockAddr(),
_cSockAddr(),
_sPort(0),
_cPort(0),
_addrLen(sizeof(_sSockAddr)),
_bufFlag(1),
_socket(-1),
_error(0),
_outBuf(std::string(MAX_SOCK_BUFFER, 0)),
_peerIpName(""),
_peerIp(""),
_timeOut(60) {
  return ;
}

Socket::Socket(const Socket& obj) {
  *this = obj;
  return ;
}

Socket::~Socket() {
  return ;
}

Socket::SocketError::SocketError(std::string error) : _error_message(error) {
  return ;
}

Socket::SocketError::~SocketError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
  return ;
}

const char* Socket::SocketError::what(void) const throw() {
  return (this->_error_message.c_str());
}

void Socket::create(void) {
  this->_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_socket < 0)
    throw Socket::SocketError("socket error");
  return ;
}

void Socket::passive(std::string ipAddress, short int port, bool opt) {
  int optval = 1;
  (void)ipAddress;

  if (opt) {
    this->_error = setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(optval));
    if (this->_error < 0)
      throw Socket::SocketError("setsockopt error");
  }
  this->_sPort = port;
  std::memset(&this->_sSockAddr, 0, sizeof(struct sockaddr_in));
  this->_sSockAddr.sin_family = AF_INET;
  this->_sSockAddr.sin_port = htons(static_cast<unsigned int>(this->_sPort));
  this->_sSockAddr.sin_addr.s_addr = INADDR_ANY;
  this->_error = bind(this->_socket, (const struct sockaddr *) &this->_sSockAddr, sizeof(this->_sSockAddr));
  if (this->_error < 0)
    throw Socket::SocketError("bind failed");
  this->_error = listen(this->_socket, SOMAXCONN);
  if (this->_error < 0)
    throw Socket::SocketError("listen error");
  if (mylib::nonBlocking(this->_socket) < 0)
    throw Socket::SocketError("fcntl error");
  return ;
}

void Socket::accept(Socket& cSocket) {
  mylib::bzero(&this->_cSockAddr, this->_addrLen);
  cSocket._socket = ::accept(this->_socket, (struct sockaddr *) &cSocket._cSockAddr, (unsigned int *) &cSocket._addrLen);
  if (cSocket._socket < 0) {
    this->_error = cSocket._socket;
    throw Socket::SocketError("accept error");
  }
  if (mylib::nonBlocking(cSocket._socket) < 0)
    throw Socket::SocketError("fcntl error");
  cSocket._peerIp = mylib::to_string(ntohs(cSocket._cSockAddr.sin_port));
  return ;
}

int Socket::recv(void) {
  int size;
  size = ::recv(this->_socket, (char *) this->_outBuf.c_str(), MAX_SOCK_BUFFER, 0);
  if (size < 0)
    this->_error = size;
  else if (size == MAX_SOCK_BUFFER) {
    std::string tmp;
    size = ::recv(this->_socket, (char *) tmp.c_str(), 1, 0);
    return (MAX_SOCK_BUFFER + size);
  }
  this->_outBuf.substr(0, size);
  return (size);
}

void Socket::send(std::string buf, size_t len) {
  const char* tmp = buf.c_str();
  this->_error = ::send(this->_socket, (char *)tmp, len, 0);
  if (this->_error < 0)
    throw Socket::SocketError("send error");
  return ;
}

void Socket::sendText(std::string fileName) {
  std::ifstream inFile;
  char buf[GETLINE_BUFFER];

  inFile.open(fileName.c_str());
  if (!inFile)
    throw Socket::SocketError("open error");
  this->_error = 0;
  while (true) {
    mylib::bzero(buf, GETLINE_BUFFER);
    inFile.getline(buf, GETLINE_BUFFER, '\n');
    if (inFile.gcount() == 0) {
        if (inFile.eof())
          break;
        else
          throw Socket::SocketError("getline error");
    }
    this->_error += ::send(this->_socket, buf, inFile.gcount(), 0);
    this->_error += ::send(this->_socket, CRLF, mylib::strlen(CRLF), 0);
  }
  inFile.close();
  return ;
}

void Socket::sendBinary(std::string fileName) {
  std::ifstream inBinary;
  char buf[GETLINE_BUFFER];

  inBinary.open(fileName.c_str(), std::ios::binary);
  if (!inBinary)
    throw Socket::SocketError("open error");
  while (!inBinary.eof()) {
    inBinary.read(buf, GETLINE_BUFFER);
    this->_error = ::send(this->_socket, buf, inBinary.gcount(), 0);
  }
  inBinary.close();
  return ;
}

void Socket::resolveName(void) {

  if (this->_peerIp.empty()) {
    char tmp[128];
    std::strncpy(tmp, mylib::inet_ntoa(this->_cSockAddr.sin_addr), 128);
    this->_peerIp = mylib::to_string(tmp);
  }
  this->_peerIpName = this->_peerIp;
  this->_error = 0;
  return ;
}

void Socket::close(void) {
  this->_error = -1;
  ::close(this->_socket);
  return ;
}

Socket& Socket::operator=(const Socket& obj) {
  if (this != &obj) {
    this->_sSockAddr = obj._sSockAddr;
    this->_cSockAddr = obj._cSockAddr;
    this->_sPort = obj._sPort;
    this->_cPort = obj._cPort;
    this->_addrLen = obj._addrLen;
    this->_bufFlag = obj._bufFlag;
    this->_socket = obj._socket;
    this->_error = obj._error;
    this->_outBuf = obj._outBuf;
    this->_peerIpName = obj._peerIpName;
    this->_peerIp = obj._peerIp;
    this->_timeOut = obj._timeOut;
  }
  else
  {
    std::cout << ERROR_COLOR
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << COLOR_RESET << std::endl;
  }
  return (*this);
}
