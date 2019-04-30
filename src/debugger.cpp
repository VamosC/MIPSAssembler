#include "debugger.h"
#include <QDebug>

Debugger::Debugger() :
    current_ptr(0),
    PC_out(0)
{
    if(!instMem.isEmpty())
        PC_out = instMem.getLineNum(0);
}

void Debugger::setInst(const InstMem& instructions) {
    instMem = instructions;
    size = instMem.getSize();
}

bool Debugger::next() {
    if(current_ptr > size)
        return false;
    QString inst = hexTobit(instMem.getInst(current_ptr));
    Controller ctrl(inst.mid(0, 6), inst.mid(26, 6));
    if(ctrl.isALU()) {
        switch(ctrl.getALUCtrl()) {
        case 0:
            break;
        case 1:
            break;
        }
    } else {
        if(ctrl.isJump()) {
            QString PC_4 = tenToString_nbit(static_cast<unsigned>(4*(current_ptr+1)), 16);
            QString address(PC_4.mid(0, 4)+inst.mid(6) + "00");
            PC_out = instMem.getLineNum(address.toInt(nullptr, 2)/4);
        } else if(ctrl.isJal()) {

        } else if(ctrl.isJalr()) {

        } else if(ctrl.isJr()) {

        }
    }
    return true;
}

int Debugger::getNextPC() const {
    return PC_out;
}
