#ifndef CGIEVENT_HPP
#define CGIEVENT_HPP

struct CgiEvent {
    int _pid;
    int _readFd;

    CgiEvent(void): _pid(0), _readFd(0) {};
    CgiEvent(int pid, int readFd): _pid(pid), _readFd(readFd) {};
};

#endif
