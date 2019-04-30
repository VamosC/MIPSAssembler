#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "instmem.h"
#include "bitskiller.h"
#include "machine.h"
#include <QString>
#include <QChar>
#include <QStringList>
#include <QRegExp>
#include <QByteArray>
#include <map>

class Assembler : public BitsKiller, public Machine
{
public:
    Assembler();
    Assembler(QStringList list);
    ~Assembler();
    void process();
    const InstMem getInstMem() const;
private:
    void initInstSet();
    void initFun();
    void initRs();
    void initRt();
    void initRegSet();
    QString& processImm(QString& inst, bool isHex, bool isNeg, const QString& imm, int bitwidth=16);

    static const int numOfReg = 32;

    InstMem instMem;
    std::map<QString, int> symbolTable;
    std::map<int, QString> resolve_branch;
    std::map<int, QString> resovle_j;
    std::map<QString, QString> instSet;
    std::map<QString, QString> regSet;
    std::map<QString, QString> Fun;
    std::map<QString, QString> Rt;
    std::map<QString, QString> Rs;
};

#endif // ASSEMBLER_H
