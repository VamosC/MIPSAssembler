#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>

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
    int getALUCtrl();
private:
    QString opcode;
    QString fun;
};

#endif // CONTROLLER_H
