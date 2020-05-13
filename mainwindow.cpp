#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <QDebug>
#include <stdlib.h>
#include <stdio.h>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineIPAdress->setText("192.168.1.100");
    ui->linePassword->setText("LM@2018-0420");
    ui->lineUsername->setText("linkmiao");
    ui->lineDir->setText("~/");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this ,"选择文件" ,"~/" ,"All files(*)");
//    //定义文件对话框标题
//    fileDialog->setWindowTitle(QStringLiteral("选中文件"));
//    //设置默认文件路径
//    fileDialog->setDirectory(".");
//    //设置文件过滤器
//    fileDialog->setNameFilter(tr("File(*.*)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    if (fileDialog->exec()) {
        fileNames << fileDialog->selectedFiles();
        ui->textFileName->clear();
        for(auto iter : fileNames){
            ui->textFileName->insertPlainText(iter);
            ui->textFileName->insertPlainText("\n");
        }
        qDebug() << fileNames;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QStringList recv;
    QString cmd_str;
    cmd_str += "sshpass -p ";
    cmd_str += ui->linePassword->text();
    cmd_str += " scp ";
    for(auto iter : fileNames){
        cmd_str += iter;
        cmd_str += ' ';
    }
    cmd_str += ' ';
    cmd_str += ui->lineUsername->text();
    cmd_str += '@';
    cmd_str += ui->lineIPAdress->text();
    cmd_str += ':';
    cmd_str += ui->lineDir->text();
    qDebug() << cmd_str;
    exec_cmd_using_popen(cmd_str.toStdString().data() ,recv);
    for(auto iter : recv){
        ui->textExecResult->insertPlainText(iter);
        ui->textExecResult->insertPlainText("\n");
    }
    qDebug() << recv;

}

int MainWindow::exec_cmd_using_popen(char const* cmd, QStringList& res_vec)
{
    if (cmd == NULL)
    {
        qDebug("popen cmd is NULL\n");
        return 1;
    }
    char tmp[1024];
    FILE* fp = popen(cmd, "r");
    if (NULL == fp)
    {
        qDebug("exec cmd: %s failed, err: %s\n", cmd,strerror(errno));
        return 1;
    }
    while (fgets(tmp, 1024, fp) != NULL)
    {
        if (tmp[strlen(tmp) - 1] == '\n')
        {
            tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        }
        res_vec.push_back(tmp);
    }
    qDebug("exec cmd: %s ,success\n", cmd);

    pclose(fp);
    return 0;
}

