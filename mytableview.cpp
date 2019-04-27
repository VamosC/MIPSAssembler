#include "mytableview.h"

myTableView::myTableView(QWidget *parent) :
    QTableView (parent), status(true)
{

}

void myTableView::keyPressEvent(QKeyEvent *e) {
    QTableView::keyPressEvent(e);
    if((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_1)) {
        status = !status;
        this->setVisible(status);
    }
}


