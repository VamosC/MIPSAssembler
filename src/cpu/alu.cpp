#include "alu.h"

ALU::ALU(int ctrl) :
    choice(ctrl)
{}

void ALU::run(const QString &a, const QString &b) {
    switch (choice) {
    case 0: //and
        res = _32bitToHexFormat(tenToString_nbit(a.toUInt(nullptr, 16) & b.toUInt(nullptr, 16), 32));
        break;
    case 1: //or
        res = _32bitToHexFormat(tenToString_nbit(a.toUInt(nullptr, 16) | b.toUInt(nullptr, 16), 32));
        break;
    case 2: //nor
        res = _32bitToHexFormat(tenToString_nbit(~(a.toUInt(nullptr, 16) | b.toUInt(nullptr, 16)), 32));
        break;
    case 3: //xor
        res = _32bitToHexFormat(tenToString_nbit(a.toUInt(nullptr, 16) ^ b.toUInt(nullptr, 16), 32));
        break;
    case 4: //add
        res = _32bitToHexFormat(tenToString_nbit(static_cast<unsigned>(stringToInt(a, 16) + stringToInt(b, 16)), 32));
        break;
    case 5: //sub
        if(a.compare(b) == 0) {
            res = "00000000";
            zero = 1;
            return;
        }
        res = _32bitToHexFormat(tenToString_nbit(static_cast<unsigned>(stringToInt(a, 16) - stringToInt(b, 16)), 32));
        break;
    case 6: //slt
        res = stringToInt(a, 16) < stringToInt(b, 16) ? "00000001" : "00000000";
        break;
    case 7: //sll
        res = _32bitToHexFormat(tenToString_nbit(a.toUInt(nullptr, 16) << b.toUInt(nullptr, 16), 32));
        break;
    case 8: //srl
        res = _32bitToHexFormat(tenToString_nbit(a.toUInt(nullptr, 16) >> b.toUInt(nullptr, 16), 32));
        break;
    default:
        break;
    }
}

const QString ALU::getRes() const {
    return res;
}

bool ALU::iszero() {
    return zero == 1;
}
