#include "CodeEdit.h"

CodeEdit::CodeEdit(QWidget *parent) :
    QPlainTextEdit (parent)
{
    initSlots();
}

void CodeEdit::keyPressEvent(QKeyEvent *e) {
    QPlainTextEdit::keyPressEvent(e);
    if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        qDebug() << "Enter" << endl;
    }
}

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

void CodeEdit::displayCurrentLine() {
    QList<QTextEdit::ExtraSelection> extras;
    if(!isReadOnly()) {
        QTextEdit::ExtraSelection sel;

        QColor lineColor = QColor(Qt::gray).lighter(30);

        sel.format.setBackground(lineColor);
        sel.format.setProperty(QTextFormat::FullWidthSelection, true);
        sel.cursor = textCursor();
        sel.cursor.clearSelection();
        extras.append(sel);
    }

    setExtraSelections(extras);
}
