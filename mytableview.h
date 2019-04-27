#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QKeyEvent>

class myTableView : public QTableView
{
public:
    explicit myTableView(QWidget *parent=nullptr);
    void keyPressEvent(QKeyEvent *e);
private:
    bool status;
};

#endif // MYTABLEVIEW_H
