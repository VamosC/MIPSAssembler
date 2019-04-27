#include "assembler.h"
#include <QDebug>
Assembler::Assembler()
{
}

Assembler::Assembler(QStringList list) :
    list(list)
{
    initRegSet();
    initFun();
    initInstSet();
    initInstTypeSet();
    initBitToHexTable();
    initHexToBitTable();
    initReverseFun();
    initReverseRegSet();
    initReverseInstSet();
}

void Assembler::assemble() {

    int count = 0;

    for(auto s : list) {
        if(!s.isEmpty()) {
            if(s.contains(':')) {
                QStringList temp = s.split(':');
                symbolTable[temp[0].trimmed()] = count;
                if(temp.size() == 1) {
                    continue;
                } else {
                    QString inst(temp[1].trimmed());
                    if(inst.isEmpty()) {
                        continue;
                    } else {
                        s = inst;
                    }
                }
            }
            s = s.trimmed();
            int index = s.indexOf("//");
            if(index != -1) {
                s = s.mid(0, index);
                s = s.trimmed();
            }
            if(s.isEmpty()) {
                continue;
            }
            //decode inst
            index = s.indexOf(" ");
            QString instcode;
            QString label;
            QString opcode = s.mid(0, index).toLower();
            QString oprand = s.mid(index+1).trimmed();
            QRegExp re;
            switch (instTypeSet[opcode]) {
            case 0:
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
            case 1:
                re = QRegExp("\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*(0x)?(\\-)?([0-9a-zA-Z]*)[(]\\$([a-zA-Z0-9]+)[)]");
                instcode += instSet[opcode];
                re.indexIn(oprand);
                instcode += regSet[re.cap(5).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                if(re.cap(2).isEmpty()) {
                    if(re.cap(4).isEmpty()) {
                        instcode += "0000000000000000";
                    } else if(re.cap(3).isEmpty()) {
                        instcode += tenToString_nbit(static_cast<unsigned>(re.cap(4).toInt()), 16);
                    } else {
                        instcode += tenToString_nbit(static_cast<unsigned>(~re.cap(4).toInt()+1), 16);
                    }
                } else {
                    instcode += Ext_n(hexTobit(re.cap(4)));
                }
                instcode = _32bitToHexFormat(instcode);
                break;
            case 2:
                re = QRegExp("\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*(0x)?(\\-)?([0-9a-zA-Z]+)");
                instcode += instSet[opcode];
                re.indexIn(oprand);
                instcode += regSet[re.cap(2).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                if(re.cap(3).isEmpty()) {
                    if(re.cap(4).isEmpty()) {
                        instcode += tenToString_nbit(static_cast<unsigned>(re.cap(5).toInt()), 16);
                    } else {
                        instcode += tenToString_nbit(static_cast<unsigned>(~re.cap(5).toInt()+1), 16);
                    }
                } else {
                    instcode += Ext_n(hexTobit(re.cap(5)));
                }
                instcode = _32bitToHexFormat(instcode);
                break;
            case 3:
                re = QRegExp("\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*\\$([a-zA-Z0-9]+)\\s*,\\s*([a-zA-Z_][0-9a-zA-Z_]*)");
                instcode += instSet[opcode];
                re.indexIn(oprand);
                instcode += regSet[re.cap(1).toLower()];
                instcode += regSet[re.cap(2).toLower()];
                label = re.cap(3);
                if(symbolTable.count(label) == 1) {
                    instcode += tenToString_nbit(static_cast<unsigned>(symbolTable[label]-(count + 1)), 16);
                    instcode = _32bitToHexFormat(instcode);
                } else {
                   instcode += "0000000000000000";
                   resolve_branch[count] = label;
                }

                break;
            case 4:
                instcode += instSet[opcode];
                label = oprand.trimmed();
                if(symbolTable.count(label) == 1) {
                    instcode += tenToString_nbit(static_cast<unsigned>(symbolTable[label]), 26);
                    instcode = _32bitToHexFormat(instcode);
                } else {
                    instcode += "00000000000000000000000000";
                    resovle_j[count] = label;
                }

                break;
            case 5:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*(0x)?([0-9a-zA-Z]+)");
                instcode += "00000000000";
                re.indexIn(oprand);
                instcode += regSet[re.cap(2).toLower()];
                instcode += regSet[re.cap(1).toLower()];
                if(re.cap(3).isEmpty()) {
                    instcode += tenToString_nbit(static_cast<unsigned>(re.cap(4).toInt()));
                } else {
                    instcode += Ext_n(hexTobit(re.cap(4)), 5);
                }
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 6:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*");
                re.indexIn(oprand);
                instcode += "000000";
                instcode += regSet[re.cap(1).toLower()];
                instcode += "000000000000000";
                instcode += Fun[opcode];
                instcode = _32bitToHexFormat(instcode);
                break;
            case 7:
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
            case 8:
                re = QRegExp("\\s*\\$([0-9a-zA-Z]+)\\s*,\\s*(0x)?(-)?([0-9a-zA-Z]+)");
                re.indexIn(oprand);
                instcode += instSet[opcode];
                instcode += "00000";
                instcode += regSet[re.cap(1).toLower()];
                if(re.cap(2).isEmpty()) {
                    if(re.cap(3).isEmpty()) {
                        instcode += tenToString_nbit(static_cast<unsigned>(re.cap(4).toInt()), 16);
                    } else {
                        instcode += tenToString_nbit(static_cast<unsigned>(~re.cap(4).toInt()+1), 16);
                    }
                } else {
                    instcode += Ext_n(hexTobit(re.cap(4)));
                }
                instcode = _32bitToHexFormat(instcode);
                break;
            default:
                break;
            }
            output << instcode;
            count++;
        }
    }

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
}

void Assembler::disassemble() {
    int count = 0;
    for(auto s: list) {
        if(s.isEmpty()) {
            continue;
        }
        QString instcode;
        s = hexTobit(s);
        QString opcode = s.mid(0, 6);
        if(opcode.compare("000000") == 0) {
            QString funCode = s.mid(26 ,6);
            QString op = reverseFun[funCode];
            bool is_ok = false;
            switch(instTypeSet[op]) {
            case 0:
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(16, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
                break;
            case 5:
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
            case 6:
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                break;
            case 7:
                instcode += op;
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(16, 5)];
                break;
            default:
                break;
            }
        } else {
            QString op = reverseInstSet[opcode];
            QString label;
            int lineNumber;
            bool is_ok = false;
            instcode += op;
            switch(instTypeSet[op]) {
            case 1:
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
                qDebug() << instcode << endl;
                break;
            case 2:
                instcode += " $";
                instcode += reverseRegSet[s.mid(11, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", ";
                instcode += QString::number(static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2)));
                if(!is_ok) {
                    return;
                }
                qDebug() << instcode << endl;
                break;
            case 3:
                instcode += " $";
                instcode += reverseRegSet[s.mid(6, 5)];
                instcode += ", $";
                instcode += reverseRegSet[s.mid(11, 5)];
                lineNumber = count+1+static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2));
                if(resolveTable.count(lineNumber) == 1) {
                    label = resolveTable[lineNumber];
                } else {
                    label = "L"+ QString::number(lineNumber);
                    resolveTable[lineNumber] = label;
                }
                instcode += ", ";
                instcode += label;
                if(!is_ok) {
                    return;
                }
                break;
            case 4:
                instcode += " ";
                lineNumber = s.mid(6).toInt(&is_ok, 2);
                if(resolveTable.count(lineNumber) == 1) {
                    label = resolveTable[lineNumber];
                } else {
                    label = "L"+ QString::number(lineNumber);
                    resolveTable[lineNumber] = label;
                }
                instcode += label;
                if(!is_ok) {
                    return;
                }
                break;
            case 8:
                instcode += " $";
                instcode += reverseRegSet[s.mid(11, 5)];
                instcode += ", ";
                instcode += QString::number(static_cast<int16_t>(s.mid(16).toInt(&is_ok, 2)));
                break;
            default:
                break;
            }
        }
        count++;
        output << instcode;
    }
    for(auto it : resolveTable) {
        output[it.first] = it.second + ": " + output[it.first];
    }
}

