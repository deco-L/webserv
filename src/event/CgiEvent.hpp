#ifndef CGIEVENT_HPP
#define CGIEVENT_HPP

#include <vector>

struct CgiEvent {
    int _pid;
    std::vector<int> _readFd;
    std::vector<int> _writeFd;
    std::string _writeBuf;

    CgiEvent(void): _pid(0), _readFd(), _writeFd(), _writeBuf() {};
    CgiEvent(int pid, std::vector<int> readFd):
    _pid(pid),
    _readFd(readFd),
    _writeFd(),
    _writeBuf() {};
    CgiEvent(int pid, std::vector<int> readFd, std::vector<int> writeFd, std::string writeBuf):
    _pid(pid),
    _readFd(readFd),
    _writeFd(writeFd),
    _writeBuf(writeBuf) {};
};

#endif
