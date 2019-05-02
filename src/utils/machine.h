#ifndef MACHINE_H
#define MACHINE_H

#include <QStringList>
#include <QString>
#include <map>

class Machine
{
public:
    Machine();
    Machine(QStringList list);
    void setInput(QStringList input);
    const QStringList getRes() const;
    virtual void process() = 0;
    virtual ~Machine();

protected:
    QStringList list;
    QStringList output;
    std::map<QString, int> instTypeSet;
private:
    void initInstTypeSet();
};

#endif // MACHINE_H
