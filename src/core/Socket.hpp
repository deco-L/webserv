/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/08 19:48:18 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAX_SOCK_BUFFER 16384
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
  std::string _buf1, _buf2;
  int _beg1, _beg2, _end1, _end2;

public:
  int _socket;
  int _error;
  std::string _outBuf;
  std::string _ipAddressName;
  std::string _ipAddress;
  unsigned long _timeOut;

  Socket(void);
  ~Socket();

  class SocketError : public std::exception {
  private:
    std::string _error_message;
  
  public:
    SocketError(std::string error);
    virtual const char* what(void) const throw();
  };

  void create(void);
  void passive(short int port, bool opt);
  Socket  accept(void);
  int connect(std::string buf, short int port);
  int recv(int bytes);
  int recvTeol(bool remove);
  int send(std::string buf);
  int sendText(std::string fileName);
  int sendBinary(std::string fileName);
  int resolveName(void);
  int close(void);
};

#endif
