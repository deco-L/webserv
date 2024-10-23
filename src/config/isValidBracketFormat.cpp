/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidBracketFormat.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:09:01 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/10/18 13:52:48 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

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

/* ====================
括弧の閉じ忘れをチェック
==================== */
// MARK: isValidBracketFormat

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
