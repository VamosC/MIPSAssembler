#include "disassembler.h"

Disassembler::Disassembler() {}

Disassembler::Disassembler(QStringList list) : Machine (list) {
        initReverseFun();
        initReverseRegSet();
        initReverseInstSet();
        initReverseRt();
        initReverseRs();
}

Disassembler::~Disassembler(){}

void Disassembler::process() {
    int count = 0;
    for(auto s: list) {
        if(s.isEmpty()) {
            continue;
        }
        QString instcode;
        s = hexTobit(s);
        if(s.compare("00000000000000000000000000000000") == 0) {
            count++;
            output << "nop";
            continue;
        }
        QString opcode = s.mid(0, 6);
        if(opcode.compare("000000") == 0) { //RR type
            QString funCode = s.mid(26 ,6);
            QString op = reverseFun[funCode];
            bool is_ok = false;
            switch(instTypeSet[op]) {
            case 0: //eg. add/sub
                //format 000000 rs rt rd shmt fun
                //add $s0, $s1, $s2
                //add rd, rs, rt
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(16, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
                break;
            case 5: //eg. sll/srl/sra
                //format: 000000 000000 rt rd shmt fun
                //sll $s0, $s1, 4
                //sll rd, rt, 4
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(16, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
                instcode += ", ";
                instcode += QString::number(s.mid(21, 5).toInt(&is_ok, 2));
                if(!is_ok) {
                    return;
                }
                break;
            case 6: //eg. jr
                //format: 000000 rs rt rd shmt fun
                //jr $s1
                //jr rs
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                break;
            case 7: //eg. jalr
                //format: 000000 rs rt rd shmt fun
                //jalr $s1, $s2
                //jalr rs, rd
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(16, 5)];
                break;
            case 10: //div...
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
                break;
            case 13:
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(16, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(6, 5)];
                break;
            case 17:
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(16, 5)];
                break;
            case 18:
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                break;
            case 21:
                instcode += op;
                break;
            default:
                break;
            }
        } else if(opcode.compare("011100") == 0){ //clo & clz
            QString funCode = s.mid(26, 6);
            if(funCode.compare("100001") == 0) { //clo
                instcode += "clo";
                instcode += " $";
                instcode += reverseRegSet[s.mid(16, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(6, 5)];
            } else if(funCode.compare("100000")  == 0) { //clz
                instcode += "clz";
                instcode += " $";
                instcode += reverseRegSet[s.mid(16, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(6, 5)];
            } else if(funCode.compare("000010") == 0) { //mul
                instcode += "mul";
                instcode += " $";
                instcode += reverseRegSet[s.mid(16, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
            } else if(funCode.compare("000000") == 0) {
                instcode += "madd";
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
            } else if(funCode.compare("000001") == 0) {
                instcode += "maddu";
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
            } else if(funCode.compare("000100") == 0) {
                instcode += "msub";
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
            } else if(funCode.compare("000101") == 0) {
                instcode += "msubu";
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
            }

        } else if(opcode.compare("000001") == 0){
            QString label;
            int lineNumber;
            bool is_ok = false;
            QString op = reverseRt[s.mid(11, 5)];
            switch (instTypeSet[op]) {
            case 15:
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                lineNumber = count+1+static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2));
                if(resolveTable.count(lineNumber) == 1) { //already exist
                    label = resolveTable[lineNumber];//just reference
                } else {
                    label = "L"+ QString::number(lineNumber);//composed of L+linenumber (no collision)
                    resolveTable[lineNumber] = label;
                }
                instcode += ", ";
                instcode += label;
                if(!is_ok) {
                    return;
                }
                break;
            case 16:
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", ";
                instcode += QString::number(static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2)));
                if(!is_ok) {
                    return;
                }
                break;
            default:
                break;
            }
        } else if(opcode.compare("010000") == 0){
            if(s.mid(26, 6).compare("011000") == 0) {
                instcode = "eret";
            } else {
                instcode += reverseRs[s.mid(6, 5)];
                if(s.mid(6, 5).compare("00000") == 0) {
                   instcode += " $";
                   instcode += reverseRegSet[s.mid(11, 5)];
                   instcode += ", $";
                   instcode += reverseRegSet[s.mid(16, 5)];
                } else {
                    instcode += " $";
                    instcode += reverseRegSet[s.mid(16, 5)];
                    instcode += ", $";
                    instcode += reverseRegSet[s.mid(11, 5)];
                }
            }
        } else {
            QString op = reverseInstSet[opcode];
            QString label;
            int lineNumber;
            bool is_ok = false;
            instcode += op;
            switch(instTypeSet[op]) {
            case 1: //eg.lw/sw
                //format: opcode rs rt imm
                //lw $s1, 2($s2)
                //lw rt, imm(rs)
                //sw rt, imm(rs)
                instcode += " $";
                instcode += reverseRegSet[s.mid(11, 5)];
                instcode += ", ";
                instcode += QString::number(static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2)));
                instcode += "($";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ")";
                if(!is_ok) {
                    return;
                }
                break;
            case 2: //eg. addi/andi....
                //format: opcode rs rt imm
                //addi $s1, $s2, 4
                //addi rt, rs, imm
                instcode += " $";
                instcode += reverseRegSet[s.mid(11, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", ";
                instcode += QString::number(static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2)));
                if(!is_ok) {
                    return;
                }
                break;
            case 3: //eg. beq/bne
                //format: opcode rs rt imm(label)
                //beq $s1, $s2, start
                //beq rs, rt, label
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
                lineNumber = count+1+static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2));
                if(resolveTable.count(lineNumber) == 1) { //already exist
                    label = resolveTable[lineNumber];//just reference
                } else {
                    label = "L"+ QString::number(lineNumber);//composed of L+linenumber (no collision)
                    resolveTable[lineNumber] = label;
                }
                instcode += ", ";
                instcode += label;
                if(!is_ok) {
                    return;
                }
                break;
            case 4: //eg. j/jal
                //format: opcode imm
                //j start
                //j imm
                instcode += " ";
                lineNumber = s.mid(6).toInt(&is_ok, 2);
                if(resolveTable.count(lineNumber) == 1) {//already exist
                    label = resolveTable[lineNumber];
                } else { //composed of L+linenumber
                    label = "L"+ QString::number(lineNumber);
                    resolveTable[lineNumber] = label;
                }
                instcode += label;
                if(!is_ok) {
                    return;
                }
                break;
            case 8: //eg. lui
                //format: opcode rs rt imm
                //lui $s1, 0xffff
                //lui rt, imm
                instcode += " $";
                instcode += reverseRegSet[s.mid(11, 5)];
                instcode += ", ";
                instcode += QString::number(static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2)));
                break;
            case 14:
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                lineNumber = count+1+static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2));
                if(resolveTable.count(lineNumber) == 1) { //already exist
                    label = resolveTable[lineNumber];//just reference
                } else {
                    label = "L"+ QString::number(lineNumber);//composed of L+linenumber (no collision)
                    resolveTable[lineNumber] = label;
                }
                instcode += ", ";
                instcode += label;
                if(!is_ok) {
                    return;
                }
                break;
            default:
                break;
            }
        }
        count++;
        output << instcode;
    }
    //resolve address
    for(auto it : resolveTable) {
        output[it.first] = it.second + ": " + output[it.first];
    }
}

