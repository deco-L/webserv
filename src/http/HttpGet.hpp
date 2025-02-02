/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/29 01:17:53 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGET_HPP
#define HTTPGET_HPP

#include "AHttpMethod.hpp"

struct Event;
class Epoll;

class HttpGet : public AHttpMethod {
private:
  HttpGet(void);

public:
  HttpGet(std::string uri, std::string version);
  HttpGet(const AHttpMethod& obj);
  HttpGet(const HttpGet& obj);
  HttpGet& operator=(const HttpGet& obj);
  ~HttpGet();

  void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response, std::pair<class Epoll*, std::vector<Event>*>& event) const;
  void execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response, std::pair<Epoll*, std::vector<Event>*>& event);
};

#endif
