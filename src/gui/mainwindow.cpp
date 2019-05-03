#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    debugger(nullptr),
    finish(false)
{
    ui->setupUi(this);
    codeWindow = this->findChild<CodeEdit*>("CodeWindow");
    consoleWindow = this->findChild<console*>("Console");
    debugWindow = this->findChild<DebugWin*>("DebugWindow");
    consoleWindow->installEventFilter(this);
    debugWindow->installEventFilter(this);
    codeWindow->installEventFilter(this);
    codeWindow->setAcceptDrops(true);
    initFileAction();
    initGenerate();
    initDebug();
    initHelp();
    h = new Highlighter(codeWindow->document());
}

MainWindow::~MainWindow()
{
    delete h;
    delete ui;
}

bool MainWindow::eventFilter(QObject*, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Escape) {
            if(codeWindow->getMode()) {
                finish = true;
                codeWindow->setMode(false);
                codeWindow->setReadOnly(false);
                codeWindow->displayCurrentLine();
                consoleWindow->append("-> Stop debug...");
                actionStart->setEnabled(true);
                actionSingle_step->setEnabled(false);
                actionRun->setEnabled(false);
                delete debugger;
                return true;
            }
        } else if((keyEvent->modifiers() == Qt::ControlModifier) && (keyEvent->key() == Qt::Key_1)) {
            debugWindow->changeVisible();
            return true;
        } else if((keyEvent->modifiers() == Qt::ControlModifier) && (keyEvent->key() == Qt::Key_2)) {
            consoleWindow->changeVisible();
            return true;
        }
    }
    return false;
}

