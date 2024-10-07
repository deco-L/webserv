/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:47:46 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/10/07 15:17:30 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"



/*

//// 文字列が特定のプレフィックスで始まるかをチェックするヘルパー関数
//bool startsWith(const std::string &str, const std::string &prefix) {
//	return str.compare(0, prefix.size(), prefix) == 0;
//}

//static bool validateDirectiveCounts(const std::vector<std::string> &lines) {
//	std::vector<ServerDirectiveCounts> servers;
//	ServerDirectiveCounts *currentServer = NULL;
//	LocationDirectiveCounts *currentLocation = NULL;
//	ParseMode mode = MODE_NONE;

//	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
//		const std::string &line = *it;

//		if (startsWith(line, "server {")) {
//			mode = MODE_SERVER;
//			servers.push_back(ServerDirectiveCounts());
//			currentServer = &servers.back();
//			currentLocation = NULL;
//		} else if (startsWith(line, "location ")) {
//			if (currentServer == NULL) {
//				std::cout << "ERROR: Found a location block outside of a server block!" << std::endl;
//				return false;
//			}
//			mode = MODE_LOCATION;
//			currentServer->locationDirectives.push_back(LocationDirectiveCounts());
//			currentLocation = &currentServer->locationDirectives.back();
//		} else if (startsWith(line, "}")) {
//			if (mode == MODE_LOCATION) {
//				mode = MODE_SERVER;
//				currentLocation = NULL;
//			} else if (mode == MODE_SERVER) {
//				mode = MODE_NONE;
//				currentServer = NULL;
//			} else {
//				std::cout << "ERROR: Unexpected closing brace!" << std::endl;
//				return false;
//			}
//		} else if (mode == MODE_SERVER) {
//			if (startsWith(line, "listen ")) {
//				currentServer->listen += 1;
//			} else if (startsWith(line, "host ")) {
//				currentServer->host += 1;
//			} else if (startsWith(line, "root ")) {
//				currentServer->root += 1;
//			} else if (startsWith(line, "server_name ")) {
//				currentServer->serverName += 1;
//			} else if (startsWith(line, "error_page ")) {
//				currentServer->errorPage += 1;
//			} else if (startsWith(line, "client_max_body_size ")) {
//				currentServer->clientMaxBodySize += 1;
//			} else if (startsWith(line, "default_server ")) {
//				currentServer->default_server += 1;
//			} else {
//				std::cout << "ERROR: \"" << line << "\" is inappropriate as an element of the server block!" << std::endl;
//				return false;
//			}
//		} else if (mode == MODE_LOCATION) {
//			if (startsWith(line, "root ")) {
//				currentLocation->root += 1;
//			} else if (startsWith(line, "index ")) {
//				currentLocation->index += 1;
//			} else if (startsWith(line, "autoindex ")) {
//				currentLocation->autoindex += 1;
//			} else if (startsWith(line, "cgi_extension ")) {
//				currentLocation->cgi_extension += 1;
//			} else if (startsWith(line, "cgi_handler ")) {
//				currentLocation->cgi_handler += 1;
//			} else if (startsWith(line, "methods ")) {
//				currentLocation->methods += 1;
//			} else if (startsWith(line, "client_max_body_size ")) {
//				currentLocation->clientMaxBodySize += 1;;
//			} else if (startsWith(line, "limit_except ")) {
//				currentLocation->limitExcept += 1;
//			} else {
//				std::cout << "ERROR: \"" << line << "\" is inappropriate as an element of the location block!" << std::endl;
//				return false;
//			}
//		} else {
//			// Lines outside of any block are ignored or can be processed as needed
//		}
//	}

//	// Validate the counts for each server and its locations
//	for (std::vector<ServerDirectiveCounts>::const_iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt) {
//		const ServerDirectiveCounts &server = *serverIt;

//		if (server.listen != 1) {
//			std::cout << "ERROR: The number of 'listen' directives is " << server.listen << "; expected 1." << std::endl;
//			return false;
//		}
//		if (server.host > 1) {
//			std::cout << "ERROR: The number of 'host' directives is " << server.host << "; expected 0 or 1." << std::endl;
//			return false;
//		}
//		if (server.root > 1) {
//			std::cout << "ERROR: The number of 'root' directives is " << server.root << "; expected 1." << std::endl;
//			return false;
//		}
//		if (server.serverName > 1) {
//			std::cout << "ERROR: The number of 'server_name' directives is " << server.serverName << "; expected 1." << std::endl;
//			return false;
//		}
//		if (server.clientMaxBodySize > 1) {
//			std::cout << "ERROR: The number of 'client_max_body_size' directives is " << server.clientMaxBodySize << "; expected 0 or 1." << std::endl;
//			return false;
//		}
//		if (server.default_server > 1) {
//			std::cout << "ERROR: The number of 'default_server' directives is " << server.default_server << "; expected 0 or 1." << std::endl;
//			return false;
//		}

//		for (std::vector<LocationDirectiveCounts>::const_iterator locIt = server.locationDirectives.begin(); locIt != server.locationDirectives.end(); ++locIt) {
//			const LocationDirectiveCounts &location = *locIt;

//			if (location.root > 1) {
//				std::cout << "ERROR: The number of 'root' directives in the location block is " << location.root << "; expected 0 or 1." << std::endl;
//				return false;
//			}
//			if (location.index > 1) {
//				std::cout << "ERROR: The number of 'index' directives in the location block is " << location.index << "; expected 0 or 1." << std::endl;
//				return false;
//			}
//			if (location.autoindex > 1) {
//				std::cout << "ERROR: The number of 'autoindex' directives in the location block is " << location.autoindex << "; expected 0 or 1." << std::endl;
//				return false;
//			}
//			if (location.cgi_extension > 1) {
//				std::cout << "ERROR: The number of 'cgi_extension' directives in the location block is " << location.cgi_extension << "; expected 0 or 1." << std::endl;
//				return false;
//			}
//			if (location.cgi_handler > 1) {
//				std::cout << "ERROR: The number of 'cgi_handler' directives in the location block is " << location.cgi_handler << "; expected 0 or 1." << std::endl;
//				return false;
//			}
//			if (location.methods > 1) {
//				std::cout << "ERROR: The number of 'methods' directives in the location block is " << location.methods << "; expected 0 or 1." << std::endl;
//				return false;
//			}
//			if (location.upload_directory > 1) {
//				std::cout << "ERROR: The number of 'upload_directory' directives in the location block is " << location.upload_directory << "; expected 0 or 1." << std::endl;
//				return false;
//			}
//			if (location.clientMaxBodySize > 1) {
//				std::cout << "ERROR: The number of 'client_max_body_size' directives in the location block is " << location.clientMaxBodySize << "; expected 0 or 1." << std::endl;
//				return false;
//			}
//		}
//	}
//	return true;
//}




//// 文字列が数字のみで構成されているかをチェックする関数
//static bool isNumeric(const std::string &str) {
//	if (str.empty())
//		return false;
//	for (size_t i = 0; i < str.length(); ++i) {
//		if (!std::isdigit(static_cast<unsigned char>(str[i])))
//			return false;
//	}
//	return true;
//}

//// 文字列を整数に変換する関数
//static int stringToInt(const std::string &str) {
//	std::istringstream iss(str);
//	int number = 0;
//	iss >> number;
//	if (iss.fail())
//		return -1; // 変換エラーを示す
//	return number;
//}

//// IPアドレスの形式をチェックする関数
//static bool isValidIpAddress(const std::string &ip) {
//	std::istringstream ss(ip);
//	std::string token;
//	int count = 0;
//	while (std::getline(ss, token, '.')) {
//		if (!isNumeric(token))
//			return false;
//		int num = stringToInt(token);
//		if (num < 0 || num > 255 || (token.length() > 1 && token[0] == '0'))
//			return false;
//		++count;
//	}
//	return count == 4; // IPv4アドレスは4つの数値で構成される
//}

//// ポート番号の妥当性をチェックする関数
//static bool isValidPort(int port) {
//	return port > 0 && port <= 65535;
//}

//// 設定ディレクティブを検証する関数
//static bool validateConfiguration(const std::vector<std::string> &lines) {
//	std::vector<std::string> tokens;
//	std::vector<std::string>::const_iterator it = lines.begin();

//	for (; it != lines.end(); ++it) {
//		tokens = mylib::split(*it, " ");
//		if (tokens.empty())
//			continue; // 空行をスキップ

//		const std::string &directive = tokens[0];

//		if (directive == "port") {
//			if (tokens.size() != 2 || !isNumeric(tokens[1]) || !isValidPort(stringToInt(tokens[1]))) {
//				std::cout << "ERROR: Invalid port value!" << std::endl;
//				return false;
//			}
//		} else if (directive == "host") {
//			if (tokens.size() != 2 || !isValidIpAddress(tokens[1])) {
//				std::cout << "ERROR: Invalid host value!" << std::endl;
//				return false;
//			}
//		} else if (directive == "root") {
//			if (tokens.size() != 2 || mylib::check_access(tokens[1].c_str()) != 2) {
//				std::cout << "ERROR: Invalid root value!" << std::endl;
//				return false;
//			}
//		} else if (directive == "server_name") {
//			if (tokens.size() < 2) {
//				std::cout << "ERROR: Invalid server_name value!" << std::endl;
//				return false;
//			}
//		} else if (directive == "error_page") {
//			if (tokens.size() < 3) {
//				std::cout << "ERROR: Invalid error_page value!" << std::endl;
//				return false;
//			}
//			if (!isNumeric(tokens[1]) || tokens[1].length() != 3) {
//				std::cout << "ERROR: Invalid error_page code!" << std::endl;
//				std::cout << tokens[2] << std::endl;
//				return false;
//			}
//			int errorCode = stringToInt(tokens[1]);
//			if (!(errorCode == 400 || errorCode == 403 || errorCode == 404 || errorCode == 405 ||
//				  errorCode == 408 || errorCode == 411 || errorCode == 413 || errorCode == 414 ||
//				  errorCode == 415 || errorCode == 429 || errorCode == 431 || errorCode == 500 ||
//				  errorCode == 501 || errorCode == 502 || errorCode == 504 || errorCode == 505)) {
//				std::cout << "ERROR: Invalid error_page code!" << std::endl;
//				return false;
//			}
//			if (mylib::check_access(tokens[tokens.size()-1].c_str()) != 1) {
//				std::cout << "ERROR: Error page file does not exist!" << std::endl;
//				std::cout << tokens[tokens.size()-1] << std::endl;
//				return false;
//			}
//		} else if (directive == "client_max_body_size") {
//			if (tokens.size() != 2) {
//				std::cout << "ERROR: Invalid client_max_body_size value!" << std::endl;
//				return false;
//			}
//			char lastChar = tokens[1][tokens[1].length() - 1];
//			if (!(lastChar == 'M' || lastChar == 'K' || lastChar == 'G' || std::isdigit(static_cast<unsigned char>(lastChar)))) {
//				std::cout << "ERROR: Invalid client_max_body_size unit!" << std::endl;
//				return false;
//			}
//			std::string sizePart = tokens[1].substr(0, tokens[1].length() - (std::isdigit(static_cast<unsigned char>(lastChar)) ? 0 : 1));
//			if (!isNumeric(sizePart)) {
//				std::cout << "ERROR: Invalid client_max_body_size value!" << std::endl;
//				return false;
//			}
//		} else if (directive == "index") {
//			if (tokens.size() < 2) {
//				std::cout << "ERROR: Invalid index value!" << std::endl;
//				return false;
//			}
//		} else if (directive == "autoindex") {
//			if (tokens.size() != 2 || (tokens[1] != "on" && tokens[1] != "off")) {
//				std::cout << "ERROR: Invalid autoindex value!" << std::endl;
//				return false;
//			}
//		} else if (directive == "cgi") {
//			if (tokens.size() < 2 || tokens.size() > 6) {
//				std::cout << "ERROR: Invalid cgi value!" << std::endl;
//				return false;
//			}
//			for (size_t i = 1; i < tokens.size(); ++i) {
//				if (!(tokens[i] == "cgi" || tokens[i] == "py" || tokens[i] == "rb" || tokens[i] == "pl" || tokens[i] == "sh")) {
//					std::cout << "ERROR: Invalid cgi extension!" << std::endl;
//					return false;
//				}
//			}
//		} else if (directive == "allow_methods") {
//			if (tokens.size() < 2 || tokens.size() > 4) {
//				std::cout << "ERROR: Invalid allow_methods value!" << std::endl;
//				return false;
//			}
//			for (size_t i = 1; i < tokens.size(); ++i) {
//				if (!(tokens[i] == "GET" || tokens[i] == "POST" || tokens[i] == "DELETE")) {
//					std::cout << "ERROR: Invalid allow_methods method!" << std::endl;
//					return false;
//				}
//			}
//		} else if (directive == "redirect") {
//			if (tokens.size() != 2) {
//				std::cout << "ERROR: Invalid redirect value!" << std::endl;
//				return false;
//			}
//		} else if (directive == "limit_except") {
//			// limit_exceptの後に続くHTTPメソッドを検証
//			if (tokens.size() < 2) {
//				std::cout << "ERROR: Invalid limit_except value!" << std::endl;
//				return false;
//			}
//			for (size_t i = 1; i < tokens.size(); ++i) {
//				if (!(tokens[i] == "GET" || tokens[i] == "POST" || tokens[i] == "PUT" || tokens[i] == "DELETE" ||
//					  tokens[i] == "PATCH" || tokens[i] == "HEAD" || tokens[i] == "OPTIONS")) {
//					std::cout << "ERROR: Invalid HTTP method in limit_except!" << std::endl;
//					return false;
//				}
//			}
//		}
//		// 必要に応じて他のディレクティブのチェックを追加
//	}
//	return true;
//}

*/


