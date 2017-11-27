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

    pid_t doFork(std::string instruction, bool front);
    int timesForked();
    void addProcess(Process* newProcess);
    Process* getProcess(pid_t pid);
    Process::ProcessStatus changeProcessStatus(pid_t pd, Process::ProcessStatus newStatus);
    bool closeAble();
    Process* getLastFrontProcess();
    Process* getLastBackProcess();
    Process* getLastHaltedProcess();
    Process* getLastStoppedProcess();
    Process* getLastZombiProcess();
    Process* getLastEndedProcess();
    //    Process* getProcess(int index);

    //ProcessHandler(const ProcessHandler& orig);
    virtual ~ProcessHandler();

private:
    std::map<pid_t, Process*> m_processes;


};

#endif /* PROCESSHANDLER_H */