void Disassembler::initReverseRegSet() {
    reverseRegSet["00000"] = "zero";
    reverseRegSet["00001"] = "at";
    reverseRegSet["00010"] = "v0";
    reverseRegSet["00011"] = "v1";
    reverseRegSet["00100"] = "a0";
    reverseRegSet["00101"] = "a1";
    reverseRegSet["00110"] = "a2";
    reverseRegSet["00111"] = "a3";
    reverseRegSet["01000"] = "t0";
    reverseRegSet["01001"] = "t1";
    reverseRegSet["01010"] = "t2";
    reverseRegSet["01011"] = "t3";
    reverseRegSet["01100"] = "t4";
    reverseRegSet["01101"] = "t5";
    reverseRegSet["01110"] = "t6";
    reverseRegSet["01111"] = "t7";
    reverseRegSet["10000"] = "s0";
    reverseRegSet["10001"] = "s1";
    reverseRegSet["10010"] = "s2";
    reverseRegSet["10011"] = "s3";
    reverseRegSet["10100"] = "s4";
    reverseRegSet["10101"] = "s5";
    reverseRegSet["10110"] = "s6";
    reverseRegSet["10111"] = "s7";
    reverseRegSet["11000"] = "t8";
    reverseRegSet["11001"] = "t9";
    reverseRegSet["11010"] = "k0";
    reverseRegSet["11011"] = "k1";
    reverseRegSet["11100"] = "gp";
    reverseRegSet["11101"] = "sp";
    reverseRegSet["11110"] = "fp";
    reverseRegSet["11111"] = "ra";
}

