
#include "Shelly.h"

Shelly::Shelly()
{
    //Search for other ProcessHandler of deeper Layers make them to yours
    //maybe has to implement a synch function to keep all PHandlers Synchron 
}

Shelly::Shelly(const Shelly& orig)
{
}

std::string Shelly::Interface()
{
    std::string input;

    std::cout << ">>";
    std::getline(std::cin, input);


    return input;
}

void Shelly::prepare()
{

    //Check Input
    this->input = Interface();
    this->inputMem.push_back(input);
    this->size = std::count(this->input.begin(), this->input.end(), ' ') + 3; //musste +3 why
    this->length = this->input.size();

    std::stringstream ss(this->input);
    this->xcute = new char*[this->size];

    //Init array
    for (int i = 0; i < this->size; i++)
    {
        this->xcute[i] = new char[this->length];
    }
    //fill array
    for (int i = 0; !(ss.eof()); i++)
    {
        ss >> this->xcute[i];
        if (ss.eof())
            this->xcute[i + 1] = nullptr;
    }
    this->iTyp = inputType();

}

bool Shelly::execute()
{
    prepare();

    int pid = pHandler.doFork();
    int status = 0;

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
            return false;
        }
    }
    return true;
}

intyp Shelly::inputType()
{
    std::string str = this->xcute[0];

    if (*(this->xcute[0]) == ' ')
        return intyp::enter;
    else if (str == "exit")
        return intyp::ext;
    else if (str == "logout")
        return intyp::logout;

    for (int i = 0; this->xcute[i] != nullptr; i++)
    {
        if (*(this->xcute[i]) == '&')
            return intyp::exec_go;
    }
    return intyp::exec_stop;
}

void Shelly::clean()
{
    for (int i = 0; i < size; i++)
    {
        delete xcute [i];
    }
}

Shelly::~Shelly()
{
    for (int i = 0; i < size; i++)
    {
        delete xcute [i];
    }
    delete xcute;
}

