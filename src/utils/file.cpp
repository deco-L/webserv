/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/30 13:31:41 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

namespace mylib {
  bool ifFdValid(int fd) {
    return (fcntl(fd, F_GETFD) != -1 || errno != EBADF);
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