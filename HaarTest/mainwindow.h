#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "workspace.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMenuBar>
#include <QMenu>
#include <QSlider>
#include <QLabel>
#include <QAction>
#include <QFileDialog>
#include <QScrollBar>


enum status{LOAD, HAAR, FILTER, SYNTH, ZOOM, SLIDE};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMenuBar* menu;
    QMenu* menu_file;
    QMenu* menu_transform;
    QMenu* menu_select;

    QGraphicsView* source_view;
    QGraphicsView* haar_view;
    QGraphicsView* filter_view;
    QGraphicsView* synthesis_view;

    QGraphicsScene* source_scene;
    QGraphicsScene* haar_scene;
    QGraphicsScene* filter_scene;
    QGraphicsScene* synthesis_scene;

    QWidget* main_widget;

    QSlider* wavelets_slider;
    QSlider* zoom_slider;

    QLabel* wavelets_label;
    QLabel* zoom_label;

    QAction* action_load;
    QAction* action_save;
    QAction* action_swap;
    QAction* action_quit;
    QAction* action_haar;
    QAction* action_reverse_haar;
    QAction* action_zero_filter;
    QAction* action_select_diagonal; // provisoire
    QAction* action_select_horizontal; // provisoire
    QAction* action_select_vertical; // provisoire
    QAction* action_select_lowres; // provisoire

    int getZoomLevel();

private:
    int zoom_level;

    void runUI();
    void updateUI(status origin);
    void resetUI();
    void sourceDisplayer();
    void haarDisplayer();
    void filterDisplayer();
    void synthesisDisplayer();
    void connectActions();

public slots:
    void zoomModifier(int val);
    void analysisModifier(int val);
    void actionLoad();
    void actionSave();
    void actionSwap();
    void actionHaar();
    void actionReverseHaar();
    void actionZeroFilter();
    void actionSelectDiagonalBlock();
    void actionSelectHorizontalBlock();
    void actionSelectVerticalBlock();
    void actionSelectLowresBlock();
    void updateHScrollBar(int val);
    void updateVScrollBar(int val);
};

#endif // MAINWINDOW_H
