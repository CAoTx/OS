#include "ProcessHandler.h"

ProcessHandler::ProcessHandler()
{
}

pid_t ProcessHandler::doFork()
{
    pid_t pid = fork();
    pidS.push_back(pid);
    return pid;
}

int ProcessHandler::timesForked()
{
    return pidS.size();
}

ProcessHandler::~ProcessHandler()
{
}


//ProcessHandler::ProcessHandler(const ProcessHandler& orig)
//{
//}