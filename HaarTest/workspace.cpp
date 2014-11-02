#include "workspace.h"
#include <cmath>

/**
  * @brief Constructeur
  * @param img image source sur laquelle nous allons travailler
  */
WorkSpace::WorkSpace(QImage img)
{
    this->source = img.convertToFormat(QImage::Format_ARGB32);
    this->nb_iteration = 0;

    this->haar_matrix = new float* [img.width()];
    this->filter_matrix = new float* [img.width()];
    this->synthesis_matrix = new float* [img.width()];

    for(int i = 0; i < img.width(); i++)
    {
        haar_matrix[i] = new float [img.height()];
        filter_matrix[i] = new float [img.height()];
        synthesis_matrix[i] = new float [img.height()];
    }
}

/**
  * @brief Destructeur
  */
WorkSpace::~WorkSpace()
{
    for(unsigned int j = 0 ; j < this->getWidth() ; j++)
    {
        delete haar_matrix[j];
        delete filter_matrix[j];
        delete synthesis_matrix[j];
    }

    delete haar_matrix;
    delete filter_matrix;
    delete synthesis_matrix;

    delete WorkSpace::instance;
}

/**
 * @brief Création d'une instance unique de la classe
 * @param img image source sur laquelle nous allons travailler
 */
void WorkSpace::newInstance(QImage img)
{
    WorkSpace::instance = new WorkSpace(img);
}

/**
 * @brief Récupération de l'instance unique de la classe
 * @return singleton
 */
WorkSpace* WorkSpace::getInstance()
{
    if(WorkSpace::instance == 0)
    {
        QImage img;
        WorkSpace::instance = new WorkSpace(img);
    }

    return WorkSpace::instance;
}

/**
 * @brief Initialisation de l'instance
 */
WorkSpace* WorkSpace::instance = 0;

/**
 * @brief Transformation en ondelettes discrète d'une image (base de Haar)
 * @param iteration nombre de fois où l'on souhaite effectuer la transformation
 */
void WorkSpace::waveletsTransform(unsigned int iteration)
{
    this->nb_iteration = iteration;

    unsigned int height = this->getHeight();
    unsigned int width = this->getWidth();

    for(unsigned int j = 0; j < height; j++) // On remplit la matrice avec les niveaux de gris de l'image source
    {
        for(unsigned int i = 0; i < width; i++)
        {
            //this->haar_matrix[i][j] = this->getSourceImage().pixel(i,j);
            this->haar_matrix[i][j] = qGray(this->getSourceImage().pixel(i,j));
            //this->haar_matrix[i][j] = qGray(this->getSourceImage().color(this->getSourceImage().pixelIndex(i,j)));
        }
    }
    for(unsigned int it = 1; it <= iteration; it++, width /= 2, height /= 2)
    {
        /* Le vecteur de calcul aura la taille de la plus grande valeur entre width et height pour éviter d'allouer 2 vecteurs */
        float vec[(width > height) ? width : height]; // Passer en float

        /* Opérations sur les lignes */
        for(unsigned int j = 0; j < height; j++)
        {
            for(unsigned int i = 0; i < width/2 ; i++)
            {
                vec[i] = (this->haar_matrix[2*i][j] + this->haar_matrix[2*i+1][j])/2;
                vec[i+width/2] = (this->haar_matrix[2*i][j] - this->haar_matrix[2*i+1][j])/2;
            }
            for(unsigned int i = 0; i < width; i++)
            {
                this->haar_matrix[i][j] = vec[i];
            }
        }

        /* Opérations sur les colonnes */
        for(unsigned int i = 0; i < width; i++)
        {
            for(unsigned int j = 0; j < height/2 ; j++)
            {
                vec[j] = (this->haar_matrix[i][2*j] + this->haar_matrix[i][2*j+1])/2;
                vec[j+height/2] = (this->haar_matrix[i][2*j] - this->haar_matrix[i][2*j+1])/2;
            }
            for(unsigned int j = 0; j < height; j++)
            {
                this->haar_matrix[i][j] = vec[j];
            }
        }
    }
}

/**
 * @brief Transformation en ondelettes discrète inverse d'une image
 * @param mat la matrice de coefficients sur laquelle appliquer la transformation
 */
