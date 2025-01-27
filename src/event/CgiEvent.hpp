#ifndef CGIEVENT_HPP
#define CGIEVENT_HPP

#include <vector>

struct CgiEvent {
    int _pid;
    std::vector<int> _readFd;

    CgiEvent(void): _pid(0), _readFd() {};
    CgiEvent(int pid, std::vector<int> readFd): _pid(pid), _readFd(readFd) {};
};

#endif
