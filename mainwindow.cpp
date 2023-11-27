#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new Scene(this);
    scene->setSceneRect(-250, -300, 500, 600);

    QColor backgroundColor(0,191,255);
    QBrush brush(backgroundColor);  // Pode ser qualquer cor que você preferir
    scene->setBackgroundBrush(brush);

    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startGameButtom_clicked()
{
    scene->startGame();
}

