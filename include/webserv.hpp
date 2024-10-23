/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/23 10:23:49 by csakamot         ###   ########.fr       */
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
#include <fstream>

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
#include <limits>

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
class Epoll;

void configMain(Config&& config, int argc, char **argv);
void socketMain(Socket& socketData);
void socketEnd(Socket& sSocket);
void eventLoop(Socket& sSocket);
void httpServer(Socket& cSocket, Epoll& epoll);

namespace mylib {
  int check_access(const char *path);
  void  spinnerOut(void);

  bool ifFdValid(int fd);
  bool isDirectory(std::string path);
  bool isPathValid(std::string path);
  bool isModeValid(std::string path, int mode);
  int countFileSize(const std::string& path);
  bool readFile(const std::string path, std::string& buf);
  int nonBlocking(int fd);

  void	bzero(void *s, size_t n);

  char* inet_ntoa(struct in_addr in);

  size_t	strlen(const char *str);
  std::vector<std::string> split(const std::string& s, const std::string& del);
  bool isNumeric(const std::string &str);
  int stringToInt(const std::string &str);
  unsigned long stringToULong(const std::string &str);
  PathType getPathType(const std::string& path);
  template <typename T>
  std::string to_string(const T& n);
  template <typename T>
  std::string nbrToS(T nbr);
}

#include "string.tpp"

#endif
