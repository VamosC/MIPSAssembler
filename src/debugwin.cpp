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
    model = new Regs(this);
    setModel(model);
}

Regs* DebugWin::getRegs() const {
    return  model;
}

DebugWin::~DebugWin() {
    delete model;
}


