/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidBracketFormat.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:09:01 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/12/13 14:52:45 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

bool isValidOpeningBracePosition(const std::string& content, size_t pos)
{
	const std::string serverKeyword = "server";
	const std::string locationKeyword = "location";

	size_t lineStart = content.rfind('\n', pos);
	if (lineStart == std::string::npos)
		lineStart = 0;
	else
		lineStart += 1;

	std::string line = content.substr(lineStart, pos - lineStart);

	line.erase(0, line.find_first_not_of(" \t"));
	line.erase(line.find_last_not_of(" \t") + 1);

	if (line.compare(0, serverKeyword.length(), serverKeyword) == 0)
	{
		return true;
	}
	else if (line.compare(0, locationKeyword.length(), locationKeyword) == 0)
	{
		size_t afterLocation = locationKeyword.length();

		afterLocation = line.find_first_not_of(" \t", afterLocation);
		if (afterLocation == std::string::npos)
			return false;
		
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

		if (!modifierFound.empty())
		{
			afterLocation = line.find_first_not_of(" \t", afterLocation);
			if (afterLocation == std::string::npos)
				return false;
		}

		size_t uriEnd = line.find_first_of(" \t{", afterLocation);
		if (uriEnd == std::string::npos)
			uriEnd = line.length();

		std::string uri = line.substr(afterLocation, uriEnd - afterLocation);

		if (uri.empty())
			return false;

		return true;
	}

	return false;
}

/* ====================
括弧の閉じ忘れをチェック
==================== */
// MARK: isValidBracketFormat

bool isValidBracketFormat(const std::string& content)
{
	std::stack<int> braceStack;

	for (size_t i = 0; i < content.size(); ++i)
	{
		if (content[i] == '{')
		{
			if (isValidOpeningBracePosition(content, i))
				braceStack.push(i);
			else
				return false;
		}
		else if (content[i] == '}')
		{
			if (braceStack.empty())
				return false;
			braceStack.pop();
		}
	}

	if (!braceStack.empty())
		return false;

	for (size_t i = 0; i < content.size(); ++i)
	{
		if (content[i] == '{' && !isValidOpeningBracePosition(content, i))
			return false;
	}

	return true;
}
