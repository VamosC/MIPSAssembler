#include "debugger.h"
#include <QDebug>

Debugger::Debugger(){}

void Debugger::setInst(const InstMem& instructions) {
    instMem = instructions;
    size = instMem.getSize();
}

bool Debugger::next() {
    int size = instMem.getSize();
    for(int i = 0; i < size; i++) {
        qDebug() << instMem.getLineNum(i) << " : " << instMem.getInst(i) << endl;
    }
}
