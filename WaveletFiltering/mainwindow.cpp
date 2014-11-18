#include "mainwindow.h"
#include <QGridLayout>
#include <QInputDialog>
#include <cmath>
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
    this->menu_file = new QMenu("&Fichier", this);
    this->menu_transform = new QMenu("Transformation", this);

    this->action_load = new QAction("Charger image", this);
    this->action_load->setShortcut(QKeySequence("Ctrl+N"));
    this->action_save = new QAction("Sauvegarder résultat", this);
    this->action_save->setShortcut(QKeySequence("Ctrl+S"));
    this->action_save_all = new QAction("Sauvegarder tout", this);
    this->action_save_all->setShortcut(QKeySequence("Ctrl+Shift+S"));
    this->action_swap = new QAction("Remonter résultat", this);
    this->action_swap->setShortcut(QKeySequence("Ctrl+Shift+N"));
    this->action_quit = new QAction("Quitter", this);
    this->action_quit->setShortcut(QKeySequence("Ctrl+W"));

    this->action_zero_filter = new QAction("Filtre zéro", this);

    this->menu_file->addAction(this->action_load);
    this->menu_file->addAction(this->action_save);
    this->menu_file->addAction(this->action_save_all);
    this->menu_file->addAction(this->action_swap);
    this->menu_file->addAction(this->action_quit);
    this->menu_transform->addAction(this->action_zero_filter);

    this->menu->addMenu(this->menu_file);
    this->menu->addMenu(this->menu_transform);
    this->setMenuBar(this->menu);

    this->action_save->setEnabled(false);
    this->action_save_all->setEnabled(false);
    this->action_swap->setEnabled(false);
    this->action_zero_filter->setEnabled(false);

    this->input_fine_scene = new QGraphicsScene(this);
    this->input_DWT_scene = new QGraphicsScene(this);
    this->output_DWT_scene = new QGraphicsScene(this);
    this->output_fine_scene = new QGraphicsScene(this);

    this->input_fine_view = new QGraphicsView(this->input_fine_scene,this);
    this->input_fine_view->setFixedSize(512, 512);
    this->input_fine_view->setDragMode(QGraphicsView::ScrollHandDrag);
    this->input_DWT_view = new QGraphicsView(this->input_DWT_scene,this);
    this->input_DWT_view->setFixedSize(512, 512);
    this->input_DWT_view->setDragMode(QGraphicsView::ScrollHandDrag);
    this->output_DWT_view = new QGraphicsView(this->output_DWT_scene,this);
    this->output_DWT_view->setFixedSize(512, 512);
    this->output_DWT_view->setDragMode(QGraphicsView::ScrollHandDrag);
    this->output_fine_view = new QGraphicsView(this->output_fine_scene,this);
    this->output_fine_view->setFixedSize(512, 512);
    this->output_fine_view->setDragMode(QGraphicsView::ScrollHandDrag);

    this->filtered = false;

    this->zoom_level = 0;

    this->zoom_label = new QLabel(this);
    this->zoom_label->setText("Zoom x " + QString::number(this->getZoomLevel()));

    this->zoom_slider = new QSlider(Qt::Vertical, this);
    this->zoom_slider->setRange(-3, 9);
    this->zoom_slider->setValue(this->getZoomLevel());
    this->zoom_slider->setTickInterval(1);
    this->zoom_slider->setEnabled(false);

    this->wavelets_label = new QLabel(this);
    this->wavelets_label->setText("Nv analyse");

    this->wavelets_spinbox = new QSpinBox(this);
    this->wavelets_spinbox->setEnabled(false);

    this->synchro_checkbox = new QCheckBox("Synchroniser vues", this);

    QVBoxLayout *wavelets_layout = new QVBoxLayout();
    wavelets_layout->addWidget(this->wavelets_spinbox);
    wavelets_layout->addWidget(this->wavelets_label);
    wavelets_layout->addWidget(this->synchro_checkbox);
    wavelets_layout->addWidget(this->zoom_slider);
    wavelets_layout->addWidget(this->zoom_label);

    this->main_widget = new QWidget(this);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(this->synchro_checkbox, 0, 0, 1, 1);
    layout->addWidget(this->input_fine_view, 0, 0, 1, 1);
    layout->addWidget(this->input_DWT_view, 0, 1, 1, 1);
    layout->addWidget(this->output_DWT_view, 1, 1, 1, 1);
    layout->addWidget(this->output_fine_view, 1, 0, 1, 1);
    layout->addLayout(wavelets_layout, 0, 2, 1, 1);

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
            this->inputFineDisplayer();
            break;
        case HAAR :
            this->inputDWTDisplayer();
            break;
        case FILTER :
            this->outputDWTDisplayer();
            break;
        case SYNTH :
            this->outputFineDisplayer();
            break;
        case ZOOM :
            this->inputFineDisplayer();
            this->inputDWTDisplayer();
            if(this->isFiltered())
            {
                this->outputDWTDisplayer();
                this->outputFineDisplayer();
            }
            break;
        case SCROLL :
            this->inputDWTDisplayer();
            if(this->isFiltered()) this->outputDWTDisplayer();
            break;
        default :
            break;
    }
}

