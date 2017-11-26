/*20.11.17 15:45
 with the & function there is still a zombiProcess there while the mainProgramm
 is running. 
 Stops the mainProcess, the zombiProcess stops too 
 */

/*      NOTE: 
 *      The functions bg, fg are implemented as follows: 
 *      they refer to the latest halted or stopped process.
 */

#include "Shelly.h"

Shelly shelly;

void handle_SIGSEGV(int signum)
{
    std::cout << "SEG_FAULT:" << signum << std::endl;
    return;
}

void handle_keyC(int signum)
{
    Process* process = shelly.getPHandler()->getLastFrontProcess();
    pid_t pid = process->getPid();
    kill(pid, SIGINT);
    std::cout << "Stopped Process - pid:" << pid;
    return;
}

void handle_keyZ(int signum)
{
    Process* process = shelly.getPHandler()->getLastFrontProcess();
    pid_t pid = process->getPid();
    kill(pid, SIGTSTP);
    std::cout << "Halted process - pid:" << pid;
    return;
}

void handle_SIGCHLD(int signum)
{
    pid_t l_pid;
    int status;
    l_pid = waitpid(-1, &status, WNOHANG);
    if (l_pid == -1);
    else
    {
        Process* process = shelly.getPHandler()->getProcess(l_pid);
        process->changeStatus(Process::ProcessStatus::endet);
        std::cout << "\nProcess " << l_pid << " terminated" << std::endl;
        return;
    }
}

int main()
{
    signal(SIGSEGV, handle_SIGSEGV);
    signal(SIGINT, handle_keyC);
    signal(SIGSTOP, handle_keyZ);
    signal(SIGCHLD, handle_SIGCHLD);


    try
    {
        while (shelly.execute());
    }
    catch (...)
    {
        std::cout << "Catch Something Mainly" << std::endl;
    }

    return 0;
}
