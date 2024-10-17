/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/17 17:49:54 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOST_HPP
#define HTTPPOST_HPP

#include "AHttpMethod.hpp"

class HttpPost : public AHttpMethod {
private:
  HttpPost(const HttpPost& obj);
  HttpPost& operator=(const HttpPost& obj);

public:
  HttpPost(void);
  ~HttpPost();

  void execute(HttpRequest& header, HttpResponse*& response);
};

#endif