void MainWindow::initFileAction() {
    QAction *new_file = findChild<QAction*>("new_file");
    QAction *open_file_action = findChild<QAction*>("open_file");
    QAction *save_file_action = findChild<QAction*>("save_file");

    connect(open_file_action, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(save_file_action, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(new_file, SIGNAL(triggered()), this, SLOT(newFile()));
}

void MainWindow::newFile() {
    codeWindow->clear();
}


void MainWindow::openFile() {
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QString fileName = QFileDialog::getOpenFileName(this);
    if(fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MIPS Assembler"), tr("无法读取文件:%1\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    //读取文件内容
    codeWindow->setPlainText(in.readAll());
    //设置光标在末尾
    codeWindow->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    consoleWindow->append("-> open file: " + fileName);
}

void MainWindow::saveFile() {
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QString fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MIPS Assembler"), tr("无法读取文件:%1\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    QStringList list = codeWindow->getPlainText();
    QString res;
    int count = list.size();
    for(int i = 0; i < count; i++) {
        res.append(list[i]);
        if(i < count - 1) {
            res.append('\n');
        }
    }
    file.write(res.toUtf8());
    consoleWindow->append("-> save file: " + fileName);
}

void MainWindow::initHelp() {
    QAction* actionHelp = findChild<QAction*>("actionManual");
    QAction* actionAbout = findChild<QAction*>("About");
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "information", "Developed by Liang Chao @ ZJU.\nCopyright 2019.All right reversed.");
}

void MainWindow::showHelp() {
    QMessageBox::about(this, "MIPSAssembler", "See <a href='https://github.com/VamosC/MIPSAssembler'>https://github.com/VamosC/MIPSAssembler<\a>");
}

void MainWindow::initGenerate() {
    QAction* generate_bit = findChild<QAction*>("generate");
    QAction* assemble_action = findChild<QAction*>("assemble");
    QAction* generate_coe_asm = findChild<QAction*>("generate_coe_asm");
    QAction* disassemble_action = findChild<QAction*>("disassemble");
    QAction* disasm_coe = findChild<QAction*>("disasm_coe");
    QAction* generate_coe = findChild<QAction*>("generate_coe");

    connect(generate_bit, SIGNAL(triggered()), this, SLOT(generate()));
    connect(assemble_action, SIGNAL(triggered()), this, SLOT(assemble()));
    connect(generate_coe_asm, SIGNAL(triggered()), this, SLOT(generateCoeAsm()));
    connect(generate_coe, SIGNAL(triggered()), this, SLOT(generateCoeBin()));
    connect(disassemble_action, SIGNAL(triggered()), this, SLOT(disassemble_bin()));
    connect(disasm_coe, SIGNAL(triggered()), this, SLOT(disassemble_coe()));
}

void MainWindow::assemble() {
    consoleWindow->clear();
    QStringList list = codeWindow->getPlainText();
    Assembler assembler(list);
    assembler.process();
    QStringList res = assembler.getRes();
    consoleWindow->append("-> output:");
    for(auto s : res) {
        consoleWindow->append(s);
    }
}

void MainWindow::generateCoeAsm() {
    QStringList list = codeWindow->getPlainText();
    Assembler assember(list);
    assember.process();
    QStringList res = assember.getRes();
    generateCoe(res);
}

void MainWindow::generateCoeBin() {
    QStringList list = codeWindow->getPlainText();
    generateCoe(list);
}

void MainWindow::generateCoe(const QStringList& list) {
    QString fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MIPS Assembler"), tr("无法读取文件:%1\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    int count = list.size();
    QTextStream in(&file);
    in << "memory_initialization_radix=16;\n";
    in << "memory_initialization_vector=\n";
    for(int i = 0; i < count; i++) {
        if(i < count-1)
            in << list[i] << ", ";
        else {
            in << list[i] << ';';
        }
        if((i+1) % 8 == 0) {
            in << '\n';
        }
    }
    consoleWindow->append("-> output file: " + fileName);
}



void MainWindow::generate() {
    QStringList list = codeWindow->getPlainText();
    Assembler assembler(list);
    assembler.process();
    QStringList res = assembler.getRes();
    QString fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MIPS Assembler"), tr("无法读取文件:%1\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    int count = res.size();
    QTextStream in(&file);
    for(int i = 0; i < count; i++) {
        if(i < count - 1) {
            in << res[i] << '\n';
        } else {
            in << res[i];
        }
    }
    consoleWindow->append("-> output file: " + fileName);
}

void MainWindow::disassemble_bin() {
    QStringList list = codeWindow->getPlainText();
    disassemble(list);
}

void MainWindow::disassemble_coe() {
    QStringList list = codeWindow->getPlainText();
    PreProcesser preProcessor;
    preProcessor.process(list);
    list = preProcessor.getRes();
    disassemble(list);
}

void MainWindow::disassemble(QStringList& list) {
    Disassembler disassembler(list);
    disassembler.process();
    QStringList res = disassembler.getRes();
    QString fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MIPS Assembler"), tr("无法读取文件:%1\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    int count = res.size();
    QTextStream in(&file);
    for(int i = 0; i < count; i++) {
        if(i < count - 1) {
            in << res[i] << '\n';
        } else {
            in << res[i];
        }
    }
    consoleWindow->append("-> output file: " + fileName);
}

void MainWindow::initDebug() {
    actionStart = findChild<QAction*>("actionStart");
    actionSingle_step = findChild<QAction*>("actionSingle_step");
    actionRun = findChild<QAction*>("actionRun");
    connect(actionStart, SIGNAL(triggered()), this, SLOT(debug()));
    connect(actionSingle_step, SIGNAL(triggered()), this, SLOT(singleStep()));
    connect(actionRun, SIGNAL(triggered()), this, SLOT(debugRun()));
    actionSingle_step->setEnabled(false);
    actionRun->setEnabled(false);
}

void MainWindow::debug() {
    consoleWindow->append("-> In the debug mode, press esc to exist......");
    codeWindow->setMode(true);
    codeWindow->setReadOnly(true);
    actionStart->setEnabled(false);
    actionSingle_step->setEnabled(true);
    actionRun->setEnabled(true);
    QStringList input = codeWindow->getPlainText();
    Assembler assembler(input);
    assembler.process();
    InstMem output = assembler.getInstMem();
    debugWindow->getRegs()->reset();
    debugger = new Debugger(debugWindow->getRegs());
    debugger->setInst(output);
    codeWindow->changeCursor(debugger->getNextPC());
    finish = false;
}

void MainWindow::singleStep() {
    if(debugger->next()) {
        codeWindow->changeCursor(debugger->getNextPC());
    } else {
       codeWindow->setMode(false);
       codeWindow->setReadOnly(false);
       codeWindow->displayCurrentLine();
       consoleWindow->append("-> Exit successfully!");
       actionStart->setEnabled(true);
       actionSingle_step->setEnabled(false);
       actionRun->setEnabled(false);
       delete debugger;
    }
}

void MainWindow::debugRun() {
    while(!finish && debugger->next()) {
        codeWindow->changeCursor(debugger->getNextPC());
        QThread::msleep(100);
    }
    if(finish)
        return;
    codeWindow->setMode(false);
    codeWindow->setReadOnly(false);
    codeWindow->displayCurrentLine();
    consoleWindow->append("-> Exit successfully!");
    actionStart->setEnabled(true);
    actionSingle_step->setEnabled(false);
    actionRun->setEnabled(false);
    delete debugger;
}

void MainWindow::initEdit() {
    QAction* actionRedo = findChild<QAction*>("actionRedo");
    QAction* actionUndo = findChild<QAction*>("actionUndo");
    QAction* actionCopy = findChild<QAction*>("actionCopy");
    QAction* actionPaste = findChild<QAction*>("actionPaste");
    QAction* actionCut = findChild<QAction*>("actionCut");
    connect(actionRedo, SIGNAL(triggered()), this, SLOT(editRedo()));
    connect(actionUndo, SIGNAL(triggered()), this, SLOT(editUndo()));
    connect(actionCut, SIGNAL(triggered()), this, SLOT(editCut()));
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(editCopy()));
    connect(actionPaste, SIGNAL(triggered()), this, SLOT(editPaste()));
}

void MainWindow::editRedo() {
    codeWindow->redo();
}

void MainWindow::editUndo() {
    codeWindow->undo();
}

void MainWindow::editCut() {
    codeWindow->cut();
}

void MainWindow::editCopy() {
    codeWindow->copy();
}

void MainWindow::editPaste() {
    codeWindow->paste();
}
