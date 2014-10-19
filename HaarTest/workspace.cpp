#include "workspace.h"
#include <cmath>

WorkSpace::WorkSpace(QImage img)
{
    this->source = img;
    this->width = img.width();
    this->height = img.height();
}

WorkSpace::~WorkSpace() {
    delete WorkSpace::instance;
}

WorkSpace* WorkSpace::getInstance(QImage img)
{
    if(WorkSpace::instance == 0)
    {
        WorkSpace::instance = new WorkSpace(img);
    }

    return WorkSpace::instance;
}

WorkSpace* WorkSpace::instance = 0;

void WorkSpace::waveletsTransform(int** mat, int iteration)
{
    unsigned int width = this->getWidth() / pow(2, iteration-1);
    unsigned int height = this->getHeight() / pow(2, iteration-1);

    /* Le vecteur de calcul aura la taille de la plus grande valeur entre width et height pour éviter d'allouer 2 vecteurs */
    int vec[(width > height) ? width : height]; // Passer en float

    /* Opérations sur les lignes */
    for(unsigned int j = 0; j < height; j++)
    {
        for(unsigned int i = 0; i < width/2 ; i++)
        {
            vec[i] = (mat[2*i][j] + mat[2*i+1][j])/2;
            vec[i+width/2] = (mat[2*i][j] - mat[2*i+1][j])/2;
        }
        for(unsigned int i = 0; i < width; i++)
        {
            mat[i][j] = vec[i];
        }
    }

    /* Opérations sur les colonnes */
    for(unsigned int i = 0; i < width; i++)
    {
        for(unsigned int j = 0; j < height/2 ; j++)
        {
            vec[j] = (mat[i][2*j] + mat[i][2*j+1])/2;
            vec[j+height/2] = (mat[i][2*j] - mat[i][2*j+1])/2;
        }
        for(unsigned int j = 0; j < height; j++)
        {
            mat[i][j] = vec[j];
        }
    }
}

void WorkSpace::saveImage(int** mat, int iteration)
{
    unsigned int width = this->getWidth();
    unsigned int height = this->getHeight();

    QImage img;
    img = this->getSourceImage();

    /* On modifie l'image à partir de la matrice d'entrée */
    for(unsigned int j = 0; j < height; j++)
    {
        for(unsigned int i = 0; i < width; i++)
        {
            if(mat[i][j] < 0) img.setPixel(i, j, -mat[i][j]);
            else img.setPixel(i, j, mat[i][j]);
        }
    }

    /* Et on la sauvegarde */
    img.save("final" + QString::number(iteration) + ".jpg", 0);
}

unsigned int WorkSpace::getWidth()
{
    return this->width;
}

unsigned int WorkSpace::getHeight()
{
    return this->height;
}

QImage WorkSpace::getSourceImage()
{
    return this->source;
}
