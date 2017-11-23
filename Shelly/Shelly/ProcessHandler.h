#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H

#include <unistd.h> //exec
#include <cstdio> //fork?
#include <sys/wait.h> //waitpid

#include "Process.h"

class ProcessHandler
{
public:
    ProcessHandler();

    pid_t doFork();
    int timesForked();
    Process* getProcess(pid_t pid);
    bool closeAble();
//    Process* getProcess(int index);

    //ProcessHandler(const ProcessHandler& orig);
    virtual ~ProcessHandler();

private:
    std::vector<Process*> processes; 
    std::map<pid_t, Process*> m_processes;
    

};

#endif /* PROCESSHANDLER_H */

