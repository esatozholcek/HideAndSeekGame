#include <QApplication>
#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QVector>
#include <QTimer>

class Window : public QWidget {
public:
    Window(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(800, 800);
        m_squareSize = 20;
        m_foodSize = 10;

        m_player1X = 0;
        m_player1Y = 0;
        m_player2X = width() - m_squareSize;
        m_player2Y = height() - m_squareSize;

        m_player1Color = Qt::red;
        m_player2Color = Qt::blue;

        generateFoods();
        startTimer(500); // Yemlerin hareket hızı (100ms)

        setFocusPolicy(Qt::StrongFocus);
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        int step = 10; // Kareleri ne kadar adım hareket ettireceğimiz

        // 1. oyuncunun hareketi (yön tuşlarıyla)
        if (event->key() == Qt::Key_Left && m_player1X > 0)
            m_player1X -= step;
        else if (event->key() == Qt::Key_Right && m_player1X < (width() - m_squareSize))
            m_player1X += step;
        else if (event->key() == Qt::Key_Up && m_player1Y > 0)
            m_player1Y -= step;
        else if (event->key() == Qt::Key_Down && m_player1Y < (height() - m_squareSize))
            m_player1Y += step;

        // 2. oyuncunun hareketi (WASD tuşlarıyla)
        if (event->key() == Qt::Key_A && m_player2X > 0)
            m_player2X -= step;
        else if (event->key() == Qt::Key_D && m_player2X < (width() - m_squareSize))
            m_player2X += step;
        else if (event->key() == Qt::Key_W && m_player2Y > 0)
            m_player2Y -= step;
        else if (event->key() == Qt::Key_S && m_player2Y < (height() - m_squareSize))
            m_player2Y += step;

        checkFoodCollisions();
        update();
    }

    void timerEvent(QTimerEvent *) override {
        moveFoods();
        update();
    }

    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);

        // 1. oyuncunun karesini çizme
        painter.fillRect(m_player1X, m_player1Y, m_squareSize, m_squareSize, m_player1Color);

        // 2. oyuncunun karesini çizme
        painter.fillRect(m_player2X, m_player2Y, m_squareSize, m_squareSize, m_player2Color);

        // Yemleri çizme
        for (const QPoint &food : m_foodPositions) {
            painter.fillRect(food.x(), food.y(), m_foodSize, m_foodSize, Qt::black);
        }
    }

private:
    int m_squareSize;
    int m_foodSize;
    int m_player1X;
    int m_player1Y;
    int m_player2X;
    int m_player2Y;
    QColor m_player1Color;
    QColor m_player2Color;
    QVector<QPoint> m_foodPositions;

    void generateFoods() {
        // Yemleri oluşturma
        int numFoods = 20;
        for (int i = 0; i < numFoods; ++i) {
            int foodX = QRandomGenerator::global()->bounded(0, width() - m_foodSize);
            int foodY = QRandomGenerator::global()->bounded(0, height() - m_foodSize);
            m_foodPositions.append(QPoint(foodX, foodY));
        }
    }

    void moveFoods() {
        // Yemleri rastgele hareket ettirme
        for (QPoint &food : m_foodPositions) {
            int direction = QRandomGenerator::global()->bounded(4); // 0-3 arası rastgele bir yön seçme
            switch (direction) {
            case 0: // Yukarı
                if (food.y() > 0)
                    food.setY(food.y() - m_squareSize);
                break;
            case 1: // Aşağı
                if (food.y() < (height() - m_squareSize))
                    food.setY(food.y() + m_squareSize);
                break;
            case 2: // Sol
                if (food.x() > 0)
                    food.setX(food.x() - m_squareSize);
                break;
            case 3: // Sağ
                if (food.x() < (width() - m_squareSize))
                    food.setX(food.x() + m_squareSize);
                break;
            }
        }
    }

    void checkFoodCollisions() {
        // Yemlerin oyuncular tarafından yenilmesini kontrol etme
        for (int i = 0; i < m_foodPositions.size(); ++i) {
            const QPoint &food = m_foodPositions[i];
            if ((food.x() >= m_player1X && food.x() < (m_player1X + m_squareSize)) &&
                (food.y() >= m_player1Y && food.y() < (m_player1Y + m_squareSize))) {
                m_foodPositions.remove(i);
                // 1. oyuncunun puanını artırma
                break;
            } else if ((food.x() >= m_player2X && food.x() < (m_player2X + m_squareSize)) &&
                       (food.y() >= m_player2Y && food.y() < (m_player2Y + m_squareSize))) {
                m_foodPositions.remove(i);
                // 2. oyuncunun puanını artırma
                break;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Window window;
    window.show();

    return app.exec();
}
