#include "bird.h"
#include <QTimer>
#include <QGraphicsScene>

Bird::Bird(QPixmap pixMap) : wingPosition(WingPosition::Up), wingDirection(false)
{
    setPixmap(pixMap);

    QTimer *birdWingsTimer = new QTimer(this);

    connect(birdWingsTimer, &QTimer::timeout,[=](){
        updatePixmap();
    });

    birdWingsTimer->start(80);

    groudPosition = scenePos().y() + 290;

    //animação de vôo
    yAimation = new QPropertyAnimation(this, "y", this);
    yAimation->setStartValue(scenePos().y());
    yAimation->setEndValue(groudPosition);
    yAimation->setEasingCurve(QEasingCurve::InQuad);
    yAimation->setDuration(1000);
    //yAimation->start();

    rotationAimation = new QPropertyAnimation(this, "rotation", this);
    //rotateTo(90, 1200, QEasingCurve::InQuad);
}

void Bird::updatePixmap()
{
    //Animação de bater de asas
    if(wingPosition == WingPosition::Middle)
    {
        if(wingDirection)
        {
            setPixmap(QPixmap(":/images/bird_blue_up.png"));
            wingPosition = WingPosition::Up;
            wingDirection = false;
        }
        else
        {
            setPixmap(QPixmap(":/images/bird_blue_down.png"));
            wingPosition = WingPosition::Down;
            wingDirection = true;
        }
    }
    else
    {
        setPixmap(QPixmap(":/images/bird_blue_middle.png"));
        wingPosition = WingPosition::Middle;
    }
}

qreal Bird::rotation() const
{
    return m_rotation;
}

void Bird::setRotation(qreal newRotation)
{
    m_rotation = newRotation;

    QPointF c = boundingRect().center();

    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(newRotation);
    t.translate(-c.x(), -c.y());
    setTransform(t);
}

qreal Bird::y() const
{
    return m_y;
}

void Bird::setY(qreal newY)
{
    m_y = newY;
    setPos(pos().x(), newY);
}

void Bird::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    //Animação de rotação
    rotationAimation->setStartValue(rotation());
    rotationAimation->setEndValue(end);
    rotationAimation->setEasingCurve(curve);
    rotationAimation->setDuration(duration);
    rotationAimation->start();
}

void Bird::shootUp()
{
    //Voar ao clicar no mouse/espaço
    yAimation->stop();
    rotationAimation->stop();

    qreal curPosY = y();
    qreal jumpHeight = scene()->sceneRect().height() / 8;

    // Animação de subida
    yAimation->setStartValue(curPosY);
    yAimation->setEndValue(curPosY - jumpHeight);
    yAimation->setEasingCurve(QEasingCurve::OutQuad);
    yAimation->setDuration(285);

    connect(yAimation, &QPropertyAnimation::finished, [=]() {
        // Inicia a animação de queda após a animação de subida
        yAimation->setStartValue(y());
        yAimation->setEndValue(groudPosition);
        yAimation->setEasingCurve(QEasingCurve::InQuad);
        yAimation->setDuration(800); // Ajuste a duração conforme necessário
        connect(yAimation, &QPropertyAnimation::finished, [=]() {
            fallToGroundIfNecessary(); // Função chamada após a animação de queda
        });
        yAimation->start();
    });

    // Animação de rotação durante a subida
    rotateTo(-20, 200, QEasingCurve::OutCubic);

    // Inicia a animação de subida
    yAimation->start();
}

void Bird::fallToGroundIfNecessary()
{
    //animação de queda apos termanr de subir
    if(y() > groudPosition)
    {
        rotationAimation->stop();
        //yAimation->stop();

        yAimation->setStartValue(y());
        yAimation->setEndValue(groudPosition);
        yAimation->setEasingCurve(QEasingCurve::InQuad);
        yAimation->setDuration(1200);
        yAimation->start();
        rotateTo(90, 1100, QEasingCurve::InCubic);
    }
}

void Bird::startFlyhing()
{
    yAimation->start();
}

void Bird::freezeBird()
{
    yAimation->stop();
    rotationAimation->stop();
}
