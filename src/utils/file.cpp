/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 15:27:29 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

namespace mylib {
  bool ifFdValid(int fd) {
    return (fcntl(fd, F_GETFD) != -1 || errno != EBADF);
  }

  bool isDirectory(std::string path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
      return (false);

    return ((info.st_mode & S_IFDIR) == 0);
  }

  bool isPathValid(std::string path) {
    struct stat info;

    return (stat(path.c_str(), &info) != 0);
  }

  bool isModeValid(std::string path, int mode) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
      return (false);

    if (info.st_mode & mode)
      return (true);
    return (false);
  }

  int readFile(std::string path, std::string& buf) {
    int size;
    std::ifstream file(path.c_str());

    if (!file.is_open())
      return (-1);

    std::string line;
    while (std::getline(file, line)) {
      buf.append(line);
      buf.append("\n");
      size += line.length();
    }
    file.close();
    return (size);
  }

  int nonBlocking(int fd) {
    int flag;

    flag = fcntl(fd, F_GETFL, 0);
    if (flag < 0)
      return (-1);
    if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) < 0)
      return (-1);
    return (0);
  }
}
