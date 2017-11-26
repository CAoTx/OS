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
    Process::ProcessStatus changeProcessStatus(pid_t pd, Process::ProcessStatus newStatus);
    bool closeAble();
    Process* getLastFrontProcess();
    Process* getLastBackProcss();
    Process* getLastHaltedProcess();
    Process* getLastStoppedProcess();
    Process* getLastZombiProcess();
    //    Process* getProcess(int index);

    //ProcessHandler(const ProcessHandler& orig);
    virtual ~ProcessHandler();

private:
    std::vector<Process*> processes;
    std::map<pid_t, Process*> m_processes;


};

#endif /* PROCESSHANDLER_H */

