#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <map>

class Process
{

    struct Event
    {
        std::string message;
        time_t time;
    };

public:

    enum ProcessStatus
    {
        sick = -2, zombi = -1, workFront = 1, halted = 2, stopped = 3, endet = 4, workBack = 11
    };
    Process(pid_t myId);
    void addEvent(std::string text);
    std::string getStringStatus(ProcessStatus stat);
    ProcessStatus getStatus();
    ProcessStatus changeStatus(ProcessStatus newStat);
    pid_t getPid();
    // Process(const Process& orig);
    virtual ~Process();

private:
    pid_t pid;
    ProcessStatus status;
    int eventCtr;
    time_t start;
    time_t end;
    std::map<std::string, Event> tLog;
    /*1.Text:start, end, eventN(string+)
      2. Text what
      3. time when
     */
};

#endif /* PROCESS_H */

