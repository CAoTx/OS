#ifndef SHELLY_H
#define SHELLY_H

#include <iostream>
#include <sstream>
#include "ProcessHandler.h"
#include <algorithm>

enum intyp
{
    ext, logout, enter, exec_stop, exec_go
};

class Shelly
{
public:
    Shelly();
    Shelly(const Shelly& orig);

    std::string Interface();
    void prepare();
    intyp inputType();
    bool execute();
    void clean();
    ProcessHandler* getPHandler();
    
    virtual ~Shelly();
    
private:
    char** xcute;
    int size;
    int length;
    std::string input;
    intyp iTyp;
    std::vector<std::string> inputMem;
    ProcessHandler pHandler;
    
};

#endif /* SHELLY_H */

