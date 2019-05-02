#include "debugger.h"
Debugger::Debugger(Regs* registers) :
    regs(registers),
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
    if(current_ptr >= size)
        return false;
    QString inst = hexTobit(instMem.getInst(current_ptr));
    Controller ctrl(inst.mid(0, 6), inst.mid(26, 6));
    QString address;
    QString PC_4 = tenToString_nbit(static_cast<unsigned>(4*(current_ptr+1)), 32);
    if(ctrl.isALU()) {
        ALU myALU(ctrl.getALUCtrl());
        if(ctrl.isShift()) {
            myALU.run(regs->getVal(inst.mid(11, 5).toInt(nullptr, 2)), _32bitToHexFormat(Unext_n(inst.mid(21, 5), 32)));
            regs->setVal(inst.mid(16, 5).toInt(nullptr, 2), myALU.getRes());
            address = PC_4;
        } else if(ctrl.isRR()) {
            myALU.run(regs->getVal(inst.mid(6, 5).toInt(nullptr, 2)), regs->getVal(inst.mid(11, 5).toInt(nullptr, 2)));
            regs->setVal(inst.mid(16, 5).toInt(nullptr, 2), myALU.getRes());
            address = PC_4;
        } else if (ctrl.isImm()) {
            QString imm;
            if(ctrl.isLogical()) {
                imm = Unext_n(inst.mid(16), 32);
            } else {
                imm = Ext_n(inst.mid(16), 32);
            }
            myALU.run(regs->getVal(inst.mid(6, 5).toInt(nullptr, 2)), _32bitToHexFormat(imm));
            regs->setVal(inst.mid(11, 5).toInt(nullptr, 2), myALU.getRes());
            address = PC_4;
        } else if(ctrl.isSW()) {
            myALU.run(regs->getVal(inst.mid(6, 5).toInt(nullptr, 2)), _32bitToHexFormat(Ext_n(inst.mid(16), 32)));
            Memory[myALU.getRes()] = regs->getVal(inst.mid(11, 5).toInt(nullptr, 2));
            address = PC_4;
        } else if(ctrl.isLW()) {
            myALU.run(regs->getVal(inst.mid(6, 5).toInt(nullptr, 2)), _32bitToHexFormat(Ext_n(inst.mid(16), 32)));
            if(Memory.count(myALU.getRes()) == 0) {
                Memory[myALU.getRes()] = "00000000";
            }
            regs->setVal(inst.mid(11,5).toInt(nullptr, 2), Memory[myALU.getRes()]);
            address = PC_4;
        } else if(ctrl.isBeq() || ctrl.isBne()) {
            myALU.run(regs->getVal(inst.mid(6, 5).toInt(nullptr, 2)), regs->getVal(inst.mid(11, 5).toInt(nullptr, 2)));
            if((ctrl.isBeq() && myALU.iszero()) || (ctrl.isBne() && !myALU.iszero())) {
                address = tenToString_nbit(PC_4.toUInt(nullptr, 2) + stringToInt(Ext_n(inst.mid(16), 32))*4, 32);
            } else {
                address = PC_4;
            }
        }
    } else {
        if(ctrl.isJump()) {
            address = PC_4.mid(0, 4)+inst.mid(6) + "00";
        } else if(ctrl.isJal()) {
            address = PC_4.mid(0, 4)+inst.mid(6) + "00";
            regs->setVal(31, _32bitToHexFormat(PC_4));
        } else if(ctrl.isJalr()) {
            address = hexTobit(regs->getVal(inst.mid(6, 5).toInt(nullptr, 2)));
            regs->setVal(inst.mid(16, 5).toInt(nullptr, 2), _32bitToHexFormat(PC_4));
        } else if(ctrl.isJr()) {
            address = hexTobit(regs->getVal(inst.mid(6, 5).toInt(nullptr, 2)));
        } else if(ctrl.isLui()) {
            regs->setVal(inst.mid(11, 5).toInt(nullptr, 2), _32bitToHexFormat(inst.mid(16) + hexTobit(regs->getVal(inst.mid(11, 5).toInt(nullptr, 2)).mid(4))));
            address = PC_4;
        }
    }
    PC_out = instMem.getLineNum(address.toUInt(nullptr, 2)/4);
    current_ptr = address.toUInt(nullptr, 2)/4;
    regs->setVal(32, _32bitToHexFormat(address));
    return true;
}

int Debugger::getNextPC() const {
    return PC_out;
}

