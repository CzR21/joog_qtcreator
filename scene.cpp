#include "scene.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include "bird.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), gamoOn(false), score(0), bestSocre(0), gameOverPix(nullptr), scoreTextItem(nullptr)
{
    setUpPillarTimer();
    addBird();
}

void Scene::addBird()
{
    bird = new Bird(QPixmap(":/images/bird_blue_up.png"));
    addItem(bird);
}

void Scene::startGame()
{
    bird->startFlyhing();

    if(!pillarTimer->isActive()){
        cleanPillars();
        hideGameOverGraphics();
        setGamoOn(true);
        pillarTimer->start(1000);
    }
}

void Scene::setUpPillarTimer()
{
    pillarTimer = new QTimer(this);
    connect(pillarTimer, &QTimer::timeout,[=](){
        Pillar *pillar = new Pillar();

        connect(pillar, &Pillar::collidedFail,[=](){
            pillarTimer->stop();
            freezeBordAndPillars();
            setGamoOn(false);
            showGameOverGraphics();
        });

        addItem(pillar);
    });

    //pillarTimer->start(1000);
}

void Scene::freezeBordAndPillars()
{
    bird->freezeBird();

    QList<QGraphicsItem *> scenesItems = items();

    foreach(QGraphicsItem *item, scenesItems) {
        Pillar *pillar = dynamic_cast<Pillar*>(item);

        if(pillar){
            pillar->freezePillar();
        }
    }

}

int Scene::getScore() const
{
    return score;
}

void Scene::setScore(int newScore)
{
    score = newScore;
}

bool Scene::getGamoOn() const
{
    return gamoOn;
}

void Scene::setGamoOn(bool newGamoOn)
{
    gamoOn = newGamoOn;
}

void Scene::incrementScore()
{
    score++;
    if(score > bestSocre){
        bestSocre = score;
    }
}

void Scene::showGameOverGraphics()
{
    gameOverPix = new QGraphicsPixmapItem(QPixmap(":/images/game_over.png"));

    addItem(gameOverPix);
    gameOverPix->setPos(QPoint(0,0) - QPointF(gameOverPix->boundingRect().width()/2, gameOverPix->boundingRect().height()/2));

    scoreTextItem = new QGraphicsTextItem();

    QString htmlString = "<p><b>Socre: " + QString::number(score) + "</b></p>" + "<p><b>Best socre: " + QString::number(bestSocre) + "</b></p>";

    QFont font("Consolas", 100, QFont::Bold);

    QColor backgroundColor(244,164,96);

    scoreTextItem->setHtml(htmlString);
    scoreTextItem->setFont(htmlString);
    scoreTextItem->setDefaultTextColor(backgroundColor);

    addItem(scoreTextItem);

    scoreTextItem->setPos(QPoint(0,0) - QPointF(gameOverPix->boundingRect().width()/2, gameOverPix->boundingRect().height()/2-60));
}

void Scene::hideGameOverGraphics()
{
    score = 0;

    if(gameOverPix){
        removeItem(gameOverPix);
        delete gameOverPix;
        gameOverPix = nullptr;
    }

    if(scoreTextItem){
        removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
}

void Scene::cleanPillars()
{
    QList<QGraphicsItem *> scenesItems = items();

    foreach(QGraphicsItem *item, scenesItems) {
        Pillar *pillar = dynamic_cast<Pillar*>(item);

        if(pillar){
            removeItem(pillar);
            delete pillar;
        }
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space){
        if(gamoOn){
            bird->shootUp();
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(gamoOn)
        {
            bird->shootUp();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}
