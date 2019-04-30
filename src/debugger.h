#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "bitskiller.h"
#include "instmem.h"

class Debugger : public BitsKiller
{
public:
    Debugger();
    void setInst(const InstMem& instructions);
    bool next();
private:
    InstMem instMem;
    int size;
};

#endif // DEBUGGER_H
