#include "chesslabel.h"
#include <QPainter>
#include <QPaintDevice>
#include <cstring>

int ChessLabel::m_size = 40;

ChessLabel::ChessLabel(int *who, bool *connected, int *current, QWidget *parent) : QLabel(parent)
{
    m_isLast = false;
    m_connected = connected;
    m_who = who;
    m_current = current;
    m_chessType = 0;
    m_specialPoint = false;
    memset(m_lineCheck, 0, sizeof(m_lineCheck));

    this->setFixedSize(m_size, m_size);
    this->setStyleSheet("background-color: #F5DEB3;font: 13pt \"Helvetica\";");
    //border-color: rgb(75, 155, 255);border-width: 1px;border-style: solid;
}

ChessLabel::~ChessLabel() {

}

void ChessLabel::mousePressEvent(QMouseEvent *event) {
    if (m_chessType != 0 || *m_connected == false || *m_current != *m_who) return;
    if (event->button() == Qt::LeftButton) {
        m_chessType = *m_who;
        m_isLast = true;
        emit(newChessPiece(this));
        this->update();
    }
}


void ChessLabel::paintEvent(QPaintEvent *event) {
    QPainter *painter = new QPainter(this);

    painter->translate(m_size/2+1, m_size/2+1);
    painter->setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < 4; i++)
    {
        if (m_lineCheck[i]) continue;
        painter->save();
        painter->rotate(90 * i);
        painter->setPen(QPen(Qt::black, 1));
        painter->drawLine(0, 0, m_size / 2 + 1, 0);
        painter->restore();
    }

    if (m_specialPoint) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::gray);
        painter->drawEllipse(QPoint(0, 0), (int)(0.15 * m_size), (int)(0.15 * m_size));
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
    if (m_isLast) {
        painter->setPen(Qt::red);
    }
    if (m_chessType == 1) {
        painter->setBrush(Qt::black);
    }
    else if (m_chessType == -1) {
        painter->setBrush(Qt::white);
    }
    painter->drawEllipse(QPoint(0, 0), (int)(0.33 * m_size), (int)(0.33 * m_size));
    QLabel::paintEvent(event);
}