/**
  * @brief Met à jour les éléments de la fenêtre à la suite d'un swap
  */
void MainWindow::resetUI()
{
    this->input_DWT_scene->clear();
    this->output_DWT_scene->clear();
    this->output_fine_scene->clear();

    this->action_save->setEnabled(false);
    this->action_save_all->setEnabled(false);
    this->action_swap->setEnabled(false);
    this->action_zero_filter->setEnabled(false);

    this->filtered = false;
    this->zoom_level = 1;

    this->zoom_label->setText("Zoom x " + QString::number(this->getZoomLevel()));
    this->zoom_slider->setValue(this->getZoomLevel());
    this->zoom_slider->setEnabled(false);

    this->wavelets_spinbox->setEnabled(false);
}

/**
  * @brief Affiche l'image source
  */
void MainWindow::inputFineDisplayer()
{
    WorkSpace* ws = WorkSpace::getInstance();

    this->zoom_slider->setEnabled(true);
    this->input_fine_scene->clear();

    QPixmap input_fine_map(QPixmap::fromImage(ws->getSourceImage()));
    if(this->getZoomLevel() >= 0) input_fine_map = input_fine_map.scaled(ws->getWidth()*(this->getZoomLevel()+1), ws->getHeight()*(this->getZoomLevel()+1));
    else input_fine_map = input_fine_map.scaled(ws->getWidth()/(1-this->getZoomLevel()), ws->getHeight()/(1-this->getZoomLevel()));
    this->input_fine_scene->addPixmap(input_fine_map);
    this->input_fine_view->setSceneRect(this->input_fine_scene->itemsBoundingRect());
}

/**
  * @brief Affiche la base de Haar
  */
