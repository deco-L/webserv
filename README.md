     ___    ___         ___        _    ___   ___        ___        __   ___   ___
    |   |  |   |   .'|=|_.'   .'|=| `. |   |=|_.'   .'|=|_.'   .'|=|  | |   | |   |
    |   |  |   | .'  |  ___ .'  | | .' `.  |      .'  |  ___ .'  | |  | |   | |   |
    |   |  |   | |   |=|_.' |   |=|'.    `.|=|`.  |   |=|_.' |   |=|.'  |   | |   |
    `.  |  |  .' |   |  ___ |   | |  |  ___  |  `.|   |  ___ |   |  |`. `.  | |  .'
      `.|/\|.'   |___|=|_.' |___|=|_.'  `._|=|___||___|=|_.' |___|  |_|   `.|=|.'


# Webserv

## Overview
A HTTP server in C++ 98

## Usage
```
sudo docker build --no-cache -t 42tokyo ./
docker run -v [full-path to current-directory]:/usr/src/app --name webserv -it 42tokyo
```

inside the container
```
make
./webserv [configuration file]
```

if rewrite the `Dockerfile`
```
sudo docker stop webserv
sudo docker rm webserv
```

## Rule
1. Use C/C++.
1. Use Makefile.
1. No leaks.
1. No clash.
1. The program should be compiled with `-Wall -Wextra -Werror`.
1. The program should be compiled with `-std=c++98`.
1. Any external library and Boost libraries are forbidden.
1. Allowed functions are below...
```
Everything in C++ 98.
execve, dup, dup2, pipe, strerror, gai_strerror,
errno, dup, dup2, fork, socketpair, htons, htonl,
ntohs, ntohl, select, poll, epoll (epoll_create,
epoll_ctl, epoll_wait), kqueue (kqueue, kevent),
socket, accept, listen, send, recv, chdir bind,
connect, getaddrinfo, freeaddrinfo, setsockopt,
getsockname, getprotobyname, fcntl, close, read,
write, waitpid, kill, signal, access, stat, open,
opendir, readdir and closedir.
```

# technology used
![cplusplus](https://img.shields.io/badge/-Cplusplus-F34B7D.svg?logo=cplusplus&style=for-the-badge)
![docker](https://img.shields.io/badge/-Docker-384D54.svg?logo=docker&style=for-the-badge)