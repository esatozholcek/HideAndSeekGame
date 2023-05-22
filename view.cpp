#include "view.h"
#include <QKeyEvent>
#include <QApplication>

View::View()
    : QGraphicsView(), m_gameScene(new GameScene(this))
{
    setScene(m_gameScene);
    setFixedSize(sceneRect().width() + 2, sceneRect().height() + 2);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle("Hide and Seek Game");
    //setBackgroundBrush(Qt::red);
    setStyleSheet("background-color: green");

}



void View::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        QApplication::instance()->quit();
        break;
    }

    QGraphicsView::keyPressEvent(event);
}
