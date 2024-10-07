/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/07 13:48:55 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Error.hpp"

Config::Config(void): _argc(0), _file_path(std::string(DEFOULT_CONF)) {}

Config::Config(int argc, std::string file_path)
: _argc(argc),
  _file_path(file_path) {}

Config::~Config() {}

int Config::getArgc(void) const {
  return (this->_argc);
}

std::string Config::getFileName(void) const {
  return (this->_file_path);
}

std::vector<ConfigServer> Config::getServers(void) const {
  return (this->_servers);
}

void Config::checkNbrArg(void) const {
  if (this->_argc != 2)
    return (
      outputError("Error: Invalid number of arguments."),
      std::exit(EXIT_FAILURE)
      );
  return ;
}

Config& Config::operator=(const Config& obj) {
  if (this != &obj) {
    this->_argc = obj.getArgc();
    this->_file_path = obj.getFileName();
    this->_servers = obj.getServers();
  }
  else
  {
    std::cout << ERROR_COLOR << "Error: "
              << "Attempted self-assignment in copy assignment operator."
              << COLOR_RESET << std::endl;
  }
  return (*this);
}
