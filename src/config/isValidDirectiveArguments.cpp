/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidDirectiveArguments.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:14:30 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/12/13 14:47:37 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

static bool hasArguments(const std::vector<std::string>& lines)
{
	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;

		if (line.find("server") != std::string::npos)
			continue;
		if (line.find("location") != std::string::npos)
		{
			std::string location = line.substr(0, line.find('{'));
			location.erase(location.find_last_not_of(" \t") + 1);

			if (location.find(' ') == std::string::npos)
				return false;
			continue;
		}
		if (line == "}")
			continue;
		if (line.find(' ') == std::string::npos)
			return false;
	}
	return true;
}

static bool hasCorrectNumberOfArguments(const std::vector<std::string>& lines)
{
    const char* directivesArray[] = {
		"listen",
        "client_max_body_size",
        "root",
        "autoindex",
        "upload_enable",
        "upload_store"
    };
    size_t arraySize = sizeof(directivesArray) / sizeof(directivesArray[0]);
    std::vector<std::string> directives1(directivesArray, directivesArray + arraySize);

	const char* directives1NArray[] = {
		"server_name",
		"index",
		"methods"
	};
	arraySize = sizeof(directives1NArray) / sizeof(directives1NArray[0]);
	std::vector<std::string> directives1N(directives1NArray, directives1NArray + arraySize);

	const char* directives2NArray[] = {
		"error_page"
	};
	arraySize = sizeof(directives2NArray) / sizeof(directives2NArray[0]);
	std::vector<std::string> directives2N(directives2NArray, directives2NArray + arraySize);

	const char* directives12Array[] = {
		"return"
	};
	arraySize = sizeof(directives12Array) / sizeof(directives12Array[0]);
	std::vector<std::string> directives12(directives12Array, directives12Array + arraySize);

	const char* directives2Array[] = {
		"cgi_extension"
	};
	arraySize = sizeof(directives2Array) / sizeof(directives2Array[0]);
	std::vector<std::string> directives2(directives2Array, directives2Array + arraySize);

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;
		const std::vector<std::string> tokens = mylib::split(line, " ");
		std::string directive = tokens[0];

		if (directive == "location")
		{
			if (tokens[tokens.size() - 1] == "{" && tokens.size() == 3)
				continue;
			else
				return false;
		}

		if (directive == "}")
			continue;

		if (std::find(directives1.begin(), directives1.end(), directive) != directives1.end() && tokens.size() != 2)
			return false;
		if (std::find(directives1N.begin(), directives1N.end(), directive) != directives1N.end() && tokens.size() < 2)
			return false;
		if (std::find(directives2N.begin(), directives2N.end(), directive) != directives2N.end() && tokens.size() < 3)
			return false;
		if (std::find(directives12.begin(), directives12.end(), directive) != directives12.end() && (tokens.size() != 2 && tokens.size() != 3))
			return false;
		if (std::find(directives2.begin(), directives2.end(), directive) != directives2.end() && tokens.size() != 3)
			return false;
	}
	return true;
}

// only IPv4
static bool isValidIpAddress(const std::string& ip)
{
	std::istringstream ss(ip);
	std::string token;
	int count = 0;

	while (std::getline(ss, token, '.'))
	{
		if (!mylib::isNumeric(token))
			return false;
		int num = mylib::stringToInt(token);
		if (num < 0 || num > 255 || (token.length() > 1 && token[0] == '0'))
			return false;
		++count;
	}
	return count == 4;
}

static bool isValidPort(int port)
{
	return port > 0 && port <= 65535;
}

