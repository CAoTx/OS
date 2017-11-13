#include "Shelly.h"

//enum intyp {
//    ext, logout, enter, exec_stop, exec_go
//};

void handle_SIGSEGV(int signum){
    std::cout<<"SEG_FAULT:"<<signum<<std::endl;
}
void handle_cKey(int signum){
    
}

intyp inTyp(char** arr);
bool shelly();

int main()
{

    Shelly shell;

    while (shell.execute());

        signal(SIGSEGV, handle_SIGSEGV);
        signal(SIGINT, handle_cKey);
    
//        try {
//            while (shelly());
//    
//            //        for(int i = 0; i<3;i++)   //funktioniert nicht ??
//            //            shelly;
//        }    catch (...) {
//            std::cerr << "Oops. That Failed, BYE /n" << std::endl;
//            return 0;
//        }
    return 0;
}

bool shelly()
{
    //INIT
    int ctr, size, length;
    std::string input;

    intyp iTyp;
    int status = 0;

    //INPUT
    std::cout << ">>";
    std::getline(std::cin, input); //only works with getline| >>str fails cause str only gets the char till spce

    //HANDLING
    size = std::count(input.begin(), input.end(), ' ') + 3; //musste +3 why
    length = input.size();

    std::stringstream ss(input);
    char** xcute;
    xcute = new char*[size];

    for (int i = 0; i < size; i++)
    {
        xcute[i] = new char[length];
    }

    for (int i = 0; !(ss.eof()); i++)
    {
        ss >> xcute[i];
        if (ss.eof())
            xcute[i + 1] = nullptr;
    }
    iTyp = inTyp(xcute);

    //EXECUTION
    int pid = fork();

    //CHILD
    if ((iTyp == intyp::exec_stop && pid == 0) || (iTyp == intyp::exec_go && pid == 0))
    {
        if (iTyp == intyp::exec_go)
        {
            xcute[size - 3] = nullptr;
        }
        std::cout << "\n1." << xcute[0] << std::endl;
        std::cout << "2." << xcute[1] << std::endl;
        std::cout << std::endl;
        execvp(xcute[0], xcute);
        //return false;
        exit(0);
    } //PARENT
    else if (iTyp == intyp::exec_stop && pid != 0)
    {
        waitpid(pid, &status, !(WNOHANG));
    }
    else if (iTyp == intyp::exec_go && pid != 0)
    {
        //waitpid(pid, &status, WNOHANG);
    }
    else if (iTyp == intyp::ext && pid != 0)
    {
        std::cout << "\nHauste" << std::endl;
        for (int i = 0; i < size; i++)
            {
                delete xcute[i];
            }
            delete xcute;
        return false;
    }
    else if (iTyp == intyp::logout && pid != 0)
    {
        std::string in;

        std::cout << "\n\n"; //4 debug
        std::cout << "Wirklich Beenden? Y or n\n>>";
        std::cin>>in;

        if (in == "Y" || in == "y" || in == "j" || in == "J")
        {
            std::cout << "\nHauste" << std::endl;

            for (int i = 0; i < size; i++)
            {
                delete xcute[i];
            }
            delete xcute;

            return false;
        }

    }
    return true;
}

intyp inTyp(char** arr)
{
    std::string str = arr[0];

    if (*arr[0] == ' ')
        return intyp::enter;
    else if (str == "exit")
        return intyp::ext;
    else if (str == "logout")
        return intyp::logout;

    for (int i = 0; arr[i] != nullptr; i++)
    {
        if (*arr[i] == '&')
            return intyp::exec_go;
    }
    return intyp::exec_stop;
}