#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

// *Synchroniser niveau d'analyse entre les 2 DWT (penser à un booléen)
// *QSpinBox pour ondelettes (marche mais moche)
// *Créer un zoom à gauche
// *Niveau de zoom négatif
// *Splitter code ondelettes en sous-fonctions
// Voir si on peut affecter l'algo de zoom aux images 1024x1024 ou plus pour des zooms négatifs
// QCheckBox pour rendre indépendant les zooms et déplacements de scrollbars gauche et à droite
// Changer taille fenêtres en 512x512
// Voir si on peut zoomer en scrollant avec la souris
// (Facultatif)Raccourcis clavier
// (Facultatif) Plugin Doxygen
// (Facultatif) Plugin Tiff