void MainWindow::inputDWTDisplayer()
{
    WorkSpace* ws = WorkSpace::getInstance();

    this->action_zero_filter->setEnabled(true);
    this->input_DWT_scene->clear();

    if(!this->wavelets_spinbox->isEnabled())
    {
        int max_value = 0;
        for(unsigned int i = (ws->getHeight() < ws->getWidth())? ws->getHeight() : ws->getWidth(); i >= 2; i/=2)
        {
            max_value++;
        }
        this->wavelets_spinbox->setRange(0, max_value);
        this->wavelets_spinbox->setValue(0);
        this->wavelets_spinbox->setEnabled(true);
    }

    if(this->getZoomLevel() == 0 || (this->getZoomLevel() > 0 && ws->getCurrentAnalysisLevel() == 0))
    {
        QPixmap input_DWT_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getInputDWTMatrix())));
        input_DWT_map = input_DWT_map.scaled(ws->getWidth()*(this->getZoomLevel()+1), ws->getHeight()*(this->getZoomLevel()+1));
        this->input_DWT_scene->addPixmap(input_DWT_map);
    }
    else if(this->getZoomLevel() < 0)
    {
        QPixmap input_DWT_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getInputDWTMatrix())));
        input_DWT_map = input_DWT_map.scaled(ws->getWidth()/(1-this->getZoomLevel()), ws->getHeight()/(1-this->getZoomLevel()));
        this->input_DWT_scene->addPixmap(input_DWT_map);
    }
    else
    {
        struct block zoom_block;
        zoom_block.top_left_x = this->input_fine_view->horizontalScrollBar()->value()/(this->getZoomLevel()+1);
        zoom_block.top_left_y = this->input_fine_view->verticalScrollBar()->value()/(this->getZoomLevel()+1);
        if(zoom_block.top_left_x > ws->getWidth() - 1 - this->input_fine_view->width()/(this->getZoomLevel()+1)) zoom_block.top_left_x = ws->getWidth() - 1 - this->input_fine_view->width()/(this->getZoomLevel()+1);
        if(zoom_block.top_left_y > ws->getHeight() - 1 - this->input_fine_view->height()/(this->getZoomLevel()+1)) zoom_block.top_left_y = ws->getHeight() - 1 - this->input_fine_view->height()/(this->getZoomLevel()+1);
        zoom_block.bottom_right_x = (zoom_block.top_left_x + this->input_fine_view->width()/(this->getZoomLevel()+1)) - 1;
        zoom_block.bottom_right_y = (zoom_block.top_left_y + this->input_fine_view->height()/(this->getZoomLevel()+1)) - 1;

        QPixmap input_DWT_map(QPixmap::fromImage(ws->zoomEditor(zoom_block, ws->getInputDWTMatrix())));
        input_DWT_map = input_DWT_map.scaled(ws->getWidth()*(this->getZoomLevel()+1), ws->getHeight()*(this->getZoomLevel()+1));
        this->input_DWT_scene->addPixmap(input_DWT_map);
    }

    this->input_DWT_view->setSceneRect(this->input_DWT_scene->itemsBoundingRect());
}

/**
  * @brief Affiche le résultat du filtrage
  */
void MainWindow::outputDWTDisplayer()
{
    WorkSpace* ws = WorkSpace::getInstance();
    if(!this->isFiltered()) this->filtered = true;

    this->output_DWT_scene->clear();

    if(this->getZoomLevel() == 0)
    {
        QPixmap output_DWT_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getOutputDWTMatrix())));
        output_DWT_map = output_DWT_map.scaled(ws->getWidth()*(this->getZoomLevel()+1), ws->getHeight()*(this->getZoomLevel()+1));
        this->output_DWT_scene->addPixmap(output_DWT_map);
    }
    else if(this->getZoomLevel() < 0)
    {
        QPixmap output_DWT_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getOutputDWTMatrix())));
        output_DWT_map = output_DWT_map.scaled(ws->getWidth()/(1-this->getZoomLevel()), ws->getHeight()/(1-this->getZoomLevel()));
        this->output_DWT_scene->addPixmap(output_DWT_map);
    }
    else
    {
        struct block zoom_block;
        zoom_block.top_left_x = this->input_fine_view->horizontalScrollBar()->value()/(this->getZoomLevel()+1);
        zoom_block.top_left_y = this->input_fine_view->verticalScrollBar()->value()/(this->getZoomLevel()+1);
        if(zoom_block.top_left_x > ws->getWidth() - 1 - this->input_fine_view->width()/(this->getZoomLevel()+1)) zoom_block.top_left_x = ws->getWidth() - 1 - this->input_fine_view->width()/(this->getZoomLevel()+1);
        if(zoom_block.top_left_y > ws->getHeight() - 1 - this->input_fine_view->height()/(this->getZoomLevel()+1)) zoom_block.top_left_y = ws->getHeight() - 1 - this->input_fine_view->height()/(this->getZoomLevel()+1);
        zoom_block.bottom_right_x = (zoom_block.top_left_x + this->input_fine_view->width()/(this->getZoomLevel()+1)) - 1;
        zoom_block.bottom_right_y = (zoom_block.top_left_y + this->input_fine_view->height()/(this->getZoomLevel()+1)) - 1;

        QPixmap output_DWT_map(QPixmap::fromImage(ws->zoomEditor(zoom_block, ws->getOutputDWTMatrix())));
        output_DWT_map = output_DWT_map.scaled(ws->getWidth()*(this->getZoomLevel()+1), ws->getHeight()*(this->getZoomLevel()+1));
        this->output_DWT_scene->addPixmap(output_DWT_map);
    }

    this->output_DWT_view->setSceneRect(this->output_DWT_scene->itemsBoundingRect());
}

