#include "workspace.h"
#include <cmath>

WorkSpace::WorkSpace(QImage img)
{
    this->source = img;
    unsigned int width = img.width();
    unsigned int height = img.height();
    int** mat; // Passer en float

    mat = new int* [width];
    for(unsigned int i = 0; i < width; i++)
    {
        mat[i] = new int [height];
    }

    for(unsigned int j = 0; j < height; j++) // On remplit la matrice d'entiers M avec les niveaux de gris de l'image
    {
        for(unsigned int i = 0; i < width; i++)
        {
            mat[i][j] = qGray(img.pixel(i,j));
            //mat[i][j] = qGray(t.color(img.pixelIndex(i,j)));
        }
    }

    matrix_list.push_back(mat);
}

WorkSpace::~WorkSpace() {
    for(int i = 0; i < matrix_list.length(); i++)
    {
        for(unsigned int j = 0 ; j < this->getWidth() ; j++)
        {
            delete matrix_list.at(i)[j];
        }

        delete matrix_list.at(i);
    }
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

void WorkSpace::waveletsTransform(int** input_mat, int iteration)
{
    /* On crée une nouvelle matrice */
    int** mat;

    mat = new int* [this->getWidth()];
    for(unsigned int i = 0; i < this->getWidth(); i++)
    {
        mat[i] = new int [this->getHeight()];
    }

    for(unsigned int j = 0; j < this->getHeight(); j++)
    {
        for(unsigned int i = 0; i < this->getWidth(); i++)
        {
            mat[i][j] = input_mat[i][j];
        }
    }

    /* Selon l'itération, on ne parcourt pas l'intégralité de l'image */
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

    /* On stocke cette matrice pour une utilisation ultérieure */
    this->matrix_list.push_back(mat);
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
    img.save("iteration" + QString::number(iteration) + ".jpg", 0);
}

QImage WorkSpace::getSourceImage()
{
    return this->source;
}

unsigned int WorkSpace::getWidth()
{
    return this->getSourceImage().width();
}

unsigned int WorkSpace::getHeight()
{
    return this->getSourceImage().height();
}

int** WorkSpace::getMatrix(unsigned int i)
{
    return this->matrix_list.at(i);
}