/* ====================
括弧の閉じ忘れをチェック
==================== */
// MARK: isValidBracketFormat

bool isValidOpeningBracePosition(const std::string& content, size_t pos)
{
	const std::string serverKeyword = "server";
	const std::string locationKeyword = "location";

	// 開き括弧のある行の開始位置を見つける
	size_t lineStart = content.rfind('\n', pos);
	if (lineStart == std::string::npos)
		lineStart = 0;
	else
		lineStart += 1; // 改行の次の位置から開始

	// '{' までの行を取得
	std::string line = content.substr(lineStart, pos - lineStart);

	// 前後の空白を削除
	line.erase(0, line.find_first_not_of(" \t"));
	line.erase(line.find_last_not_of(" \t") + 1);

	// 行が 'server' で始まる場合
	if (line.compare(0, serverKeyword.length(), serverKeyword) == 0)
	{
		return true;
	}
	// 行が 'location' で始まる場合
	else if (line.compare(0, locationKeyword.length(), locationKeyword) == 0)
	{
		size_t afterLocation = locationKeyword.length();

		// 空白をスキップ
		afterLocation = line.find_first_not_of(" \t", afterLocation);
		if (afterLocation == std::string::npos)
			return false; // URIまたは修飾子がない

		// 修飾子をチェック
		std::string modifiers[] = {"=", "^~", "~*", "~"};
		std::string modifierFound;
		for (size_t i = 0; i < sizeof(modifiers)/sizeof(modifiers[0]); ++i)
		{
			if (line.compare(afterLocation, modifiers[i].length(), modifiers[i]) == 0)
			{
				modifierFound = modifiers[i];
				afterLocation += modifiers[i].length();
				break;
			}
		}

		// 修飾子があった場合、後続の空白をスキップ
		if (!modifierFound.empty())
		{
			afterLocation = line.find_first_not_of(" \t", afterLocation);
			if (afterLocation == std::string::npos)
				return false; // 修飾子の後にURIがない
		}

		// URIを取得
		size_t uriEnd = line.find_first_of(" \t{", afterLocation);
		if (uriEnd == std::string::npos)
			uriEnd = line.length();

		std::string uri = line.substr(afterLocation, uriEnd - afterLocation);

		// URIが空でないことを確認
		if (uri.empty())
			return false;

		// 追加のURI検証が必要な場合はここに記述

		return true;
	}

	return false; // 'server' または 'location' で始まらない場合
}

