#include "controller.h"

Controller::Controller() {}

Controller::Controller(const QString& opcode, const QString& fun) :
    opcode(opcode),
    fun(fun) {
    initReverseInstSet();
    initReverseFun();
    initOperation();
}

bool Controller::isALU() {
    if(!isLui() && !isJr() && !isJal() && !isJalr() && !isJump())
        return true;
    return false;
}
bool Controller::isJump() {
    if(reverseInstSet[opcode].compare("j") == 0)
        return true;
    return false;
}
bool Controller::isJal() {
    if(reverseInstSet[opcode].compare("jal") == 0)
        return true;
    return false;
}
bool Controller::isJalr() {
    if((opcode.compare("000000") == 0) && (reverseFun[fun].compare("jalr") == 0))
        return true;
    return false;
}
bool Controller::isJr() {
    if((opcode.compare("000000") == 0) && (reverseFun[fun].compare("jr") == 0))
        return true;
    return false;
}

bool Controller::isLW() {
    if(reverseInstSet[opcode].compare("lw") == 0)
        return true;
    return false;
}

bool Controller::isSW() {
    if(reverseInstSet[opcode].compare("sw") == 0)
        return true;
    return false;
}

bool Controller::isLogical() {
    if(reverseInstSet[opcode].compare("ori") == 0)
        return true;
    if(reverseInstSet[opcode].compare("xori") == 0)
        return true;
    if(reverseInstSet[opcode].compare("andi") == 0)
        return true;
    return false;
}

bool Controller::isRR() {
    if(opcode.compare("000000")  == 0)
        return true;
    return false;
}

bool Controller::isBeq() {
    if(reverseInstSet[opcode].compare("beq") == 0)
        return true;
    return false;
}

bool Controller::isImm() {
    if(reverseInstSet[opcode].compare("addi") == 0)
        return true;
    if(reverseInstSet[opcode].compare("andi") == 0)
        return true;
    if(reverseInstSet[opcode].compare("ori") == 0)
        return true;
    if(reverseInstSet[opcode].compare("xori") == 0)
        return true;
    if(reverseInstSet[opcode].compare("slti") == 0)
        return true;
    return false;
}

bool Controller::isBne() {
    if(reverseInstSet[opcode].compare("bne") == 0)
        return true;
    return false;
}

bool Controller::isShift() {
    if((opcode.compare("000000") == 0) && ((reverseFun[fun].compare("srl") == 0) || (reverseFun[fun].compare("sll") == 0)))
        return true;
    return false;
}
int Controller::getALUCtrl() {
    if(opcode.compare("000000") == 0) {
        return operations[reverseFun[fun]];
    } else {
        return operations[reverseInstSet[opcode]];
    }
}

bool Controller::isLui() {
    if(reverseInstSet[opcode].compare("lui") == 0)
        return true;
    return false;
}

void Controller::initReverseInstSet() {
    reverseInstSet["001000"] = "addi";
    reverseInstSet["001100"] = "andi";
    reverseInstSet["001101"] = "ori";
    reverseInstSet["001110"] = "xori";
    reverseInstSet["001111"] = "lui";
    reverseInstSet["001010"] = "slti";
    reverseInstSet["000100"] = "beq";
    reverseInstSet["000101"] = "bne";
    reverseInstSet["000010"] = "j";
    reverseInstSet["000011"] = "jal";
    reverseInstSet["100011"] = "lw";
    reverseInstSet["101011"] = "sw";
}

void Controller::initReverseFun() {
    reverseFun["100000"] = "add";
    reverseFun["100100"] = "and";
    reverseFun["100111"] = "nor";
    reverseFun["100101"] = "or";
    reverseFun["000000"] = "sll";
    reverseFun["000010"] = "srl";
    reverseFun["100010"] = "sub";
    reverseFun["100110"] = "xor";
    reverseFun["101010"] = "slt";
    reverseFun["001001"] = "jalr";
    reverseFun["001000"] = "jr";
}

void Controller::initOperation() {
    operations["and"] = 0;
    operations["andi"] = 0;
    operations["or"] = 1;
    operations["ori"] = 1;
    operations["nor"] = 2;
    operations["xor"] = 3;
    operations["xori"] = 3;
    operations["sw"] = 4;
    operations["lw"] = 4;
    operations["add"] = 4;
    operations["addi"] = 4;
    operations["sub"] = 5;
    operations["beq"] = 5;
    operations["bne"] = 5;
    operations["slt"] = 6;
    operations["slti"] = 6;
    operations["sll"] = 7;
    operations["srl"] = 8;
}
