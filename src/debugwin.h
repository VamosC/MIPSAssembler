#ifndef DEBUGWIN_H
#define DEBUGWIN_H

#include "regs.h"
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QFont>

class DebugWin : public QTableView
{
public:
    explicit DebugWin(QWidget *parent=nullptr);
    void changeVisible();
    Regs* getRegs() const;
    ~DebugWin();
private:
    void initModel();
    bool status;
    Regs* model;
};

#endif // DEBUGWIN_H
