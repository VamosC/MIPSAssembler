#include "register.h"

Register::Register(const QString& id, const QString& val) {
    identifier = new QStandardItem(id);
    value = new QStandardItem(val);
}

Register::~Register() {
    delete identifier;
    delete value;
}

const QString Register::getData() const {
    return value->text();
}

void Register::setData(const QString &val) {
    value->setText(val);
}

QStandardItem* Register::first() {
    return identifier;
}

QStandardItem* Register::second() {
    return value;
}