static bool hasValidArguments(const std::vector<std::string>& lines)
{
	std::vector<int> ports;
	std::vector<std::string> domains;
	std::vector<int> error_code;
	std::vector<std::string> location_path;
	std::vector<std::vector<std::string> > cgi_extensions;

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;
		const std::vector<std::string> tokens = mylib::split(line, " ");
		std::string directive = tokens[0];

		if (directive == "server")
		{
			ports.clear();
			domains.clear();
			error_code.clear();
			location_path.clear();
			cgi_extensions.clear();
		}

		if (directive == "}")
			continue;

		if (directive == "listen") //[address:]port
		{
			if (tokens[1].find(':') != std::string::npos)
			{
				std::vector<std::string> addressPort = mylib::split(tokens[1], ":");
				if (!isValidIpAddress(addressPort[0]) || !mylib::isNumeric(addressPort[1]) || !isValidPort(mylib::stringToInt(addressPort[1])))
					return false;
				if (std::find(ports.begin(), ports.end(), mylib::stringToInt(addressPort[1])) != ports.end())
					return false;
				ports.push_back(mylib::stringToInt(addressPort[1]));
			}
			else
			{
				if (!mylib::isNumeric(tokens[1]) || !isValidPort(mylib::stringToInt(tokens[1])))
					return false;
				if (std::find(ports.begin(), ports.end(), mylib::stringToInt(tokens[1])) != ports.end())
					return false;
				ports.push_back(mylib::stringToInt(tokens[1]));
			}
		}
		
		if (directive == "server_name") // name [name ...]
		{
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				if (tokens[i].empty())
					return false;
				if (tokens[i].length() > 253)
					return false;
				if (tokens[i].find("..") != std::string::npos)
					return false;
				std::vector<std::string> labels = mylib::split(tokens[i], ".");
				for (size_t j = 0; j < labels.size(); ++j)
				{
					if (labels[j].empty())
						return false;
					if (labels[j].length() > 63)
						return false;
					if (labels[j].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-") != std::string::npos)
						return false;
					if (labels[j][0] == '-' || labels[j][labels[j].length() - 1] == '-')
						return false;
				}
				if (domains.size() > 0 && std::find(domains.begin(), domains.end(), tokens[i]) != domains.end())
					return false;
				domains.push_back(tokens[i]);
			}
		}
		
		if (directive == "error_page")
		{
			for (size_t i = 1; i < tokens.size() - 1; ++i)
			{
				if (!mylib::isNumeric(tokens[i]) || tokens[i].length() != 3)
					return false;
				int code = mylib::stringToInt(tokens[i]);
				if (code < 100 || code > 599)
					return false;
				if (std::find(error_code.begin(), error_code.end(), code) != error_code.end())
					return false;
				error_code.push_back(code);
			}
		}

		if (directive == "client_max_body_size")
		{
			if (tokens[1][tokens[1].length() - 1] == 'k' || tokens[1][tokens[1].length() - 1] == 'm' || tokens[1][tokens[1].length() - 1] == 'g' ||
				tokens[1][tokens[1].length() - 1] == 'K' || tokens[1][tokens[1].length() - 1] == 'M' || tokens[1][tokens[1].length() - 1] == 'G')
			{
				if (!mylib::isNumeric(tokens[1].substr(0, tokens[1].length() - 1)))
					return false;
			}
			else
			{
				if (!mylib::isNumeric(tokens[1]))
					return false;
			}
		}

		if (directive == "index")
		{
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				if (tokens[i].empty())
					return false;
				if (tokens[i].length() > 255)
					return false;
				if (tokens[i].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_") != std::string::npos)
					return false;
			}
		}

		if (directive == "autoindex")
		{
			if (tokens[1] != "on" && tokens[1] != "off"
				&& tokens[1] != "ON" && tokens[1] != "OFF")
				return false;
		}

		if (directive == "location")
		{
			if (std::find(location_path.begin(), location_path.end(), tokens[1]) != location_path.end())
				return false;
			location_path.push_back(tokens[1]);
		}

		if (directive == "methods")
		{
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				if (tokens[i] != "GET" && tokens[i] != "POST" && tokens[i] != "DELETE")
					return false;
				if (std::find(tokens.begin() + 1, tokens.begin() + i, tokens[i]) != tokens.begin() + i)
					return false;
			}
		}

		if (directive == "return")
		{
			if (!mylib::isNumeric(tokens[1]) || tokens[1].length() != 3)
				return false;
			int code = mylib::stringToInt(tokens[1]);
			if (code < 100 || code > 599)
				return false;
		}
		
		if (directive == "cgi_extension") // cgi_extention extension path_to_cgi_executable
		{
			if (cgi_extensions.size() > 0)
			{
				for (size_t i = 0; i < cgi_extensions.size(); ++i)
				{
					if (cgi_extensions[i][0] == tokens[1] || cgi_extensions[i][1] == tokens[2])
						return false;
				}
			}
			std::vector<std::string> extension_args;
			extension_args.push_back(tokens[1]);
			extension_args.push_back(tokens[2]);
			cgi_extensions.push_back(extension_args);
		}

		if (directive == "upload_enable") // upload_enable on | off
		{
			if (tokens[1] != "on" && tokens[1] != "off"
				&& tokens[1] != "ON" && tokens[1] != "OFF")
				return false;
		}
	}
	return true;
}

