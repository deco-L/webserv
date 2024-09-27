/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/27 21:09:53 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

namespace mylib{
  size_t	strlen(const char *str) {
    size_t	len;

    len = 0;
    while (str[len] != '\0')
      len++;
    return (len);
  }

  std::vector<std::string> split(const std::string& s, const std::string& del) {
      std::vector<std::string> tokens;
      if (del.empty()) {
          tokens.push_back(s);
          return tokens;
      }

      size_t pos = 0;
      size_t prev = 0;

      while ((pos = s.find(del, prev)) != std::string::npos) {
          // 空のトークンを避けるため、トークンが空でない場合のみ追加
          if (pos > prev) {
              tokens.push_back(s.substr(prev, pos - prev));
          }
          // prev をデリミタの次の位置に更新（連続するデリミタを無視）
          prev = pos + del.length();
      }

      // 最後のトークンを追加（文字列の末尾にデリミタがない場合）
      if (prev < s.length()) {
          tokens.push_back(s.substr(prev));
      }

      return tokens;
  }

}
