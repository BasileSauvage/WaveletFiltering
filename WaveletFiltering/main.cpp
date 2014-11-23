/**
  * @file main.cpp
  * @brief Programme permettant d'étudier la transformée en ondelettes et le résultat de filtres sur diverses images
  * @author Julien Vogel
  * @version 1.0
  * @date 11 octobre 2014
  */

#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Fonction main du programme
 * @param argc nombre d'arguments
 * @param argv liste d'arguments
 * @return l'application
 */
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
