#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QImage img, int** mat, QWidget *parent = 0);
    QGraphicsView * source_view;
    QGraphicsView * haar_view;

    QGraphicsScene * source_scene;
    QGraphicsScene * haar_scene;

signals:

public slots:

};

#endif // MAINWINDOW_H
