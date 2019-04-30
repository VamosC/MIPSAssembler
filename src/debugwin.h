#ifndef DEBUGWIN_H
#define DEBUGWIN_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QKeyEvent>
#include <QStandardItemModel>

class DebugWin : public QTableView
{
public:
    explicit DebugWin(QWidget *parent=nullptr);
    void changeVisible();
    void changeData(int index, QString value);
    void resetData();
    ~DebugWin();
private:
    void initModel();
    static const int numOfReg = 32;

    QString t[numOfReg] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                           "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};
    bool status;
    QStandardItemModel* model;
};

#endif // DEBUGWIN_H
