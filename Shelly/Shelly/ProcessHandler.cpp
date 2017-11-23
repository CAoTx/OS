#include "ProcessHandler.h"

ProcessHandler::ProcessHandler()
{
}

pid_t ProcessHandler::doFork()
{
    pid_t pid = fork();
    //If Parent
    if (pid != 0)
    {
        Process* newy = new Process(pid);
        m_processes.insert(std::pair<pid_t, Process*>(pid, newy));
    }
    return pid;

}

int ProcessHandler::timesForked()
{
    return m_processes.size();
}

Process* ProcessHandler::getProcess(pid_t pid)
{
    return m_processes[pid];
}

bool ProcessHandler::closeAble()
{


    //    for (int i = 0; i < m_processes.size(); i++)
    //    {
    //        if (m_processes[i]->getStatus() != Process::ProcessStatus::endet || m_processes[i]->getStatus() != Process::ProcessStatus::zombi)
    //            return false;
    //    }
    for (auto it : m_processes)
    {
        if (it.second->getStatus() != Process::ProcessStatus::endet || it.second->getStatus() != Process::ProcessStatus::zombi)
            return false;
    }


    return true;

}

ProcessHandler::~ProcessHandler()
{
}


//ProcessHandler::ProcessHandler(const ProcessHandler& orig)
//{
//}