/* 可読性を向上させるため、ヘルパー関数 'isValidOpeningBracePosition' を作成しました */
bool isValidBracketFormat(const std::string& content)
{
	/* 'braces' を 'braceStack' にリネームしました */
	std::stack<int> braceStack;

	for (size_t i = 0; i < content.size(); ++i)
	{
		if (content[i] == '{')
		{
			if (isValidOpeningBracePosition(content, i))
				braceStack.push(i);
			else
				return false; // 無効な '{' の配置
		}
		else if (content[i] == '}')
		{
			if (braceStack.empty())
				return false; // 対応する '{' がありません
			braceStack.pop();
		}
	}

	/* すべての括弧が対応していることを確認するためのチェックを追加しました */
	if (!braceStack.empty())
		return false; // 対応していない '{' があります

	// '{' の前に、'server' または 'location' があることを確認するためのチェック
	for (size_t i = 0; i < content.size(); ++i)
	{
		if (content[i] == '{' && !isValidOpeningBracePosition(content, i))
			return false; // 無効な '{' の配置
	}

	return true;
}

/* ====================
ネストが正しいことを確認
==================== */
// MARK: isValidNest

static std::vector<std::pair<size_t, size_t> > findBlockRanges(const std::string& content, const std::string& keyword)
{
	std::vector<std::pair<size_t, size_t> > blockRanges;
	size_t pos = 0;

	while (pos < content.size())
	{
		// キーワードを検索
		size_t keywordPos = content.find(keyword, pos);

		if (keywordPos == std::string::npos)
			break;
		if (keywordPos > 0 && std::isalnum(content[keywordPos - 1]))
		{
			pos = keywordPos + keyword.length();
			continue;
		}
		if (keywordPos + keyword.length() < content.size() && std::isalnum(content[keywordPos + keyword.length()]))
		{
			pos = keywordPos + keyword.length();
			continue;
		}
		// キーワードの後の '{' を見つける
		size_t openBracePos = content.find('{', keywordPos + keyword.length());

		if (openBracePos == std::string::npos)
		{
			// 開き中括弧が見つからない場合
			pos = keywordPos + keyword.length();
			continue;
		}
		// 対応する閉じ中括弧をスタックで見つける
		std::stack<char> braceStack;
		size_t closeBracePos = openBracePos;

		braceStack.push('{');
		while (!braceStack.empty() && ++closeBracePos < content.size())
		{
			if (content[closeBracePos] == '{')
				braceStack.push('{');
			else if (content[closeBracePos] == '}')
				braceStack.pop();
		}
		if (!braceStack.empty()) // 対応する閉じ中括弧が見つからない場合
			return std::vector<std::pair<size_t, size_t> >(); // 不一致を示すため空のベクターを返す

		blockRanges.push_back(std::make_pair(openBracePos, closeBracePos));
		pos = closeBracePos + 1;
	}
	return blockRanges;
}

