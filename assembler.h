#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QString>
#include <QChar>
#include <QStringList>
#include <QRegExp>
#include <QByteArray>
#include <map>

class Assembler
{
public:
    Assembler();
    Assembler(QStringList list);
    void assemble();
    void disassemble();
    const QStringList getRes();
private:
    void initInstSet();
    void initInstTypeSet();
    void initFun();
    void initRegSet();
    void initHexToBitTable();
    void initBitToHexTable();
    void initReverseRegSet();
    void initReverseInstSet();
    void initReverseFun();
    const QString _32bitToHexFormat(const QString& s);
    const QString hexTobit(const QString& x);
    const QChar _1bitToQChar_2(int x);
    const QString tenToString_nbit(unsigned int x, int n=5);
    const QString Unext_n(const QString& s, int n=16);

    static const int numOfReg = 32;

    QStringList list;
    std::map<QString, int> symbolTable;
    std::map<int, QString> resolveTable;
    std::map<int, QString> resolve_branch;
    std::map<int, QString> resovle_j;
    std::map<QString, QString> instSet;
    std::map<QString, QString> regSet;
    std::map<QString, int> instTypeSet;
    std::map<QString, QString> Fun;
    std::map<QString, QString> hexToBitTable;
    std::map<QString, QString> bitToHexTable;
    std::map<QString, QString> reverseRegSet;
    std::map<QString, QString> reverseInstSet;
    std::map<QString, QString> reverseFun;
    QStringList output;
};

#endif // ASSEMBLER_H
