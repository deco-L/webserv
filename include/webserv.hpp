/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/02 18:11:08 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* c++ std library */
#include <cstdlib>
#include <ctime>
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
#define KILOBYTE 1024

#define LF    (u_char) '\n'
#define CR    (u_char) '\r'
#define CRLF  "\r\n"

#define DEFOULT_CONF "conf/default.conf"

template <typename ConfigClass, typename SocketClass>
struct t_root {
  ConfigClass config;
  std::vector<SocketClass> socket;
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
struct ConfigServer;

void configMain(Config& config, int argc, char **argv);
void socketMain(std::vector<Socket>& sockets, const std::vector<ConfigServer>& config);
void socketEnd(std::vector<Socket>& Sockets);
void eventLoop(std::vector<Socket>& sSockets, const std::vector<ConfigServer>& config);
void execEvent(Epoll& epoll, const std::vector<ConfigServer>& configs, std::vector<Socket>& sockets);
void httpServer(Socket& cSocket, const ConfigServer& config, Epoll& epoll);

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
  bool isFileOpen(int fd);
  std::string fileSizeToString(off_t size);

  void	bzero(void *s, size_t n);

  char* inet_ntoa(struct in_addr in);

  size_t	strlen(const char *str);
  std::vector<std::string> split(const std::string& s, const std::string& del);
  bool isNumeric(const std::string &str);
  short stringToShort(const std::string &str);
  int stringToInt(const std::string &str);
  unsigned long stringToULong(const std::string &str);
  size_t stringToSize(const std::string& str);
  PathType getPathType(const std::string& path);
  template <typename T>
  std::string to_string(const T& n);
  template <typename T>
  std::string nbrToS(T nbr);

  std::string formatTime(time_t rawTime);
}

#include "string.tpp"

#endif
