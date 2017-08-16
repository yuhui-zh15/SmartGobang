#include "chessboard.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QString>
#include <QVector>
#include <QList>
#include <QDebug>
#include <QMessageBox>
#include <QPoint>

#define STRONG_INFORMATION 2
#define WEAK_INFORMATION 1

ChessBoard::ChessBoard(int *who, bool *connected, int* current, int *warningType, QWidget *parent): QWidget(parent)
{
    setFixedSize(600, 600);
    m_who = who;
    m_connected = connected;
    m_current = current;
    m_warningType = warningType;

    m_chessBoardSize = 15;
    m_lastPosiX = 0;
    m_lastPosiY = 0;

    createChessBoard();
    setLayout(m_chessBoardLayout);
}

ChessBoard::~ChessBoard() {

}

void ChessBoard::createChessBoard() {
    m_chessBoardLayout = new QGridLayout(this);
    m_chessBoardLayout->setHorizontalSpacing(0);
    m_chessBoardLayout->setVerticalSpacing(0);
    for (int i = 0; i < m_chessBoardSize; i++) {
        QVector<ChessLabel*> horizontalChessBoard;
        for (int j = 0; j < m_chessBoardSize; j++) {
            ChessLabel* label = new ChessLabel(m_who, m_connected, m_current, this);
            if (i == 0) label->setLineCheck(3, 1);
            if (j == 0) label->setLineCheck(2, 1);
            if (i == m_chessBoardSize - 1) label->setLineCheck(1, 1);
            if (j == m_chessBoardSize - 1) label->setLineCheck(0, 1);
            horizontalChessBoard.append(label);
            m_chessBoardLayout->addWidget(label, i, j);
            connect(label, SIGNAL(newChessPiece(ChessLabel*)), this, SLOT(nextChessPiece(ChessLabel*)));
        }
        m_chessBoard.append(horizontalChessBoard);
    }
    m_chessBoard[7][7]->setSpecialPoint(true);
    m_chessBoard[3][3]->setSpecialPoint(true);
    m_chessBoard[3][11]->setSpecialPoint(true);
    m_chessBoard[11][3]->setSpecialPoint(true);
    m_chessBoard[11][11]->setSpecialPoint(true);

}

void ChessBoard::resetChessBoard() {
    for (int i = 0; i < m_chessBoardSize; i++) {
        for (int j = 0; j < m_chessBoardSize; j++) {
            m_chessBoard[i][j]->setIsLast(false);
            m_chessBoard[i][j]->setChessType(0);
        }
    }
}

QList<QPoint> ChessBoard::isRisky() {
    QList<QPoint> riskPoint;
    int sum = 0;
    for (int i = 0; i < m_chessBoardSize; i++) {
        for (int j = 0; j < m_chessBoardSize; j++) {
            if (m_chessBoard[i][j]->getChessType() != 0) continue;
            int cnt = 0;
            int i_u, i_d, j_l, j_r;
            //左右
            for (i_u = i, j_l = j - 1; j_l >= 0 && m_chessBoard[i_u][j_l]->getChessType() == -*m_who ; j_l--);
            for (i_u = i, j_r = j + 1; j_r < m_chessBoardSize && m_chessBoard[i_u][j_r]->getChessType() == -*m_who; j_r++);
            if (j_l >= 0 && j_r < m_chessBoardSize && m_chessBoard[i_u][j_l]->getChessType() == 0 && m_chessBoard[i_u][j_r]->getChessType() == 0 && j_r - j_l - 1 >= 3) cnt++;
            else if (j_l >= 0 && j_r < m_chessBoardSize && ((m_chessBoard[i_u][j_l]->getChessType() == 0) ^ (m_chessBoard[i_u][j_r]->getChessType() == 0)) && j_r - j_l - 1 >= 4) cnt++;
            //上下
            for (i_u = i - 1, j_l = j; i_u >= 0 && m_chessBoard[i_u][j_l]->getChessType() == -*m_who; i_u--);
            for (i_d = i + 1, j_l = j; i_d < m_chessBoardSize && m_chessBoard[i_d][j_l]->getChessType() == -*m_who; i_d++);
            if (i_u >= 0 && i_d < m_chessBoardSize && m_chessBoard[i_u][j_l]->getChessType() == 0 && m_chessBoard[i_d][j_l]->getChessType() == 0 && i_d - i_u - 1 >= 3) cnt++;
            else if (i_u >= 0 && i_d < m_chessBoardSize && ((m_chessBoard[i_u][j_l]->getChessType() == 0) ^ (m_chessBoard[i_d][j_l]->getChessType() == 0)) && i_d - i_u - 1 >= 4) cnt++;
            //左斜
            for (i_u = i - 1, j_l = j - 1; j_l >= 0 && i_u >= 0 && m_chessBoard[i_u][j_l]->getChessType() == -*m_who; j_l--, i_u--);
            for (i_d = i + 1, j_r = j + 1; j_r < m_chessBoardSize && i_d < m_chessBoardSize && m_chessBoard[i_d][j_r]->getChessType() == -*m_who; j_r++, i_d++);
            if (j_l >= 0 && i_u >= 0 && j_r < m_chessBoardSize && i_d < m_chessBoardSize && m_chessBoard[i_u][j_l]->getChessType() == 0 && m_chessBoard[i_d][j_r]->getChessType() == 0 && j_r - j_l - 1 >= 3) cnt++;
            else if (j_l >= 0 && i_u >= 0 && j_r < m_chessBoardSize && i_d < m_chessBoardSize && ((m_chessBoard[i_u][j_l]->getChessType() == 0) ^ (m_chessBoard[i_d][j_r]->getChessType() == 0)) && j_r - j_l - 1 >= 4) cnt++;
            //右斜
            for (i_u = i - 1, j_r = j + 1; j_r < m_chessBoardSize && i_u >= 0 && m_chessBoard[i_u][j_r]->getChessType() == -*m_who; j_r++, i_u--);
            for (i_d = i + 1, j_l = j - 1; j_l > 0 && i_d < m_chessBoardSize && m_chessBoard[i_d][j_l]->getChessType() == -*m_who; j_l--, i_d++);
            if (j_r < m_chessBoardSize && i_u >= 0 && j_l > 0 && i_d < m_chessBoardSize && m_chessBoard[i_u][j_r]->getChessType() == 0 && m_chessBoard[i_d][j_l]->getChessType() == 0 && j_r - j_l - 1 >= 3) cnt++;
            else if (j_r < m_chessBoardSize && i_u >= 0 && j_l > 0 && i_d < m_chessBoardSize && ((m_chessBoard[i_u][j_r]->getChessType() == 0) ^ (m_chessBoard[i_d][j_l]->getChessType() == 0)) && j_r - j_l - 1 >= 4) cnt++;
            sum += cnt;
            if (cnt >= *m_warningType) riskPoint.append(QPoint(i, j));
        }
    }
    //if (sum / 3 < 2) riskPoint.clear();
    return riskPoint;
}

