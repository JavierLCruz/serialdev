#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

void MainWindow::busca(){
    if(QSerialPortInfo::availablePorts().length()){
        foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
            if(serialPortInfo.productIdentifier()){            
                ui->comboBox->addItem(serialPortInfo.portName());
                    QString msg1 = "Descripcion: " + serialPortInfo.description() + "\n"
                                + "Manufacturer: " + serialPortInfo.manufacturer() + "\n"
                                + "Port Name: " + serialPortInfo.portName() + "\n"
                                + "Product ID: " + serialPortInfo.productIdentifier() + "\n"
                                + "Serial Number: " + serialPortInfo.serialNumber() + "\n"
                                + "System location: " + serialPortInfo.systemLocation() + "\n"
                                + "Vendedor ID: " + serialPortInfo.vendorIdentifier() + "\n";
                            ui->plainTextEdit_2->appendPlainText(msg1);
            }
        }
    }
    else{
            ui->plainTextEdit_2->appendPlainText("No Hay Dispositivo Conectado.");
            ui->pushButton_2->setEnabled(false);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_3->setEnabled(false);
    serialdev = new QSerialPort();
    busca();
}

void MainWindow::RecibirArreglo(){
    QByteArray arreglo;
    arreglo=serialdev->readLine();
    cadena=cadena+arreglo;
    if(cadena.contains("\n")){
        ui->plainTextEdit->appendPlainText(cadena);
        cadena="";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    serialdev->close();
}

void MainWindow::on_pushButton_clicked(){
    ui->comboBox->clear();
    busca();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    ui->plainTextEdit->clear();
    serialdev->open(QIODevice::ReadWrite);
    connect(serialdev,SIGNAL(readyRead()),this, SLOT(RecibirArreglo()));
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    serialdev->close();
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1){
    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_2->appendPlainText(arg1);
    serialdev->setPortName(arg1);
    serialdev->setBaudRate(QSerialPort::Baud9600);
    serialdev->setParity(QSerialPort::NoParity);
    serialdev->setDataBits(QSerialPort::Data8);
    serialdev->setFlowControl(QSerialPort::NoFlowControl);
    serialdev->setStopBits(QSerialPort::OneStop);
}
