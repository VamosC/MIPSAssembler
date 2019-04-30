#include "codeedit.h"

CodeEdit::CodeEdit(QWidget *parent) :
    QPlainTextEdit (parent),
    mode_debug(false)
{
    initSlots();
}

void CodeEdit::keyPressEvent(QKeyEvent *e) {
    QPlainTextEdit::keyPressEvent(e);
}

void CodeEdit::mousePressEvent(QMouseEvent *e) {
    if(mode_debug)
        return;
    QPlainTextEdit::mousePressEvent(e);
}

void CodeEdit::mouseDoubleClickEvent(QMouseEvent *e) {
    if(mode_debug)
        return;
    QPlainTextEdit::mouseDoubleClickEvent(e);
}

//get content of plaintext
const QStringList CodeEdit::getPlainText() {
    QStringList list;
    int count = this->document()->blockCount();
    for(int i = 0; i < count; i++) {
        list << this->document()->findBlockByLineNumber(i).text();
    }
    return list;
}

void CodeEdit::initSlots() {
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(displayCurrentLine()));
}

void CodeEdit::changeCursor(int index) {
    QTextCursor cursor(document());
    cursor.setPosition(document()->findBlockByLineNumber(index).position());
    setTextCursor(cursor);
    displayCurrentLine();
}

void CodeEdit::displayCurrentLine() {
    QList<QTextEdit::ExtraSelection> extras;
    QTextEdit::ExtraSelection sel;
    if(!isReadOnly()) {

        QColor lineColor = QColor(Qt::gray).lighter(35);

        sel.format.setBackground(lineColor);
        sel.format.setProperty(QTextFormat::FullWidthSelection, true);
        sel.cursor = textCursor();
        sel.cursor.clearSelection();
        extras.append(sel);
    } else {
        QColor lineColor = QColor(Qt::yellow).lighter(35);

        sel.format.setBackground(lineColor);
        sel.format.setProperty(QTextFormat::FullWidthSelection, true);
        sel.cursor = textCursor();
        sel.cursor.clearSelection();
        extras.append(sel);
    }

    setExtraSelections(extras);
}

void CodeEdit::setMode(bool mode) {
    mode_debug = mode;
}

bool CodeEdit::getMode() const {
    return mode_debug;
}
