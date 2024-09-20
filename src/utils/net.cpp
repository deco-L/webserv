/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   net.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/20 17:07:04 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static __thread char buffer[18];

namespace mylib {
  char* inet_ntoa(struct in_addr in) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&in);
    std::ostringstream oss;
    
    oss << static_cast<int>(bytes[0]) << '.'
      << static_cast<int>(bytes[1]) << '.'
      << static_cast<int>(bytes[2]) << '.'
      << static_cast<int>(bytes[3]);

    std::string ip_str = oss.str();

    std::strncpy(buffer, ip_str.c_str(), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    return buffer;
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