#include "mainwindow.h"
#include "workspace.h"
#include <QGridLayout>

MainWindow::MainWindow(QImage img, QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(1024,800);
    this->source_scene = new QGraphicsScene(this);
    this->haar_scene = new QGraphicsScene(this);
    this->filter_scene = new QGraphicsScene(this);
    this->synthesis_scene = new QGraphicsScene(this);

    this->source_view = new QGraphicsView(this->source_scene,this);
    this->source_view->setMinimumSize(300,300);
    this->haar_view = new QGraphicsView(this->haar_scene,this);
    this->haar_view->setMinimumSize(300,300);
    this->filter_view = new QGraphicsView(this->filter_scene,this);
    this->filter_view->setMinimumSize(300,300);
    this->synthesis_view = new QGraphicsView(this->synthesis_scene,this);
    this->synthesis_view->setMinimumSize(300,300);

    QWidget* central = new QWidget(this);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(this->source_view, 0, 0, 1, 1);
    layout->addWidget(this->haar_view, 0, 1, 1, 1);
    layout->addWidget(this->filter_view, 1, 1, 1, 1);
    layout->addWidget(this->synthesis_view, 1, 0, 1, 1);

    central->setLayout(layout);

    this->setCentralWidget(central);

    WorkSpace *ws = WorkSpace::getInstance(img);

    QPixmap source(QPixmap::fromImage(ws->getSourceImage()));
    QPixmap haar(QPixmap::fromImage(ws->getImageFromMatrix(ws->getHaarMatrix())));
    QPixmap filter(QPixmap::fromImage(ws->getImageFromMatrix(ws->getFilterMatrix())));
    QPixmap synthesis(QPixmap::fromImage(ws->getImageFromMatrix(ws->getSynthesisMatrix())));

    this->source_scene->addPixmap(source);
    this->haar_scene->addPixmap(haar);
    this->filter_scene->addPixmap(filter);
    this->synthesis_scene->addPixmap(synthesis);
}
