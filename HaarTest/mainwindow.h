#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMenuBar>
#include <QMenu>
#include <QSlider>
#include <QAction>
#include <QFileDialog>

enum status{LOAD, HAAR, FILTER, SYNTH, ZOOM};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMenuBar* menu;
    QMenu* menu_file;
    QMenu* menu_transform;

    QGraphicsView* source_view;
    QGraphicsView* haar_view;
    QGraphicsView* filter_view;
    QGraphicsView* synthesis_view;

    QGraphicsScene* source_scene;
    QGraphicsScene* haar_scene;
    QGraphicsScene* filter_scene;
    QGraphicsScene* synthesis_scene;

    QWidget* main_widget;
    QWidget* tool_widget;

    QSlider* zoom_slider;

    QAction* action_load;
    QAction* action_save;
    QAction* action_quit;
    QAction* action_haar;
    QAction* action_reverse_haar;
    QAction* action_zero_filter;

    int getZoomLevel();

private:
    int zoom_level;

    void runUI();
    void updateUI(status origin);
    void connectActions();

public slots:
    void zoomModifier(int val);
    void actionLoad();
    void actionSave();
    void actionHaar();
    void actionReverseHaar();
    void actionZeroFilter();
};

#endif // MAINWINDOW_H
