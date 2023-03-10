#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SendToServer(QString s)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << QTime::currentTime() << s;
    socket->write(Data);
    ui->lineEdit->clear();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    if(in.status() == QDataStream::Ok)
    {
        QTime time;
        QString s;
        in >> time >> s;
        ui->textBrowser->append(time.toString() + " " + s);
    }
    else
    {
        ui->textBrowser->append("error");
    }

}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(this, "1", "1");
    socket->connectToHost("127.0.0.1", 2323);
}

void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}
