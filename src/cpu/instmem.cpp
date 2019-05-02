#include "instmem.h"

InstMem::InstMem(){}

bool InstMem::isEmpty() {
    return lineNum.empty() && inst.empty();
}

int InstMem::getLineNum(int index) const {
    return lineNum[index];
}

const QString InstMem::getInst(int index) const {
    return inst[index];
}

void InstMem::push_back(int index) {
    lineNum.push_back(index);
}

void InstMem::setInst(QStringList list) {
    inst = list;
}

int InstMem::getSize() const {
    return static_cast<int>(lineNum.size());
}
