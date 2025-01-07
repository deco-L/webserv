/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/07 21:20:02 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define NO_EOL 1
#define REUSE_PORT 1

class Socket {
protected:
  struct sockaddr_in  _sSockAddr;
  struct sockaddr_in  _cSockAddr;
  short int _sPort;
  short int _cPort;
  int _addrLen;
  int _bufFlag;

public:
  int _socket;
  ssize_t _error;
  std::string _outBuf;
  std::string _peerIpName;
  std::string _peerIp;
  unsigned long _timeOut;

  Socket(void);
  Socket(const Socket& obj);
  ~Socket();

  class SocketError : public std::exception {
  private:
    std::string _error_message;
  
  public:
    SocketError(std::string error);
    virtual ~SocketError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
    virtual const char* what(void) const throw();
  };

  bool isSocketOpen(void);
  void create(void);
  void passive(short int port, bool opt);
  void accept(Socket& cSocket);
  ssize_t recv(void);
  void send(std::string buf, size_t len);
  void sendText(std::string fileName);
  void sendBinary(std::string fileName);
  void resolveName(void);
  void close(void);
  
  struct sockaddr_in getCSockAddr(void) const;

  Socket& operator=(const Socket& obj);
};

#endif