/**
  * @brief Affiche l'image synthèse
  */
void MainWindow::outputFineDisplayer()
{
    WorkSpace* ws = WorkSpace::getInstance();

    this->action_save->setEnabled(true);
    this->action_save_all->setEnabled(true);
    this->action_swap->setEnabled(true);
    this->output_fine_scene->clear();

    QPixmap output_fine_map(QPixmap::fromImage(ws->getImageFromMatrix(ws->getOutputFineMatrix())));
    if(this->getZoomLevel() >= 0) output_fine_map = output_fine_map.scaled(ws->getWidth()*(this->getZoomLevel()+1), ws->getHeight()*(this->getZoomLevel()+1));
    else output_fine_map = output_fine_map.scaled(ws->getWidth()/(1-this->getZoomLevel()), ws->getHeight()/(1-this->getZoomLevel()));
    this->output_fine_scene->addPixmap(output_fine_map);
    this->output_fine_view->setSceneRect(this->output_fine_scene->itemsBoundingRect());
}

/**
  * @brief Connecte les actions et les slots
  */
void MainWindow::connectActions()
{
    QObject::connect(this->zoom_slider, SIGNAL(valueChanged(int)), this, SLOT(zoomModifier(int)));
    QObject::connect(this->wavelets_spinbox, SIGNAL(valueChanged(int)), this, SLOT(analysisModifier(int)));
    QObject::connect(this->action_load, SIGNAL(triggered()), this, SLOT(actionLoad()));
    QObject::connect(this->action_save, SIGNAL(triggered()), this, SLOT(actionSave()));
    QObject::connect(this->action_save_all, SIGNAL(triggered()), this, SLOT(actionSaveAll()));
    QObject::connect(this->action_swap, SIGNAL(triggered()), this, SLOT(actionSwap()));
    QObject::connect(this->action_quit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(this->action_zero_filter, SIGNAL(triggered()), this, SLOT(actionZeroFilter()));
    QObject::connect(this->input_fine_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateHScrollBar(int)));
    QObject::connect(this->input_fine_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateVScrollBar(int)));
    QObject::connect(this->output_fine_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateHScrollBar(int)));
    QObject::connect(this->output_fine_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateVScrollBar(int)));
    QObject::connect(this->input_DWT_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateZoomedHScrollBar(int)));
    QObject::connect(this->input_DWT_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateZoomedVScrollBar(int)));
    QObject::connect(this->output_DWT_view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateZoomedHScrollBar(int)));
    QObject::connect(this->output_DWT_view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateZoomedVScrollBar(int)));
}

/**
  * @brief Slot qui va gérer les déplacements du slider de zoom
  */
void MainWindow::zoomModifier(int val)
{
    this->zoom_level = val;
    this->zoom_label->setText("Zoom x " + QString::number(val));
    this->updateUI(ZOOM);
}

/**
  * @brief Slot qui va gérer les déplacements du slider du niveau d'analyse
  */
void MainWindow::analysisModifier(int val)
{
    WorkSpace* ws = WorkSpace::getInstance();

    if(this->isFiltered())
    {
        ws->waveletTransform(true, val);
        this->updateUI(FILTER);
    }
    else ws->waveletTransform(false, val);

    this->updateUI(HAAR);
}

/**
  * @brief Slot qui va charger l'image sur laquelle travailler
  */
void MainWindow::actionLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Charger une image"), "../Images", tr("Images (*.jpg *.bmp *.tif)"));

    if(fileName != 0 && fileName.length() > 0)
    {
        QImage source(fileName);
        WorkSpace::newInstance(source);
        this->updateUI(LOAD);
        this->updateUI(HAAR);
    }
}

/**
  * @brief Slot qui va sauvegarder le résultat
  */
