#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "bitskiller.h"
#include "instmem.h"
#include "controller.h"

class Debugger : public BitsKiller
{
public:
    Debugger();
    void setInst(const InstMem& instructions);
    int getNextPC() const;
    bool next();
private:
    InstMem instMem;
    int size;
    int current_ptr;
    int PC_out;
};

#endif // DEBUGGER_H
