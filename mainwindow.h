#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <CodeEdit.h>
#include <mytableview.h>
#include <QTextBrowser>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QTextCodec>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextBlock>
#include <assembler.h>
#include <QStringList>
#include "highlighter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
     void newFile();
     void saveFile();
     void assemble();
     void disassemble();
     void generateCoeAsm();
     void generate();

private:
    void initFileAction();
    void initGenerate();
    Ui::MainWindow *ui;
    CodeEdit *codeWindow;
    QTextBrowser *console;
    myTableView *debugWindow;
};

#endif // MAINWINDOW_H
