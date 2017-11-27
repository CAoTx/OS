#include "Process.h"

Process::Process(pid_t myID, std::string instruction)
{

    start = time_t(NULL);
    if (start == -1)
    {
        status = ProcessStatus::sick;
        throw "Failure in Process(): StartTime = ";
    }
    status = ProcessStatus::workFront;
    pid = myID;

    Event eventStart{("Start of this Process, " + getStringStatus(status)) +" "+instruction, time(nullptr)};
    tLog.insert(std::pair<std::string, Event>("start", eventStart));
}

void Process::addEvent(std::string text)
{
    std::string eventTyp = "event";
    eventTyp.push_back(static_cast<char> (eventCtr));
    Event newEve{text, time(nullptr)};
    tLog.insert(std::pair<std::string, Event>(eventTyp, newEve));
}

std::string Process::getStringStatus(ProcessStatus stat)
{
    if (stat == ProcessStatus::workFront)
        return "normal";
    else if (stat == ProcessStatus::halted)
        return "halted";
    else if (stat == ProcessStatus::stopped)
        return "stop";
    else if (stat == ProcessStatus::endet)
        return "endet";
    else if (stat == ProcessStatus::sick)
        return "sick";
    else if (stat == ProcessStatus::workFront)
        return "zombi";
    else if (stat == ProcessStatus::workBack)
        return "normalBack";
}

Process::ProcessStatus Process::changeStatus(ProcessStatus newStat)
{
    //maybe a check of correctnisss before
    ProcessStatus actual = this->status;
    this->status = newStat;
    return actual;
}

Process::ProcessStatus Process::getStatus()
{
    return this->status;
}

pid_t Process::getPid()
{
    return this->pid;
}


//Process::Process(const Process& orig)
//{
//}

Process::~Process()
{
}

