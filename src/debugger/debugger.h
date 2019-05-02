#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "cpu/regs.h"
#include "utils/bitskiller.h"
#include "cpu/instmem.h"
#include "cpu/controller.h"
#include "cpu/alu.h"
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
