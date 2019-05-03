#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gui/codeedit.h"
#include "gui/console.h"
#include "gui/debugwin.h"
#include "gui/highlighter.h"
#include "assembler/assembler.h"
#include "disassembler/disassembler.h"
#include "disassembler/preprocesser.h"
#include "cpu/instmem.h"
#include "debugger/debugger.h"
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

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void assemble();
    void disassemble_coe();
    void disassemble_bin();
    void generateCoeAsm();
    void generateCoeBin();
    void generate();
    void debug();
    void singleStep();
    void debugRun();
    void showHelp();
    void editUndo();
    void editCopy();
    void editRedo();
    void editPaste();
    void editCut();
    void showAbout();

protected:
    bool eventFilter(QObject *, QEvent *event) override;
private:
    void initFileAction();
    void initGenerate();
    void initConsole();
    void initDebug();
    void initHelp();
    void initEdit();
    void disassemble(QStringList& list);
    void generateCoe(const QStringList& list);

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
