#ifndef CONSOLE_H
#define CONSOLE_H

#include<QTextBrowser>
#include<QKeyEvent>

class console : public QTextBrowser
{
    Q_OBJECT
public:
    explicit console(QWidget *parent=nullptr);
    void changeVisible();
private:
    bool status;
};

#endif // CONSOLE_H