static bool isValidNest(const std::string& content)
{
	std::vector<std::pair<size_t, size_t> > serverBlockRanges = findBlockRanges(content, "server");

	if (serverBlockRanges.empty())
		return false; // サーバーブロックが見つからない、または中括弧が不一致

	std::vector<std::pair<size_t, size_t> > locationBlockRanges = findBlockRanges(content, "location");

	if (locationBlockRanges.empty())
		return true; // location ブロックがない場合は正しくネストされているとみなす

	// locationブロックがserverブロック内にネストされていることを確認
	for (std::vector<std::pair<size_t, size_t> >::const_iterator locationIt = locationBlockRanges.begin();
		locationIt != locationBlockRanges.end(); ++locationIt)
	{
		bool isNested = false;

		for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt = serverBlockRanges.begin();
			serverIt != serverBlockRanges.end(); ++serverIt)
		{
			if (locationIt->first > serverIt->first && locationIt->second <= serverIt->second)
			{
				isNested = true;
				break;
			}
		}
		if (!isNested)
			return false; // この location ブロックはどの server ブロックにもネストされていない
	}

	// serverブロックの中にserverブロックがネストされていないことを確認
	for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt = serverBlockRanges.begin();
		serverIt != serverBlockRanges.end(); ++serverIt)
	{
		for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt2 = serverBlockRanges.begin();
			serverIt2 != serverBlockRanges.end(); ++serverIt2)
		{
			if (serverIt != serverIt2 && serverIt->first < serverIt2->first && serverIt->second > serverIt2->second)
				return false; // この server ブロックは別の server ブロックにネストされている
		}
	}

	// serverブロックの中にserverブロックがネストされていないことを確認
	for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt = serverBlockRanges.begin();
		serverIt != serverBlockRanges.end(); ++serverIt)
	{
		for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt2 = serverBlockRanges.begin();
			serverIt2 != serverBlockRanges.end(); ++serverIt2)
		{
			if (serverIt != serverIt2 && serverIt->first < serverIt2->first && serverIt->second > serverIt2->second)
				return false; // この server ブロックは別の server ブロックにネストされている
		}
	}

	return true; // すべての location ブロックが server ブロック内にネストされている
}

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

/* ====================
ディレクティブの引数が正しいことを確認
	- 適切な引数があることを確認

	- 引数の数が正しいことを確認

	- 有効な値が設定されていることを確認
	- 引数がIPアドレスである場合、有効なIPアドレスであることを確認
	- 引数がポート番号である場合、有効なポート番号であることを確認
	- 引数が数値である場合、有効な数値であることを確認
	- 引数がHTTPメソッドである場合、有効なHTTPメソッドであることを確認
	- 引数が拡張子である場合、有効な拡張子であることを確認

	- 引数がファイルパスである場合、ファイルが存在することを確認
	- 引数がディレクトリパスである場合、ディレクトリが存在することを確認
==================== */
// MARK: isValidDirectiveArguments

static bool hasArguments(const std::vector<std::string>& lines)
{
	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;

		if (line.find("server") != std::string::npos)
			continue;

		// location [path] {}
		if (line.find("location") != std::string::npos)
		{
			// "location [path] {" を、"location [path]"にする
			// '{'を消した後の空白も削除する
			std::string location = line.substr(0, line.find('{'));
			location.erase(location.find_last_not_of(" \t") + 1);

			if (location.find(' ') == std::string::npos)
				return false; // 引数がない
			continue;
		}

		if (line == "}")
			continue;

		if (line.find(' ') == std::string::npos)
			return false; // 引数がない
	}
	return true;
}

