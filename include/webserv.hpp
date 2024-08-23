/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/23 14:11:00 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* c++ std library */
#include <cstdlib>
#include <iostream>

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
#define WSV_ERROR -1

#define LF    (u_char) '\n'
#define CR    (u_char) '\r'
#define CRLF  "\r\n"

template <typename ConfigClass>
struct t_root {
    ConfigClass config;

    t_root() : config() {}
};

class Config;

void  configMain(Config config, int argc, char** argv);

#endif
