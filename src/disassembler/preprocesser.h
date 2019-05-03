#ifndef PREPROCESSER_H
#define PREPROCESSER_H

#include "utils/bitskiller.h"
#include <QStringList>
#include <QRegExp>

class PreProcesser : public BitsKiller
{
public:
    PreProcesser();
    void process(const QStringList& input);
    QStringList getRes() const;
private:
    QStringList output;
};

#endif // PREPROCESSER_H
