#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>
#include <map>

class Controller
{
public:
    Controller();
    Controller(const QString& opcode, const QString& fun);
    bool isALU();
    bool isJump();
    bool isJal();
    bool isJalr();
    bool isJr();
    bool isLW();
    bool isSW();
    bool isBeq();
    bool isBne();
    bool isRR();
    bool isImm();
    bool isLui();
    bool isShift();
    bool isLogical();
    int getALUCtrl();
private:
    void initReverseInstSet();
    void initReverseFun();
    void initOperation();
    QString opcode;
    QString fun;
    std::map<QString, QString> reverseInstSet;
    std::map<QString, QString> reverseFun;
    std::map<QString, int> operations;
};

#endif // CONTROLLER_H
