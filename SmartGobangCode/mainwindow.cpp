#include "mainwindow.h"
#include "chessboard.h"
#include "createdialog.h"
#include "connectdialog.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

#define DISCONNECT_CODE -1
#define RESET_CODE -2
#define DET 30

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi();
    m_connected = false;
    m_current = 1;
    m_warningType = 2;
    connect(createButton, SIGNAL(clicked()), this, SLOT(on_createButton_clicked()));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(on_connectButton_clicked()));
    connect(stopListeningButton, SIGNAL(clicked()), this, SLOT(on_stopListeningButton_clicked()));
    connect(stopConnectionButton, SIGNAL(clicked()), this, SLOT(on_stopConnectionButton_clicked()));
    connect(warningButton, SIGNAL(clicked()), this, SLOT(on_warningButton_clicked()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(on_resetButton_clicked()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(on_quitButton_clicked()));
    connect(chessBoard, SIGNAL(sendChessPiece(int,int)), this, SLOT(sendMessage(int, int)));
    connect(warningSettingComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_warningSettingComboBox_indexChanged(int)));
    connect(this, SIGNAL(receiveChessPiece(int,int)), chessBoard, SLOT(receiveChessPiece(int,int)));

    createButton->setEnabled(true);
    connectButton->setEnabled(true);
    stopListeningButton->setEnabled(false);
    stopConnectionButton->setEnabled(false);

    m_listenSocket = new QTcpServer;
    m_readWriteSocket = new QTcpSocket;
    connect(m_listenSocket, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    connect(m_readWriteSocket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
}

void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    horizontalLayout = new QHBoxLayout(centralWidget);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);

    chessBoard = new ChessBoard(&m_who, &m_connected, &m_current, &m_warningType, centralWidget);
    horizontalLayout->addWidget(chessBoard);

    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(20);

    //add widget to vertical layout
    createButton = new QPushButton(centralWidget);
    createButton->setIcon(QIcon(":/create.png"));
    verticalLayout->addWidget(createButton);
    connectButton = new QPushButton(centralWidget);
    connectButton->setIcon(QIcon(":/connect.png"));
    verticalLayout->addWidget(connectButton);
    stopListeningButton = new QPushButton(centralWidget);
    stopListeningButton->setIcon(QIcon(":/stoplistening.png"));
    verticalLayout->addWidget(stopListeningButton);
    stopConnectionButton = new QPushButton(centralWidget);
    stopConnectionButton->setIcon(QIcon(":/stopconnection.png"));
    verticalLayout->addWidget(stopConnectionButton);
    connectionStatusLabel = new QLabel(centralWidget);
    verticalLayout->addWidget(connectionStatusLabel);
    whoStatusLabel = new QLabel(centralWidget);
    verticalLayout->addWidget(whoStatusLabel);
    verticalLayout->addStretch();
    chessStatusLabel = new QLabel(centralWidget);
    verticalLayout->addWidget(chessStatusLabel);
    verticalLayout->addStretch();
    warningButton = new QPushButton(centralWidget);
    warningButton->setIcon(QIcon(":/warning.png"));
    verticalLayout->addWidget(warningButton);
    warningSettingComboBox = new QComboBox(centralWidget);
    warningSettingComboBox->addItem(tr("弱提醒"));
    warningSettingComboBox->addItem(tr("强提醒"));
    verticalLayout->addWidget(warningSettingComboBox);
    resetButton = new QPushButton(centralWidget);
    resetButton->setIcon(QIcon(":/restart.png"));
    verticalLayout->addWidget(resetButton);
    verticalLayout->addStretch();
    quitButton = new QPushButton(centralWidget);
    quitButton->setIcon(QIcon(":/exit.png"));
    verticalLayout->addWidget(quitButton);

    horizontalLayout->addLayout(verticalLayout);

    setCentralWidget(centralWidget);

    menuBar = new QMenuBar;
    menuBar->setObjectName(QStringLiteral("menuBar"));
    setMenuBar(menuBar);
    setWindowTitle(tr("SmartGobang"));

    createButton->setText(tr("创建服务器"));
    connectButton->setText(tr("连接服务器"));
    stopListeningButton->setText(tr("停止监听"));
    stopConnectionButton->setText(tr("停止连接"));
    connectionStatusLabel->setText(tr("等待创建/连接"));
    warningButton->setText(tr("提醒"));
    resetButton->setText(tr("重新开始"));
    quitButton->setText(tr("退出"));
}

MainWindow::~MainWindow() {

}

void MainWindow::checkEnd(int who) {
    if (chessBoard->isEnd(who)) {
        if (m_current == 1) {
            chessStatusLabel->setText(tr("白方获胜"));
        }
        else {
            chessStatusLabel->setText(tr("黑方获胜"));
        }
        m_connected = false;
    }
}

void MainWindow::on_warningButton_clicked() {
    chessBoard->clearBomb();
    chessBoard->setBomb();
}

void MainWindow::on_createButton_clicked()
{
    CreateDialog *createDialog = new CreateDialog(this);
    if (createDialog->exec() == QDialog::Accepted) {
        m_listenSocket->listen(QHostAddress::Any, 8888);
        connectionStatusLabel->setText(tr("等待连接"));
        createButton->setEnabled(false);
        connectButton->setEnabled(false);
        stopListeningButton->setEnabled(true);
        stopConnectionButton->setEnabled(false);
    }
}

