#include "createdialog.h"
#include "ui_createdialog.h"
#include <QTcpServer>
#include <QTcpServer>
#include <QtNetwork>
#include <QDebug>

CreateDialog::CreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDialog)
{
    ui->setupUi(this);
    ui->ipEdit->setText(getHostIP());
}

QString CreateDialog::getHostIP() {
    QString localHostName = QHostInfo::localHostName();
    //qDebug() <<"localHostName:"<<localHostName;
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address, info.addresses()) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol) {
            return address.toString();
            //qDebug() <<"IPV4 Address: "<< address.toString();
        }
    }
}

QString CreateDialog::getInputIP() {
    return ui->ipEdit->text();
}

CreateDialog::~CreateDialog()
{
    delete ui;
}
