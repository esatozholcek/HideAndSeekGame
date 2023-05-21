#include "widget.h"
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Game");
    resize(700, 400);
    setStyleSheet("background-color:green;");
    setFixedSize(QSize(700,400));
}

Widget::~Widget()
{
}