static std::string getRoot(std::vector<std::string>::const_iterator& it, const std::vector<std::string>& lines)
{
    std::stack<char> braceStack;
    std::string root("");
    std::vector<std::string>::const_iterator it2 = it;

    bool inFirstBraceBlock = false;

    while (it2 != lines.end())
    {
        const std::string& line = *it2;
        ++it2;

        if (line.find('{') != std::string::npos) {
            braceStack.push('{');
            if (braceStack.size() == 1)
                inFirstBraceBlock = true;
        }
        else if (line.find('}') != std::string::npos) {
            braceStack.pop();
            if (braceStack.empty())
                break;
        }

        if (inFirstBraceBlock && braceStack.size() == 1 && line.find("root") != std::string::npos)
        {
            std::vector<std::string> tokens = mylib::split(line, " ");

            if (tokens.size() > 1) {
                root = tokens[1];
            }
        }
    }

    return root;
}

static bool hasValidPaths(const std::vector<std::string>& lines)
{

	ParseMode mode = MODE_GLOBAL;
	std::string server_root_path("");
	std::string location_root_path("");
	std::vector<std::string> location_paths;

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;
		const std::vector<std::string> tokens = mylib::split(line, " ");
		std::string directive = tokens[0];

		if (directive == "server")
		{
			mode = MODE_SERVER;
			server_root_path = "";
			server_root_path = getRoot(it, lines);
		}

		if (directive == "location")
		{
			mode = MODE_LOCATION;
			if (tokens[1][0] != '/')
				return false;

			location_root_path = getRoot(it, lines);
			if (location_root_path.empty())
				location_root_path = server_root_path;
			std::string location_path(location_root_path + tokens[1]);

			if (location_path[location_path.length() - 1] == '/')
				location_path = location_path.substr(0, location_path.length() - 1);

			if (std::find(location_paths.begin(), location_paths.end(), location_path) != location_paths.end())
				return false;
			location_paths.push_back(location_path);

			if (mylib::getPathType(location_path) != IS_DIRECTORY)
				return false;

			if (access(location_path.c_str(), W_OK) != 0)
				return false;
		}

		if (directive == "}")
		{
			if (mode == MODE_SERVER)
			{
				mode = MODE_GLOBAL;
				server_root_path = "";
				location_paths.clear();
			}
			else if (mode == MODE_LOCATION)
			{
				mode = MODE_SERVER;
				location_root_path = "";
			}
		}

		// error_pageのpath
		if (directive == "error_page")
		{
			if (tokens[tokens.size() - 1][0] != '/')
				return false;

			std::string error_page_path(server_root_path + tokens[tokens.size() - 1]);

			if (mylib::getPathType(error_page_path) != IS_FILE)
				return false;
			if (access(error_page_path.c_str(), R_OK) != 0)
				return false;
		}

		if (directive == "root")
		{
			if (tokens[1][0] != '/')
				return false;
			if (mylib::getPathType(tokens[1]) != IS_DIRECTORY)
				return false;
			if (access(tokens[1].c_str(), W_OK) != 0)
				return false;
		}

		if (directive == "cgi_extension")
		{
			if (tokens[1] != ".cgi" && tokens[1] != ".py" && tokens[1] != ".sh")
				return false;

			std::string path_to_cgi_executable(tokens[2]);
			if (mylib::getPathType(path_to_cgi_executable) != IS_FILE)
				return false;
			if (access(path_to_cgi_executable.c_str(), X_OK) != 0)
				return false;
		}

		if (directive == "upload_store")
		{
			if (tokens[1][0] != '/')
				return false;

			std::string upload_store_path(location_root_path + tokens[1]);

			if (mylib::getPathType(upload_store_path) != IS_DIRECTORY)
				return false;
			if (access(upload_store_path.c_str(), X_OK) != 0)
				return false;
		}
	}
	return true;
}

/* ====================
ディレクティブの引数が有効かどうかを確認する関数
==================== */
// MARK: isValidDirectiveArguments

bool isValidDirectiveArguments(const std::vector<std::string>& lines)
{
	if (hasArguments(lines) == false)
	{
		outputError("ERROR: Invalid number of arguments!");
		return false;
	}
	if (hasCorrectNumberOfArguments(lines) == false)
	{
		outputError("ERROR: Invalid number of arguments!");
		return false;
	}
	if (hasValidArguments(lines) == false)
	{
		outputError("ERROR: Invalid argument value!");
		return false;
	}
	if (hasValidPaths(lines) == false)
	{
		outputError("ERROR: File or Directory is Invalid!");
		return false;
	}
	return true;
}
