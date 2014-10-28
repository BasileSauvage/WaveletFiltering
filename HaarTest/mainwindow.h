#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

enum status{LOAD, HAAR, FILTER, SYNTH};

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

    QAction* action_load;
    QAction* action_save;
    QAction* action_quit;
    QAction* action_haar;
    QAction* action_reverse_haar;
    QAction* action_zero_filter;

private:
    void runUI();
    void updateUI(status origin);
    void connectActions();

public slots:
    void actionLoad();
    //void actionSave();
    void actionHaar();
    void actionReverseHaar();
    void actionZeroFilter();
};

#endif // MAINWINDOW_H
