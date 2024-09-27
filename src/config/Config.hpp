/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/27 17:30:23 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "webserv.hpp"

// パースのモードを表す列挙型
enum ParseMode {
    MODE_NONE = 0,
    MODE_SERVER,
    MODE_LOCATION
};

// ロケーションブロック内のディレクティブの数を保持する構造体
struct LocationDirectiveCounts {
    size_t root;
    size_t index;
    size_t autoindex;
    size_t cgi_extension;
    size_t cgi_handler;
    size_t methods;
    size_t upload_directory;
    size_t clientMaxBodySize;  // location内のclient_max_body_size
    size_t limitExcept;        // location内のlimit_except

    LocationDirectiveCounts()
        : root(0), index(0), autoindex(0), cgi_extension(0), cgi_handler(0),
          methods(0), upload_directory(0), clientMaxBodySize(0) {}
};

// サーバーブロック内のディレクティブの数を保持する構造体
struct ServerDirectiveCounts {
    size_t listen;
    size_t host;
    size_t root;
    size_t serverName;
    size_t errorPage;
    size_t clientMaxBodySize;
    size_t default_server;
    std::vector<LocationDirectiveCounts> locationDirectives;

    ServerDirectiveCounts()
        : listen(0), host(0), root(0), serverName(0), errorPage(0),
          clientMaxBodySize(0), default_server(0) {}
};






struct ConfigServer {
	std::string	root;
	std::string host;
	std::string	port;
	std::string serverName;
	bool defaultServer;
	std::map<int, std::string> errorPages;
	size_t limitBodySize;

};

class Config {
private:
  int         _argc;
  std::string _file_path;
  std::vector<ConfigServer> _servers;

public:
  Config(void);
  Config(int argc, std::string argv);
  ~Config();

  int         getArgc(void) const;
  std::string getFileName(void) const;
  std::vector<ConfigServer> getServers(void) const;
  void        checkNbrArg(void) const;
  void        parseConfig(void);
  bool isValidConfig(std::string file_content);

  Config&      operator=(const Config& obj);
};

#endif
