/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketMain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/29 18:53:41 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"
#include "Error.hpp"

int httpServer(int sock) {
  return (WSV_OK);
}

void  socketMain(Socket socketData) {
  socketData = Socket();

  int w_addr, c_sock;
  struct sockaddr_in a_addr;

  w_addr = socket(AF_INET, SOCK_STREAM, 0);
  if (w_addr == -1) {
    std::cerr << ERROR_COLOR << "socket error." << COLOR_RESET << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::memset(&a_addr, 0, sizeof(struct sockaddr_in));
  a_addr.sin_family = AF_INET;
  a_addr.sin_port = htons((unsigned int)8080);
  a_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(w_addr, (const struct sockaddr*)&a_addr, sizeof(a_addr) == -1)) {
    std::cerr << ERROR_COLOR << "bind error." << COLOR_RESET << std::endl;
    close(w_addr);
    std::exit(EXIT_FAILURE);
  }

  if (listen(w_addr, WSV_LISTEN_BACKLOG) == -1) {
    std::cout << ERROR_COLOR << "listen error." << COLOR_RESET << std::endl;
    close(w_addr);
    std::exit(EXIT_FAILURE);
  }

  while (true) {
    std::cout << "Waiting connect..." << std::endl;
    c_sock = accept(w_addr, NULL, NULL);
    if (c_sock == -1) {
      std::cerr << ERROR_COLOR << "accept error." << COLOR_RESET << std::endl;
      close(w_addr);
      std::exit(EXIT_FAILURE);
    }
    std::cout << "Connected!" << std::endl;
    close(c_sock);
  }
  close(w_addr);
  return ;
}