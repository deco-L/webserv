/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/29 01:18:11 by miyazawa.ka      ###   ########.fr       */
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

  void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response, std::pair<class Epoll*, std::vector<Event>*>& event) const;
  void execute(const ConfigServer& config, HttpRequest& header, HttpResponse*& response, std::pair<Epoll*, std::vector<Event>*>& event);
};

#endif