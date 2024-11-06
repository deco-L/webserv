/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/03 23:03:12 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "webserv.hpp"

// パースのモードを表す列挙型
enum ParseMode {
    MODE_GLOBAL = 0,
    MODE_SERVER = 1,
    MODE_LOCATION = 2
};

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

struct ConfigLocation {
    std::string path;
    std::string root;
    std::vector<std::string> methods;
    std::pair<int, std::string> return_;
    bool autoindex;
    std::vector<std::string> index;
    std::vector<std::pair<std::string, std::string> > cgi_extension; // 拡張子とパス
    bool upload_enable;
    std::string upload_store;
    
    ConfigLocation(void) : return_(0, ""), autoindex(false), upload_enable(false) {}
};

struct ConfigServer {
    std::vector<std::pair<std::string, std::string> > listen; // IPアドレスとポート番号
    std::vector<std::string> server_name;
    std::vector<std::pair<int, std::string> > error_page; // ステータスコードとパス
    unsigned long client_max_body_size; // 0~1024GB 0=無制限
    std::string root;
    std::vector<std::string> index;
    bool autoindex;
    std::vector<ConfigLocation> locations;
    
    ConfigServer(void) : client_max_body_size(0), autoindex(false) {}
};

class Config {
private:
  int         _argc;
  std::string _file_path;
  std::string _file_content;
  std::vector<ConfigServer> _servers;

public:
  Config(void);
  Config(int argc, std::string argv);
  ~Config();

  int         getArgc(void) const;
  std::string getFileName(void) const;
  const std::vector<ConfigServer>& getServers(void) const;
  void        checkNbrArg(void) const;
  void        checkConfig(void);
  bool        isValidConfig(std::string file_content);
  void        parseConfig(void);

  Config&      operator=(const Config& obj);
};

#endif