const QStringList Assembler::getRes() {
    return output;
}

const QString Assembler::tenToString_nbit(unsigned int x, int n) {
    QString res;
    int i = 0;

    while(x > 0) {
        res = _1bitToQChar_2(x%2) + res;
        x /= 2;
        i++;
    }
    for(int j = n-i; j > 0; j--) {
        res = '0' + res;
    }
    if(res.size() > n) {
        res = res.mid(res.size() - n);
    }
    return res;
}

const QChar Assembler::_1bitToQChar_2(int x) {
    return x == 1 ? '1' : '0';
}

const QString Assembler::_32bitToHexFormat(const QString& s) {
    QString res;
    for(int i = 0; i < 32; i= i+4) {
        res += bitToHexTable[(s.mid(i, 4))];
    }
    return res;
}

const QString Assembler::hexTobit(const QString& x) {
    QString res;
    for(auto s : x) {
        res += hexToBitTable[s];
    }
    return res;
}

const QString Assembler::Ext_n(const QString& s, int n) {
    QString res(s);
    int size = s.size();
    for(int i = size; i < n; i++) {
        res = '0' + res;
    }
    return res;
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
    instSet["lw"] = "100011";
    instSet["sw"] = "101011";
}

void Assembler::initInstTypeSet() {
    instTypeSet["add"] = 0;
    instTypeSet["addu"] = 0;
    instTypeSet["and"] = 0;
    instTypeSet["nor"] = 0;
    instTypeSet["or"] = 0;
    instTypeSet["sll"] = 5;
    instTypeSet["srl"] = 5;
    instTypeSet["sub"] = 0;
    instTypeSet["subu"] = 0;
    instTypeSet["xor"] = 0;
    instTypeSet["slt"] = 0;
    instTypeSet["sltu"] = 0;
    instTypeSet["jalr"] = 7;
    instTypeSet["jr"] = 6;
    instTypeSet["lw"] = 1;
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
}

