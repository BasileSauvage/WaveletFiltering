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

    this->selected_block.top_left_x = 0;
    this->selected_block.top_left_y = 0;
    this->selected_block.bottom_right_x = img.width()-1;
    this->selected_block.bottom_right_y = img.height()-1;

    for(int i = 0; i < img.width(); i++)
    {
        haar_matrix[i] = new float [img.height()];
        filter_matrix[i] = new float [img.height()];
        synthesis_matrix[i] = new float [img.height()];
    }

    for(int j = 0; j < img.height(); j++)
    {
        for(int i = 0; i < img.width(); i++)
        {
            haar_matrix[i][j] = 0;
            filter_matrix[i][j] = 0;
            synthesis_matrix[i][j] = 0;
        }
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

    for(unsigned int i = 0; i < this->getWidth()/pow(2, this->getNbIteration()-1); i++)
    {
        for(unsigned int j = 0; j < this->getHeight()/pow(2, this->getNbIteration()-1); j++)
        {
            if(i < this->getWidth()/pow(2, this->getNbIteration()) && j < this->getHeight()/pow(2, this->getNbIteration()))
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
    img.save(fileName, 0, 100);
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
 * @brief Réinitialise le WorkSpace en partant de l'image résultat
 */
void WorkSpace::swap()
{
    this->source = this->getImageFromMatrix(this->synthesis_matrix);
    for(unsigned int i = 0; i < this->getWidth(); i++)
    {
        for(unsigned int j = 0; j < this->getHeight(); j++)
        {
            haar_matrix[i][j] = 0;
            filter_matrix[i][j] = 0;
            synthesis_matrix[i][j] = 0;
        }
    }

    this->nb_iteration = 0;

    this->selected_block.top_left_x = 0;
    this->selected_block.top_left_y = 0;
    this->selected_block.bottom_right_x = this->getWidth()-1;
    this->selected_block.bottom_right_y = this->getHeight()-1;
}

/**
 * @brief Remplit la structure contenant le bloc sélectionné
 * @param choice le bloc désiré
 * @param iteration le niveau d'analyse désiré
 */
void WorkSpace::setSelectedBlock(block_choice choice, unsigned int iteration)
{
    switch(choice)
    {
        case LOWRES:
            this->selected_block.top_left_x = 0;
            this->selected_block.top_left_y = 0;
            this->selected_block.bottom_right_x = this->getWidth()/pow(2, iteration)-1;
            this->selected_block.bottom_right_y = this->getHeight()/pow(2, iteration)-1;
            break;
        case DIAGONAL:
            this->selected_block.top_left_x = this->getWidth()/pow(2, iteration);
            this->selected_block.top_left_y = this->getHeight()/pow(2, iteration);
            this->selected_block.bottom_right_x = this->getWidth()/pow(2, iteration-1)-1;
            this->selected_block.bottom_right_y = this->getHeight()/pow(2, iteration-1)-1;
            break;
        case VERTICAL:
            this->selected_block.top_left_x = this->getWidth()/pow(2, iteration);
            this->selected_block.top_left_y = 0;
            this->selected_block.bottom_right_x = this->getWidth()/pow(2, iteration-1)-1;
            this->selected_block.bottom_right_y = this->getHeight()/pow(2, iteration)-1;
            break;
        case HORIZONTAL:
            this->selected_block.top_left_x = 0;
            this->selected_block.top_left_y = this->getHeight()/pow(2, iteration);
            this->selected_block.bottom_right_x = this->getWidth()/pow(2, iteration)-1;
            this->selected_block.bottom_right_y = this->getHeight()/pow(2, iteration-1)-1;
            break;
        default:
            break;
    }
}
/**
 * @brief Va zoomer sur une sous-partie de chaque bloc
 * @param zoom_block le bloc de l'image initiale sur lequel on veut zoomer
 * @return l'image calculée
 */
QImage WorkSpace::zoomEditor(struct block zoom_block, float** mat)
{
    // Etape 1
    unsigned int block_height = 1 + zoom_block.bottom_right_y - zoom_block.top_left_y;
    unsigned int block_width = 1 + zoom_block.bottom_right_x - zoom_block.top_left_x;

    float** result = new float* [this->getWidth()];

    for(unsigned int i = 0; i < this->getWidth(); i++)
    {
        result[i] = new float [this->getHeight()];
    }

    for(unsigned int j = 0; j < this->getHeight(); j++)
    {
        for(unsigned int i = 0; i < this->getWidth(); i++)
        {
            result[i][j] = 0;
        }
    }

    // Etape 2
    this->setSelectedBlock(LOWRES, this->getNbIteration());
    for(unsigned int i = 0; i < block_height/pow(2, this->getNbIteration()); i++)
    {
        for(unsigned int j = 0; j < block_width/pow(2, this->getNbIteration()); j++)
        {
            result[i][j] = mat[i + this->getSelectedBlock().top_left_x + zoom_block.top_left_x/(unsigned int)pow(2, this->getNbIteration())][j + this->getSelectedBlock().top_left_y + zoom_block.top_left_y/(unsigned int)pow(2, this->getNbIteration())];
        }
    }
    // Etape 3
    for(unsigned int it = this->getNbIteration(); it > 0; it--)
    {
        this->setSelectedBlock(DIAGONAL, it);
        for(unsigned int i = 0; i < block_height/pow(2, it); i++)
        {
            for(unsigned int j = 0; j < block_width/pow(2, it); j++)
            {
                result[i + block_width/(unsigned int)pow(2, it)][j + block_height/(unsigned int)pow(2, it)] = mat[i + this->getSelectedBlock().top_left_x + zoom_block.top_left_x/(unsigned int)pow(2, it)][j + this->getSelectedBlock().top_left_y + zoom_block.top_left_y/(unsigned int)pow(2, it)];
            }
        }

        this->setSelectedBlock(HORIZONTAL, it);
        for(unsigned int i = 0; i < block_height/pow(2, it); i++)
        {
            for(unsigned int j = 0; j < block_width/pow(2, it); j++)
            {
                result[i][j + block_height/(unsigned int)pow(2, it)] = mat[i + this->getSelectedBlock().top_left_x + zoom_block.top_left_x/(unsigned int)pow(2, it)][j + this->getSelectedBlock().top_left_y + zoom_block.top_left_y/(unsigned int)pow(2, it)];
            }
        }

        this->setSelectedBlock(VERTICAL, it);
        for(unsigned int i = 0; i < block_height/pow(2, it); i++)
        {
            for(unsigned int j = 0; j < block_width/pow(2, it); j++)
            {
                result[i + block_width/(unsigned int)pow(2, it)][j] = mat[i + this->getSelectedBlock().top_left_x + zoom_block.top_left_x/(unsigned int)pow(2, it)][j + this->getSelectedBlock().top_left_y + zoom_block.top_left_y/(unsigned int)pow(2, it)];
            }
        }
    }

    QImage img_result = this->getImageFromMatrix(result);

    for(unsigned int j = 0 ; j < this->getWidth(); j++)
    {
        delete result[j];
    }

    delete result;

    return img_result;
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

/**
 * @brief Renvoie le bloc sélectionné dans l'image
 * @return bloc sélectionné
 */
struct block WorkSpace::getSelectedBlock()
{
    return this->selected_block;
}
