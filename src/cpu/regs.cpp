#include "regs.h"

Regs::Regs(QObject* parent) :
    QStandardItemModel (parent)
{
    for(int i = 0; i < numOfReg; i++) {
        registers[i] = new Register("$"+t[i], "00000000");
    }
    PC = new Register("PC", "00000000");
    init();
}

Regs::~Regs() {
    delete PC;
    for(int i = 0; i < numOfReg; i++) {
        delete registers[i];
    }
}

void Regs::init() {
    setColumnCount(2);
    setItem(0, 0, PC->first());
    setItem(0, 1, PC->second());
    for(int i = 0; i < numOfReg; i++) {
        setItem(i+1, 0, registers[i]->first());
        setItem(i+1, 1, registers[i]->second());
    }
}

void Regs::reset() {
    for(int i = 0; i < numOfReg; i++)
        registers[i]->setData("00000000");
    PC->setData("00000000");
}

const QString Regs::getVal(int index) {
    if(index == 32)
        return PC->getData();
    return registers[index]->getData();
}

void Regs::setVal(int index, const QString &val) {
    if(index == 32)
        return PC->setData(val);
    registers[index]->setData(val);
}



