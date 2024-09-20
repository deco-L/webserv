/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/20 18:46:34 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Http.hpp"
#include "Socket.hpp"
#include "Error.hpp"

void httpServer(Socket& cSocket) {
  Http http;

  while (true) {
    try {
      http.recvRequestMessage(cSocket);
      // http.parseRequestMessage(cSocket);
      // std::cout << "Method: " << http.getMethod() << std::endl;
      // std::cout << "uri: " << http.getUri() << std::endl;
      // std::cout << "version" << http.getVersion() << std::endl;
    }
    catch(const std::exception& e) {
      std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << '\n';
      break ;
    }
  }
  return ;
}