void ChessBoard::setBomb() {
    QList<QPoint> riskPoint = isRisky();
    for (int i = 0; i < riskPoint.count(); i++) {
        m_chessBoard[riskPoint.at(i).x()][riskPoint.at(i).y()]->setPixmap(QPixmap(":/bomb.png").scaled(40, 40));
    }
}

void ChessBoard::clearBomb() {
    for (int i = 0; i < m_chessBoardSize; i++) {
        for (int j = 0; j < m_chessBoardSize; j++) {
            m_chessBoard[i][j]->setPixmap(QPixmap());
        }
    }
}

bool ChessBoard::isEnd(int current) {
    //横
    int i_l = m_lastPosiY;
    int i_r = m_lastPosiY;
    while (i_l >= 0 && m_chessBoard[m_lastPosiX][i_l]->getChessType() == current) i_l--;
    while (i_r < m_chessBoardSize && m_chessBoard[m_lastPosiX][i_r]->getChessType() == current) i_r++;
    if (i_r - i_l - 1 >= 5) return true;
    //竖
    int j_u = m_lastPosiX;
    int j_d = m_lastPosiX;
    while (j_u >= 0 && m_chessBoard[j_u][m_lastPosiY]->getChessType() == current) j_u--;
    while (j_d < m_chessBoardSize && m_chessBoard[j_d][m_lastPosiY]->getChessType() == current) j_d++;
    if (j_d - j_u - 1 >= 5) return true;
    //左斜
    i_l = i_r = m_lastPosiY;
    j_u = j_d = m_lastPosiX;
    while (i_l >= 0 && j_u >= 0 && m_chessBoard[j_u][i_l]->getChessType() == current) i_l--, j_u--;
    while (i_r < m_chessBoardSize && j_d < m_chessBoardSize && m_chessBoard[j_d][i_r]->getChessType() == current) i_r++, j_d++;
    if (i_r - i_l - 1 >= 5) return true;
    //右斜
    i_l = i_r = m_lastPosiY;
    j_u = j_d = m_lastPosiX;
    while (i_r < m_chessBoardSize && j_u >= 0 && m_chessBoard[j_u][i_r]->getChessType() == current) i_r++, j_u--;
    while (i_l >= 0 && j_d < m_chessBoardSize && m_chessBoard[j_d][i_l]->getChessType() == current) i_l--, j_d++;
    if (i_r - i_l - 1 >= 5) return true;
    return false;
}

void ChessBoard::nextChessPiece(ChessLabel* label) {
    m_chessBoard[m_lastPosiX][m_lastPosiY]->setIsLast(false);
    for (int i = 0; i < m_chessBoardSize; i++) {
        for (int j = 0; j < m_chessBoardSize; j++) {
            if (m_chessBoard[i][j] == label) {
                m_lastPosiX = i;
                m_lastPosiY = j;
                break;
            }
        }
    }
    emit(sendChessPiece(m_lastPosiX, m_lastPosiY));
}

void ChessBoard::receiveChessPiece(int row, int column) {
    m_chessBoard[row][column]->setChessType(-*m_who);
    for (int i = 0; i < m_chessBoardSize; i++) {
        for (int j = 0; j < m_chessBoardSize; j++) {
            if (m_chessBoard[i][j]->isLast()) {
                m_chessBoard[i][j]->setIsLast(false);
                break;
            }
        }
    }
    m_chessBoard[row][column]->setIsLast(true);
    m_lastPosiX = row;
    m_lastPosiY = column;
}
