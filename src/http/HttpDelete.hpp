/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/29 01:26:49 by miyazawa.ka      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETE_HPP
#define HTTPDELETE_HPP

#include "AHttpMethod.hpp"
#include <cstdio>

struct Event;
class Epoll;

class HttpDelete : public AHttpMethod {
private:
  HttpDelete(void);

public:
  HttpDelete(std::string uri, std::string version);
  HttpDelete(const AHttpMethod& obj);
  HttpDelete(const HttpDelete& obj);
  HttpDelete& operator=(const HttpDelete& obj);
  ~HttpDelete();

  void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response, std::pair<class Epoll*, std::vector<Event>*>& event) const;
  void execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response, std::pair<Epoll*, std::vector<Event>*>& event);
};

#endif
