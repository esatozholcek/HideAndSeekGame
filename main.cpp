#include <QApplication>
#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QVector>
#include <QTimer>
#include <iostream>
#include <QFont>

class Window : public QWidget {
public:
    Window(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(800, 830);
        m_squareSize = 25;
        m_foodSize = m_squareSize;
        m_height = 800;
        m_width = 800;
        m_player1X = 3*m_squareSize;
        m_player1Y = 3*m_squareSize;
        m_player2X = m_width - 4 * m_squareSize;
        m_player2Y = m_height - 4 * m_squareSize;
        m_player1Score = 0;
        m_player2Score = 0;

        m_ghostImage.load(":/img/ghost.png");
        m_ghostImage = m_ghostImage.scaled(m_foodSize, m_foodSize);

        m_player1Color = Qt::red;
        m_player2Color = Qt::blue;
        numFoods = 20;

        generateFoods();
        m_timerId = startTimer(600); // Yemlerin hareket hızı (100ms)
        m_moveTimerId = startTimer(100);

        setFocusPolicy(Qt::StrongFocus);
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (m_gameOver) {
            // If the game is over, ignore key events
            if(event->key() == Qt::Key_Escape)
                QApplication::instance()->quit();
            return;
        }
        int step = m_squareSize; // Kareleri ne kadar adım hareket ettireceğimiz

        // 1. oyuncunun hareketi (yön tuşlarıyla)
        if (event->key() == Qt::Key_Left && m_player1X > 0)
            m_player1X -= step;
        else if (event->key() == Qt::Key_Right && m_player1X < (m_width - m_squareSize))
            m_player1X += step;
        else if (event->key() == Qt::Key_Up && m_player1Y > 0)
            m_player1Y -= step;
        else if (event->key() == Qt::Key_Down && m_player1Y < (m_height - m_squareSize))
            m_player1Y += step;

        // 2. oyuncunun hareketi (WASD tuşlarıyla)
        if (event->key() == Qt::Key_A && m_player2X > 0)
            m_player2X -= step;
        else if (event->key() == Qt::Key_D && m_player2X < (m_width - m_squareSize))
            m_player2X += step;
        else if (event->key() == Qt::Key_W && m_player2Y > 0)
            m_player2Y -= step;
        else if (event->key() == Qt::Key_S && m_player2Y < (m_height - m_squareSize))
            m_player2Y += step;

        if(event->key() == Qt::Key_Escape)
            QApplication::instance()->quit();

        checkFoodCollisions();
        update();
    }


    void timerEvent(QTimerEvent *event) override {
        if (event->timerId() == m_timerId) {
            moveFoods();
            //update();
        }
        if (event->timerId() == m_moveTimerId) {
            update();
        }
    }


    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);

        // Paint background
        painter.fillRect(0, 800, 800, 30, QColor(17, 114, 43));
        painter.fillRect(0, 0, 800, 800, QColor(0, 0, 0));

        /*
        // Draw player 1
        QRect player1Rect(m_player1X - m_squareSize, m_player1Y - m_squareSize, m_squareSize * 3, m_squareSize * 3);
        painter.fillRect(player1Rect, QColor(246, 154, 84));

        // Draw player 2
        QRect player2Rect(m_player2X - m_squareSize, m_player2Y - m_squareSize, m_squareSize * 3, m_squareSize * 3);
        painter.fillRect(player2Rect, QColor(246, 154, 84));
        */



        // Draw player 1
        QRect player1Rect(m_player1X - m_squareSize*(3), m_player1Y - m_squareSize*(3), m_squareSize * 7, m_squareSize * 7);
        painter.fillRect(player1Rect, QColor(246, 154, 84));

        // Draw player 2
        QRect player2Rect(m_player2X - m_squareSize*(3), m_player2Y - m_squareSize*(3), m_squareSize * 7, m_squareSize * 7);
        painter.fillRect(player2Rect, QColor(246, 154, 84));



        // 1. oyuncunun karesini çizme
        painter.fillRect(m_player1X, m_player1Y, m_squareSize, m_squareSize, m_player1Color);
        // 2. oyuncunun karesini çizme
        painter.fillRect(m_player2X, m_player2Y, m_squareSize, m_squareSize, m_player2Color);

        // Yemleri çizme
        for (const Food &food : m_foodPositions) {
            QRect foodRect(food.position.x(), food.position.y(), m_foodSize, m_foodSize);
            //painter.drawImage(food.position, m_ghostImage);

            if (player1Rect.intersects(foodRect) || player2Rect.intersects(foodRect)) {
                painter.drawImage(food.position, m_ghostImage);

            }
        }

        /*
        QImage ghostImage(":/img/ghost2.png");
        //ghostImage = ghostImage.scaled(m_foodSize, m_foodSize);
        for (const Food &food : m_foodPositions) {
            painter.drawImage(food.position, ghostImage);
        }
        */


        // Draw player scores
        // Düzenleme yapılacak
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        painter.drawText(10, 823, QString("Player 1 Score: %1").arg(m_player1Score));
        painter.drawText(250, 823, QString("Player 2 Score: %1").arg(m_player2Score));
        painter.drawText(510, 823, QString("Number of Ghosts: %1").arg(numFoods));


        if (m_gameOver) {
            // Display "Game Over" text
            painter.setPen(Qt::black);
            painter.setFont(QFont("Arial", 40));
            if(m_player1Score > m_player2Score) {
                painter.drawText(rect(), Qt::AlignCenter, "Game Over\nPlayer 1 Won");
            } else if(m_player2Score > m_player1Score){
                painter.drawText(rect(), Qt::AlignCenter, "Game Over\nPlayer 2 Won");
            } else {
                painter.drawText(rect(), Qt::AlignCenter, "Game Over\nScores are equal");
            }
        }


        painter.end();
    }

    void paintGhost(QPaintEvent *) {

    }

