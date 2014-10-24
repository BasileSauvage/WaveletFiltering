#include "workspace.h"
#include "mainwindow.h"
#include <QApplication>
#include <iostream>

using namespace std;

//int main(int argc, char* argv[])
int main()
{
    //QApplication a(argc, argv);

    QImage img("lenna"); // Image sur laquelle on travaille
    WorkSpace *ws = WorkSpace::getInstance(img);
    unsigned int nb_it;

    cout << "Combien d'itérations?" << endl;
    cin >> nb_it;

    ws->waveletsTransform(nb_it);
    ws->zeroFilter(ws->getHaarMatrix());
    ws->waveletsReverseTransform(ws->getFilterMatrix());
    ws->saveImage(ws->getSynthesisMatrix());

    //MainWindow w(img, ws->getMatrix(nb_it));
    //w.show();

    //return a.exec();
    return 0;
}

// TODO : Convertir en float
// TODO : Affichage résultat semblable à celui de TER
// TODO : Réfléchir au zoom (algo)

// hautes fréquences en image = variations importantes
// on devrait avoir des blocs sur la synthèse
