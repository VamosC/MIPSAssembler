#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    debugger(nullptr)
{
    ui->setupUi(this);
    codeWindow = this->findChild<CodeEdit*>("CodeWindow");
    consoleWindow = this->findChild<console*>("Console");
    debugWindow = this->findChild<DebugWin*>("DebugWindow");
    consoleWindow->installEventFilter(this);
    debugWindow->installEventFilter(this);
    codeWindow->installEventFilter(this);
    initFileAction();
    initGenerate();
    initDebug();
    h = new Highlighter(codeWindow->document());
//    codeWindow->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

MainWindow::~MainWindow()
{
    delete h;
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Escape) {
            if(codeWindow->getMode()) {
                codeWindow->setMode(false);
                codeWindow->setReadOnly(false);
                codeWindow->displayCurrentLine();
                consoleWindow->clear();
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
    QAction *new_file_action = findChild<QAction*>("open_file");
    QAction *save_file_action = findChild<QAction*>("save_file");

    connect(new_file_action, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(save_file_action, SIGNAL(triggered()), this, SLOT(saveFile()));
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
}

void MainWindow::saveFile() {
//    QTextCodec *codec = QTextCodec::codecForName("GBK");
//    QTextCodec::setCodecForLocale(codec);
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
}

void MainWindow::initGenerate() {
    QAction* generate_bit = findChild<QAction*>("generate");
    QAction* assemble_action = findChild<QAction*>("assemble");
    QAction* generate_coe_asm = findChild<QAction*>("generate_coe_asm");
    QAction* disassemble_action = findChild<QAction*>("disassemble");

    connect(generate_bit, SIGNAL(triggered()), this, SLOT(generate()));
    connect(assemble_action, SIGNAL(triggered()), this, SLOT(assemble()));
    connect(generate_coe_asm, SIGNAL(triggered()), this, SLOT(generateCoeAsm()));
    connect(disassemble_action, SIGNAL(triggered()), this, SLOT(disassemble()));
}

void MainWindow::assemble() {
    consoleWindow->clear();
    QStringList list = codeWindow->getPlainText();
    Assembler assembler(list);
    assembler.process();
    QStringList res = assembler.getRes();
    consoleWindow->append("output:");
    for(auto s : res) {
        consoleWindow->append(s);
    }
}

void MainWindow::generateCoeAsm() {
    QStringList list = codeWindow->getPlainText();
    Assembler assember(list);
    assember.process();
    QStringList res = assember.getRes();
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
    in << "memory_initialization_radix=16;\n";
    in << "memory_initialization_vector=\n";
    for(int i = 0; i < count; i++) {
        if(i < count-1)
            in << res[i] << ", ";
        else {
            in << res[i] << ";";
        }
        if((i+1) % 8 == 0) {
            in << "\n";
        }
    }
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
}

void MainWindow::disassemble() {
    QStringList list = codeWindow->getPlainText();
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
}

void MainWindow::initDebug() {
    QAction *actionStart = findChild<QAction*>("actionStart");
    QAction *actionSingle_step = findChild<QAction*>("actionSingle_step");
    connect(actionStart, SIGNAL(triggered()), this, SLOT(debug()));
    connect(actionSingle_step, SIGNAL(triggered()), this, SLOT(singleStep()));
}

void MainWindow::debug() {
    consoleWindow->setText("In the debug mode, press esc to exist......");
    codeWindow->setMode(true);
    codeWindow->setReadOnly(true);
    QStringList input = codeWindow->getPlainText();
    Assembler assembler(input);
    assembler.process();
    InstMem output = assembler.getInstMem();
    debugWindow->getRegs()->reset();
    debugger = new Debugger(debugWindow->getRegs());
    debugger->setInst(output);
    codeWindow->changeCursor(debugger->getNextPC());
}

void MainWindow::singleStep() {
    if(debugger->next()) {
        codeWindow->changeCursor(debugger->getNextPC());
    } else {
       codeWindow->setMode(false);
       codeWindow->setReadOnly(false);
       codeWindow->displayCurrentLine();
       consoleWindow->clear();
       delete debugger;
    }
}
