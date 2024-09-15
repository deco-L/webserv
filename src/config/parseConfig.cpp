/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:40:46 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/09/15 22:40:46 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

bool getFileContent(std::string file_path, std::string &file_content)
{
	std::ifstream		input(file_path.c_str());

	if (!input.is_open())
	{
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

//文字列strがallowed_charsに含まれる文字だけで構成されているかどうかを判定する
bool is_string_from_set(const std::string& str, const std::string& allowed_chars) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (allowed_chars.find(*it) == std::string::npos) {
            return false; // 許可されていない文字が見つかった
        }
    }
    return true; // 全ての文字が許可されたセット内
}

// 末尾から指定された文字セットに含まれる文字を削除する関数
std::string trim_trailing_chars(const std::string& str, const std::string& charSet) {
    // strの末尾から、charSetに含まれる文字が現れなくなる場所を見つける
    std::string::size_type pos = str.find_last_not_of(charSet);

    // その場所までの部分文字列を返す
    if (pos == std::string::npos) {
        return "";  // 全ての文字が削除対象だった場合、空文字列を返す
    } else {
        return str.substr(0, pos + 1);  // 必要な部分を返す
    }
}

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
		line = trim_trailing_chars(line, " \t;");
		new_file_content += line + "\n";
	}
	return new_file_content;
}

void Config::parseConfig(void)
{
  std::string file_content;

  if (getFileContent(this->_file_path, file_content) == false)
	std::exit(0);
  file_content = removeUnnecessaryLines(file_content);

  std::cout << "File content: ======================="
  << std::endl << file_content
  << "===============================" << std::endl;

  if (isValidConfig(file_content) == false)
  {
	outputError("Error: Invalid config file.");
	std::exit(0);
  }

}