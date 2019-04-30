#ifndef BITSKILLER_H
#define BITSKILLER_H

#include <QString>
#include <map>

class BitsKiller
{
public:
    BitsKiller();

private:
    void initHexToBitTable();
    void initBitToHexTable();
    std::map<QString, QString> hexToBitTable;
    std::map<QString, QString> bitToHexTable;

protected:
    const QString _32bitToHexFormat(const QString& s);
    const QString hexTobit(const QString& x);
    const QChar _1bitToQChar_2(int x);
    const QString tenToString_nbit(unsigned int x, int n=5);
    const QString Unext_n(const QString& s, int n=16);
};

#endif // BITSKILLER_H
