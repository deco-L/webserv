/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2025/01/27 13:39:41 by csakamot         ###   ########.fr       */
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

  void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response, std::pair<class Epoll&, std::vector<Event>&>& evnet) const;
  void execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response, std::pair<Epoll&, std::vector<Event>&>& event);
};

#endif
