#ifndef CHESSLABEL_H
#define CHESSLABEL_H

#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QMouseEvent>


class ChessLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ChessLabel(int* who, bool* connected, int* current, QWidget *parent = 0);
    ~ChessLabel();

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void setChessType(int chessType) { m_chessType = chessType; update(); }
    int getChessType() const { return m_chessType; }
    bool isLast() const { return m_isLast; }
    void setIsLast(bool isLast) { m_isLast = isLast; update(); }
    void setLineCheck(int i, int num) { m_lineCheck[i] = num; }
    void setSpecialPoint(bool specialPoint) { m_specialPoint = specialPoint; }

signals:
    void newChessPiece(ChessLabel*);

private:
    int m_chessType;
    int *m_who;
    bool *m_connected;
    int *m_current;
    static int m_size;
    bool m_isLast;
    int m_lineCheck[4];
    bool m_specialPoint;
};

#endif // CHESSLABEL_H
