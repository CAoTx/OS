
#include "Shelly.h"

Shelly::Shelly() {
    pHandler = new ProcessHandler();
    //Search for other ProcessHandler of deeper Layers make them to yours
    //maybe has to implement a synch function to keep all PHandlers Synchron 
}

Shelly::Shelly(const Shelly& orig) {
}

std::string Shelly::Interface() {
    std::string input;

    std::cout << ">>";
    std::getline(std::cin, input);


    return input;
}

void Shelly::prepare() {

    //Check Input
    this->input = Interface();
    this->inputMem.push_back(input); //Safe up 
    this->size = std::count(this->input.begin(), this->input.end(), ' ') + 3; //musste +3 why
    this->length = this->input.size();

    std::stringstream ss(this->input);
    this->xcute = new char*[this->size];

    //Init array
    for (int i = 0; i < this->size; i++) {
        this->xcute[i] = new char[this->length];
    }
    //fill array
    for (int i = 0; !(ss.eof()); i++) {
        ss >> this->xcute[i];
        if (ss.eof())
            this->xcute[i + 1] = nullptr;
    }
    this->iTyp = inputType();

}

bool Shelly::execute() {
    int status = 0;
    pid_t pid;
    prepare();
    
    if (iTyp != intyp::ext && iTyp != intyp::logout ){
    std::string instruction = xcute[0];
    //pid = pHandler->doFork(instruction);
    pid = fork();
    if (pid != 0){
    Process* newy = new Process(pid, instruction);
    pHandler->addProcess(newy);
    }
    }

    //CHILD
    if ((iTyp == intyp::exec_foreg && pid == 0) || (iTyp == intyp::exec_backg && pid == 0)) {
        if (iTyp == intyp::exec_backg) {
            xcute[size - 3] = nullptr;
        }
        std::cout << "\n1." << xcute[0] << std::endl;
        std::cout << "2." << xcute[1] << std::endl;
        std::cout << std::endl;
        execvp(xcute[0], xcute);
        return false;                 ///HAVE TO TEST
       // exit(0);
    }
    
    //PARENT
    else if (iTyp == intyp::exec_foreg && pid != 0) {
        waitpid(pid, &status, WNOHANG);
        pHandler->getLastFrontProcess()->changeStatus(Process::ProcessStatus::endet);
    }

    else if (iTyp == intyp::exec_backg && pid != 0) {
        pHandler->getProcess(pid)->changeStatus(Process::ProcessStatus::workBack);
        waitpid(pid, &status, WUNTRACED | WNOHANG);
        
    }

    else if (iTyp == intyp::hc_fg && pid != 0) {
        Process * process = nullptr;
        if (xcute[1] != nullptr && *xcute[1]>-20 && *xcute[1] < 4500) {
            process = pHandler->getProcess(*xcute[1]);
        } else {
            process = pHandler->getLastHaltedProcess();
        }
        pid_t pid = process->getPid();
        Process::ProcessStatus status = process->getStatus();
        if (status == Process::ProcessStatus::halted || status == Process::ProcessStatus::stopped) {
            kill(pid, SIGCONT);
            process->changeStatus(Process::ProcessStatus::workFront);
        }
        waitpid(pid, 0, WUNTRACED);
        return true;
    }

    else if (iTyp == intyp::hc_bg && pid != 0) {
        Process * process = nullptr;
        if (xcute[1] != nullptr && *xcute[1]>-20 && *xcute[1] < 4500) {
            process = pHandler->getProcess(*xcute[1]);
        } else {
            process = pHandler->getLastHaltedProcess();
        }
        pid_t pid = process->getPid();
        Process::ProcessStatus status = process->getStatus();
        if (status == Process::ProcessStatus::halted || status == Process::ProcessStatus::stopped) {
            kill(pid, SIGCONT);
            process->changeStatus(Process::ProcessStatus::workFront);
        }
        waitpid(pid, 0, WUNTRACED | WNOHANG);
        return true;
    }

    else if (iTyp == intyp::ext && pid != 0) {
        if (pHandler->closeAble()) {
            std::cout << "\nHauste" << std::endl;
            return false;
        } else {
            std::cout << "There are still working Jobs that need to be closed" << std::endl;
        }

    }

    else if (iTyp == intyp::logout && pid != 0) {

        if (pHandler->closeAble()) {
            std::string in;

            std::cout << "\n\n"; //4 debug
            std::cout << "Wirklich Beenden? Y or n\n>>";
            std::cin>>in;

            if (in == "Y" || in == "y" || in == "j" || in == "J") {
                std::cin.ignore(187, '\n');
                std::cout << "\nHauste" << std::endl;
                return false;
            }
        } else {
            std::cout << "There are still working Jobs that need to be closed" << std::endl;
        }
    }
    return true;
}

ProcessHandler* Shelly::getPHandler() {
    return this->pHandler;
}

intyp Shelly::inputType() {
    std::string str = this->xcute[0];

    if (*(this->xcute[0]) == ' ')
        return intyp::enter;
    else if (str == "exit")
        return intyp::ext;
    else if (str == "logout")
        return intyp::logout;
    else if (str == "bg")
        return intyp::hc_bg;
    else if (str == "fg")
        return intyp::hc_fg;

    for (int i = 0; this->xcute[i] != nullptr; i++) {
        if (*(this->xcute[i]) == '&')
            return intyp::exec_backg;
    }
    return intyp::exec_foreg;
}

void Shelly::clean() {
    for (int i = 0; i < size; i++) {
        delete xcute [i];
    }
}

Shelly::~Shelly() {
    clean();
    delete xcute;
}

