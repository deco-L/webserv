/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:47:46 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/09/27 21:06:12 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"


//static bool bracesCheck(const std::string &str) {
//	std::stack<int> braces;
//	for (size_t i = 0; i < str.size(); ++i) {
//		if (str[i] == '{') {
//			// '{' の前に "server " または "location /" があるかをチェック
//			bool isValid = false;
//			if (i > 6 && str.substr(i - 7, 7) == "server ") {
//				isValid = true;
//			} else if (i > 10) { // "location /" のために少なくとも10文字必要
//				size_t locationPos = str.rfind("location ", i - 1);
//				if (locationPos != std::string::npos && locationPos + 9 < i && str[locationPos + 9] == '/') {
//					// "location" の後に '/' があり、その後にパスが続いているか確認
//					isValid = str[i - 1] == ' ';
//				}
//			}
//			if (isValid) {
//				braces.push(i);
//			} else {
//				return false; // 無効な '{' の配置
//			}
//		} else if (str[i] == '}') {
//			if (braces.empty()) {
//				return false; // 対応する '{' がない
//			}
//			braces.pop();
//		}
//	}
//	return braces.empty(); // スタックが空なら正しく対応している
//}

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

    return true;
}




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

static bool areLocationsNestedInServers(const std::string& content)
{
    std::vector<std::pair<size_t, size_t> > serverBlockRanges = findBlockRanges(content, "server");

    if (serverBlockRanges.empty())
        return false; // サーバーブロックが見つからない、または中括弧が不一致

    std::vector<std::pair<size_t, size_t> > locationBlockRanges = findBlockRanges(content, "location");

    if (locationBlockRanges.empty())
        return true; // location ブロックがない場合は正しくネストされているとみなす

    /* ネストのチェックをブロック範囲を使って簡略化 */
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


    return true; // すべての location ブロックが server ブロック内にネストされている
}








// 文字列が特定のプレフィックスで始まるかをチェックするヘルパー関数
bool startsWith(const std::string &str, const std::string &prefix) {
    return str.compare(0, prefix.size(), prefix) == 0;
}

