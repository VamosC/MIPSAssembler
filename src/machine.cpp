#include "machine.h"

Machine::Machine() {}

Machine::Machine(QStringList list) : list(list){
    initInstTypeSet();
}

Machine::~Machine() {}

void Machine::setInput(QStringList input) {
    list = input;
}

const QStringList Machine::getRes() const {
    return output;
}

void Machine::initInstTypeSet() {
    instTypeSet["add"] = 0;
    instTypeSet["movz"] = 0;
    instTypeSet["movn"] = 0;
    instTypeSet["addu"] = 0;
    instTypeSet["and"] = 0;
    instTypeSet["nor"] = 0;
    instTypeSet["or"] = 0;
    instTypeSet["sll"] = 5;
    instTypeSet["srl"] = 5;
    instTypeSet["sra"] = 5;
    instTypeSet["sub"] = 0;
    instTypeSet["subu"] = 0;
    instTypeSet["xor"] = 0;
    instTypeSet["slt"] = 0;
    instTypeSet["sltu"] = 0;
    instTypeSet["jalr"] = 7;
    instTypeSet["jr"] = 6;
    instTypeSet["lb"] = 1;
    instTypeSet["lbu"] = 1;
    instTypeSet["lh"] = 1;
    instTypeSet["lhu"] = 1;
    instTypeSet["lwl"] = 1;
    instTypeSet["lwr"] = 1;
    instTypeSet["ll"] = 1;
    instTypeSet["lw"] = 1;
    instTypeSet["sb"] = 1;
    instTypeSet["sh"] = 1;
    instTypeSet["swl"] = 1;
    instTypeSet["swr"] = 1;
    instTypeSet["sc"] = 1;
    instTypeSet["sw"] = 1;
    instTypeSet["addi"] = 2;
    instTypeSet["addiu"] = 2;
    instTypeSet["andi"] = 2;
    instTypeSet["ori"] = 2;
    instTypeSet["xori"] = 2;
    instTypeSet["lui"] = 8;
    instTypeSet["slti"] = 2;
    instTypeSet["sltiu"] = 2;
    instTypeSet["beq"] = 3;
    instTypeSet["bne"] = 3;
    instTypeSet["j"] = 4;
    instTypeSet["jal"] = 4;
    instTypeSet["clo"] = 9;
    instTypeSet["clz"] = 9;
    instTypeSet["div"] = 10;
    instTypeSet["divu"] = 10;
    instTypeSet["mult"] = 10;
    instTypeSet["multu"] = 10;
    instTypeSet["teq"] = 10;
    instTypeSet["tne"] = 10;
    instTypeSet["tge"] = 10;
    instTypeSet["tgeu"] = 10;
    instTypeSet["tlt"] = 10;
    instTypeSet["tltu"] = 10;
    instTypeSet["mul"] = 11;
    instTypeSet["madd"] = 12;
    instTypeSet["maddu"] = 12;
    instTypeSet["msub"] = 12;
    instTypeSet["msubu"] = 12;
    instTypeSet["sllv"] = 13;
    instTypeSet["srav"] = 13;
    instTypeSet["srlv"] = 13;
    instTypeSet["bgtz"] = 14;
    instTypeSet["blez"] = 14;
    instTypeSet["bgez"] = 15;
    instTypeSet["bgezal"] = 15;
    instTypeSet["bltzal"] = 15;
    instTypeSet["bltz"] = 15;
    instTypeSet["teqi"] = 16;
    instTypeSet["tnei"] = 16;
    instTypeSet["tgei"] = 16;
    instTypeSet["tgeiu"] = 16;
    instTypeSet["tlti"] = 16;
    instTypeSet["tltiu"] = 16;
    instTypeSet["mfhi"] = 17;
    instTypeSet["mflo"] = 17;
    instTypeSet["mthi"] = 18;
    instTypeSet["mtlo"] = 18;
    instTypeSet["mfc0"] = 19;
    instTypeSet["mtc0"] = 19;
    instTypeSet["eret"] = 20;
    instTypeSet["syscall"] = 21;
}
