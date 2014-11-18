#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

// Voir si on peut zoomer en scrollant avec la souris
// Raccourcis clavier
// (Facultatif) Plugin Doxygen
// (Facultatif) Plugin Tiff