static bool validateDirectiveCounts(const std::vector<std::string> &lines) {
    std::vector<ServerDirectiveCounts> servers;
    ServerDirectiveCounts *currentServer = NULL;
    LocationDirectiveCounts *currentLocation = NULL;
    ParseMode mode = MODE_NONE;

    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        const std::string &line = *it;

        if (startsWith(line, "server {")) {
            mode = MODE_SERVER;
            servers.push_back(ServerDirectiveCounts());
            currentServer = &servers.back();
            currentLocation = NULL;
        } else if (startsWith(line, "location ")) {
            if (currentServer == NULL) {
                std::cout << "ERROR: Found a location block outside of a server block!" << std::endl;
                return false;
            }
            mode = MODE_LOCATION;
            currentServer->locationDirectives.push_back(LocationDirectiveCounts());
            currentLocation = &currentServer->locationDirectives.back();
        } else if (startsWith(line, "}")) {
            if (mode == MODE_LOCATION) {
                mode = MODE_SERVER;
                currentLocation = NULL;
            } else if (mode == MODE_SERVER) {
                mode = MODE_NONE;
                currentServer = NULL;
            } else {
                std::cout << "ERROR: Unexpected closing brace!" << std::endl;
                return false;
            }
        } else if (mode == MODE_SERVER) {
            if (startsWith(line, "listen ")) {
                currentServer->listen += 1;
            } else if (startsWith(line, "host ")) {
                currentServer->host += 1;
            } else if (startsWith(line, "root ")) {
                currentServer->root += 1;
            } else if (startsWith(line, "server_name ")) {
                currentServer->serverName += 1;
            } else if (startsWith(line, "error_page ")) {
                currentServer->errorPage += 1;
            } else if (startsWith(line, "client_max_body_size ")) {
                currentServer->clientMaxBodySize += 1;
            } else if (startsWith(line, "default_server ")) {
                currentServer->default_server += 1;
            } else {
                std::cout << "ERROR: \"" << line << "\" is inappropriate as an element of the server block!" << std::endl;
                return false;
            }
        } else if (mode == MODE_LOCATION) {
            if (startsWith(line, "root ")) {
                currentLocation->root += 1;
            } else if (startsWith(line, "index ")) {
                currentLocation->index += 1;
            } else if (startsWith(line, "autoindex ")) {
                currentLocation->autoindex += 1;
            } else if (startsWith(line, "cgi_extension ")) {
                currentLocation->cgi_extension += 1;
            } else if (startsWith(line, "cgi_handler ")) {
                currentLocation->cgi_handler += 1;
            } else if (startsWith(line, "methods ")) {
                currentLocation->methods += 1;
            } else if (startsWith(line, "upload_directory ")) {
                currentLocation->upload_directory += 1;
            } else if (startsWith(line, "client_max_body_size ")) {
                currentLocation->clientMaxBodySize += 1;;
            } else if (startsWith(line, "limit_except ")) {
                currentLocation->limitExcept += 1;
            } else {
                std::cout << "ERROR: \"" << line << "\" is inappropriate as an element of the location block!" << std::endl;
                return false;
            }
        } else {
            // Lines outside of any block are ignored or can be processed as needed
        }
    }

    // Validate the counts for each server and its locations
    for (std::vector<ServerDirectiveCounts>::const_iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt) {
        const ServerDirectiveCounts &server = *serverIt;

        if (server.listen != 1) {
            std::cout << "ERROR: The number of 'listen' directives is " << server.listen << "; expected 1." << std::endl;
            return false;
        }
        if (server.host > 1) {
            std::cout << "ERROR: The number of 'host' directives is " << server.host << "; expected 0 or 1." << std::endl;
            return false;
        }
        if (server.root > 1) {
            std::cout << "ERROR: The number of 'root' directives is " << server.root << "; expected 1." << std::endl;
            return false;
        }
        if (server.serverName > 1) {
            std::cout << "ERROR: The number of 'server_name' directives is " << server.serverName << "; expected 1." << std::endl;
            return false;
        }
        if (server.clientMaxBodySize > 1) {
            std::cout << "ERROR: The number of 'client_max_body_size' directives is " << server.clientMaxBodySize << "; expected 0 or 1." << std::endl;
            return false;
        }
        if (server.default_server > 1) {
            std::cout << "ERROR: The number of 'default_server' directives is " << server.default_server << "; expected 0 or 1." << std::endl;
            return false;
        }

        for (std::vector<LocationDirectiveCounts>::const_iterator locIt = server.locationDirectives.begin(); locIt != server.locationDirectives.end(); ++locIt) {
            const LocationDirectiveCounts &location = *locIt;

            if (location.root > 1) {
                std::cout << "ERROR: The number of 'root' directives in the location block is " << location.root << "; expected 0 or 1." << std::endl;
                return false;
            }
            if (location.index > 1) {
                std::cout << "ERROR: The number of 'index' directives in the location block is " << location.index << "; expected 0 or 1." << std::endl;
                return false;
            }
            if (location.autoindex > 1) {
                std::cout << "ERROR: The number of 'autoindex' directives in the location block is " << location.autoindex << "; expected 0 or 1." << std::endl;
                return false;
            }
            if (location.cgi_extension > 1) {
                std::cout << "ERROR: The number of 'cgi_extension' directives in the location block is " << location.cgi_extension << "; expected 0 or 1." << std::endl;
                return false;
            }
            if (location.cgi_handler > 1) {
                std::cout << "ERROR: The number of 'cgi_handler' directives in the location block is " << location.cgi_handler << "; expected 0 or 1." << std::endl;
                return false;
            }
            if (location.methods > 1) {
                std::cout << "ERROR: The number of 'methods' directives in the location block is " << location.methods << "; expected 0 or 1." << std::endl;
                return false;
            }
            if (location.upload_directory > 1) {
                std::cout << "ERROR: The number of 'upload_directory' directives in the location block is " << location.upload_directory << "; expected 0 or 1." << std::endl;
                return false;
            }
            if (location.clientMaxBodySize > 1) {
                std::cout << "ERROR: The number of 'client_max_body_size' directives in the location block is " << location.clientMaxBodySize << "; expected 0 or 1." << std::endl;
                return false;
            }
        }
    }
    return true;
}




