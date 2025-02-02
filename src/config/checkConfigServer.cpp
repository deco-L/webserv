/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkConfigServer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmiyazaw <kmiyazaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/02/02 15:15:12 by kmiyazaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Error.hpp"

void Config::checkConfigServer(void)
{
    const std::vector<ConfigServer>& servers = this->getServers();
    std::vector<std::pair<std::string, std::string> > all_listens;


    for (std::vector<ConfigServer>::const_iterator it = servers.begin(); it != servers.end(); ++it)
    {
        const ConfigServer& server = *it;
        std::vector<std::pair<std::string, std::string> > listens = server.listen;

        for (std::vector<std::pair<std::string, std::string> >::const_iterator it = listens.begin(); it != listens.end(); ++it)
        {
            all_listens.push_back(*it);
        }
    }
    // ip:portの重複チェック
    for (std::vector<std::pair<std::string, std::string> >::const_iterator it = all_listens.begin(); it != all_listens.end(); ++it)
    {
        std::pair<std::string, std::string> listen = *it;
        int count = 0;

        for (std::vector<std::pair<std::string, std::string> >::const_iterator it2 = all_listens.begin(); it2 != all_listens.end(); ++it2)
        {
            if (listen.first == it2->first && listen.second == it2->second)
            {
                count++;
            }
        }
        if (count > 1)
        {
            outputError("Error: Duplicate ip:port in listen directive.");
            std::exit(EXIT_FAILURE);
        }
    }
    return ;
}