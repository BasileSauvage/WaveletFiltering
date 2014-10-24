#include "mainwindow.h"
#include "workspace.h"
#include <QGridLayout>

MainWindow::MainWindow(QImage img, int** mat, QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(1024,800);
    this->source_scene = new QGraphicsScene(this);
    this->haar_scene = new QGraphicsScene(this);

    this->source_view = new QGraphicsView(this->source_scene,this);
    this->source_view->setMinimumSize(500,500);
    this->haar_view = new QGraphicsView(this->haar_scene,this);
    this->haar_view->setMinimumSize(500,500);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(this->source_view, 0, 0, 1, 1);
    layout->addWidget(this->haar_view, 0, 1, 1, 1);
    this->setLayout(layout);

    WorkSpace *ws = WorkSpace::getInstance(img);

    QImage img2;
    img2 = ws->getSourceImage();

    /* On modifie l'image à partir de la matrice d'entrée */
    for(unsigned int j = 0; j < ws->getHeight(); j++)
    {
        for(unsigned int i = 0; i < ws->getWidth(); i++)
        {
            if(mat[i][j] < 0) img2.setPixel(i, j, -mat[i][j]);
            else img2.setPixel(i, j, mat[i][j]);
        }
    }

    QPixmap source(QPixmap::fromImage(img2));
    QPixmap haar(QPixmap::fromImage(img2));

    this->source_scene->addPixmap(source);
    this->haar_scene->addPixmap(haar);
}
