#include "ProcessHandler.h"

ProcessHandler::ProcessHandler() {
}

pid_t ProcessHandler::doFork(std::string instruction, bool front ) {

    pid_t pid = fork();
    setpgid(pid, pid); //separate Parent & Child Process into separat ProcessGroups   
    //Ab Hier gehts zum SIGCHLT
    if (pid < 0) {
        std::cerr << "Failed of Fork" << std::endl;
        exit(0);
    }//If Parent
    else if (pid > 0) {
        Process* newy = new Process(pid, instruction,  front);
        m_processes.insert(std::pair<pid_t, Process*>(pid, newy));
    }
    return pid;
}

int ProcessHandler::timesForked() {
    return m_processes.size();
}

Process* ProcessHandler::getProcess(pid_t pid) {
    return m_processes[pid];
}

Process::ProcessStatus ProcessHandler::changeProcessStatus(pid_t pd, Process::ProcessStatus newStatus) {
    for (int i = 0; i < m_processes.size(); i++) {
        if (m_processes.at(i)->getPid() == pd)
            m_processes.at(i)->changeStatus(newStatus);
    }
}

bool ProcessHandler::closeAble() {
    for (auto it : m_processes) {
        if (it.second->getStatus() != Process::ProcessStatus::endet || it.second->getStatus() != Process::ProcessStatus::zombi)
            return false;
    };
    return true;
}

Process* ProcessHandler::getLastFrontProcess() {
    if (m_processes.size() > 0) {
        for (int i = m_processes.size()-1; i > 0; i--) {
            if (m_processes.at(i)->getStatus() == Process::ProcessStatus::workFront)
                return m_processes.at(i);
        }
    }
    return nullptr;
}

Process* ProcessHandler::getLastBackProcess() {
    if (m_processes.size() > 0) {
        for (int i = m_processes.size()-1; i > 0; i--) {
            if (m_processes.at(i)->getStatus() == Process::ProcessStatus::workBack)
                return m_processes.at(i);
        }
    }
    return nullptr;
}

Process* ProcessHandler::getLastHaltedProcess() {
    if (m_processes.size() > 0) {
        for (int i = m_processes.size()-1; i > 0; i--) {
            if (m_processes.at(i)->getStatus() == Process::ProcessStatus::halted)
                return m_processes.at(i);
        }
    }
    return nullptr;
}

Process* ProcessHandler::getLastStoppedProcess() {
    if (m_processes.size() > 0) {
        for (int i = m_processes.size()-1; i > 0; i--) {
            if (m_processes.at(i)->getStatus() == Process::ProcessStatus::stopped)
                return m_processes.at(i);
        }
    }
    return nullptr;
}

Process* ProcessHandler::getLastZombiProcess() {
    if (m_processes.size() > 0) {
        for (int i = m_processes.size()-1; i > 0; i--) {
            if (m_processes.at(i)->getStatus() == Process::ProcessStatus::zombi)
                return m_processes.at(i);
        }
    }
    return nullptr;
}

Process* ProcessHandler::getLastEndedProcess() {
 if (m_processes.size() > 0) {
        for (int i = m_processes.size()-1; i > 0; i--) {
            if (m_processes.at(i)->getStatus() == Process::ProcessStatus::endet)
                return m_processes.at(i);
        }
    }
    return nullptr;
}


void ProcessHandler::addProcess(Process* newProcess) {
    this->m_processes.insert(std::pair<pid_t, Process*>(newProcess->getPid(), newProcess));
}

ProcessHandler::~ProcessHandler() {
}


//ProcessHandler::ProcessHandler(const ProcessHandler& orig)
//{
//}