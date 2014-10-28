#include "workspace.h"
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
// TODO : UI -> Pouvoir changer le niveau d'analyse dans l'UI / Rendre le tout plus esthétique et ergonomique