void Disassembler::initReverseInstSet() {
    reverseInstSet["001000"] = "addi";
    reverseInstSet["001001"] = "addiu";
    reverseInstSet["001100"] = "andi";
    reverseInstSet["001101"] = "ori";
    reverseInstSet["001110"] = "xori";
    reverseInstSet["001111"] = "lui";
    reverseInstSet["001010"] = "slti";
    reverseInstSet["001011"] = "sltiu";
    reverseInstSet["000100"] = "beq";
    reverseInstSet["000101"] = "bne";
    reverseInstSet["000010"] = "j";
    reverseInstSet["000011"] = "jal";
    reverseInstSet["100000"] = "lb";
    reverseInstSet["100100"] = "lbu";
    reverseInstSet["100001"] = "lh";
    reverseInstSet["100101"] = "lhu";
    reverseInstSet["100010"] = "lwl";
    reverseInstSet["100110"] = "lwr";
    reverseInstSet["110000"] = "ll";
    reverseInstSet["100011"] = "lw";
    reverseInstSet["101000"] = "sb";
    reverseInstSet["101001"] = "sh";
    reverseInstSet["101010"] = "swl";
    reverseInstSet["101110"] = "swr";
    reverseInstSet["111000"] = "sc";
    reverseInstSet["101011"] = "sw";
    reverseInstSet["000111"] = "bgtz";
    reverseInstSet["000110"] = "blez";
}

void Disassembler::initReverseFun() {
    reverseFun["100000"] = "add";
    reverseFun["100001"] = "addu";
    reverseFun["100100"] = "and";
    reverseFun["100111"] = "nor";
    reverseFun["100101"] = "or";
    reverseFun["000000"] = "sll";
    reverseFun["000010"] = "srl";
    reverseFun["100010"] = "sub";
    reverseFun["100011"] = "subu";
    reverseFun["100110"] = "xor";
    reverseFun["101010"] = "slt";
    reverseFun["101011"] = "sltu";
    reverseFun["001001"] = "jalr";
    reverseFun["001000"] = "jr";
    reverseFun["011010"] = "div";
    reverseFun["011011"] = "divu";
    reverseFun["011000"] = "mult";
    reverseFun["011001"] = "multu";
    reverseFun["000100"] = "sllv";
    reverseFun["000011"] = "sra";
    reverseFun["000111"] = "srav";
    reverseFun["000110"] = "srlv";
    reverseFun["110100"] = "teq";
    reverseFun["110110"] = "tne";
    reverseFun["110000"] = "tge";
    reverseFun["110001"] = "tgeu";
    reverseFun["110010"] = "tlt";
    reverseFun["110011"] = "tltu";
    reverseFun["001010"] = "movz";
    reverseFun["001011"] = "movn";
    reverseFun["010000"] = "mfhi";
    reverseFun["010010"] = "mflo";
    reverseFun["010001"] = "mthi";
    reverseFun["010011"] = "mtlo";
    reverseFun["001100"] = "syscall";
}

void Disassembler::initReverseRt() {
    reverseRt["00001"] = "bgez";
    reverseRt["10001"] = "bgezal";
    reverseRt["10000"] = "bltzal";
    reverseRt["00000"] = "bltz";
    reverseRt["01100"] = "teqi";
    reverseRt["01110"] = "tnei";
    reverseRt["01000"] = "tgei";
    reverseRt["01001"] = "tgeiu";
    reverseRt["01010"] = "tlti";
    reverseRt["01011"] = "tltiu";
}

void Disassembler::initReverseRs() {
    reverseRs["00000"] = "mfc0";
    reverseRs["00100"] = "mtc0";
}
