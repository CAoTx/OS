#include "Process.h"

Process::Process(pid_t myID)
{

    start = time_t(NULL);
    if (start == -1)
    {
        status = ProcessStatus::sick;
        throw "Failure in Process(): StartTime = ";
    }
    status = ProcessStatus::normalFront;
    pid = myID;

    Event eventStart{("Start of this Process, " + getStringStatus(status)), time(nullptr)};
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
    if (stat == ProcessStatus::normalFront)
        return "normal";
    else if (stat == ProcessStatus::halted)
        return "halted";
    else if (stat == ProcessStatus::stopp)
        return "stop";
    else if (stat == ProcessStatus::endet)
        return "endet";
    else if (stat == ProcessStatus::sick)
        return "sick";
    else if (stat == ProcessStatus::normalFront)
        return "zombi";
    else if (stat == ProcessStatus::normalBack)
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


//Process::Process(const Process& orig)
//{
//}

Process::~Process()
{
}

