#ifndef BIRD_H
#define BIRD_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class Bird : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal y READ y WRITE setY)

public:
    explicit Bird(QPixmap pixMap);

    qreal rotation() const;
    void setRotation(qreal newRotation);

    qreal y() const;
    void setY(qreal newY);

    void rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve);

    void shootUp();

    void fallToGroundIfNecessary();

    void startFlyhing();

    void freezeBird();


signals:

private:
    enum WingPosition{
        Up,
        Middle,
        Down
    };

    void updatePixmap();

    WingPosition wingPosition;
    bool wingDirection; // false - down     true - up

    qreal m_rotation;
    qreal m_y;

    QPropertyAnimation *yAimation;
    QPropertyAnimation *rotationAimation;
    qreal groudPosition;
};

#endif // BIRD_H
