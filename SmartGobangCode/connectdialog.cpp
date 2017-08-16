#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QPushButton>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    createKeyboard();
}

void ConnectDialog::createKeyboard() {
    for (int i = 1; i <= 12; i++) {
        QPushButton* button = new QPushButton;

        switch (i) {
        case 10:
            button->setText("AC"); break;
        case 11:
            button->setText("0"); break;
        case 12:
            button->setText("."); break;
        default:
            button->setText(QString::number(i)); break;
        }
        ui->gridLayout->addWidget(button, (i - 1) / 3, (i - 1) % 3);
        connect(button, &QPushButton::clicked, [=]() {
            switch (i) {
            case 10:
                ui->ipEdit->setText("");
                break;

            case 11:
                ui->ipEdit->setText(ui->ipEdit->text() + "0");
                break;

            case 12:
                ui->ipEdit->setText(ui->ipEdit->text() + ".");
                break;

            default:
                ui->ipEdit->setText(ui->ipEdit->text() + QString::number(i));
                break;
            }
        });
    }
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

QString ConnectDialog::getInputIP() {
    return ui->ipEdit->text();
}
