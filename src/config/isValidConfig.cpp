/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:47:46 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/10/18 20:47:35 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

/* ====================
未知のディレクティブがないことを確認
==================== */
// MARK: isValidDirective

static const std::string serverDirectivesArray[] = {
    "listen",
    "server_name",
    "error_page",
    "client_max_body_size",
    "root",
    "index",
    "autoindex",
    "location"
};

static const std::vector<std::string> serverDirectives(
    serverDirectivesArray,
    serverDirectivesArray + sizeof(serverDirectivesArray) / sizeof(serverDirectivesArray[0])
);

// ロケーションブロック内のディレクティブ
static const std::string locationDirectivesArray[] = {
    "root",
    "methods",
    "return",
    "autoindex",
    "index",
    "cgi_extension",
    "upload_enable",
    "upload_store"
};

static const std::vector<std::string> locationDirectives(
    locationDirectivesArray,
    locationDirectivesArray + sizeof(locationDirectivesArray) / sizeof(locationDirectivesArray[0])
);

static bool isValidDirective(const std::vector<std::string>& lines)
{
	ParseMode mode = MODE_GLOBAL;

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;

		if (line.find("server") != std::string::npos)
		{
			mode = MODE_SERVER;
			continue;
		}
		else if (line.find("location") != std::string::npos)
		{
			mode = MODE_LOCATION;
			continue;
		}
		else if (line == "}")
		{
			mode = mode == MODE_LOCATION ? MODE_SERVER : MODE_GLOBAL;
			continue;
		}

		const std::vector<std::string>& directives = (mode == MODE_SERVER) ? serverDirectives : locationDirectives;

		std::string directive = line.substr(0, line.find(' '));

		bool isValid = false;
		for (std::vector<std::string>::const_iterator directiveIt = directives.begin(); directiveIt != directives.end(); ++directiveIt)
		{
			if (directive == *directiveIt)
			{
				isValid = true;
				break;
			}
		}
		if (!isValid)
			return false; // 未知のディレクティブが見つかりました
	}
	return true;
}

/* ====================
必須ディレクティブが存在することを確認
- server
	- listen
	- root
==================== */
// MARK: hasRequiredDirectives

static bool hasRequiredDirectives(const std::vector<std::string>& lines)
{
	bool hasServer = false;
	bool hasListen = false;
	bool hasRoot = false;
	ParseMode mode = MODE_GLOBAL;

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;

		if (line.find("server") != std::string::npos)
		{
			mode = MODE_SERVER;
			hasServer = true;
			continue;
		}
		else if (line.find("location") != std::string::npos)
		{
			mode = MODE_LOCATION;
			continue;
		}
		else if (line == "}")
		{
			mode = mode == MODE_LOCATION ? MODE_SERVER : MODE_GLOBAL;
			continue;
		}

		if (mode == MODE_SERVER)
		{
			if (line.find("listen") != std::string::npos)
				hasListen = true;
			if (line.find("root") != std::string::npos)
				hasRoot = true;
		}
	}
	return hasServer && hasListen && hasRoot;
}

bool isValidBracketFormat(const std::string& content);
bool isValidNest(const std::string& content);
bool isValidDirectiveArguments(const std::vector<std::string>& lines);

/* =================
file_contentが有効な設定ファイルの内容であるかを確認するメソッド
================= */
// MARK: isValidConfig

bool Config::isValidConfig(std::string file_content)
{
	// ファイルが空でないことを確認
	if (file_content.empty())
	{
		outputError("Error: Configuration file is empty.");
		return false;
	}
	// 括弧の閉じ忘れをチェック
	if (isValidBracketFormat(file_content) == false)
	{
		outputError("Error: Invalid bracket format in the configuration file.");
		return false;
	}
	// ネストが正しいことを確認
	if (isValidNest(file_content) == false)
	{
		outputError("Error: Invalid nesting in the configuration file.");
		return false;
	}

	std::vector<std::string> lines = mylib::split(file_content, "\n");

	// 未知のディレクティブがないことを確認
	if (isValidDirective(lines) == false)
	{
		outputError("Error: Invalid directive in the configuration file.");
		return false;
	}
	// 必須ディレクティブが存在することを確認
	if (hasRequiredDirectives(lines) == false)
	{
		outputError("Error: Missing required directives in the configuration file.");
		return false;
	}
	// ディレクティブの引数が正しいことを確認
	if (isValidDirectiveArguments(lines) == false)
		return false;
	return true;
}
