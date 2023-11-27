#ifndef SCENE_H
#define SCENE_H

#include <QTimer>
#include <QGraphicsScene>
#include "pillar.h"
#include "bird.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void addBird();

    void startGame();

    bool getGamoOn() const;
    void setGamoOn(bool newGamoOn);

    void incrementScore();

    int getScore() const;
    void setScore(int newScore);

signals:

private:
    void showGameOverGraphics();
    void hideGameOverGraphics();
    void cleanPillars();
    void setUpPillarTimer();
    void freezeBordAndPillars();

    Bird *bird;

    QTimer *pillarTimer;

    bool gamoOn;

    int score;
    int bestSocre;

    QGraphicsPixmapItem *gameOverPix;
    QGraphicsTextItem *scoreTextItem;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
