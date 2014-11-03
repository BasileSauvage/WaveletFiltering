#include "mainwindow.h"
#include "workspace.h"
#include <QGridLayout>
#include <QInputDialog>
#include <iostream>

/**
  * @brief Constructeur
  * @param parent
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->runUI();
    this->connectActions();
}

/**
  * @brief Destructeur
  */
MainWindow::~MainWindow() {}

/**
  * @brief Initialise les éléments de la fenêtre
  */
void MainWindow::runUI()
{
    this->resize(1024, 800);

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
    this->source_view->setMinimumSize(400, 400);
    this->haar_view = new QGraphicsView(this->haar_scene,this);
    this->haar_view->setMinimumSize(400, 400);
    this->filter_view = new QGraphicsView(this->filter_scene,this);
    this->filter_view->setMinimumSize(400, 400);
    this->synthesis_view = new QGraphicsView(this->synthesis_scene,this);
    this->synthesis_view->setMinimumSize(400, 400);

    this->zoom_level = 1;

    this->zoom_slider = new QSlider(Qt::Vertical, this);
    this->zoom_slider->setRange(1, 10);
    this->zoom_slider->setValue(this->getZoomLevel());
    this->zoom_slider->setTickInterval(1);
    this->zoom_slider->setEnabled(false);

    this->main_widget = new QWidget(this);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(this->source_view, 0, 0, 1, 1);
    layout->addWidget(this->haar_view, 0, 1, 1, 1);
    layout->addWidget(this->filter_view, 1, 1, 1, 1);
    layout->addWidget(this->synthesis_view, 1, 0, 1, 1);
    layout->addWidget(this->zoom_slider, 1, 2, 1, 1);

    this->main_widget->setLayout(layout);

    this->setCentralWidget(this->main_widget);
}

/**
  * @brief Met à jour les éléments de la fenêtre
  * @param origin source qui requiert une mise à jour de la fenêtre
  */
void MainWindow::updateUI(status origin)
{
    switch(origin)
    {
        case LOAD :
            this->sourceDisplayer();
            break;
        case HAAR :
            this->haarDisplayer();
            break;
        case FILTER :
            this->filterDisplayer();
            break;
        case SYNTH :
            this->synthesisDisplayer();
            break;
        case ZOOM :
            this->sourceDisplayer();
            this->haarDisplayer();
            this->filterDisplayer();
            this->synthesisDisplayer();
            break;
        default :
            break;
    }
}

/**
  * @brief Affiche l'image source
  */
void MainWindow::sourceDisplayer()
{
    WorkSpace* ws = WorkSpace::getInstance();

    this->action_haar->setEnabled(true);
    this->source_scene->clear();

    QPixmap source_map(QPixmap::fromImage(ws->getSourceImage()));
    source_map = source_map.scaled(ws->getWidth()*this->getZoomLevel(), ws->getHeight()*this->getZoomLevel());
    this->source_scene->addPixmap(source_map);
    this->source_view->setSceneRect(this->source_scene->itemsBoundingRect());
}

/**
  * @brief Affiche la base de Haar
  */
void MainWindow::haarDisplayer()
{
    WorkSpace* ws = WorkSpace::getInstance();

    this->action_zero_filter->setEnabled(true);
    this->haar_scene->clear();

    QPixmap haar_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getHaarMatrix())));
    haar_map = haar_map.scaled(ws->getWidth()*this->getZoomLevel(), ws->getHeight()*this->getZoomLevel()); // à changer
    this->haar_scene->addPixmap(haar_map);
    this->haar_view->setSceneRect(this->haar_scene->itemsBoundingRect());
}

/**
  * @brief Affiche le résultat du filtrage
  */
void MainWindow::filterDisplayer()
{
    WorkSpace* ws = WorkSpace::getInstance();

    this->action_reverse_haar->setEnabled(true);
    this->filter_scene->clear();

    QPixmap filter_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getFilterMatrix())));
    filter_map = filter_map.scaled(ws->getWidth()*this->getZoomLevel(), ws->getHeight()*this->getZoomLevel()); // à changer
    this->filter_scene->addPixmap(filter_map);
    this->filter_view->setSceneRect(this->filter_scene->itemsBoundingRect());
}

/**
  * @brief Affiche l'image synthèse
  */
void MainWindow::synthesisDisplayer()
{
    WorkSpace* ws = WorkSpace::getInstance();

    this->action_save->setEnabled(true);
    this->zoom_slider->setEnabled(true);
    this->synthesis_scene->clear();

    QPixmap synthesis_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getSynthesisMatrix())));
    synthesis_map = synthesis_map.scaled(ws->getWidth()*this->getZoomLevel(), ws->getHeight()*this->getZoomLevel());
    this->synthesis_scene->addPixmap(synthesis_map);
    this->synthesis_view->setSceneRect(this->synthesis_scene->itemsBoundingRect());
}

