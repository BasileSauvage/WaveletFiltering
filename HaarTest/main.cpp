#include "workspace.h"
#include "mainwindow.h"
#include <QApplication>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QImage img("lenna"); // Image sur laquelle on travaille
    WorkSpace *ws = WorkSpace::getInstance(img);
    //unsigned int nb_it;

    //cout << "Combien d'itérations?" << endl;
    //cin >> nb_it;

    ws->waveletsTransform(3);
    ws->zeroFilter(ws->getHaarMatrix());
    ws->waveletsReverseTransform(ws->getFilterMatrix());
    //ws->saveImage(ws->getHaarMatrix());

    MainWindow w(img);
    w.show();

    return a.exec();
}

// TODO : Réfléchir au zoom (algo)
// TODO : UI -> Charger une image / Pouvoir changer le niveau d'analyse dans l'UI / Diviser code en fonctions
