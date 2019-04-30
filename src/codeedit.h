#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QDebug>
#include <QStringList>
#include <QTextBlock>
#include <QTextEdit>
#include <QList>
#include <QTextFormat>
#include <QTextCursor>

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEdit(QWidget *parent=nullptr);
    void keyPressEvent(QKeyEvent *e);
    void changeCursor(int index);
    const QStringList getPlainText();
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void setMode(bool mode);
    bool getMode() const;
public slots:
    void displayCurrentLine();
private:
    void initSlots();
    bool mode_debug;


};

#endif // CODEEDIT_H
