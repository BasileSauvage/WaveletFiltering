#include "workspace.h"
#include <iostream>
using namespace std;

int main()
{
    QImage img("lenna"); // Image sur laquelle on travaille
    WorkSpace *ws = WorkSpace::getInstance(img);
    unsigned int nb_it = 0;

    cout << "Combien d'itérations?" << endl;
    cin >> nb_it;

    for(unsigned int i = 1; i <= nb_it; i++)
    {
        ws->waveletsTransform(ws->getMatrix(i-1), i);
        ws->saveImage(ws->getMatrix(i), i);
    }

    return 0;
}

// TODO : Convertir en float
// TODO : Affichage résultat semblable à celui de TER
