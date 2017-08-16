#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include "chessboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupUi();
    void checkEnd(int who);
    void closeEvent(QCloseEvent *event);

private slots:
    void acceptConnection();
    void receiveMessage();
    void sendMessage(int row, int column);

    void on_createButton_clicked();
    void on_connectButton_clicked();
    void on_stopListeningButton_clicked();
    void on_stopConnectionButton_clicked();
    void on_warningButton_clicked();
    void on_quitButton_clicked();
    void on_resetButton_clicked();
    void on_warningSettingComboBox_indexChanged(int);


signals:
    void receiveChessPiece(int row, int column);

private:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *chessStatusLabel;
    ChessBoard *chessBoard;
    QVBoxLayout *verticalLayout;
    QPushButton *createButton;
    QPushButton *connectButton;
    QPushButton *stopListeningButton;
    QPushButton *stopConnectionButton;
    QLabel *connectionStatusLabel;
    QSpacerItem *verticalSpacer;
    QPushButton *warningButton;
    QPushButton *resetButton;
    QPushButton *quitButton;
    QMenuBar *menuBar;
    QLabel *whoStatusLabel;
    QComboBox *warningSettingComboBox;

    QTcpServer *m_listenSocket;
    QTcpSocket *m_readWriteSocket;
    bool m_connected;
    int m_who;
    int m_current;
    int m_warningType;
};

#endif // MAINWINDOW_H
