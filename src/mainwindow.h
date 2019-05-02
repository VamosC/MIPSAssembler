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
#include <QMouseEvent>
#include <QTextCodec>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextBlock>
#include <QStringList>
#include <QEvent>
#include <QThread>
#include <QMutex>
#include <QDropEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void openFile();
    void saveFile();
    void assemble();
    void disassemble();
    void generateCoeAsm();
    void generate();
    void debug();
    void singleStep();
    void debugRun();
    void showHelp();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    void initFileAction();
    void initGenerate();
    void initConsole();
    void initDebug();
    void initHelp();

    Ui::MainWindow *ui;
    CodeEdit *codeWindow;
    console *consoleWindow;
    DebugWin *debugWindow;
    QAction *actionSingle_step;
    QAction *actionStart;
    QAction *actionRun;
    Highlighter* h;
    Debugger* debugger;
    bool finish;
};

#endif // MAINWINDOW_H
