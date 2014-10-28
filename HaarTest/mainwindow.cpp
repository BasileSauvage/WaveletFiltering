#include "mainwindow.h"
#include "workspace.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->runUI();
    this->connectActions();
}

MainWindow::~MainWindow() {}

void MainWindow::runUI()
{
    this->resize(1024,800);

    this->menu = new QMenuBar(this);
    this->menu_file = new QMenu("Fichier", this);
    this->menu_transform = new QMenu("Transformations", this);

    this->action_load = new QAction("Charger image", this);
    this->action_save = new QAction("Sauvegarder résultat", this);
    this->action_quit = new QAction("Quitter", this);

    this->action_haar = new QAction("Transformée de Haar", this);
    this->action_reverse_haar = new QAction("Transformée inverse", this);
    this->action_zero_filter = new QAction("Filtre zéro", this);

    this->menu_file->addAction(this->action_load);
    this->menu_file->addAction(this->action_save);
    this->menu_file->addAction(this->action_quit);
    this->menu_transform->addAction(this->action_haar);
    this->menu_transform->addAction(this->action_reverse_haar);
    this->menu_transform->addAction(this->action_zero_filter);

    this->menu->addMenu(this->menu_file);
    this->menu->addMenu(this->menu_transform);
    this->setMenuBar(this->menu);

    this->action_save->setEnabled(false);
    this->action_haar->setEnabled(false);
    this->action_reverse_haar->setEnabled(false);
    this->action_zero_filter->setEnabled(false);

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

    this->main_widget = new QWidget(this);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(this->source_view, 0, 0, 1, 1);
    layout->addWidget(this->haar_view, 0, 1, 1, 1);
    layout->addWidget(this->filter_view, 1, 1, 1, 1);
    layout->addWidget(this->synthesis_view, 1, 0, 1, 1);

    this->main_widget->setLayout(layout);

    this->setCentralWidget(this->main_widget);
}

void MainWindow::updateUI(status action)
{
    WorkSpace* ws = WorkSpace::getInstance();

    switch(action)
    {
        case LOAD :
        {
            this->action_haar->setEnabled(true);
            QPixmap source_map(QPixmap::fromImage(ws->getSourceImage()));
            this->source_scene->addPixmap(source_map);
            break;
        }
        case HAAR :
        {
            this->action_zero_filter->setEnabled(true);
            QPixmap haar_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getHaarMatrix())));
            this->haar_scene->addPixmap(haar_map);
            break;
        }
        case FILTER :
        {
            this->action_reverse_haar->setEnabled(true);
            QPixmap filter_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getFilterMatrix())));
            this->filter_scene->addPixmap(filter_map);
            break;
        }
        case SYNTH :
        {
            QPixmap synthesis_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getSynthesisMatrix())));
            this->synthesis_scene->addPixmap(synthesis_map);
            break;
        }
        default :
            break;
    }
}

void MainWindow::connectActions()
{
    QObject::connect(this->action_load, SIGNAL(triggered()), this, SLOT(actionLoad()));
    //QObject::connect(this->action_save, SIGNAL(triggered()), this, SLOT(actionSave()));
    QObject::connect(this->action_quit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(this->action_haar, SIGNAL(triggered()), this, SLOT(actionHaar()));
    QObject::connect(this->action_reverse_haar, SIGNAL(triggered()), this, SLOT(actionReverseHaar()));
    QObject::connect(this->action_zero_filter, SIGNAL(triggered()), this, SLOT(actionZeroFilter()));
}

void MainWindow::actionLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choisir une image"), ".", tr("Images (*.jpg *.bmp)"));

    if(fileName != 0 && fileName.length() > 0)
    {
        QImage source(fileName);
        WorkSpace::newInstance(source);
        this->updateUI(LOAD);
    }
}

void MainWindow::actionHaar()
{
    WorkSpace* ws = WorkSpace::getInstance();
    ws->waveletsTransform(3);
    this->updateUI(HAAR);
}

void MainWindow::actionReverseHaar()
{
    WorkSpace* ws = WorkSpace::getInstance();
    ws->waveletsReverseTransform(ws->getFilterMatrix());
    this->updateUI(SYNTH);
}

void MainWindow::actionZeroFilter()
{
    WorkSpace* ws = WorkSpace::getInstance();
    ws->zeroFilter(ws->getHaarMatrix());
    this->updateUI(FILTER);
}