void MainWindow::actionSave()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder une image"), "../Images", tr("Images (*.jpg *.bmp *.tif)"));

    if(fileName != 0 && fileName.length() > 0)
    {
        WorkSpace* ws = WorkSpace::getInstance();
        ws->saveImage(ws->getImageFromMatrix(ws->getOutputFineMatrix()), fileName);
    }
}

/**
  * @brief Slot qui va sauvegarder les différentes images
  */
void MainWindow::actionSaveAll()
{
    WorkSpace* ws = WorkSpace::getInstance();
    ws->saveImage(ws->getImageFromMatrix(ws->getInputDWTMatrix()), "haar.jpg");
    ws->saveImage(ws->getImageFromMatrix(ws->getOutputDWTMatrix()), "filter.jpg");
    ws->saveImage(ws->getImageFromMatrix(ws->getOutputFineMatrix()), "synthesis.jpg");
}

/**
  * @brief Slot qui va permettre de recommencer les étapes à partir du dernier résultat
  */
void MainWindow::actionSwap()
{
    WorkSpace* ws = WorkSpace::getInstance();
    ws->swap();
    this->resetUI();
    this->updateUI(LOAD);
    this->updateUI(HAAR);
}

/**
  * @brief Slot qui appliquer le "filtre zéro" sur l'image
  */
void MainWindow::actionZeroFilter()
{
    WorkSpace* ws = WorkSpace::getInstance();
    if(!this->isFiltered()) ws->copyMatrix(ws->getInputDWTMatrix(), ws->getOutputDWTMatrix());
    ws->zeroFilter();
    ws->updateOutputFineFromDWT();
    this->updateUI(FILTER);
    this->updateUI(SYNTH);
}

/**
  * @brief Slot qui gère le comportement des scrollbars horizontales
  */
void MainWindow::updateHScrollBar(int val)
{
    this->input_fine_view->horizontalScrollBar()->setValue(val);
    this->output_fine_view->horizontalScrollBar()->setValue(val);
    if(this->getZoomLevel() > 0 && !this->synchro_checkbox->isChecked())
    {
        this->updateUI(SCROLL);
    }
    else
    {
        this->input_DWT_view->horizontalScrollBar()->setValue(val);
        this->output_DWT_view->horizontalScrollBar()->setValue(val);
    }
}

/**
  * @brief Slot qui gère le comportement des scrollbars verticales
  */
void MainWindow::updateVScrollBar(int val)
{
    this->input_fine_view->verticalScrollBar()->setValue(val);
    this->output_fine_view->verticalScrollBar()->setValue(val);
    if(this->getZoomLevel() > 0)
    {
        this->updateUI(SCROLL);
    }
    else
    {
        this->input_DWT_view->verticalScrollBar()->setValue(val);
        this->output_DWT_view->verticalScrollBar()->setValue(val);
    }
}

/**
  * @brief Slot qui gère le comportement des scrollbars horizontales lors d'un zoom
  */
void MainWindow::updateZoomedHScrollBar(int val)
{
    if(this->getZoomLevel() < 1)
    {
        this->input_fine_view->horizontalScrollBar()->setValue(val);
        this->output_fine_view->horizontalScrollBar()->setValue(val);
    }
    this->input_DWT_view->horizontalScrollBar()->setValue(val);
    this->output_DWT_view->horizontalScrollBar()->setValue(val);
}

/**
  * @brief Slot qui gère le comportement des scrollbars verticales lors d'un zoom
  */
void MainWindow::updateZoomedVScrollBar(int val)
{
    if(this->getZoomLevel() < 1 && this->synchro_checkbox->isChecked())
    {
        this->input_fine_view->verticalScrollBar()->setValue(val);
        this->output_fine_view->verticalScrollBar()->setValue(val);
    }
    this->input_DWT_view->verticalScrollBar()->setValue(val);
    this->output_DWT_view->verticalScrollBar()->setValue(val);
}


/**
  * @brief Accesseur du niveau de zoom
  * @return niveau de zoom
  */
int MainWindow::getZoomLevel()
{
    return this->zoom_level;
}/**
  * @brief Retourne la valeur du booléen disant si un filtre a été appliqué ou non
  * @return true si un filtre a été appliqué, false sinon
  */


bool MainWindow::isFiltered()
{
    return this->filtered;
}
