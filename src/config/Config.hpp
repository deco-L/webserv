/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/15 23:15:06 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "webserv.hpp"

struct ConfigServer {
	std::string	root;
	std::string host;
	std::string	port;
	std::string serverName;
	bool defaultServer;
	size_t limitBodySize;
};

class Config {
private:
  int         _argc;
  std::string _file_path;
  std::vector<ConfigServer> _servers;

public:
  Config(void);
  Config(int argc, std::string argv);
  ~Config();

  int         getArgc(void) const;
  std::string getFileName(void) const;
  std::vector<ConfigServer> getServers(void) const;
  void        checkNbrArg(void) const;
  void        parseConfig(void);

  Config&      operator=(const Config& obj);
};

bool isValidConfig(std::string file_path);

#endif
