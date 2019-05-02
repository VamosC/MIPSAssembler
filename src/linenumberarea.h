#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include "codeedit.h"
#include <QPaintEvent>
#include <QSize>
#include <QWidget>

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(CodeEdit* editor);
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    CodeEdit *codeEditor;
};

#endif // LINENUMBERAREA_H
