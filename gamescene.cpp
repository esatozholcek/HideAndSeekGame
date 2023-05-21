#include "gamescene.h"
#include <QGraphicsRectItem>


GameScene::GameScene(QObject *parent) : QGraphicsScene(parent)
{
    QGraphicsRectItem *rect = new QGraphicsRectItem();
    rect->setRect(400,400,100,100);
    addItem(rect);
    setSceneRect(0,0,800,600);



}
