#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

// TODO : Remonter les 2 images du bas (on les remplace)
// TODO : Slider pour les transformées en ondelettes, plutôt que pop-up
// TODO : Regarder si on peut éviter trop de compression lors de la sauvegarde
// TODO : S'intéresser aux différents formats d'image

// Priorité zoom : Voir schéma
// Prio à ces 2 choses : Zoom et récupération de sous-arbre
