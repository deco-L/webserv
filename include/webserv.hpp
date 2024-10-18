/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:41 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* c++ std library */
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cstdio>
#include <fstream>
#include <stack>
#include <map>
#include <algorithm>

/* c library for communication */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* c library for data transmission */
#include <unistd.h>

/* c library for multiplexing */
#include <sys/select.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/times.h>
#include <sys/types.h>

/* c library for network utilities */
#include <sys/param.h>
#include <arpa/inet.h>

/* c library for process */
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

/* c library for directory handling */
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>

/* clibrary for file handling */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

/* c library for error handling */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define WSV_OK    0
#define WSV_ERROR 1

#define DEBUG 1

#define LF    (u_char) '\n'
#define CR    (u_char) '\r'
#define CRLF  "\r\n"

#define DEFOULT_CONF "conf/default.conf"

template <typename ConfigClass, typename SocketClass>
struct t_root {
  ConfigClass config;
  SocketClass socketData;
};

enum PathType {
  NOT_EXIST,    // パスが存在しない
  IS_FILE,      // パスがファイル
  IS_DIRECTORY, // パスがディレクトリ
  OTHER         // その他（シンボリックリンクなど）
};

class Config;
class Socket;

void configMain(Config config, int argc, char** argv);
void socketMain(Socket socketData);
void eventLoop(Socket socket);
void httpServerMain(void);

namespace mylib {
  int check_access(const char *path);
  void  spinnerOut(void);
  void	bzero(void *s, size_t n);
  size_t	strlen(const char *str);
  std::vector<std::string> split(const std::string& s, const std::string& del);
  bool isNumeric(const std::string &str);
  int stringToInt(const std::string &str);
  PathType getPathType(const std::string& path);
  template <typename T>
  std::string to_string(const T& n);
  char* inet_ntoa(struct in_addr in);
}

#include "string.tpp"

#endif