static bool hasCorrectNumberOfArguments(const std::vector<std::string>& lines)
{
	//- server
	//  - listen				1~N
	//  - server_name			1~N
	//  - error_page			2~N
	//  - client_max_body_size	1
	//  - root					1
	//  - index					1~N
	//  - autoindex				1
	//  - location				1
	//  - methods				1~N
	//  - return				1~2
	//  - cgi_extension			2
	//  - upload_enable			1
	//  - upload_store			1
	// locationは特別

	// 1
    const char* directivesArray[] = {
        "client_max_body_size",
        "root",
        "autoindex",
        "upload_enable",
        "upload_store"
    };
    size_t arraySize = sizeof(directivesArray) / sizeof(directivesArray[0]);
    std::vector<std::string> directives1(directivesArray, directivesArray + arraySize);

	// 1~N
	const char* directives1NArray[] = {
		"listen",
		"server_name",
		"index",
		"methods"
	};
	arraySize = sizeof(directives1NArray) / sizeof(directives1NArray[0]);
	std::vector<std::string> directives1N(directives1NArray, directives1NArray + arraySize);

	// 2~N
	const char* directives2NArray[] = {
		"error_page"
	};
	arraySize = sizeof(directives2NArray) / sizeof(directives2NArray[0]);
	std::vector<std::string> directives2N(directives2NArray, directives2NArray + arraySize);

	// 1~2
	const char* directives12Array[] = {
		"return"
	};
	arraySize = sizeof(directives12Array) / sizeof(directives12Array[0]);
	std::vector<std::string> directives12(directives12Array, directives12Array + arraySize);

	// 2
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
		//std::cout << directive << std::endl;

		if (directive == "location") // "location" "[path]" "{"
		{
			if (tokens[tokens.size() - 1] == "{" && tokens.size() == 3)
				continue;
			else
				return false;
		}

		if (directive == "}")
			continue;

		if (std::find(directives1.begin(), directives1.end(), directive) != directives1.end())
		{
			if (tokens.size() != 2)
				return false;
		}

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

// 有効な値が設定されていることを確認
// 引数がIPアドレスである場合、有効なIPアドレスであることを確認
// 引数がポート番号である場合、有効なポート番号であることを確認
// 引数が数値である場合、有効な数値であることを確認
// 引数がHTTPメソッドである場合、有効なHTTPメソッドであることを確認
// 引数が拡張子である場合、有効な拡張子であることを確認
// 引数が正しい数であることは確認済みの入力を前提とする
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
		//std::cout << directive << std::endl;

		// server
		// serverに引数はない
		// ports, domains, error_code, location_path, cgi_extensionsをクリア
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

		// listen [address:]port
		// addressはIPv4のみ
		// portは1~65535
		// 重複は追加なので、重複がある場合はエラー
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

		// server_name name [name ...]
		// ワイルドカードがない前提
		//英数字（A〜Z、a〜z、0〜9）、ハイフン（-）、およびドット（.）のみで構成されていること。
		//ハイフン（-） は、ラベル（ドットで区切られた各部分）の先頭または末尾に使用できない。
		//ドット（.）はラベルの区切り文字として使用され、連続して現れないこと。
		//各ラベルは 1〜63文字以内であること。
		//ラベルは空であってはならない（連続するドットは不可）。
		//ドメイン名全体の長さが 253文字以内であること。
		// 重複は追加なので、重複がある場合はエラー
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

		// error_page code [code ...] path
		// codeの部分のみチェックする
		// pathは別の部分でチェックするのでここではチェックしない
		// codeは3桁の数字であること
		// 100〜599
		// 重複は追加なので、重複がある場合はエラー
		if (directive == "error_page") // code [code ...] path
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

		// client_max_body_size size
		// sizeは数値であること
		//数値と単位（k、m、g, K, M, G）が正しく指定されていること（例: 10k、1m）。単位の省略あり。
		//サイズが正の数であること。
		//少数は指定できないこと。
		//　重複は上書き
		if (directive == "client_max_body_size") // size
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

		// root path
		// pathは別の部分でチェックするのでここではチェックしない
		// 重複は上書き

		// index file [file ...]
		// '/'や'\'は含まれないか
		// ファイル名が255文字以内であること
		//許可される文字:
		//英数字（A-Z, a-z, 0-9）
		//特殊文字：ドット（.）、ハイフン（-）、アンダースコア（_）
		// 重複は上書き
		if (directive == "index") // file [file ...]
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

		// autoindex on | off
		// 大文字小文字は区別しない
		// 重複は上書き
		if (directive == "autoindex") // on | off
		{
			if (tokens[1] != "on" && tokens[1] != "off"
				&& tokens[1] != "ON" && tokens[1] != "OFF")
				return false;
		}

		// location [path] {}
		// pathは別の部分でチェックするのでここではチェックしない
		// 重複は追加なので、重複がある場合はエラー
		// pathが重複しているかどうかのみチェック
		if (directive == "location") // [path] {}
		{
			if (std::find(location_path.begin(), location_path.end(), tokens[1]) != location_path.end())
				return false;
			location_path.push_back(tokens[1]);
		}

		// methods method [method ...]
		// GET, POST, DELETE のみ
		// 大文字のみ
		// 重複は上書き
		// methods GET GET GET はエラー
		if (directive == "methods") // method [method ...]
		{
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				if (tokens[i] != "GET" && tokens[i] != "POST" && tokens[i] != "DELETE")
					return false;
				if (std::find(tokens.begin() + 1, tokens.begin() + i, tokens[i]) != tokens.begin() + i)
					return false;
			}
		}

		// return code [URL]
		// codeは3桁の数字であること
		// 100〜599
		// URLは省略可能
		// URLがあった場合、別の部分でチェックするのでここではチェックしない
		// 重複は上書き
		if (directive == "return") // code [URL]
		{
			if (!mylib::isNumeric(tokens[1]) || tokens[1].length() != 3)
				return false;
			int code = mylib::stringToInt(tokens[1]);
			if (code < 100 || code > 599)
				return false;
		}

		// cgi_extension extension path_to_cgi_executable
		// pathが絡むので、別の部分でチェックするのでここではチェックしない
		// ここではargumentsの重複だけチェック
		// argumentsのどちらか片方だけでも、過去のものと重複していたらエラー
		// 重複は追加なので、重複がある場合はエラー
		if (directive == "cgi_extension") // extension path_to_cgi_executable
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

		// upload_enable on | off
		// 大文字小文字は区別しない
		// 重複は上書き
		if (directive == "upload_enable") // on | off
		{
			if (tokens[1] != "on" && tokens[1] != "off"
				&& tokens[1] != "ON" && tokens[1] != "OFF")
				return false;
		}

		// upload_store path
		// pathは別の部分でチェックするのでここではチェックしない
		// 重複は上書き
	}
	return true;
}

//server_root_path = getRoot(it, lines);
//'{', '}'をstackで管理
// '}'がちゃんと閉じるまでの間の、rootのpathを取得
// rootは上書きされるので、最後のrootが有効
static std::string getRoot(std::vector<std::string>::const_iterator& it, const std::vector<std::string>& lines)
{
    std::stack<char> braceStack;
    std::string root("");
    std::vector<std::string>::const_iterator it2 = it;

    // フラグを使用して、最初の '{' を見つけたかどうかを追跡
    bool inFirstBraceBlock = false;

    while (it2 != lines.end())
    {
        const std::string& line = *it2;
        ++it2;

        // '{' を見つけたらスタックに積む
        if (line.find('{') != std::string::npos) {
            braceStack.push('{');

            // 最初の '{' を見つけたときにフラグをセット
            if (braceStack.size() == 1) {
                inFirstBraceBlock = true;
            }
        }
        // '}' を見つけたらスタックから取り出す
        else if (line.find('}') != std::string::npos) {
            braceStack.pop();

            // 最初の '{}' ブロックが閉じたら終了
            if (braceStack.empty()) {
                break;
            }
        }

        // 最初の '{' の直後のブロック内にいる場合のみ root をチェック
        if (inFirstBraceBlock && braceStack.size() == 1 && line.find("root") != std::string::npos)
        {
            std::vector<std::string> tokens = mylib::split(line, " ");

            // root ディレクティブの後にパスが続くと仮定し、root パスを設定
            if (tokens.size() > 1) {
                root = tokens[1];
            }
        }
    }

    return root;
}
//static std::string getRoot(std::vector<std::string>::const_iterator& it, const std::vector<std::string>& lines)
//{
//	std::stack<char> braceStack;
//	std::string root("");
//	std::vector<std::string>::const_iterator it2 = it;


