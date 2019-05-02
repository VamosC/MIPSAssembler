#include "assembler.h"
Assembler::Assembler()
{
}

Assembler::Assembler(QStringList list) :
    Machine(list)
{
    //initialize related table
    initRegSet();
    initFun();
    initRs();
    initRt();
    initInstSet();
}

Assembler::~Assembler() {}

void Assembler::process()  {

    int count = 0;//line number ignore no instruction line
    int line = 0;
    for(auto s : list) {
        line++;
        if(!s.isEmpty()) { //ignore empty line
            if(s.contains(':')) { // check tag
                QStringList temp = s.split(':');
                symbolTable[temp[0].trimmed()] = count; //insert in the symble table in the format <tag_name, line number>
                if(temp.size() == 1) { // only tag eg: label:
                    continue;
                } else { // capture the instruction
                    QString inst(temp[1].trimmed());
                    if(inst.isEmpty()) {
                        continue;
                    } else {
                        s = inst;
                    }
                }
            }

            s = s.trimmed(); //ignore space

            int index = s.indexOf("//");
            if(index != -1) {  //delete comment
                s = s.mid(0, index);
                s = s.trimmed();
            }

            if(s.isEmpty()) { //eg. label: //xxxxxx
                continue;
            }

            //decode instructon
            index = s.indexOf(" ");// opcode oprand eg. add $s1, $s3, $s2
            QString instcode;
            if(index == -1) {
                if(s.compare("nop") == 0) {
                    instcode = _32bitToHexFormat("00000000000000000000000000000000");
                } else if(s.compare("eret") == 0) {
                   instcode = _32bitToHexFormat("01000010000000000000000000011000");
                } else if(s.compare("syscall") == 0) {
                    instcode = _32bitToHexFormat("00000000000000000000000000001100");
                }
                output << instcode;
                count++;
                continue;
            }
            QString label;
            QString opcode = s.mid(0, index).toLower();
            QString oprand = s.mid(index+1).trimmed();
            QRegExp re;
            switch (instTypeSet[opcode]) {
            case 0: //eg. add/sub.....
                //format: 000000 rs rt rd shmt fun
                re = QRegExp("\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*\\$([a-zA-Z0-9]+)");
                instcode += "000000";
                re.indexIn(oprand);
                instcode += regSet[re.cap(2).toLower()];
                instcode += regSet[re.cap(3).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                instcode += "00000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 1: //eg. lw/sw
                //lw $s0, 20($s1) / lw $s0, ($s1) / lw $s0, 0xffff($s1) / lw $s0, -20($s1)
                //format: opcode rs rt imm
                re = QRegExp("\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*(0x)?(\\-)?([0-9a-zA-Z]*)[(]\\$([a-zA-Z0-9]+)[)]");
                instcode += instSet[opcode];
                re.indexIn(oprand);
                instcode += regSet[re.cap(5).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                if(re.cap(4).isEmpty()) {
                    instcode += "0000000000000000";
                } else {
                    instcode = processImm(instcode, !re.cap(2).isEmpty(), !re.cap(3).isEmpty(), re.cap(4));
                }
                instcode = _32bitToHexFormat(instcode);
                break;
            case 2://eg. addi/andi....
                //format: opcode rs rt imm
                re = QRegExp("\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*(0x)?(\\-)?([0-9a-zA-Z]+)");
                instcode += instSet[opcode];
                re.indexIn(oprand);
                instcode += regSet[re.cap(2).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                instcode = processImm(instcode, !re.cap(3).isEmpty(), !re.cap(4).isEmpty(), re.cap(5));
                instcode = _32bitToHexFormat(instcode);
                break;
            case 3: //eg. beq/bne
                //format: opcode rs rt label(imm)
                re = QRegExp("\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*([a-zA-Z_][0-9a-zA-Z_]*)");
                instcode += instSet[opcode];
                re.indexIn(oprand);
                instcode += regSet[re.cap(1).toLower()];
                instcode += regSet[re.cap(2).toLower()];
                label = re.cap(3);
                if(symbolTable.count(label) == 1) { //lookup label have been scanned?
                    instcode += tenToString_nbit(static_cast<unsigned>(symbolTable[label]-(count + 1)), 16);
                    instcode = _32bitToHexFormat(instcode);
                } else {
                   instcode += "0000000000000000";
                   resolve_branch[count] = label;//resolve after the pass
                }

                break;
            case 4://eg. j/jal
                //format: opcode (label)imm
                instcode += instSet[opcode];
                label = oprand.trimmed();
                if(symbolTable.count(label) == 1) { //lookup label have been scanned?
                    instcode += tenToString_nbit(static_cast<unsigned>(symbolTable[label]), 26);
                    instcode = _32bitToHexFormat(instcode);
                } else {
                    instcode += "00000000000000000000000000";
                    resovle_j[count] = label;
                }

                break;
            case 5: //eg. srl/sll
                //format: 000000 00000 rt rd shmt fun
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*(0x)?([0-9a-zA-Z]+)");
                instcode += "00000000000";
                re.indexIn(oprand);
                instcode += regSet[re.cap(2).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                instcode = processImm(instcode, !re.cap(3).isEmpty(), false, re.cap(4), 5);
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 6: //eg. jr
                //format: 000000 rs 00000 00000 00000 fun
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*");
                re.indexIn(oprand);
                instcode += "000000";
                instcode += regSet[re.cap(1).toLower()];
                instcode += "000000000000000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 7: //eg. jalr
                //format: 000000 rs 00000 rd 00000 fun
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += "000000";
                instcode += regSet[re.cap(1).toLower()];
                instcode += "00000";
                instcode += regSet[re.cap(2).toLower()];
                instcode += "00000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 8: //eg. lui
                //format: opcode 00000 rt imm
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*(0x)?(-)?([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += instSet[opcode];
                instcode += "00000";
                instcode += regSet[re.cap(1).toLower()];
                instcode = processImm(instcode, !re.cap(2).isEmpty(), !re.cap(3).isEmpty(), re.cap(4));
                instcode = _32bitToHexFormat(instcode);
                break;
            case 9: //clo & clz
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += instSet[opcode];
                instcode += regSet[re.cap(2).toLower()];
                instcode += "00000";
                instcode += regSet[re.cap(1).toLower()];
                instcode += "00000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 10: //div...
                instcode += "000000";
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += regSet[re.cap(1).toLower()];
                instcode += regSet[re.cap(2).toLower()];
                instcode += "0000000000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 11: //mul
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += instSet[opcode];
                instcode += regSet[re.cap(2).toLower()];
                instcode += regSet[re.cap(3).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                instcode += "00000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 12: //madd...
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += instSet[opcode];
                instcode += regSet[re.cap(1).toLower()];
                instcode += regSet[re.cap(2).toLower()];
                instcode += "0000000000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 13: //sllv...
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += "000000";
                instcode += regSet[re.cap(3).toLower()];
                instcode += regSet[re.cap(2).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                instcode += "00000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 14:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*([a-zA-Z_][0-9a-zA-Z_]*)");
                re.indexIn(oprand);
                instcode += instSet[opcode];
                instcode += regSet[re.cap(1).toLower()];
                instcode += "00000";
                label = re.cap(2);
                if(symbolTable.count(label) == 1) { //lookup label have been scanned?
                    instcode += tenToString_nbit(static_cast<unsigned>(symbolTable[label]-(count + 1)), 16);
                    instcode = _32bitToHexFormat(instcode);
                } else {
                   instcode += "0000000000000000";
                   resolve_branch[count] = label;//resolve after the pass
                }
                break;
            case 15:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*([a-zA-Z_][0-9a-zA-Z_]*)");
                re.indexIn(oprand);
                instcode += "000001";
                instcode += regSet[re.cap(1).toLower()];
                instcode += Rt[opcode];
                label = re.cap(2);
                if(symbolTable.count(label) == 1) { //lookup label have been scanned?
                    instcode += tenToString_nbit(static_cast<unsigned>(symbolTable[label]-(count + 1)), 16);
                    instcode = _32bitToHexFormat(instcode);
                } else {
                   instcode += "0000000000000000";
                   resolve_branch[count] = label;//resolve after the pass
                }
                break;
            case 16:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*(0x)?(-)?([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += "000001";
                instcode += regSet[re.cap(1).toLower()];
                instcode += Rt[opcode];
                instcode = processImm(instcode, !re.cap(2).isEmpty(), !re.cap(3).isEmpty(), re.cap(4));
                instcode = _32bitToHexFormat(instcode);
                break;
            case 17:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*");
                re.indexIn(oprand);
                instcode += "0000000000000000";
                instcode += regSet[re.cap(1).toLower()];
                instcode += "00000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 18:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*");
                re.indexIn(oprand);
                instcode += "000000";
                instcode += regSet[re.cap(1).toLower()];
                instcode += "000000000000000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 19:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += instSet[opcode];
                instcode += Rs[opcode];
                if(Rs[opcode].compare("00000") == 0) {
                    instcode += regSet[re.cap(1).toLower()];
                    instcode += regSet[re.cap(2).toLower()];
                } else {
                    instcode += regSet[re.cap(2).toLower()];
                    instcode += regSet[re.cap(1).toLower()];
                }
                instcode += "00000000000";
                instcode = _32bitToHexFormat(instcode);
                break;
            default:
                break;
            }
            output << instcode;
            instMem.push_back(line-1);
            count++;
        }
    }

    //resolve address
    for(auto it : resolve_branch) {
        QString instcode = output[it.first];
        instcode = instcode.mid(0, 16);
        instcode += tenToString_nbit(static_cast<unsigned>(symbolTable[it.second]-(it.first+1)), 16);
        output[it.first] = _32bitToHexFormat(instcode);
    }

    for(auto it : resovle_j) {
        QString instcode = output[it.first];
        instcode = instcode.mid(0, 6);
        instcode += tenToString_nbit(static_cast<unsigned>(symbolTable[it.second]), 26);
        output[it.first] = _32bitToHexFormat(instcode);
    }
    instMem.setInst(output);
}

QString& Assembler::processImm(QString& inst, bool isHex, bool isNeg, const QString& imm, int bitwidth) {
    if(isHex) {
        inst += Unext_n(hexTobit(imm), bitwidth);
    } else {
        if(isNeg) {
            inst += tenToString_nbit(static_cast<unsigned>(~imm.toInt() + 1), bitwidth);
        } else {
            inst += tenToString_nbit(static_cast<unsigned>(imm.toInt()), bitwidth);
        }
    }
    return inst;
}

const InstMem Assembler::getInstMem() const {
    return instMem;
}

void Assembler::initInstSet() {
    instSet["addi"] = "001000";
    instSet["addiu"] = "001001";
    instSet["andi"] = "001100";
    instSet["ori"] = "001101";
    instSet["xori"] = "001110";
    instSet["lui"] = "001111";
    instSet["slti"] = "001010";
    instSet["sltiu"] = "001011";
    instSet["beq"] = "000100";
    instSet["bne"] = "000101";
    instSet["j"] = "000010";
    instSet["jal"] = "000011";
    instSet["lb"] = "100000";
    instSet["lbu"] = "100100";
    instSet["lh"] = "100001";
    instSet["lhu"] = "100101";
    instSet["lwl"] = "100010";
    instSet["lwr"] = "100110";
    instSet["ll"] = "110000";
    instSet["lw"] = "100011";
    instSet["sb"] = "101000";
    instSet["sh"] = "101001";
    instSet["sw"] = "101011";
    instSet["swl"] = "101010";
    instSet["swr"] = "101110";
    instSet["sc"] = "111000";
    instSet["clo"] = "011100";
    instSet["clz"] = "011100";
    instSet["mul"] = "011100";
    instSet["madd"] = "011100";
    instSet["maddu"] = "011100";
    instSet["msub"] = "011100";
    instSet["msubu"] = "011100";
    instSet["bgtz"] = "000111";
    instSet["blez"] = "000110";
    instSet["mfc0"] = "010000";
    instSet["mtc0"] = "010000";
    instSet["eret"] = "010000";
}

void Assembler::initRt() {
    Rt["bgez"] = "00001";
    Rt["bgezal"] = "10001";
    Rt["bltzal"] = "10000";
    Rt["bltz"] = "00000";
    Rt["teqi"] = "01100";
    Rt["tnei"] = "01110";
    Rt["tgei"] = "01000";
    Rt["tgeiu"] = "01001";
    Rt["tlti"] = "01010";
    Rt["tltiu"] = "01011";
}


void Assembler::initRegSet() {
    //e.g regSet["zero"] = "00000"
    QString t[numOfReg] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                           "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};
    for(int i = 0; i < numOfReg; i++) {
        regSet[t[i]] = tenToString_nbit(static_cast<unsigned>(i));
    }
}

void Assembler::initFun() {
    Fun["add"] = "100000";
    Fun["addu"] = "100001";
    Fun["and"] = "100100";
    Fun["nor"] = "100111";
    Fun["or"] = "100101";
    Fun["sll"] = "000000";
    Fun["srl"] = "000010";
    Fun["sub"] = "100010";
    Fun["subu"] = "100011";
    Fun["xor"] = "100110";
    Fun["slt"] = "101010";
    Fun["sltu"] = "101011";
    Fun["jalr"] = "001001";
    Fun["jr"] = "001000";
    Fun["clo"] = "100001";
    Fun["clz"] = "100000";
    Fun["div"] = "011010";
    Fun["divu"] = "011011";
    Fun["mult"] = "011000";
    Fun["multu"] = "011001";
    Fun["mul"] = "000010";
    Fun["madd"] = "000000";
    Fun["maddu"] = "000001";
    Fun["msub"] = "000100";
    Fun["msubu"] = "000101";
    Fun["sllv"] = "000100";
    Fun["sra"] = "000011";
    Fun["srav"] = "000111";
    Fun["srlv"] = "000110";
    Fun["teq"] = "110100";
    Fun["tne"] = "110110";
    Fun["tge"] = "110000";
    Fun["tgeu"] = "110001";
    Fun["tlt"] = "110010";
    Fun["tltu"] = "110011";
    Fun["movz"] = "001010";
    Fun["movn"] = "001011";
    Fun["mfhi"] = "010000";
    Fun["mflo"] = "010010";
    Fun["mthi"] = "010001";
    Fun["mtlo"] = "010011";
    Fun["eret"] = "011000";
    Fun["syscall"] = "001100";
}

void Assembler::initRs() {
    Rs["mfc0"] = "00000";
    Rs["mtc0"] = "00100";
}

