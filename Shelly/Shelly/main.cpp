/*20.11.17 15:45
 with the & function there is still a zombiProcess there while the mainProgramm
 is running. 
 Stops the mainProcess, the zombiProcess stops too 
 */

#include "Shelly.h"

void handle_SIGSEGV(int signum)
{
    std::cout << "SEG_FAULT:" << signum << std::endl;
}

void handle_keyC(int signum)
{
    std::cout << "test" << std::endl;
}

void handle_keyZ(int signum)
{

}

intyp inTyp(char** arr);
bool shelly();

int main()
{
    signal(SIGSEGV, handle_SIGSEGV);
    signal(SIGINT, handle_keyC);
    signal(SIGSTOP,handle_keyZ);
    
    Shelly shell;
    try
    {
        while (shell.execute());
    }
    catch (...)
    {
        std::cout << "Catche Something" << std::endl;
    }

    return 0;
}