//	while (it2 != lines.end())
//	{
//		const std::string& line = *it2;
//		++it2;

//		if (line.find('{') != std::string::npos)
//			braceStack.push('{');
//		else if (line.find('}') != std::string::npos)
//			braceStack.pop();

//		if (braceStack.empty())
//			break;

//		if (line.find("root") != std::string::npos)
//		{
//			std::vector<std::string> tokens = mylib::split(line, " ");
//			root = tokens[1];
//		}
//	}
//	return root;
//}

// 引数がファイルパスである場合、ファイルが存在することを確認
// 引数がディレクトリパスである場合、ディレクトリが存在することを確認
// error_pageのpath, rootのpath, locationのpath, returnのURL, cgi_extensionのextensionとpath_to_cgi_executable, upload_storeのpath
static bool hasValidPaths(const std::vector<std::string>& lines)
{
	// serverごとに区切って処理
	// serverブロック直下にrootがあることは確定済み。前提とする。
	// locationブロック直下にrootがある場合は、locationブロックのrootが優先される
	// locationブロック直下にrootがない場合は、serverブロックのrootが使われる
	// locationブロック内のrootは、locationブロック内でのみ有効

	// error_pageのpath
		// error_pageは重複で追加の挙動だが、codeは重複でエラーになるので、pathの重複は問題ない
		// 重複OK
		// rootのpathがある場合は、root_path + error_page_path で検証
		// '/'で始まるか
		// ファイルが存在すること
		// 相対パスNG
		// 無効文字チェック

	// rootのpath
		// 重複OK
		// 絶対パスか
		// ディレクトリであること
		// ディレクトリが存在すること
		// 書き込み権限があること
		// 無効文字チェック


	// locationのpath
		// 同一server内では重複NG
		// '/'で始まるか
		// rootのpathがある場合は、root_path + location_path で検証
		// ディレクトリであること
		// ディレクトリが存在すること
		// 書き込み権限があること
		// 無効文字チェック

	// returnのURL
		// 現状は、URL=相対パスのみで。
		/*
		// URLは、絶対URLか相対パス
			//絶対URLと相対パスの定義
			//絶対URL（Absolute URL）:
			//スキーム（プロトコル）を含む完全なURL。
			//例: http://example.com/page, https://www.example.com/path?query=123

			//相対パス（Relative Path）:
			//スキームを含まず、現在のドメインに対するパス。
			//例: /newpage, /images/logo.png
			//判定方法
			//絶対URLか相対パスかを判定する主な方法は、URLの先頭にスキームが含まれているかどうかです。具体的には、URLが特定のスキーム（http://, https://）で始まる場合、絶対URLと見なされます。それ以外の場合、相対パスと見なされます。

			//スキームの存在確認:
			//URLが http:// または https:// で始まるかを確認します。
			//大文字・小文字の区別はしない（例: HTTP://, https:// も有効）。
			//スキーム以外の形式:
			//スキームが存在しない場合、URLは相対パスと見なします。
		// それぞれのvalid チェック
		*/
		/* 以下は、一旦無視。
		//絶対URLの検証:
			//スキームの正当性:
			//スキームが http または https のいずれかであること。
			//他のスキーム（例: ftp://, mailto:）は無効とする。

			//ホスト名の形式:
			//有効なドメイン名またはIPアドレスであること。
			//ドメイン名の構造が正しい（例: example.com, sub.domain.example.com）。
			//IPv6は無効とする。IPv4のみを許可する。

			//パスの形式チェック

			//全体のURL構造の検証:
			//URLがRFC 3986に準拠していること。
		*/
		// 相対パスの検証:
			//先頭スラッシュの有無:
			//相対パスは通常、スラッシュ / で始まる絶対パスとして解釈されます。相対パス（例: newpage）も許容する場合がありますが、NGINXのreturnディレクティブではスラッシュ始まりのパスが一般的です。

			//有効なURIエンコーディング:
			//パス内に不正な文字（スペースや制御文字）が含まれていないこと。
			//必要な文字が正しくエンコードされていること。

			//パスの相対性:
			//親ディレクトリへの遡り（例: ../）が不正に使用されていないこと。

			//特殊文字の使用制限:
			//セキュリティ上問題となる文字（例: NULL文字）が含まれていないこと。
		// 重複OK

	// cgi_extensionのextension
		// 重複NGだけど、すでにチェック済みなので無視
		// 許可する拡張子は, .cgi, .py, .pl, .rb, .sh (.php?)

	// cgi_extensionのpath_to_cgi_executable
		// 重複NGだけど、すでにチェック済みなので無視
		// 絶対パスであること
		// 実行可能ファイルであること
			// ファイルの存在
			// 実行権限があること
			// パスがファイルであること

	// upload_storeのpath
		// 重複OK
		// ディレクトリであること
		// 書き込み権限があること

	ParseMode mode = MODE_GLOBAL;
	std::string server_root_path("");
	std::string location_root_path("");
	std::vector<std::string> location_paths;

	int i = 0;

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const std::string& line = *it;
		const std::vector<std::string> tokens = mylib::split(line, " ");
		std::string directive = tokens[0];
		//std::cout << directive << std::endl;
		//std::cout << i++ << std::endl;

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
			// 最後の'/'を削除
			if (location_path[location_path.length() - 1] == '/')
				location_path = location_path.substr(0, location_path.length() - 1);

			//std::cout << location_path << std::endl;
			//std::cout << location_root_path << std::endl;

			if (std::find(location_paths.begin(), location_paths.end(), location_path) != location_paths.end())
				return false;
			location_paths.push_back(location_path);

			//std::cout << location_path << std::endl;
			if (mylib::getPathType(location_path) != IS_DIRECTORY)
				return false;
			//std::cout << "here" << std::endl;
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
			// error_pageは重複で追加の挙動だが、codeは重複でエラーになるので、pathの重複は問題ない
			// 重複OK
			// rootのpathがある場合は、server_root_path + error_page_path で検証
			// '/'で始まるか
			// ファイルが存在すること
			// 相対パスNG
			// 無効文字チェック
		if (directive == "error_page")
		{
			if (tokens[tokens.size() - 1][0] != '/')
				return false;

			std::string error_page_path(server_root_path + tokens[tokens.size() - 1]);
			//std::cout << error_page_path << std::endl;
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

		// 相対パスの検証:
			//先頭スラッシュの有無:
			//相対パスは通常、スラッシュ / で始まる絶対パスとして解釈されます。相対パス（例: newpage）も許容する場合がありますが、NGINXのreturnディレクティブではスラッシュ始まりのパスが一般的です。

			//有効なURIエンコーディング:
			//パス内に不正な文字（スペースや制御文字）が含まれていないこと。
			//必要な文字が正しくエンコードされていること。

			//パスの相対性:
			//親ディレクトリへの遡り（例: ../）が不正に使用されていないこと。

			//特殊文字の使用制限:
			//セキュリティ上問題となる文字（例: NULL文字）が含まれていないこと。
		if (directive == "return" && tokens.size() == 3)
		{
			if (tokens[2][0] != '/')
				return false;
			std::string return_path(location_root_path + tokens[2]);
			//std::cout << return_path << std::endl;
			if (mylib::getPathType(return_path) != IS_FILE)
				return false;
			if (access(return_path.c_str(), R_OK) != 0)
				return false;
		}

		// cgi_extension extension path_to_cgi_executable
		// 重複NGだけど、すでにチェック済みなので無視
		// 許可する拡張子は, .cgi, .py, .sh (.php?, .pl?, .rb?)
		if (directive == "cgi_extension")
		{
			// extensionチェック
			if (tokens[1] != ".cgi" && tokens[1] != ".py" && tokens[1] != ".sh")
				return false;


			// path_to_cgi_executableチェック
			std::string path_to_cgi_executable(tokens[2]);
			if (mylib::getPathType(path_to_cgi_executable) != IS_FILE)
				return false;
			if (access(path_to_cgi_executable.c_str(), X_OK) != 0)
				return false;
		}

		// upload_storeのpath
		// 重複OK
		// ディレクトリであること
		// 書き込み権限があること
		if (directive == "upload_store")
		{
			if (tokens[1][0] != '/')
				return false;

			std::string upload_store_path(location_root_path + tokens[1]);
			//std::cout << upload_store_path << std::endl;
			if (mylib::getPathType(upload_store_path) != IS_DIRECTORY)
				return false;
			if (access(upload_store_path.c_str(), X_OK) != 0)
				return false;
		}
	}
	return true;
}


