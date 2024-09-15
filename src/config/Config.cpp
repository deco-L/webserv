/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/29 18:53:10 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Error.hpp"

Config::Config(void): _argc(0), _file_path("") {}

Config::Config(int argc, char** argv)
: _argc(argc),
  _file_path((argc >= 2) ? std::string(argv[1]) : std::string("")) {}

Config::~Config() {}

int Config::getArgc(void) const {
  return (this->_argc);
}

std::string Config::getFileName(void) const {
  return (this->_file_path);
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
  }
  else
  {
    std::cout << "\e[1;31mError: "
              << "Attempted self-assignment in copy assignment operator.\e[0m"
              << std::endl;
  }
  return (*this);
}
