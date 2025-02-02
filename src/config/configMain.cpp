/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configMain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmiyazaw <kmiyazaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/02/02 15:05:28 by kmiyazaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"

void configMain(Config& config, int argc, char **argv)
{
  std::string file_path((argc >= 2) ? std::string(argv[1]) : std::string(DEFOULT_CONF));
  config = Config(argc, file_path);
  // config.checkNbrArg();
  config.checkConfig();
  config.parseConfig();
  config.checkConfigServer();

  #ifdef DEBUG
    const std::vector<ConfigServer>& servers = config.getServers();

    std::cout << "Servers: ===========================================================================" << std::endl;
    for (std::vector<ConfigServer>::const_iterator it = servers.begin(); it != servers.end(); ++it)
    {
      const ConfigServer& server = *it;
      std::cout << "Server: =======================" << std::endl;
      std::cout << "Listen: ";

      for (std::vector<std::pair<std::string, std::string> >::const_iterator it = server.listen.begin(); it != server.listen.end(); ++it)
      {
        std::cout << it->first << ":" << it->second << " ";
      }
      std::cout << std::endl;
      std::cout << "Server name: ";
      for (std::vector<std::string>::const_iterator it = server.server_name.begin(); it != server.server_name.end(); ++it)
      {
        std::cout << *it << " ";
      }
      std::cout << std::endl;
      std::cout << "Error page: ";
      for (std::vector<std::pair<int, std::string> >::const_iterator it = server.error_page.begin(); it != server.error_page.end(); ++it)
      {
        std::cout << it->first << " " << it->second << " ";
      }
      std::cout << std::endl;
      std::cout << "Client max body size: " << server.client_max_body_size << std::endl;
      std::cout << "Root: " << server.root << std::endl;
      std::cout << "Index: ";
      for (std::vector<std::string>::const_iterator it = server.index.begin(); it != server.index.end(); ++it)
      {
        std::cout << *it << " ";
      }
      std::cout << std::endl;
      std::cout << "Autoindex: " << server.autoindex << std::endl;
      std::cout << "Locations: =======================" << std::endl;
      for (std::vector<ConfigLocation>::const_iterator it = server.locations.begin(); it != server.locations.end(); ++it)
      {
        const ConfigLocation& location = *it;
        std::cout << "Location: =======================" << std::endl;
        std::cout << "Path: " << location.path << std::endl;
        std::cout << "Root: " << location.root << std::endl;
        std::cout << "Methods: ";
        for (std::vector<std::string>::const_iterator it = location.methods.begin(); it != location.methods.end(); ++it)
        {
          std::cout << *it << " ";
        }
        std::cout << std::endl;
        std::cout << "Return: " << location.return_.first << " " << location.return_.second << std::endl;

        std::cout << std::endl;
        std::cout << "Autoindex: " << location.autoindex << std::endl;
        std::cout << "Index: ";
        for (std::vector<std::string>::const_iterator it = location.index.begin(); it != location.index.end(); ++it)
        {
          std::cout << *it << " ";
        }
        std::cout << std::endl;
        std::cout << "CGI extension: ";
        for (std::vector<std::pair<std::string, std::string> >::const_iterator it = location.cgi_extension.begin(); it != location.cgi_extension.end(); ++it)
        {
          std::cout << it->first << " " << it->second << " ";
        }
        std::cout << std::endl;
        std::cout << "Upload enable: " << location.upload_enable << std::endl;
        std::cout << "Upload store: " << location.upload_store << std::endl;
      }
    }
  #endif
}