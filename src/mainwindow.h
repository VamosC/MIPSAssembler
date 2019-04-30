#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeedit.h"
#include "console.h"
#include "debugwin.h"
#include "assembler.h"
#include "disassembler.h"
#include "highlighter.h"
#include "instmem.h"
#include "debugger.h"
#include <QMainWindow>
#include <QTextBrowser>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QTextCodec>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextBlock>
#include <QStringList>
#include <QEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *event);

public slots:
     void openFile();
     void saveFile();
     void assemble();
     void disassemble();
     void generateCoeAsm();
     void generate();
     void debug();

private:
    void initFileAction();
    void initGenerate();
    void initConsole();
    void initDebug();

    Ui::MainWindow *ui;
    CodeEdit *codeWindow;
    console *consoleWindow;
    DebugWin *debugWindow;

    bool mode_debug;
};

#endif // MAINWINDOW_H
