#ifndef ALU_H
#define ALU_H

#include "bitskiller.h"

class ALU : public BitsKiller
{
public:
    ALU(int ctrl);
    bool iszero();
    void run(const QString& a, const QString& b);
    const QString getRes() const;
private:
    int choice;
    QString res;
    int zero;
};

#endif // ALU_H
