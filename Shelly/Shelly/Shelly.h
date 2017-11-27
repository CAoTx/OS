#ifndef SHELLY_H
#define SHELLY_H


#include <sstream>
#include "ProcessHandler.h"
#include <algorithm>

enum intyp
{
    ext, logout, enter, exec_foreg, exec_backg, hc_fg, hc_bg
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
    ProcessHandler* pHandler;
    
};

#endif /* SHELLY_H */

