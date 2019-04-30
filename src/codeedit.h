#ifndef CODEEDIT_H
#define CODEEDIT_H

#include<QPlainTextEdit>
#include<QKeyEvent>
#include<QDebug>
#include<QStringList>
#include<QTextBlock>
#include<QTextEdit>
#include<QList>
#include<QTextFormat>

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEdit(QWidget *parent=nullptr);
    void keyPressEvent(QKeyEvent *e);
    const QStringList getPlainText();
public slots:
    void displayCurrentLine();
private:
    void initSlots();


};

#endif // CODEEDIT_H
