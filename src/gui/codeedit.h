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
#include <QPaintEvent>
#include <QRect>
#include <QColor>
#include <QResizeEvent>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QFile>

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEdit(QWidget *parent=nullptr);
    void changeCursor(int index);
    const QStringList getPlainText();
    void setMode(bool mode);
    bool getMode() const;
    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();
protected:
    void keyPressEvent(QKeyEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;

public slots:
    void displayCurrentLine();
private slots:
    void updateLineNumberAreaWidth(int);
    void updateLineNumberArea(const QRect&, int);
private:
    void initSlots();
    bool mode_debug;
    QWidget *lineNumberArea;
    QString fileName;
    QString fileNameUrl;
};

#endif // CODEEDIT_H
