#ifndef REGS_H
#define REGS_H

#include "cpu/register.h"
#include <QString>
#include <QStandardItemModel>

class Regs : public QStandardItemModel
{
    Q_OBJECT
public:
    Regs(QObject* parent=nullptr);
    ~Regs();
    void reset();
    const QString getVal(int index);
    void setVal(int index, const QString& val);
private:
    void init();
    static const int numOfReg = 32;

    QString t[numOfReg] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                           "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};
    Register* registers[numOfReg];
    Register* PC;
};

#endif // REGS_H
