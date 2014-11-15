#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

// *Renommer les variables correspondant aux 4 matrices
// *Remplacer toutes les occurences de iteration par analysis_level
// *Renommer projet en WaveletFiltering
// *Créer un niveau d'analyse 0
// *Supprimer QActions pour la sélection de blocs
// *Créer matrice de coefficients pour l'image de départ
// *Changer tout ce qui est en rapport avec le swap
// *Une fonction de sauvegarde pour toutes les images sauf source, une pour synthèse uniquement
// *Supprimer ActionHaar
// *Changer code du filtre zéro en prenant en compte les blocs
// *Quand on loade une image, on calcule directement sa transformée en ondelettes
// *Quand on active un filtre, on calcule directement la synthèse
// *Supprimer ActionReverseHaar
// *Créer fonction de copie de matrice pour alléger le code des autres fonctions
// *Drag & Drop sur l'image pour se déplacer
// Synchroniser niveau d'analyse entre les 2 DWT
// QCheckBox pour rendre indépendant les zooms et déplacements de scroolbars gauche et à droite
// Pas slider mais comme pour Transformée de Haar pour les niveaux d'analyse
// Créer un zoom à gauche
// Niveau de zoom négatif pour les très grandes images, O de base jusqu'à 9 (au lieu de 1 - 10) -> -3 à 9
// Splitter code ondelettes en sous-fonctions
// Changer taille fenêtres en 512x512
// Raccourcis clavier
// Tenter de débugger les pixels bruités
// Voir si on peut zoomer en scrollant avec la souris
// (Facultatif) Plugin Doxygen
// (Facultatif) Plugin Tiff
