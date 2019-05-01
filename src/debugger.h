#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "regs.h"
#include "bitskiller.h"
#include "instmem.h"
#include "controller.h"
#include "alu.h"
#include <map>

class Debugger : public BitsKiller
{
public:
    Debugger(Regs* registers);
    void setInst(const InstMem& instructions);
    int getNextPC() const;
    bool next();
private:
    Regs* regs;
    InstMem instMem;
    std::map<QString, QString> Memory;
    int size;
    int current_ptr;
    int PC_out;
};

#endif // DEBUGGER_H
