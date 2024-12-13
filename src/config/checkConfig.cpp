/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:40:46 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/12/13 14:40:57 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

/* ====================
ファイルの内容を取得する
==================== */
// MARK: getFileContent

bool getFileContent(std::string file_path, std::string &file_content)
{
	std::ifstream	input(file_path.c_str());

	if (!input.is_open())
	{
		outputError("Error: Cannot open the configuration file.");
		return false;
	}
	std::string	line;
	file_content = "";
	while (std::getline(input, line))
	{
		file_content += line + "\n";
	}
	input.close();
	return true;
}

bool is_string_from_set(const std::string& str, const std::string& allowed_chars)
{
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (allowed_chars.find(*it) == std::string::npos) {
			return false;
		}
	}
	return true;
}

std::string trim_head_chars(const std::string& str, const std::string& charSet)
{
	std::string::size_type pos = str.find_first_not_of(charSet);

	if (pos == std::string::npos) {
		return "";
	} else {
		return str.substr(pos);
	}
}

std::string trim_tail_chars(const std::string& str, const std::string& charSet)
{
	std::string::size_type pos = str.find_last_not_of(charSet);

	if (pos == std::string::npos) {
		return "";
	} else {
		return str.substr(0, pos + 1);
	}
}

/* ====================
コメント行や空行を削除する関数 行末の空白文字やセミコロンも削除する
==================== */
// MARK: removeUnnecessaryLines

std::string removeUnnecessaryLines(std::string file_content)
{
	std::string		new_file_content;
	std::string		line;
	std::istringstream	iss(file_content);

	while (std::getline(iss, line))
	{
		if (line.find("#") != std::string::npos)
		{
			line = line.substr(0, line.find("#"));
		}
		if (line.size() == 0)
			continue;
		if (is_string_from_set(line, " \t"))
			continue;
		line = trim_head_chars(line, " \t");
		line = trim_tail_chars(line, " \t;");
		new_file_content += line + "\n";
	}
	return new_file_content;
}

/* ====================
設定ファイルが正しいかどうかを確認する
==================== */
// MARK: CheckConfig

void Config::checkConfig(void)
{
	std::string file_content;

	if (getFileContent(this->_file_path, file_content) == false)
		std::exit(0);
	
	file_content = removeUnnecessaryLines(file_content);
	this->_file_content = file_content;

	if (DEBUG)
	{
		std::cout << "File content: ======================="
		<< std::endl << file_content
		<< "===============================" << std::endl;
	}

	if (isValidConfig(file_content) == false)
		std::exit(0);
}
