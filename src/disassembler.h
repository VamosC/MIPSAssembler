#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "machine.h"
#include "bitskiller.h"
#include <QStringList>
#include <QString>
#include <map>

class Disassembler : public BitsKiller, public Machine
{
public:
    Disassembler();
    Disassembler(QStringList list);
    ~Disassembler() override;

    void process() override;
private:
    void initReverseRegSet();
    void initReverseInstSet();
    void initReverseRs();
    void initReverseRt();
    void initReverseFun();

    std::map<int, QString> resolveTable;
    std::map<QString, QString> reverseRegSet;
    std::map<QString, QString> reverseInstSet;
    std::map<QString, QString> reverseFun;
    std::map<QString, QString> reverseRt;
    std::map<QString, QString> reverseRs;
};

#endif // DISASSEMBLER_H
