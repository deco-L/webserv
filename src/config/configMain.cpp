/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configMain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/07 13:50:37 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"

void  configMain(Config config, int argc, char**argv)
{
  std::string file_path((argc >= 2) ? std::string(argv[1]) : std::string(DEFOULT_CONF));
  config = Config(argc, file_path);
  config.checkNbrArg(); // check the number of arguments
  config.checkConfig(); // check the configuration file
  //config.parseConfig(); // parse the configuration file
}
