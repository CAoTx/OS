#include "Shelly.h"
#include <exception>

Shelly shelly;

void handle_keyC(int signum) {
    std::cout << "\n pressed C" << std::endl;
    if (shelly.getLastProcessOf(pStatus::workBack) != NULL) {
        Process* process = shelly.getLastProcessOf(pStatus::workFront);
        pid_t pid = process->pid;
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
    Process* process = shelly.getLastProcessOf(pStatus::workFront);
    pid_t pid = process->pid;
    kill(pid, SIGTSTP);
    std::cout << "Halted process - pid:" << pid;

}

void handle_SIGCHLD(int signum) {
    pid_t l_pid;
    int status;
    l_pid = waitpid(-1, &status, WNOHANG);
    if (l_pid == -1);
    else if (WIFEXITED(status)) {
        try {
            Process* process = shelly.getProcess(l_pid);
            process->status = pStatus::endet; //Hier crash
            std::cout << "\nProcess " << l_pid << " terminated" << std::endl;
            return;
        }        catch (std::exception ex) {
            std::cerr << "SIGCHLD: " << ex.what() << std::endl;
        }
    }
}

int main() {
    signal(SIGINT, handle_keyC);
    signal(SIGSTOP, handle_keyZ);
    signal(SIGCHLD, handle_SIGCHLD);


    try {
        while (shelly.execute());
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Catch Something Mainly" << std::endl;
    }

    return 0;
}
