#include "console.h"

console::console(QWidget *parent) :
    QTextBrowser (parent),
    status(true)
{
    setTextColor(QColor(0, 242, 1));
    setText("ZJU@CS@Liang Chao 2019");
}

void console::changeVisible() {
    status = !status;
    setVisible(status);
}
