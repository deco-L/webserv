/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/02 15:18:17 by csakamot         ###   ########.fr       */
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

  int countFileSize(const std::string& path) {
    int fileSize;
    std::ifstream file(path.c_str(), std::ios::binary);
    
    if (!file) {
        std::cerr << "Failed to open the file." << std::endl;
        return (-1);
    }

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.close();
    return (fileSize);
}

  bool readFile(const std::string path, std::string& buf) {
    std::ifstream file(path.c_str());

    if (!file.is_open())
      return (false);

    std::string line;
    while (std::getline(file, line)) {
      buf.append(line);
      buf.append("\n");
    }
    file.close();
    return (true);
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

  bool isFileOpen(int fd) {
    int flags = fcntl(fd, F_GETFD);
    return (flags != -1);
  }
}
