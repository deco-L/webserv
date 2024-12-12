/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmiyazaw <kmiyazaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 20:49:24 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/12/07 13:40:26 by kmiyazaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

void Config::parseConfig(void)
{
	ParseMode mode = MODE_GLOBAL;
	std::vector<std::string> lines = mylib::split(this->_file_content, "\n");

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;
		const std::vector<std::string> tokens = mylib::split(line, " ");
		std::string directive = tokens[0];

		if (directive == "server")
		{
			mode = MODE_SERVER;
			ConfigServer server;
			this->_servers.push_back(server);
		}

		if (directive == "location")
		{
			mode = MODE_LOCATION;
			ConfigLocation location;
			this->_servers.back().locations.push_back(location);

			// location path
			this->_servers.back().locations.back().path = tokens[1];
		}

		if (directive == "}")
		{
			mode = mode == MODE_LOCATION ? MODE_SERVER : MODE_GLOBAL;
			continue;
		}

		if (mode == MODE_SERVER)
		{
			// 当初はlistenの重複をを追加としていたが上書きに変更
			if (directive == "listen")
			{
				std::pair<std::string, std::string> new_pair;
				if (tokens[1].find(':') != std::string::npos) // address:port
				{
					std::vector<std::string> addressPort = mylib::split(tokens[1], ":");
					new_pair = std::make_pair(addressPort[0], addressPort[1]);
				}
				else // port
				{
					new_pair = std::make_pair("", tokens[1]);
				}

				if (this->_servers.back().listen.empty())
				{
					// ベクターが空の場合、要素を追加
					this->_servers.back().listen.push_back(new_pair);
				}
				else
				{
					// 最初の要素を上書き
					this->_servers.back().listen[0] = new_pair;
				}
			}

			else if (directive == "server_name")
			{
				for (size_t i = 1; i < tokens.size(); ++i)
				{
					this->_servers.back().server_name.push_back(tokens[i]);
				}
			}
			else if (directive == "error_page") // error_page code [code ...] path
			{
				for (size_t i = 1; i < tokens.size() - 1; ++i)
				{
					this->_servers.back().error_page.push_back(std::make_pair(mylib::stringToInt(tokens[i]), tokens[tokens.size() - 1]));
				}
			}
			else if (directive == "client_max_body_size") // size = { 0 ~ 1024GB } K, M, G
			{
				char last_char = tokens[1][tokens[1].length() - 1];

				if (last_char == 'k' || last_char == 'm' || last_char == 'g' || last_char == 'K' || last_char == 'M' || last_char == 'G')
				{
					int size_num = mylib::stringToInt(tokens[1].substr(0, tokens[1].length() - 1));
					if (last_char == 'k' || last_char == 'K')
					{
						this->_servers.back().client_max_body_size = size_num * 1024;
					}
					else if (last_char == 'm' || last_char == 'M')
					{
						this->_servers.back().client_max_body_size = size_num * 1024 * 1024;
					}
					else if (last_char == 'g' || last_char == 'G')
					{
						this->_servers.back().client_max_body_size = size_num * 1024 * 1024 * 1024;
					}
				}
				else
				{
					this->_servers.back().client_max_body_size = mylib::stringToULong(tokens[1]);
				}
			}
			else if (directive == "root")
			{
				this->_servers.back().root = tokens[1];
			}
			else if (directive == "index") // index file [file ...]
			{
				for (size_t i = 1; i < tokens.size(); ++i)
				{
					this->_servers.back().index.push_back(tokens[i]);
				}
			}
			else if (directive == "autoindex")
			{
				if (tokens[1] == "on")
					this->_servers.back().autoindex = true;
				else if (tokens[1] == "off")
					this->_servers.back().autoindex = false;
			}
		}

		if (mode == MODE_LOCATION)
		{
			if (directive == "root")
			{
				this->_servers.back().locations.back().root = tokens[1];
			}
			else if (directive == "methods")
			{
				for (size_t i = 1; i < tokens.size(); ++i)
				{
					this->_servers.back().locations.back().methods.push_back(tokens[i]);
				}
			}
			else if (directive == "return") // return code [URL]
			{
				if (tokens.size() == 2)
				{
					this->_servers.back().locations.back().return_ = std::make_pair(mylib::stringToInt(tokens[1]), "");
				}
				else if (tokens.size() == 3)
				{
					this->_servers.back().locations.back().return_ = std::make_pair(mylib::stringToInt(tokens[1]), tokens[2]);
				}
			}
			else if (directive == "autoindex")
			{
				if (tokens[1] == "on")
					this->_servers.back().locations.back().autoindex = true;
				else if (tokens[1] == "off")
					this->_servers.back().locations.back().autoindex = false;
			}
			else if (directive == "index") // index file [file ...]
			{
				for (size_t i = 1; i < tokens.size(); ++i)
				{
					this->_servers.back().locations.back().index.push_back(tokens[i]);
				}
			}
			else if (directive == "cgi_extension") // cgi_extention extension path_to_cgi_executable
			// vector<pair<string, string> > cgi_extension; // 拡張子とパス
			{
				if (this->_servers.back().locations.back().cgi_extension.empty())
					this->_servers.back().locations.back().cgi_extension.push_back(std::make_pair(tokens[1], tokens[2]));
				else
					this->_servers.back().locations.back().cgi_extension.front() = std::make_pair(tokens[1], tokens[2]);
			}
			else if (directive == "upload_enable") // upload_enable on | off
			{
				if (tokens[1] == "on")
					this->_servers.back().locations.back().upload_enable = true;
				else if (tokens[1] == "off")
					this->_servers.back().locations.back().upload_enable = false;
			}
			else if (directive == "upload_store") // upload_store path
			{
				this->_servers.back().locations.back().upload_store = tokens[1];
			}
		}
	}
	return ;
}