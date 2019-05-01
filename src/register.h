#ifndef REGISTER_H
#define REGISTER_H

#include <QString>
#include <QStandardItem>

class Register
{
public:
    Register(const QString& id, const QString& val);
    ~Register();
    void setData(const QString& val);
    const QString getData() const;
    QStandardItem* first();
    QStandardItem* second();
private:
    QStandardItem* identifier;
    QStandardItem* value;
};

#endif // REGISTER_H
