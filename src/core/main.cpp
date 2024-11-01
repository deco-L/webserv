/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/01 18:34:37 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Socket.hpp"

static t_root<Config, Socket> root;

int main(int argc, char **argv, char **envp)
{
  (void)envp;

  configMain(root.config, argc, argv);
  const std::vector<ConfigServer>& config = root.config.getServers();
  for (std::vector<ConfigServer>::const_iterator it = config.begin(); it != config.end(); it++) {
    for (std::vector<std::pair<std::string, std::string> >::const_iterator listen_it = it->listen.begin(); listen_it != it->listen.end(); listen_it++) {
        std::cout << "IP: " << listen_it->first << ", Port: " << listen_it->second << std::endl;
    }
  }
  socketMain(root.socket, root.config.getServers());
  eventLoop(root.socket, root.config.getServers());
  socketEnd(root.socket);
  std::cout << "finish" << std::endl;
  return (EXIT_SUCCESS);
}