void MainWindow::acceptConnection() {
//    qDebug() << "connect!!";
    m_readWriteSocket = m_listenSocket->nextPendingConnection();
    connect(m_readWriteSocket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connectionStatusLabel->setText(tr("已连接"));
    whoStatusLabel->setText(tr("你是黑方"));
    chessStatusLabel->setText(tr("黑方回合"));
    m_connected = true;
    m_who = 1;
    createButton->setEnabled(false);
    connectButton->setEnabled(false);
    stopListeningButton->setEnabled(false);
    stopConnectionButton->setEnabled(true);
}

void MainWindow::receiveMessage() {
    chessBoard->clearBomb();
    QByteArray info;

    info = m_readWriteSocket->readAll();
    int row = info[0] - DET;
    int column = info[1] - DET;

    if (row == DISCONNECT_CODE) {
        //停止连接
        m_readWriteSocket->disconnectFromHost();
        connectionStatusLabel->setText(tr("断开连接"));
        m_current = 1;
        m_connected = false;
        createButton->setEnabled(true);
        connectButton->setEnabled(true);
        stopListeningButton->setEnabled(false);
        stopConnectionButton->setEnabled(false);
        return;
    }
    else if (row == RESET_CODE) {
        connectionStatusLabel->setText(tr("重新开始"));
        chessBoard->resetChessBoard();
        m_connected = true;
        return;
    }
    m_current = -m_current;
    emit(receiveChessPiece(row, column));
    if (m_current == 1) chessStatusLabel->setText(tr("黑方回合"));
    else chessStatusLabel->setText(tr("白方回合"));
    checkEnd(-m_who);
}

void MainWindow::on_connectButton_clicked()
{
    ConnectDialog *connectDialog = new ConnectDialog(this);
    if (connectDialog->exec() == QDialog::Accepted) {

        m_readWriteSocket->connectToHost(QHostAddress(connectDialog->getInputIP()), 8888);

        if (m_readWriteSocket->state() == QAbstractSocket::ConnectingState) {
            if (m_readWriteSocket->waitForConnected(2000)) {
                connectionStatusLabel->setText(tr("已连接"));
                whoStatusLabel->setText(tr("你是白方"));
                chessStatusLabel->setText(tr("黑方回合"));
                m_connected = true;
                m_who = -1;
                createButton->setEnabled(false);
                connectButton->setEnabled(false);
                stopListeningButton->setEnabled(false);
                stopConnectionButton->setEnabled(true);
            }
            else connectionStatusLabel->setText(tr("服务器无响应"));
        }
        else connectionStatusLabel->setText(tr("连接失败，请检查IP"));
    }
}

void MainWindow::sendMessage(int row, int column) {
    chessBoard->clearBomb();
    char data[2];
    data[0] = row + DET;
    data[1] = column + DET;
    m_current = -m_current;
    m_readWriteSocket->write(data);
    m_readWriteSocket->flush();
    if (m_current == 1) chessStatusLabel->setText(tr("黑方回合"));
    else chessStatusLabel->setText(tr("白方回合"));
    checkEnd(m_who);

}

void MainWindow::on_stopListeningButton_clicked()
{
    if (m_listenSocket->isListening()) {
        m_listenSocket->close();
        connectionStatusLabel->setText(tr("停止监听"));
        createButton->setEnabled(true);
        connectButton->setEnabled(true);
        stopListeningButton->setEnabled(false);
        stopConnectionButton->setEnabled(false);
    }
}

void MainWindow::on_stopConnectionButton_clicked()
{
    if (m_readWriteSocket->state() == QAbstractSocket::ConnectedState) {
        char disconnect[2];
        disconnect[0] = DISCONNECT_CODE + DET;
        m_readWriteSocket->write(disconnect);
        m_readWriteSocket->flush();
        m_readWriteSocket->disconnectFromHost();
        connectionStatusLabel->setText(tr("断开连接"));
        m_connected = false;
        m_current = 1;
        createButton->setEnabled(true);
        connectButton->setEnabled(true);
        stopListeningButton->setEnabled(false);
        stopConnectionButton->setEnabled(false);
    }
}

void MainWindow::on_resetButton_clicked() {
    if (m_readWriteSocket->state() == QAbstractSocket::ConnectedState) {
        char reset[2];
        reset[0] = RESET_CODE + DET;
        m_readWriteSocket->write(reset);
        m_readWriteSocket->flush();
        connectionStatusLabel->setText(tr("重新开始"));
        chessBoard->resetChessBoard();
        chessBoard->clearBomb();
        m_connected = true;
    }
}

void MainWindow::on_quitButton_clicked() {
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_readWriteSocket->state() == QAbstractSocket::ConnectedState) {
        connectionStatusLabel->setText(tr("请先断开连接"));
        event->ignore();
        return;
    }
    event->accept();
}

void MainWindow::on_warningSettingComboBox_indexChanged(int index) {
    if (index == 0) m_warningType = 2;
    else m_warningType = 1;
}
