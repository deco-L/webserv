/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/27 13:40:21 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOST_HPP
#define HTTPPOST_HPP

#include <fstream>
#include "AHttpMethod.hpp"

class HttpPost : public AHttpMethod {
private:
  HttpPost(void);

  bool _uploadFile(HttpRequest& request);

public:
  HttpPost(std::string uri, std::string version);
  HttpPost(const AHttpMethod& obj);
  HttpPost(const HttpPost& obj);
  HttpPost& operator=(const HttpPost& obj);
  ~HttpPost();

  void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response, std::pair<class Epoll&, std::vector<Event>&>& event) const;
  void execute(const ConfigServer& config, HttpRequest& header, HttpResponse*& response, std::pair<Epoll&, std::vector<Event>&>& event);
};

#endif