static bool isValidDirectiveArguments(const std::vector<std::string>& lines)
{
	// 適切な引数があることを確認
	if (hasArguments(lines) == false)
	{
		outputError("ERROR: Invalid number of arguments!");
		return false;
	}

	// 引数の数が正しいことを確認
	if (hasCorrectNumberOfArguments(lines) == false)
	{
		outputError("ERROR: Invalid number of arguments!");
		return false;
	}

	// 有効な値が設定されていることを確認
	// 引数がIPアドレスである場合、有効なIPアドレスであることを確認
	// 引数がポート番号である場合、有効なポート番号であることを確認
	// 引数が数値である場合、有効な数値であることを確認
	// 引数がHTTPメソッドである場合、有効なHTTPメソッドであることを確認
	// 引数が拡張子である場合、有効な拡張子であることを確認
	if (hasValidArguments(lines) == false)
	{
		outputError("ERROR: Invalid argument value!");
		return false;
	}

	// 引数がファイルパスである場合、ファイルが存在することを確認
	// 引数がディレクトリパスである場合、ディレクトリが存在することを確認
	// error_pageのpath, rootのpath, locationのpath, returnのURL, cgi_extensionのextensionとpath_to_cgi_executable, upload_storeのpath
	if (hasValidPaths(lines) == false)
	{
		outputError("ERROR: File or Directory is Invalid!");
		return false;
	}

	return true;
}

/*
- **`server`** `{ ... }`
  仮想サーバーの設定を指定するためのサーバーブロックを定義します。

  - **`listen`** `[address:]port;`
    サーバーがリッスンするアドレスとポートを指定します。

  - **`server_name`** `name [name ...];`
    サーバーの名前を設定します。

  - **`error_page`** `code [code ...] path;`
    指定したHTTPエラーコードに対してカスタムエラーページを設定します。

  - **`client_max_body_size`** `size;`
    クライアントリクエストボディのサイズ制限を設定します。

  - **`root`** `path;`
    リクエストに対してルートディレクトリを設定します。

  - **`index`** `file [file ...];`
    ディレクトリがリクエストされたときに提供されるデフォルトファイルを定義します。

  - **`autoindex`** `on | off;`
    ディレクトリリストの表示を有効または無効にします。

  - **`location`** `[path] { ... }`
    ルートおよびその特定の設定を定義します。

    - **`root`** `path;`
      このlocationブロック内でルートディレクトリを上書きします。

    - **`methods`** `method [method ...];`
      ルートで受け付けるHTTPメソッドのリストを定義します。

    - **`return`** `code [URL];`
      HTTPリダイレクトを発行するか、特定のステータスコードを返します。

    - **`autoindex`** `on | off;`
      このルートに対してディレクトリリストの表示を有効または無効にします。

    - **`index`** `file [file ...];`
      このルートのデフォルトファイルを設定します。

    - **`cgi_extension`** `extension path_to_cgi_executable;`
      特定のファイル拡張子に対してCGIの実行を指定します。

    - **`upload_enable`** `on | off;`
      このルートでファイルアップロードを有効または無効にします。

    - **`upload_store`** `path;`
      アップロードされたファイルを保存するディレクトリを指定します。

*/

//- server
//  - listen
//  - server_name
//  - error_page
//  - client_max_body_size
//  - root
//  - index
//  - autoindex
//  - location
//    - root
//    - methods
//    - return
//    - autoindex
//    - index
//    - cgi_extension
//    - upload_enable
//    - upload_store