void Assembler::initRegSet() {
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
}

void Assembler::initHexToBitTable() {
    hexToBitTable["0"] = "0000";
    hexToBitTable["1"] = "0001";
    hexToBitTable["2"] = "0010";
    hexToBitTable["3"] = "0011";
    hexToBitTable["4"] = "0100";
    hexToBitTable["5"] = "0101";
    hexToBitTable["6"] = "0110";
    hexToBitTable["7"] = "0111";
    hexToBitTable["8"] = "1000";
    hexToBitTable["9"] = "1001";
    hexToBitTable["A"] = "1010";
    hexToBitTable["B"] = "1011";
    hexToBitTable["C"] = "1100";
    hexToBitTable["D"] = "1101";
    hexToBitTable["E"] = "1110";
    hexToBitTable["F"] = "1111";
    hexToBitTable["a"] = "1010";
    hexToBitTable["b"] = "1011";
    hexToBitTable["c"] = "1100";
    hexToBitTable["d"] = "1101";
    hexToBitTable["e"] = "1110";
    hexToBitTable["f"] = "1111";
}

void Assembler::initBitToHexTable() {
    bitToHexTable["0000"] = "0";
    bitToHexTable["0001"] = "1";
    bitToHexTable["0010"] = "2";
    bitToHexTable["0011"] = "3";
    bitToHexTable["0100"] = "4";
    bitToHexTable["0101"] = "5";
    bitToHexTable["0110"] = "6";
    bitToHexTable["0111"] = "7";
    bitToHexTable["1000"] = "8";
    bitToHexTable["1001"] = "9";
    bitToHexTable["1010"] = "A";
    bitToHexTable["1011"] = "B";
    bitToHexTable["1100"] = "C";
    bitToHexTable["1101"] = "D";
    bitToHexTable["1110"] = "E";
    bitToHexTable["1111"] = "F";
}

void Assembler::initReverseRegSet() {
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

void Assembler::initReverseInstSet() {
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
    reverseInstSet["100011"] = "lw";
    reverseInstSet["101011"] = "sw";
}

void Assembler::initReverseFun() {
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
}
