/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/04 15:49:28 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAX_SOCK_BUFFER 16384
#define NO_EOL 1
#define REUSE_PORT 1

class Socket {
protected:
  struct sockaddr_in  siUs;
  struct sockaddr_in  siThem;
  short int sPortUs;
  short int sPortThem;
  int iLen;
  int iBuf;
  std::string szBuf1, szBuf2;
  int iBeg1, iEnd1, iBeg2, iEnd2;

public:
  int iSock;
  int iErr;
  std::string szOutBuf;
  std::string szPeerName;
  std::string szPeerIp;
  unsigned long ulTimeout;

  Socket(void);
  ~Socket();

  int create(void);
  int passive(short int port, int opt);
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
