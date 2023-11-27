#include "pillar.h"
#include "bird.h"
#include "scene.h"
#include <QRandomGenerator>
#include <QGraphicsScene>

Pillar::Pillar(): topPillar(new QGraphicsPixmapItem(QPixmap(":/images/pipe_down.png"))), bottomPillar(new QGraphicsPixmapItem(QPixmap(":/images/pipe_up.png"))), pastBird(false)
{
    // Inicializando as posições iniciais dos pilares superior e inferior
    topPillar->setPos(QPoint(0,0) - QPointF(topPillar->boundingRect().width()/2, topPillar->boundingRect().height() + 60));
    bottomPillar->setPos(QPoint(0,0) + QPointF(-bottomPillar->boundingRect().width()/2, 60));

    addToGroup(topPillar);
    addToGroup(bottomPillar);

    // Gerando valores aleatórios para a posição vertical e horizontal do pilar.
    yPos = QRandomGenerator::global()->bounded(150);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    setPos(QPoint(0,0) + QPoint(260 + xRandomizer, yPos));

    //Animação horizontal
    xAnimation = new QPropertyAnimation(this, "x", this);

    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-260);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    // Conectando um sinal ao término da animação para remover o pilar da cena.
    connect(xAnimation, &QPropertyAnimation::finished,[=](){
        scene()->removeItem(this);
        delete this;
    });
    xAnimation->start();

}

Pillar::~Pillar()
{
    //Desalcando da memória
    delete topPillar;
    delete bottomPillar;
}

qreal Pillar::x() const
{
    return m_x;
}

void Pillar::setX(qreal newX)
{
    m_x = newX;

    if(newX < 0 && !pastBird){
        pastBird = true;
        QGraphicsScene *mScene = scene();
        Scene *myScene = dynamic_cast<Scene *>(mScene);

        if(myScene){
            myScene->incrementScore();
        }
    }

    if(collisedWithBird()){
        emit collidedFail();
    }
    setPos(QPoint(0,0) + QPoint(newX, yPos));
}

void Pillar::freezePillar()
{
    xAnimation->stop();
}

bool Pillar::collisedWithBird()
{
    // Verifica a colisão do passaro
    QList<QGraphicsItem*> collidingItens = topPillar->collidingItems();

    collidingItens.append(bottomPillar->collidingItems());

    foreach(QGraphicsItem *item, collidingItens) {
        Bird *bird = dynamic_cast<Bird*>(item);

        if(bird){
            return true;
        }
    }

    return false;
}
