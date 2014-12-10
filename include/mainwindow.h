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


enum status{ALL, OUTPUT, DWT};

/**
  * @class MainWindow
  * @brief Interface graphique
  * @author Julien Vogel
  * @version 1.0
  * @date 27 octobre 2014
  */
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

    QPixmap input_fine_map;
    QPixmap input_DWT_map;
    QPixmap output_DWT_map;
    QPixmap output_fine_map;

    QWidget* main_widget;

    QCheckBox* synchro_checkbox;

    QSpinBox* wavelets_spinbox;

	QSlider* zoom_slider_fine;
	QSlider* zoom_slider_DWT;

    QLabel* wavelets_label;
	QLabel* zoom_label_fine;
	QLabel* zoom_label_DWT;

    QAction* action_load;
    QAction* action_save;
    QAction* action_save_all;
    QAction* action_swap;
    QAction* action_quit;

    QAction* filter_vanish_coarse_details;
    QAction* filter_random_coarse_details;

	int getZoomLevelFine();
	int getZoomLevelDWT();

private:
	int zoom_level_fine;
	int zoom_level_DWT;

    void runUI();
    void updateUI(status origin);
    void resetUI();
    void inputFineDisplayer();
    block getFineZoomedBlock();
    void inputDWTDisplayer();
    void outputDWTDisplayer();
    void outputFineDisplayer();
    void connectActions();

public slots:
	void zoomModifierFine(int val);
	void zoomModifierDWT(int val);
	void analysisModifier(int val);
	void synchroModifier();
	void actionLoad();
    void actionSave();
    void actionSaveAll();
    void actionSwap();
    void filterVanishCoarseDetails();
    void filterRandomCoarseDetails();
    void updateFineHorizontalScrollBar(int val);
    void updateFineVerticalScrollBar(int val);
    void updateDWTHorizontalScrollBar(int val);
    void updateDWTVerticalScrollBar(int val);
};

#endif // MAINWINDOW_H
