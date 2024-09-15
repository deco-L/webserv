/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/12 16:56:31 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"
#include <fstream>

Socket::Socket(void) :
_sSockAddr(),
_cSockAddr(),
_sPort(0),
_cPort(0),
_addrLen(sizeof(_sSockAddr)),
_bufFlag(1),
_buf1(std::string(MAX_SOCK_BUFFER / 2, 0)),
_buf2(std::string(MAX_SOCK_BUFFER / 2, 0)),
_beg1(0),
_beg2(0),
_end1(0),
_end2(0),
_socket(-1),
_error(0),
_outBuf(std::string(MAX_SOCK_BUFFER, 0)),
_peerIpName(""),
_peerIp(""),
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

Socket::SocketError::~SocketError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
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
  this->_error = listen(this->_socket, SOMAXCONN);
  if (this->_error < 0)
    throw Socket::SocketError("listen error");
  return ;
}

void Socket::accept(Socket& cSocket) {
  mylib::bzero (&this->_cSockAddr, this->_addrLen);
  cSocket._socket = ::accept(this->_socket, (struct sockaddr *) &cSocket._cSockAddr, (unsigned int *) &cSocket._addrLen);
  if (cSocket._socket < 0) {
    this->_error = cSocket._socket;
    throw Socket::SocketError("accept error");
  }
  cSocket._peerIp = mylib::to_string(ntohs(cSocket._cSockAddr.sin_port));
  return ;
}

void Socket::recv(int bytes) {
  if ((this->_bufFlag == 1) && this->_end1 != 0) {
    if (bytes >= (this->_end1 - this->_beg1)) {
      this->_outBuf.append(this->_buf1.substr(this->_beg1), this->_end1 - this->_beg1);
      this->_error = this->_end1 - this->_beg1;
      this->_beg1 = this->_end1 = 0;
      this->_bufFlag = 2;
    } else {
      this->_outBuf.append(this->_buf1.substr(this->_beg1), bytes);
      this->_error = bytes;
      this->_beg1 += bytes;
    }
  } else if ((this->_bufFlag == 2) && (this->_end2 != 0)) {
    if (bytes >= (this->_end2 - this->_beg2)) {
      this->_outBuf.append(this->_buf2.substr(this->_beg2), this->_end2 - this->_beg2);
      this->_bufFlag = 1;
    } else {
      this->_outBuf.append(this->_buf2.substr(this->_beg2), bytes);
      this->_error = bytes;
      this->_beg1 += bytes;
    }
  }
  return ;
}

int Socket::recvTeol(bool remove) {
  int state, idx;

  state = 1;
  idx = 0;
  while (state != 0) {
    switch (state) {
      case (1): {
        if ((this->_end1 == 0) && (this->_end2 == 0))
          state = 2;
        else
          state = 3;
        break ;
      }
      case (2): {
        const char* tmp = this->_buf1.c_str();
        this->_error = ::recv(this->_socket, (char *)tmp, MAX_SOCK_BUFFER / 2, 0);
        if (this->_error == -1) {
          state = 0;
          break ;
        }
        this->_beg1 = 0;
        this->_end1 = this->_error;
        if (this->_error == MAX_SOCK_BUFFER / 2) {
          const char* tmp2 = this->_buf2.c_str();
          this->_error = ::recv(this->_socket, (char *)tmp2, MAX_SOCK_BUFFER / 2, 0);
          if (this->_error == -1) {
            state = 0;
            break ;
          }
          this->_beg2 = 0;
          this->_end2 = this->_error;
        }
        this->_bufFlag = 1;
        state = 3;
        break ;
      }
      case (3): {
        if ((this->_bufFlag == 1) && (this->_end1 != 0)) {
          for (; this->_beg1 < this->_end1; this->_beg1++) {
            this->_outBuf[idx] = this->_buf1[this->_beg1];
            if ((this->_outBuf[idx] == '\n') || (this->_outBuf[idx] == 'r')) {
              this->_beg1++;
              if ((this->_outBuf[idx] == '\r') && (this->_buf1[this->_beg1] == '\n')) {
                idx++;
                this->_outBuf[idx] = this->_buf1[this->_beg1];
                this->_beg1++;
              }
              this->_outBuf[idx + 1] = '\0';
              state = 4;
              break ;
            }
            idx++;
            if ((idx + 1) == MAX_SOCK_BUFFER) {
              this->_outBuf[MAX_SOCK_BUFFER] = '\0';
              state = 4;
              break ;
            }
          }
          if (this->_beg1 == this->_end1)
            this->_beg1 = this->_end1 = 0;
          if (state == 3)
            this->_bufFlag = 2;
          else if ((this->_bufFlag == 2) && (this->_end2 != 0)) {
            for (; this->_beg2 < this->_end2; this->_beg2++) {
              this->_outBuf[idx] = this->_buf2[this->_beg2];
              if ((this->_outBuf[idx] == '\n') || (this->_outBuf[idx] == '\r')) {
                this->_beg2++;
                if ((this->_outBuf[idx] == '\r') && (this->_buf2[this->_beg2] == '\n')) {
                  idx++;
                  this->_outBuf[idx] = this->_buf2[this->_beg2];
                  this->_beg2++;
                }
                this->_outBuf[idx + 1] = '\0';
                state = 4;
                break ;
              }
              idx++;
              if ((idx + 1) == MAX_SOCK_BUFFER) {
                this->_outBuf[MAX_SOCK_BUFFER] = '\0';
                state = 4;
                break ;
              }
            }
            if (this->_beg2 == this->_end2)
              this->_beg2 = this->_end2 = 0;
            if (state == 3)
              this->_bufFlag = 1;
          }
          else {
            if (idx < MAX_SOCK_BUFFER)
              state = 2;
            else
              state = 4;
          }
        }
        break ;
      }
      case (4): {
        state = 0;
        break ;
      }
    }
  }
  if (remove > 0) {
    while ((this->_outBuf[idx] == '\r') || (this->_outBuf[idx] == '\n')) {
      this->_outBuf[idx] = '\0';
      idx--;
    }
  }
  return (idx + 1);
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
  this->_beg1 = this->_beg2 = this->_end1 = this->_end2 = 0;
  this->_error = -1;
  ::close(this->_socket);
  return ;
}
