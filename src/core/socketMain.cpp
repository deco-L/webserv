/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketMain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/13 21:14:49 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Error.hpp"

// ソケットを作成します.
void socketMain(std::vector<Socket>& sockets, const std::vector<ConfigServer>& config) {
  Socket tmpSocket;
  try {
    for (std::vector<ConfigServer>::const_iterator it = config.begin(); it != config.end(); it++) {
      tmpSocket.create();
      tmpSocket.passive(mylib::stringToShort(it->listen.begin()->second), REUSE_PORT);
      sockets.push_back(tmpSocket);
    }
  } catch (const std::exception& e) {
    for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
      it->close();
    std::cerr << ERROR_COLOR << e.what() << COLOR_RESET << std::endl;
    std::exit(WSV_ERROR);
  }
  return ;
}

// ソケットを閉じます.
void socketEnd(std::vector<Socket>& sockets) {
  for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
    it->close();
}
