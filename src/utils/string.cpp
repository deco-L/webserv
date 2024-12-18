/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/18 14:12:22 by csakamot         ###   ########.fr       */
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

  bool isNumeric(const std::string &str) {
    if (str.empty())
      return false;
    for (size_t i = 0; i < str.length(); ++i) {
      if (!std::isdigit(static_cast<unsigned char>(str[i])))
        return false;
    }
    return true;
  }

  short stringToShort(const std::string &str) {
    std::istringstream iss(str);
    short number = 0;
    iss >> number;
    if (iss.fail())
      return (-1);
    return (number);
  }

  int stringToInt(const std::string &str) {
    std::istringstream iss(str);
    int number = 0;
    iss >> number;
    if (iss.fail())
      return -1; // 変換エラーを示す
    return number;
  }

  unsigned long stringToULong(const std::string &str) {
    std::istringstream iss(str);
    unsigned long number = 0;
    iss >> number;
    if (iss.fail())
      return -1; // 変換エラーを示す
    return number;
  }

  size_t stringToSize(const std::string& str) {
    std::istringstream iss(str);
    size_t number = 0;
    iss >> number;
    if (iss.fail())
      return -1;
    return number;
  }

  PathType getPathType(const std::string& path) {
    struct stat pathStat;
    // stat 関数はファイルの情報を取得します。成功すると 0 を返します。
    if (stat(path.c_str(), &pathStat) != 0) {
        // パスが存在しない、またはアクセスできない場合
        return NOT_EXIST;
    }

    if (S_ISREG(pathStat.st_mode)) {
        // 通常のファイル
        return IS_FILE;
    } else if (S_ISDIR(pathStat.st_mode)) {
        // ディレクトリ
        return IS_DIRECTORY;
    } else {
        // その他のタイプ（例: シンボリックリンク、デバイスファイルなど）
        return OTHER;
    }
  }
}
