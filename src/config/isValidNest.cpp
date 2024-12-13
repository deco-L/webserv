/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidNest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:10:01 by miyazawa.ka       #+#    #+#             */
/*   Updated: 2024/12/13 14:46:03 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "webserv.hpp"
#include "Error.hpp"

static std::vector<std::pair<size_t, size_t> > findBlockRanges(const std::string& content, const std::string& keyword)
{
	std::vector<std::pair<size_t, size_t> > blockRanges;
	size_t pos = 0;

	while (pos < content.size())
	{
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
		size_t openBracePos = content.find('{', keywordPos + keyword.length());

		if (openBracePos == std::string::npos)
		{
			pos = keywordPos + keyword.length();
			continue;
		}
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
		if (!braceStack.empty())
			return std::vector<std::pair<size_t, size_t> >();

		blockRanges.push_back(std::make_pair(openBracePos, closeBracePos));
		pos = closeBracePos + 1;
	}
	return blockRanges;
}

/* ====================
ネストが正しいことを確認
==================== */
// MARK: isValidNest

bool isValidNest(const std::string& content)
{
	std::vector<std::pair<size_t, size_t> > serverBlockRanges = findBlockRanges(content, "server");

	if (serverBlockRanges.empty())
		return false;

	std::vector<std::pair<size_t, size_t> > locationBlockRanges = findBlockRanges(content, "location");

	if (locationBlockRanges.empty())
		return true;

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
			return false;
	}

	for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt = serverBlockRanges.begin();
		serverIt != serverBlockRanges.end(); ++serverIt)
	{
		for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt2 = serverBlockRanges.begin();
			serverIt2 != serverBlockRanges.end(); ++serverIt2)
		{
			if (serverIt != serverIt2 && serverIt->first < serverIt2->first && serverIt->second > serverIt2->second)
				return false;
		}
	}

	for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt = serverBlockRanges.begin();
		serverIt != serverBlockRanges.end(); ++serverIt)
	{
		for (std::vector<std::pair<size_t, size_t> >::const_iterator serverIt2 = serverBlockRanges.begin();
			serverIt2 != serverBlockRanges.end(); ++serverIt2)
		{
			if (serverIt != serverIt2 && serverIt->first < serverIt2->first && serverIt->second > serverIt2->second)
				return false;
		}
	}

	return true;
}