private:
    struct Food {
        QPoint position;
        QColor color;
        QTimer *timer;
    };
    int m_squareSize;
    int m_foodSize;
    int m_player1X;
    int m_player1Y;
    int m_player2X;
    int m_player2Y;
    int m_player1Score;
    int m_player2Score;
    int m_width;
    int m_height;
    int numFoods;
    int m_timerId;
    int m_moveTimerId;
    QColor m_player1Color;
    QColor m_player2Color;
    QVector<Food> m_foodPositions;
    QImage m_ghostImage;
    bool m_gameOver = false;


    void generateFoods() {
        // Yemleri oluşturma
        for (int i = 0; i < numFoods; ++i) {
            int foodX = QRandomGenerator::global()->bounded(m_width / m_foodSize) * m_foodSize;
            int foodY = QRandomGenerator::global()->bounded(m_height / m_foodSize) * m_foodSize;
            Food food;
            food.position = QPoint(foodX, foodY);
            food.color = Qt::black;
            food.timer = nullptr;
            m_foodPositions.append(food);
        }
    }


    void moveFoods() {
        // Yemleri rastgele hareket ettirme
        for (Food &food : m_foodPositions) {
            int direction = QRandomGenerator::global()->bounded(4); // 0-3 arası rastgele bir yön seçme
            switch (direction) {
            case 0: // Yukarı
                if (food.position.y() > 0)
                    food.position.setY(food.position.y() - m_squareSize);
                break;
            case 1: // Aşağı
                if (food.position.y() < (m_height - m_squareSize))
                    food.position.setY(food.position.y() + m_squareSize);
                break;
            case 2: // Sol
                if (food.position.x() > 0)
                    food.position.setX(food.position.x() - m_squareSize);
                break;
            case 3: // Sağ
                if (food.position.x() < (m_width - m_squareSize))
                    food.position.setX(food.position.x() + m_squareSize);
                break;
            }
        }
        checkFoodCollisions();
    }


    void checkFoodCollisions() {

        // Yemlerin oyuncular tarafından yenilmesini kontrol etme
        for (int i = 0; i < m_foodPositions.size(); ++i) {
            Food &food = m_foodPositions[i];
            if ((food.position.x() >= m_player1X - 3 * m_squareSize && food.position.x() <= (m_player1X + 3 * m_squareSize)) &&
                (food.position.y() >= m_player1Y - 3 * m_squareSize && food.position.y() <= (m_player1Y + 3*m_squareSize))) {
                numFoods--;
                m_player1Score++;
                //m_foodPositions.remove(i);
                removeFood(i);
                break;
            } else if ((food.position.x() >= m_player2X - 3*m_squareSize && food.position.x() <= (m_player2X + 3*m_squareSize)) &&
                       (food.position.y() >= m_player2Y - 3*m_squareSize && food.position.y() <= (m_player2Y + 3*m_squareSize))) {
                numFoods--;
                m_player2Score++;
                //m_foodPositions.remove(i);
                removeFood(i);
                break;
            }
        }
        if (m_foodPositions.isEmpty()) {
            m_gameOver = true;
        }
    }

    void removeFood(int index) {
        // Yemi silme
        Food &food = m_foodPositions[index];
        if (food.timer == nullptr) {
            food.timer = new QTimer(this);
            connect(food.timer, &QTimer::timeout, [this, index]() {
                delete m_foodPositions[index].timer;
                m_foodPositions[index].timer = nullptr;
                m_foodPositions.remove(index);
            });
            food.timer->start(300); // Yeme animasyonu süresi (ms)
        }
    }


};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Window window;
    window.show();

    return app.exec();
}