void WorkSpace::waveletsReverseTransform(float** mat)
{
    unsigned int height = this->getHeight()/pow(2, this->getNbIteration()-1);
    unsigned int width = this->getWidth()/pow(2, this->getNbIteration()-1);

    for(unsigned int i = 0; i < this->getWidth(); i++)
    {
        for(unsigned int j = 0; j < this->getHeight(); j++)
        {
            synthesis_matrix[i][j] = mat[i][j];
        }
    }

    for(unsigned int it = this->getNbIteration(); it >= 1; it--, width *= 2, height *= 2)
    {
        /* Le vecteur de calcul aura la taille de la plus grande valeur entre width et height pour éviter d'allouer 2 vecteurs */
        float vec[(width > height) ? width : height]; // Passer en float

        /* Opérations sur les colonnes */
        for(unsigned int i = 0; i < width; i++)
        {
            for(unsigned int j = 0; j < height/2 ; j++)
            {
                vec[2*j] = this->synthesis_matrix[i][j] + this->synthesis_matrix[i][j+height/2];
                vec[2*j+1] = this->synthesis_matrix[i][j] - this->synthesis_matrix[i][j+height/2];
            }
            for(unsigned int j = 0; j < height; j++)
            {
                this->synthesis_matrix[i][j] = vec[j];
            }
        }

        /* Opérations sur les lignes */
        for(unsigned int j = 0; j < height; j++)
        {
            for(unsigned int i = 0; i < width/2 ; i++)
            {
                vec[2*i] = this->synthesis_matrix[i][j] + this->synthesis_matrix[i+width/2][j];
                vec[2*i+1] = this->synthesis_matrix[i][j] - this->synthesis_matrix[i+width/2][j];
            }
            for(unsigned int i = 0; i < width; i++)
            {
                this->synthesis_matrix[i][j] = vec[i];
            }
        }
    }
}

/**
 * @brief Transforme une matrice de coefficients en remplaçant ceux générés lors du dernier niveau d'analyse par 0
 * @param mat la matrice de coefficients sur laquelle appliquer la transformation
 */
void WorkSpace::zeroFilter(float** mat)
{
    for(unsigned int i = 0; i < this->getWidth(); i++)
    {
        for(unsigned int j = 0; j < this->getHeight(); j++)
        {
            filter_matrix[i][j] = mat[i][j];
        }
    }

    for(unsigned int i = 0; i < this->getWidth()/pow(2,this->getNbIteration()-1); i++)
    {
        for(unsigned int j = 0; j < this->getHeight()/pow(2,this->getNbIteration()-1); j++)
        {
            if(i < this->getWidth()/pow(2,this->getNbIteration()) && j < this->getHeight()/pow(2,this->getNbIteration()))
            {
                continue;
            }
            else
            {
                filter_matrix[i][j] = 0;
            }
        }
    }
}

/**
 * @brief Sauvegarde une image dans le répertoire courant
 * @param img l'image à sauvegarder
 * @param fileName le nom de l'image
 */
void WorkSpace::saveImage(QImage img, QString fileName)
{
    img.save(fileName, 0);
}

/**
 * @brief Transforme une matrice de coefficients en image, les coefficients hors des bornes des niveaux de gris sont modifiés
 * @param mat la matrice de coefficients à convertir en image
 * @return matrice convertie
 */
QImage WorkSpace::getImageFromMatrix(float** mat)
{
    QImage img(this->getWidth(), this->getHeight(), QImage::Format_ARGB32);

    for(unsigned int j = 0; j < this->getHeight(); j++)
    {
        for(unsigned int i = 0; i < this->getWidth(); i++)
            if(mat[i][j] < 0)
            {
                img.setPixel(i, j, qRgb(-mat[i][j], -mat[i][j], -mat[i][j]));
            }
            else if(mat[i][j] > 255)
            {
                img.setPixel(i, j, qRgb(mat[i][j]-255, mat[i][j]-255, mat[i][j]-255));
            }
            else
            {
                img.setPixel(i, j, qRgb(mat[i][j], mat[i][j], mat[i][j]));
            }
    }

    return img;
}

/**
 * @brief Renvoie l'image source
 * @return image source
 */
QImage WorkSpace::getSourceImage()
{
    return this->source;
}

/**
 * @brief Renvoie la largeur de l'image source
 * @return largeur de l'image source
 */
unsigned int WorkSpace::getWidth()
{
    return this->getSourceImage().width();
}

/**
 * @brief Renvoie la hauteur de l'image source
 * @return hauteur de l'image source
 */
unsigned int WorkSpace::getHeight()
{
    return this->getSourceImage().height();
}

/**
 * @brief Renvoie le niveau d'analyse actuel
 * @return nombre de fois où on a appliqué la transformation en ondelettes discrète
 */
unsigned int WorkSpace::getNbIteration()
{
    return this->nb_iteration;
}

/**
 * @brief Renvoie la matrice de coefficients d'ondelettes
 * @return matrice de coefficients de Haar
 */
float** WorkSpace::getHaarMatrix()
{
    return this->haar_matrix;
}

/**
 * @brief Renvoie la matrice résultant de l'application d'un filtre
 * @return matrice de coefficients après application d'un filtre
 */
float** WorkSpace::getFilterMatrix()
{
    return this->filter_matrix;
}

/**
 * @brief Renvoie la matrice de coefficients après transformation en ondelettes inverse
 * @return matrice de coefficients après synthèse
 */
float** WorkSpace::getSynthesisMatrix()
{
    return this->synthesis_matrix;
}
