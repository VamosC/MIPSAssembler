#include "debugwin.h"

DebugWin::DebugWin(QWidget *parent) :
    QTableView (parent),
    status(true)
{
    setStyleSheet("QTableView{selection-background-color:transparent;}"); //remove selection-color (default blue)
    horizontalHeader()->hide();
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setEditTriggers(QAbstractItemView::NoEditTriggers); //no edit
    setGridStyle(Qt::NoPen);
    initModel();
}

void DebugWin::changeVisible() {
    status = !status;
    setVisible(status);
}

void DebugWin::initModel() {
    model = new QStandardItemModel(this);
    model->setColumnCount(2);
    for(int i = 0; i < numOfReg; i++) {
        model->setItem(i+1, 0, new QStandardItem("$"+t[i]));
        model->setItem(i+1, 1, new QStandardItem("00000000"));
    }
    model->setItem(0, 0, new QStandardItem("PC"));
    model->setItem(0, 1, new QStandardItem("00000000"));
    setModel(model);
}

void DebugWin::changeData(int index, QString value) {
    model->setItem(index, 1, new QStandardItem(value));
}

void DebugWin::resetData() {
    for(int i = 0; i < numOfReg; i++)
        changeData(i+1, "00000000");
    changeData(0, "00000000");
}

DebugWin::~DebugWin() {
    delete model;
}


