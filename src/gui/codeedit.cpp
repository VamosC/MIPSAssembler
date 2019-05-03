#include "codeedit.h"
#include "linenumberarea.h"

CodeEdit::CodeEdit(QWidget *parent) :
    QPlainTextEdit (parent),
    mode_debug(false)
{
    lineNumberArea = new LineNumberArea(this);
    initSlots();
    updateLineNumberAreaWidth(0);
}

void CodeEdit::keyPressEvent(QKeyEvent *e) {
    QPlainTextEdit::keyPressEvent(e);
}

void CodeEdit::mousePressEvent(QMouseEvent *e) {
    if(mode_debug)
        return;
    QPlainTextEdit::mousePressEvent(e);
}

void CodeEdit::mouseReleaseEvent(QMouseEvent *e) {
    QPlainTextEdit::mouseReleaseEvent(e);
}

void CodeEdit::mouseDoubleClickEvent(QMouseEvent *e) {
    if(mode_debug)
        return;
    QPlainTextEdit::mouseDoubleClickEvent(e);
}

void CodeEdit::dragEnterEvent(QDragEnterEvent *e) {
    fileName = e->mimeData()->urls()[0].fileName();
    e->acceptProposedAction();
}

void CodeEdit::dragMoveEvent(QDragMoveEvent *e) {
    e->acceptProposedAction();
}

void CodeEdit::dropEvent(QDropEvent *e) {
    QFile file(e->mimeData()->urls()[0].toLocalFile());
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this->parentWidget(), tr("MIPS Assembler"), tr("无法读取文件:%1\n%2.").arg(e->mimeData()->urls()[0].fileName()).arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    setPlainText(in.readAll());
    moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    this->parentWidget()->findChild<console*>("Console")->append("-> open file: " + e->mimeData()->urls()[0].toLocalFile());
    e->acceptProposedAction();
}

//get content of plaintext
const QStringList CodeEdit::getPlainText() {
    QStringList list;
    int count = this->document()->blockCount();
    for(int i = 0; i < count; i++) {
        list << this->document()->findBlockByNumber(i).text();
    }
    return list;
}

void CodeEdit::initSlots() {
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(displayCurrentLine()));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
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

int CodeEdit::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while(max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEdit::updateLineNumberAreaWidth(int) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEdit::updateLineNumberArea(const QRect& rect, int dy) {
    if(dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    if(rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEdit::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(30, 30, 30));
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while(block.isValid() && top <= event->rect().bottom()) {
        if(block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(144, 145, 139));
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
