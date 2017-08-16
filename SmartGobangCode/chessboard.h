#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QGridLayout>
#include <QGroupBox>
#include <QPoint>
#include "chesslabel.h"

class ChessBoard: public QWidget
{
    Q_OBJECT
public:
    ChessBoard(int *who, bool *connected, int *current, int *warningType, QWidget* parent = 0);
    virtual ~ChessBoard();
    void createChessBoard();
    void resetChessBoard();
    bool isEnd(int current);
    QList<QPoint> isRisky();
    void setBomb();
    void clearBomb();

protected slots:
    void nextChessPiece(ChessLabel*);
    void receiveChessPiece(int, int);


signals:
    void checkIsEnd();
    void sendChessPiece(int, int);

private:
    int m_chessBoardSize;
    QVector<QVector<ChessLabel*>> m_chessBoard;
    QGridLayout* m_chessBoardLayout;

    int m_lastPosiX;
    int m_lastPosiY;

    bool *m_connected;
    int *m_who;
    int *m_current;
    int *m_warningType;
};

#endif // CHESSBOARD_H
