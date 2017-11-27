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

void handle_SIGSEGV(int signum) {
    std::cout << "SEG_FAULT:" << signum << std::endl;

}

void handle_keyC(int signum) {
    std::cout << "\n pressed C" << std::endl;
    if (shelly.getPHandler()->getLastBackProcess() != NULL) {
        Process* process = shelly.getPHandler()->getLastFrontProcess();
        pid_t pid = process->getPid();
        kill(pid, SIGINT);
        std::cout << "Stopped Process - pid:" << pid;
    } else {
        std::string in;

        std::cout << "\n\n"; //4 debug
        std::cout << "Wirklich Beenden? Y or n\n>>";
        std::cin>>in;

        if (in == "Y" || in == "y" || in == "j" || in == "J") {
            std::cin.ignore(187, '\n');
            std::cout << "\nHauste" << std::endl;
        }
    }
}

void handle_keyZ(int signum) {
    std::cout << "\n pressed Z" << std::endl;
    Process* process = shelly.getPHandler()->getLastFrontProcess();
    pid_t pid = process->getPid();
    kill(pid, SIGTSTP);
    std::cout << "Halted process - pid:" << pid;

}

void handle_SIGCHLD(int signum) {
    pid_t l_pid;
    int status;
    l_pid = waitpid(-1, &status, WNOHANG);
    if (l_pid == -1);
    else {
        std::cout<<"SIGCHLT SEB"<<std::endl;
        Process* process = shelly.getPHandler()->getProcess(l_pid);
        process->changeStatus(Process::ProcessStatus::endet);               //Hier crash
        std::cout << "\nProcess " << l_pid << " terminated" << std::endl;
        return;
    }
}

int main() {
    signal(SIGSEGV, handle_SIGSEGV);
    signal(SIGINT, handle_keyC);
    signal(SIGSTOP, handle_keyZ);
    signal(SIGCHLD, handle_SIGCHLD);


    try {
        while (shelly.execute());
    } catch (...) {
        std::cout << "Catch Something Mainly" << std::endl;
    }

    return 0;
}