bool Config::isValidConfig(std::string file_content)
{
	/*
	//- ファイルが空でないことを確認
	//- 括弧の閉じ忘れをチェック
	//- ネストが正しいことを確認
	//- 未知のディレクティブがないことを確認
	//- 必須ディレクティブが存在することを確認
	//	- server
	//		- listen
	//		- root
	- ディレクティブの引数が正しいことを確認
	 */

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

//### **`server` ブロック内のディレクティブ**
//- **`listen`**: `[IPアドレス:]ポート [オプション]`
//- **`server_name`**: `ドメイン名1 ドメイン名2 ...`
//- **`error_page`**: `ステータスコード パス`
//- **`client_max_body_size`**: `サイズ`
//- **`root`**: `パス`
//- **`index`**: `ファイル名1 ファイル名2 ...`
//- **`autoindex`**: `on|off`

//### **`location` ブロック内のディレクティブ**
//- **`root`**: `パス`
//- **`methods`**: `METHOD1 METHOD2 ...`
//- **`return`**: `ステータスコード [URL]`
//- **`autoindex`**: `on|off`
//- **`index`**: `ファイル名1 ファイル名2 ...`
//- **`cgi_extension`**: `拡張子1 拡張子2 ...`
//- **`upload_enable`**: `on|off`
//- **`upload_store`**: `パス`


/*
以下に、指定されたディレクティブの中で**複数の引数を取るもの**を一覧にまとめました。それぞれのディレクティブがどのような複数引数を受け取るのか、簡潔に説明します。

---

## 複数の引数を取るディレクティブ一覧

### 1. `server_name`

- **フォーマット**:
  ```nginx
  server_name ドメイン名1 ドメイン名2 ...;
  ```
- **例**:
  ```nginx
  server_name example.com www.example.com api.example.com;
  ```
- **説明**:
  このディレクティブは、サーバーブロックが応答する複数のドメイン名を指定します。ワイルドカードや正規表現も利用可能です。

### 2. `error_page`

- **フォーマット**:
  ```nginx
  error_page ステータスコード1 ステータスコード2 ... パス;
  ```
- **例**:
  ```nginx
  error_page 404 500 502 /custom_error.html;
  ```
- **説明**:
  指定した複数のHTTPステータスコードに対して、共通のカスタムエラーページを設定します。これにより、異なるエラーコードに対して同一のエラーページを表示できます。

### 3. `index`

- **フォーマット**:
  ```nginx
  index ファイル名1 ファイル名2 ...;
  ```
- **例**:
  ```nginx
  index index.html index.htm default.html;
  ```
- **説明**:
  ディレクトリにアクセスされた際に表示されるデフォルトのインデックスファイルを複数指定します。サーバーは指定された順にファイルの存在を確認し、最初に見つかったファイルを表示します。

### 4. `methods`

- **フォーマット**:
  ```nginx
  methods METHOD1 METHOD2 ...;
  ```
- **例**:
  ```nginx
  methods GET POST DELETE;
  ```
- **説明**:
  特定の`location`ブロック内で許可するHTTPメソッドを複数指定します。これにより、許可されたメソッドのみがその`location`に対して有効になります。

### 5. `return`

- **フォーマット**:
  ```nginx
  return ステータスコード [URL];
  ```
- **例**:
  ```nginx
  return 301 https://www.example.com$request_uri;
  return 404 "Not Found";
  ```
- **説明**:
  クライアントに対して指定したHTTPステータスコードと、オプションでリダイレクト先のURLやメッセージを返します。`return`ディレクティブは、単純なリダイレクトやエラーメッセージの返却に使用されます。

### 6. `cgi_extension`

- **フォーマット**:
  ```nginx
  cgi_extension .拡張子1 .拡張子2 ...;
  ```
- **例**:
  ```nginx
  cgi_extension .php .py .cgi;
  ```
- **説明**:
  指定した複数のファイル拡張子を持つリクエストに対して、CGIスクリプトとして外部プログラムを実行します。これにより、動的なコンテンツの生成が可能となります。

---

## `location` ブロック内のディレクティブ

### 1. `location` ブロック自体

- **フォーマット**:
  ```nginx
  location パス {
      # ロケーションブロック内のディレクティブ
  }
  ```
- **例**:
  ```nginx
  location /images/ {
      # /images/ に対する設定
  }
  ```
- **説明**:
  特定のURLパスに対する処理を定義するブロックです。パスにマッチするリクエストに対して、内部のディレクティブが適用されます。

---

## その他のディレクティブ

以下のディレクティブは、複数の引数を取る場合がありますが、上記の一覧には含まれていません。必要に応じて追加の検討を行ってください。

### `listen`

- **フォーマット**:
  ```nginx
  listen [IPアドレス:]ポート [オプション1 オプション2 ...];
  ```
- **例**:
  ```nginx
  listen 80;
  listen 127.0.0.1:8080 ssl;
  ```
- **説明**:
  サーバーがリッスンするIPアドレスとポートを指定します。オプションとして`ssl`や`default_server`などを複数指定できます。

---

## まとめ

以下は、**複数の引数を取る主要なディレクティブ**の一覧です：

1. **`server_name`**
   - 複数のドメイン名を指定可能。
2. **`error_page`**
   - 複数のステータスコードに対して同一のエラーページを設定可能。
3. **`index`**
   - 複数のインデックスファイル名を指定可能。
4. **`methods`**
   - 複数のHTTPメソッドを指定可能。
5. **`return`**
   - ステータスコードに加えてオプションでURLやメッセージを指定可能。
6. **`cgi_extension`**
   - 複数のファイル拡張子を指定可能。

これらのディレクティブを適切に設定することで、柔軟かつ強力なHTTPサーバーの構成が可能となります。各ディレクティブの引数の取り扱いを正確に実装し、設定ファイルの解析時に正しく検証することが重要です。

---

**補足**: 実装時には、これらのディレクティブの引数が適切に解析・検証されるよう、設定ファイルのパーサーや検証ロジックを慎重に設計してください。また、ディレクティブごとの相互関係や優先順位にも注意を払い、期待通りの動作を実現するよう心掛けましょう。
*/