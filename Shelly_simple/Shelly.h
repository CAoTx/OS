#ifndef SHELLY_H
#define SHELLY_H

#include <unistd.h> //exec
#include <cstdio> //fork?
#include <sys/wait.h> //waitpid

#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>

enum intyp
{
    ext, logout, enter, exec_foreg, exec_backg, hc_fg, hc_bg
};

enum pStatus
{
    sick = -2, zombi = -1, workFront = 1, halted = 2,
    stopped = 3, endet = 4, workBack = 11
};

struct Process
{
    pid_t pid;
    pStatus status;
    std::string instruction;
};

class Shelly
{
public:
    Shelly();
    Shelly(const Shelly& orig);

    std::string Interface();
    void prepare();
    bool execute();
    intyp inputType();
    void clean();
    std::vector< Process> getPHandler();
    bool closeAble();
    Process* getLastProcessOf(pStatus search);
    Process* getLastProcess();
    Process* getProcess(pid_t id);

    virtual ~Shelly();

private:
    pid_t doFork();

    char** xcute;
    int size;
    int length;
    std::string input;
    intyp iTyp;
    std::vector<std::string> inputMem;
    std::vector<Process> pHandler;

};

#endif /* SHELLY_H */

