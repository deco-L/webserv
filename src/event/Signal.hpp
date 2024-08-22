/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/22 14:06:06 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <errno.h>
#include <csignal>

class Signal {
private:
  volatile sig_atomic_t _signal;
  void                  signal_handler(int signal, siginfo_t *info, void *ucontext);

public:
  Signal(volatile sig_atomic_t status);
  ~Signal();

  volatile sig_atomic_t getStatus(void) const;
  void                  addSignal(volatile sig_atomic_t signal);
};

#endif
