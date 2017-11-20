#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H

#include <unistd.h> //exec
#include <cstdio> //fork?
#include <sys/wait.h> //waitpid

#include <vector>
#include <string>

class ProcessHandler
{
public:
    
    ProcessHandler();
    pid_t doFork();
    int timesForked();
    //ProcessHandler(const ProcessHandler& orig);
    virtual ~ProcessHandler();
private:
    
    std::vector<pid_t> pidS;

};

#endif /* PROCESSHANDLER_H */

