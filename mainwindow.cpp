#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    codeWindow = this->findChild<CodeEdit*>("CodeWindow");
    console = this->findChild<QTextBrowser*>("Console");
    console->setTextColor(QColor(0, 242, 1));
    debugWindow = this->findChild<myTableView*>("DebugWindow");
    initFileAction();
    initGenerate();
    QStandardItemModel* model = new QStandardItemModel(debugWindow);
    model->setColumnCount(2);
    model->setItem(0, 0, new QStandardItem("$zero"));
    model->setItem(0, 1, new QStandardItem("00000000"));
    model->setItem(1, 0, new QStandardItem("$at"));
    model->setItem(1, 1, new QStandardItem("00000001"));
    model->setItem(2, 0, new QStandardItem("$v0"));
    model->setItem(2, 1, new QStandardItem("00000003"));
    debugWindow->setModel(model);
    //    debugWindow->verticalHeader()->hide();
    debugWindow->horizontalHeader()->hide();
    debugWindow->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    debugWindow->setEditTriggers(QAbstractItemView::NoEditTriggers);
    debugWindow->setGridStyle(Qt::NoPen);
    Highlighter* h = new Highlighter(codeWindow->document());
//    console->setTextColor(Qt::green);
//    console->setText("hhh");
    //设置文本
//    codeWindow->setPlainText("Test");
    //移动光标至末尾
//    codeWindow->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initFileAction() {
    QAction *new_file_action = this->findChild<QAction*>("open_file");
    connect(new_file_action, SIGNAL(triggered()), this, SLOT(newFile()));
    QAction *save_file_action = this->findChild<QAction*>("save_file");
    connect(save_file_action, SIGNAL(triggered()), this, SLOT(saveFile()));
}

void MainWindow::newFile() {
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QString fileName = QFileDialog::getOpenFileName(this);
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
    QAction* generate_bit = this->findChild<QAction*>("generate");
    QAction* assemble_action = this->findChild<QAction*>("assemble");
    QAction* generate_coe_asm = this->findChild<QAction*>("generate_coe_asm");
    QAction* disassemble_action = this->findChild<QAction*>("disassemble");

    connect(generate_bit, SIGNAL(triggered()), this, SLOT(generate()));
    connect(assemble_action, SIGNAL(triggered()), this, SLOT(assemble()));
    connect(generate_coe_asm, SIGNAL(triggered()), this, SLOT(generateCoeAsm()));
    connect(disassemble_action, SIGNAL(triggered()), this, SLOT(disassemble()));
}

void MainWindow::assemble() {
    console->clear();
    QStringList list = codeWindow->getPlainText();
    Assembler assembler(list);
    assembler.assemble();
    QStringList res = assembler.getRes();
    for(auto s : res) {
        console->append(s);
    }
}

void MainWindow::generateCoeAsm() {
    QStringList list = codeWindow->getPlainText();
    Assembler assember(list);
    assember.assemble();
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
    assembler.assemble();
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
    Assembler assembler(list);
    assembler.disassemble();
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