// 文字列が数字のみで構成されているかをチェックする関数
static bool isNumeric(const std::string &str) {
    if (str.empty())
        return false;
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(str[i])))
            return false;
    }
    return true;
}

// 文字列を整数に変換する関数
static int stringToInt(const std::string &str) {
    std::istringstream iss(str);
    int number = 0;
    iss >> number;
    if (iss.fail())
        return -1; // 変換エラーを示す
    return number;
}

// IPアドレスの形式をチェックする関数
static bool isValidIpAddress(const std::string &ip) {
    std::istringstream ss(ip);
    std::string token;
    int count = 0;
    while (std::getline(ss, token, '.')) {
        if (!isNumeric(token))
            return false;
        int num = stringToInt(token);
        if (num < 0 || num > 255 || (token.length() > 1 && token[0] == '0'))
            return false;
        ++count;
    }
    return count == 4; // IPv4アドレスは4つの数値で構成される
}

// ポート番号の妥当性をチェックする関数
static bool isValidPort(int port) {
    return port > 0 && port <= 65535;
}

// 設定ディレクティブを検証する関数
static bool validateConfiguration(const std::vector<std::string> &lines) {
    std::vector<std::string> tokens;
    std::vector<std::string>::const_iterator it = lines.begin();
    for (; it != lines.end(); ++it) {
        tokens = mylib::split(*it, " ");
        if (tokens.empty())
            continue; // 空行をスキップ

        const std::string &directive = tokens[0];

        if (directive == "port") {
            if (tokens.size() != 2 || !isNumeric(tokens[1]) || !isValidPort(stringToInt(tokens[1]))) {
                std::cout << "ERROR: Invalid port value!" << std::endl;
                return false;
            }
        } else if (directive == "host") {
            if (tokens.size() != 2 || !isValidIpAddress(tokens[1])) {
                std::cout << "ERROR: Invalid host value!" << std::endl;
                return false;
            }
        } else if (directive == "root") {
            if (tokens.size() != 2 || mylib::check_access(tokens[1].c_str()) != 2) {
                std::cout << "ERROR: Invalid root value!" << std::endl;
                return false;
            }
        } else if (directive == "server_name") {
            if (tokens.size() < 2) {
                std::cout << "ERROR: Invalid server_name value!" << std::endl;
                return false;
            }
        } else if (directive == "error_page") {
            if (tokens.size() < 3) {
                std::cout << "ERROR: Invalid error_page value!" << std::endl;
                return false;
            }
            if (!isNumeric(tokens[1]) || tokens[1].length() != 3) {
                std::cout << "ERROR: Invalid error_page code!" << std::endl;
                std::cout << tokens[2] << std::endl;
                return false;
            }
            int errorCode = stringToInt(tokens[1]);
            if (!(errorCode == 400 || errorCode == 403 || errorCode == 404 || errorCode == 405 ||
                  errorCode == 408 || errorCode == 411 || errorCode == 413 || errorCode == 414 ||
                  errorCode == 415 || errorCode == 429 || errorCode == 431 || errorCode == 500 ||
                  errorCode == 501 || errorCode == 502 || errorCode == 504 || errorCode == 505)) {
                std::cout << "ERROR: Invalid error_page code!" << std::endl;
                return false;
            }
            if (mylib::check_access(tokens[tokens.size()-1].c_str()) != 1) {
                std::cout << "ERROR: Error page file does not exist!" << std::endl;
                std::cout << tokens[tokens.size()-1] << std::endl;
                return false;
            }
        } else if (directive == "client_max_body_size") {
            if (tokens.size() != 2) {
                std::cout << "ERROR: Invalid client_max_body_size value!" << std::endl;
                return false;
            }
            char lastChar = tokens[1][tokens[1].length() - 1];
            if (!(lastChar == 'M' || lastChar == 'K' || lastChar == 'G' || std::isdigit(static_cast<unsigned char>(lastChar)))) {
                std::cout << "ERROR: Invalid client_max_body_size unit!" << std::endl;
                return false;
            }
            std::string sizePart = tokens[1].substr(0, tokens[1].length() - (std::isdigit(static_cast<unsigned char>(lastChar)) ? 0 : 1));
            if (!isNumeric(sizePart)) {
                std::cout << "ERROR: Invalid client_max_body_size value!" << std::endl;
                return false;
            }
        } else if (directive == "index") {
            if (tokens.size() < 2) {
                std::cout << "ERROR: Invalid index value!" << std::endl;
                return false;
            }
        } else if (directive == "autoindex") {
            if (tokens.size() != 2 || (tokens[1] != "on" && tokens[1] != "off")) {
                std::cout << "ERROR: Invalid autoindex value!" << std::endl;
                return false;
            }
        } else if (directive == "cgi") {
            if (tokens.size() < 2 || tokens.size() > 6) {
                std::cout << "ERROR: Invalid cgi value!" << std::endl;
                return false;
            }
            for (size_t i = 1; i < tokens.size(); ++i) {
                if (!(tokens[i] == "cgi" || tokens[i] == "py" || tokens[i] == "rb" || tokens[i] == "pl" || tokens[i] == "sh")) {
                    std::cout << "ERROR: Invalid cgi extension!" << std::endl;
                    return false;
                }
            }
        } else if (directive == "allow_methods") {
            if (tokens.size() < 2 || tokens.size() > 4) {
                std::cout << "ERROR: Invalid allow_methods value!" << std::endl;
                return false;
            }
            for (size_t i = 1; i < tokens.size(); ++i) {
                if (!(tokens[i] == "GET" || tokens[i] == "POST" || tokens[i] == "DELETE")) {
                    std::cout << "ERROR: Invalid allow_methods method!" << std::endl;
                    return false;
                }
            }
        } else if (directive == "redirect") {
            if (tokens.size() != 2) {
                std::cout << "ERROR: Invalid redirect value!" << std::endl;
                return false;
            }
        } else if (directive == "upload") {
            if (tokens.size() != 2 || mylib::check_access(tokens[1].c_str()) != 2) {
                std::cout << "ERROR: Invalid upload directory!" << std::endl;
                return false;
            }
        } else if (directive == "limit_except") {
            // limit_exceptの後に続くHTTPメソッドを検証
            if (tokens.size() < 2) {
                std::cout << "ERROR: Invalid limit_except value!" << std::endl;
                return false;
            }
            for (size_t i = 1; i < tokens.size(); ++i) {
                if (!(tokens[i] == "GET" || tokens[i] == "POST" || tokens[i] == "PUT" || tokens[i] == "DELETE" ||
                      tokens[i] == "PATCH" || tokens[i] == "HEAD" || tokens[i] == "OPTIONS")) {
                    std::cout << "ERROR: Invalid HTTP method in limit_except!" << std::endl;
                    return false;
                }
            }
        }
        // 必要に応じて他のディレクティブのチェックを追加
    }
    return true;
}




bool Config::isValidConfig(std::string file_content)
{

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

  if (areLocationsNestedInServers(file_content) == false)
  {
    outputError("Error: Locations must be nested in servers.");
    return false;
  }

  std::vector<std::string> lines;

  lines = mylib::split(file_content, "\n");

  if (validateDirectiveCounts(lines) == false)
    return false;

  //std::string root_path = getRoot();

  if (validateConfiguration(lines) == false)
    return false;



  return true;
}





