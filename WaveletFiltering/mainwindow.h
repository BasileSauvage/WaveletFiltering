#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "workspace.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMenuBar>
#include <QMenu>
#include <QCheckBox>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QAction>
#include <QFileDialog>
#include <QScrollBar>


enum status{LOAD, HAAR, FILTER, SYNTH, ZOOM, SCROLL};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMenuBar* menu;
    QMenu* menu_file;
    QMenu* menu_transform;

    QGraphicsView* input_fine_view;
    QGraphicsView* input_DWT_view;
    QGraphicsView* output_DWT_view;
    QGraphicsView* output_fine_view;

    QGraphicsScene* input_fine_scene;
    QGraphicsScene* input_DWT_scene;
    QGraphicsScene* output_DWT_scene;
    QGraphicsScene* output_fine_scene;

    QWidget* main_widget;

    QCheckBox* synchro_checkbox;

    QSpinBox* wavelets_spinbox;

    QSlider* zoom_slider;

    QLabel* wavelets_label;
    QLabel* zoom_label;

    QAction* action_load;
    QAction* action_save;
    QAction* action_save_all;
    QAction* action_swap;
    QAction* action_quit;
    QAction* action_zero_filter;

    int getZoomLevel();
    bool isFiltered();

private:
    int zoom_level;
    bool filtered;

    void runUI();
    void updateUI(status origin);
    void resetUI();
    void inputFineDisplayer();
    void inputDWTDisplayer();
    void outputDWTDisplayer();
    void outputFineDisplayer();
    void connectActions();

public slots:
    void zoomModifier(int val);
    void analysisModifier(int val);
    void actionLoad();
    void actionSave();
    void actionSaveAll();
    void actionSwap();
    void actionZeroFilter();
    void updateHScrollBar(int val);
    void updateVScrollBar(int val);
    void updateZoomedHScrollBar(int val);
    void updateZoomedVScrollBar(int val);
};

#endif // MAINWINDOW_H