/**
  * @brief Connecte les actions et les slots
  */
void MainWindow::connectActions()
{
    QObject::connect(this->zoom_slider, SIGNAL(valueChanged(int)), this, SLOT(zoomModifier(int)));
    QObject::connect(this->action_load, SIGNAL(triggered()), this, SLOT(actionLoad()));
    QObject::connect(this->action_save, SIGNAL(triggered()), this, SLOT(actionSave()));
    QObject::connect(this->action_quit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(this->action_haar, SIGNAL(triggered()), this, SLOT(actionHaar()));
    QObject::connect(this->action_reverse_haar, SIGNAL(triggered()), this, SLOT(actionReverseHaar()));
    QObject::connect(this->action_zero_filter, SIGNAL(triggered()), this, SLOT(actionZeroFilter()));
    QObject::connect(this->source_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateHScrollBar(int)));
    QObject::connect(this->source_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateVScrollBar(int)));
    QObject::connect(this->haar_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateHScrollBar(int)));
    QObject::connect(this->haar_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateVScrollBar(int)));
    QObject::connect(this->filter_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateHScrollBar(int)));
    QObject::connect(this->filter_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateVScrollBar(int)));
    QObject::connect(this->synthesis_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateHScrollBar(int)));
    QObject::connect(this->synthesis_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateVScrollBar(int)));
}

/**
  * @brief Slot qui va gérer les déplacements du slider de zoom
  */
void MainWindow::zoomModifier(int val)
{
    this->zoom_level = val;
    this->updateUI(ZOOM);
}

/**
  * @brief Slot qui va charger l'image sur laquelle travailler
  */
void MainWindow::actionLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choisir une image"), "../Images", tr("Images (*.jpg *.bmp)"));

    if(fileName != 0 && fileName.length() > 0)
    {
        QImage source(fileName);
        WorkSpace::newInstance(source);
        this->updateUI(LOAD);
    }
}

/**
  * @brief Slot qui va sauvegarder les différents résultats
  */
void MainWindow::actionSave()
{
    WorkSpace* ws = WorkSpace::getInstance();
    ws->saveImage(ws->getImageFromMatrix(ws->getHaarMatrix()), "haar.jpg");
    ws->saveImage(ws->getImageFromMatrix(ws->getFilterMatrix()), "filter.jpg");
    ws->saveImage(ws->getImageFromMatrix(ws->getSynthesisMatrix()), "synthesis.jpg");
}

/**
  * @brief Slot qui va déclencher la transformée en ondelettes discrète
  */
void MainWindow::actionHaar()
{   
    WorkSpace* ws = WorkSpace::getInstance();
    int max_value = 0;
    for(unsigned int i = (ws->getHeight() < ws->getWidth())? ws->getHeight() : ws->getWidth(); i >= 2; i/=2)
    {
        max_value++;
    }

    bool ok;
    int analysis_level = QInputDialog::getInt(this, tr("Choisir niveau d'analyse"), tr("Niveau d'analyse :"), 3, 1, max_value, 1, &ok);

    if(ok)
    {
        ws->waveletsTransform(analysis_level);
        this->updateUI(HAAR);
    }
}

/**
  * @brief Slot qui va déclencher la transformée en ondelettes inverse
  */
void MainWindow::actionReverseHaar()
{
    WorkSpace* ws = WorkSpace::getInstance();
    ws->waveletsReverseTransform(ws->getFilterMatrix());
    this->updateUI(SYNTH);
}

/**
  * @brief Slot qui appliquer le "filtre zéro" sur l'image
  */
void MainWindow::actionZeroFilter()
{
    WorkSpace* ws = WorkSpace::getInstance();
    ws->zeroFilter(ws->getHaarMatrix());
    this->updateUI(FILTER);
}

/**
  * @brief Slot qui déplacement simultanément tous les sliders horizontaux
  */
void MainWindow::updateHScrollBar(int val)
{
    this->source_view->horizontalScrollBar()->setValue(val);
    this->haar_view->horizontalScrollBar()->setValue(val);
    this->filter_view->horizontalScrollBar()->setValue(val);
    this->synthesis_view->horizontalScrollBar()->setValue(val);
}

/**
  * @brief Slot qui déplacement simultanément tous les sliders verticaux
  */
void MainWindow::updateVScrollBar(int val)
{
    this->source_view->verticalScrollBar()->setValue(val);
    this->haar_view->verticalScrollBar()->setValue(val);
    this->filter_view->verticalScrollBar()->setValue(val);
    this->synthesis_view->verticalScrollBar()->setValue(val);
}

/**
  * @brief Accesseur du niveau de zoom
  * @return niveau de zoom
  */
int MainWindow::getZoomLevel()
{
    return this->zoom_level;
}
