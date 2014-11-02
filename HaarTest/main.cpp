#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

// TODO : Réfléchir au zoom (algo)
// TODO : UI -> Mettre chaque affichage dans une fonction différente à appeler
// TODO : UI -> Synchroniser les vues
