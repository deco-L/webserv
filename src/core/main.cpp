/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/16 13:15:12 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Socket.hpp"

static t_root<Config, Socket> root;

int main(int argc, char **argv)
{
  configMain(root.config, argc, argv);
  socketMain(root.socket, root.config.getServers());
  eventLoop(root.socket, root.config.getServers());
  socketEnd(root.socket);
  std::cout << "finish" << std::endl;
  return (EXIT_SUCCESS);
}
