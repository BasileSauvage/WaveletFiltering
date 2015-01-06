#include "workspace.h"
#include <cmath>

/**
  * @brief Constructeur
  * @param img image source sur laquelle nous allons travailler
  */
WorkSpace::WorkSpace(QImage img)
{
	this->input_fine_img = img.convertToFormat(QImage::Format_ARGB32);
    this->current_analysis_level = 0;

    this->input_fine_matrix = new float* [img.width()];
    this->input_DWT_matrix = new float* [img.width()];
    this->output_DWT_matrix = new float* [img.width()];
    this->output_fine_matrix = new float* [img.width()];

    this->selected_block.top_left_x = 0;
    this->selected_block.top_left_y = 0;
    this->selected_block.bottom_right_x = img.width()-1;
    this->selected_block.bottom_right_y = img.height()-1;

    for(int i = 0; i < img.width(); i++)
    {
        input_fine_matrix[i] = new float [img.height()];
        input_DWT_matrix[i] = new float [img.height()];
        output_DWT_matrix[i] = new float [img.height()];
        output_fine_matrix[i] = new float [img.height()];
    }

    for(int j = 0; j < img.height(); j++)
    {
        for(int i = 0; i < img.width(); i++)
        {
            input_fine_matrix[i][j] = qGray(img.pixel(i,j));
            input_DWT_matrix[i][j] = qGray(img.pixel(i,j));
            output_DWT_matrix[i][j] = qGray(img.pixel(i,j));
            output_fine_matrix[i][j] = qGray(img.pixel(i,j));
        }
    }

    this->input_DWT_img = this->getImageFromDWTMatrix(this->getInputDWTMatrix());
    this->output_DWT_img = this->getImageFromDWTMatrix(this->getOutputDWTMatrix());
    this->output_fine_img = this->getImageFromFineMatrix(this->getOutputFineMatrix());
}

/**
  * @brief Destructeur
  */
WorkSpace::~WorkSpace()
{
    for(unsigned int j = 0 ; j < this->getWidth() ; j++)
    {
        delete input_fine_matrix[j];
        delete input_DWT_matrix[j];
        delete output_DWT_matrix[j];
        delete output_fine_matrix[j];
    }

    delete input_fine_matrix;
    delete input_DWT_matrix;
    delete output_DWT_matrix;
    delete output_fine_matrix;

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
 * @brief Copie le contenu d'une matrice dans une autre
 * @param src matrice source
 * @param dest matrice destination
 */
void WorkSpace::copyMatrix(float **src, float **dest)
{
    for(unsigned int j = 0; j < this->getHeight(); j++) // On remplit la matrice avec les niveaux de gris de l'image source
    {
        for(unsigned int i = 0; i < this->getWidth(); i++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

/**
 * @brief Applique une transformée en ondelettes ou son inverse
 * @param apply_to_output le booléen indiquant si on modifie également output_DWT_matrix ou non
 * @param target_level le niveau d'analyse à atteindre
 */
void WorkSpace::waveletTransform(unsigned int target_level)
{
    if(this->current_analysis_level < target_level)
    {        
        unsigned int height = this->getHeight()/pow(2, current_analysis_level);
        unsigned int width = this->getWidth()/pow(2, current_analysis_level);
        for(unsigned int lvl = current_analysis_level; lvl < target_level; lvl++, height/=2, width/=2)
        {
            this->waveletAnalysis(this->getInputDWTMatrix(), width, height);
            this->waveletAnalysis(this->getOutputDWTMatrix(), width, height);
        }
    }
    else
    {
        unsigned int height = this->getHeight()/pow(2, current_analysis_level - 1);
        unsigned int width = this->getWidth()/pow(2, current_analysis_level - 1);
        for(unsigned int lvl = target_level; lvl < current_analysis_level; lvl++, height*=2, width*=2)
        {
            this->waveletSynthesis(this->getInputDWTMatrix(), width, height);
            this->waveletSynthesis(this->getOutputDWTMatrix(), width, height);
        }
    }

    this->current_analysis_level = target_level;
    this->input_DWT_img = this->getImageFromDWTMatrix(this->getInputDWTMatrix());
    this->output_DWT_img = this->getImageFromDWTMatrix(this->getOutputDWTMatrix());
}

/**
 * @brief Applique un niveau d'analyse (DWT) sur une matrice donnée
 * @param mat la matrice de coefficients sur laquelle appliquer la transformation
 * @param width le nombre de colonnes à traiter
 * @param height le nombre de lignes à traiter
 */
void WorkSpace::waveletAnalysis(float **mat, unsigned int width, unsigned int height)
{
    /* Le vecteur de calcul aura la taille de la plus grande valeur entre width et height pour éviter d'allouer 2 vecteurs */
    float vec[(width > height) ? width : height];

    /* Opérations sur les lignes */
    for(unsigned int j = 0; j < height; j++)
    {
        for(unsigned int i = 0; i < width/2 ; i++)
        {
            vec[i] = (mat[2*i][j] + mat[2*i+1][j])/2.0f;
            vec[i+width/2] = (mat[2*i][j] - mat[2*i+1][j])/2.0f;
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
            vec[j] = (mat[i][2*j] + mat[i][2*j+1])/2.0f;
            vec[j+height/2] = (mat[i][2*j] - mat[i][2*j+1])/2.0f;
        }
        for(unsigned int j = 0; j < height; j++)
        {
            mat[i][j] = vec[j];
        }
    }
}

/**
 * @brief Applique un niveau de synthèse (DWT inverse) sur une matrice donnée
 * @param mat la matrice de coefficients sur laquelle appliquer la transformation
 * @param width le nombre de colonnes à traiter
 * @param height le nombre de lignes à traiter
 */
void WorkSpace::waveletSynthesis(float **mat, unsigned int width, unsigned int height)
{
    /* Le vecteur de calcul aura la taille de la plus grande valeur entre width et height pour éviter d'allouer 2 vecteurs */
    float vec[(width > height) ? width : height];

    /* Opérations sur les colonnes */
    for(unsigned int i = 0; i < width; i++)
    {
        for(unsigned int j = 0; j < height/2 ; j++)
        {
            vec[2*j] = mat[i][j] + mat[i][j+height/2];
            vec[2*j+1] = mat[i][j] - mat[i][j+height/2];
        }
        for(unsigned int j = 0; j < height; j++)
        {
            mat[i][j] = vec[j];
        }
    }

    /* Opérations sur les lignes */
    for(unsigned int j = 0; j < height; j++)
    {
        for(unsigned int i = 0; i < width/2 ; i++)
        {
            vec[2*i] = mat[i][j] + mat[i+width/2][j];
            vec[2*i+1] = mat[i][j] - mat[i+width/2][j];
        }
        for(unsigned int i = 0; i < width; i++)
        {
            mat[i][j] = vec[i];
        }
    }
}

/**
 * @brief met à 0 les coefficients de détail du niveau le plus grossier
 */
void WorkSpace::filterVanishCoarseDetails()
{
    this->setSelectedBlock(DIAGONAL, this->getCurrentAnalysisLevel());
    for(unsigned int i = getSelectedBlock().top_left_x; i < getSelectedBlock().bottom_right_x + 1; i++)
    {
        for(unsigned int j = getSelectedBlock().top_left_y; j < getSelectedBlock().bottom_right_y + 1; j++)
        {
            this->output_DWT_matrix[i][j] = 0;
        }
    }
    this->setSelectedBlock(HORIZONTAL, this->getCurrentAnalysisLevel());
    for(unsigned int i = getSelectedBlock().top_left_x; i < getSelectedBlock().bottom_right_x + 1; i++)
    {
        for(unsigned int j = getSelectedBlock().top_left_y; j < getSelectedBlock().bottom_right_y + 1; j++)
        {
            this->output_DWT_matrix[i][j] = 0;
        }
    }
    this->setSelectedBlock(VERTICAL, this->getCurrentAnalysisLevel());
    for(unsigned int i = getSelectedBlock().top_left_x; i < getSelectedBlock().bottom_right_x + 1; i++)
    {
        for(unsigned int j = getSelectedBlock().top_left_y; j < getSelectedBlock().bottom_right_y + 1; j++)
        {
            this->output_DWT_matrix[i][j] = 0;
        }
    }

    this->output_DWT_img = this->getImageFromDWTMatrix(this->getOutputDWTMatrix());
}

/**
 * @brief met des valeurs aléatoires aux coefficients de détail du niveau le plus grossier
 */
void WorkSpace::filterRandomCoarseDetails()
{
    srand (time(NULL));

    this->setSelectedBlock(DIAGONAL, this->getCurrentAnalysisLevel());
    for(unsigned int i = getSelectedBlock().top_left_x; i < getSelectedBlock().bottom_right_x + 1; i++)
    {
        for(unsigned int j = getSelectedBlock().top_left_y; j < getSelectedBlock().bottom_right_y + 1; j++)
        {
            this->output_DWT_matrix[i][j] = (rand() % 256) - 127.5;
        }
    }
    this->setSelectedBlock(HORIZONTAL, this->getCurrentAnalysisLevel());
    for(unsigned int i = getSelectedBlock().top_left_x; i < getSelectedBlock().bottom_right_x + 1; i++)
    {
        for(unsigned int j = getSelectedBlock().top_left_y; j < getSelectedBlock().bottom_right_y + 1; j++)
        {
            this->output_DWT_matrix[i][j] = (rand() % 256) - 127.5;
        }
    }
    this->setSelectedBlock(VERTICAL, this->getCurrentAnalysisLevel());
    for(unsigned int i = getSelectedBlock().top_left_x; i < getSelectedBlock().bottom_right_x + 1; i++)
    {
        for(unsigned int j = getSelectedBlock().top_left_y; j < getSelectedBlock().bottom_right_y + 1; j++)
        {
            this->output_DWT_matrix[i][j] = (rand() % 256) - 127.5;
        }
    }

    this->output_DWT_img = this->getImageFromDWTMatrix(this->getOutputDWTMatrix());
}

/**
 * @brief Génère l'image synthèse (output_fine_matrix)
 */
void WorkSpace::updateOutputFineFromDWT()
{
    this->copyMatrix(this->getOutputDWTMatrix(), this->getOutputFineMatrix());

    unsigned int height = this->getHeight()/pow(2, this->getCurrentAnalysisLevel()-1);
    unsigned int width = this->getWidth()/pow(2, this->getCurrentAnalysisLevel()-1);

    for(int lvl = this->getCurrentAnalysisLevel(); lvl >= 1; lvl--, height*=2, width*=2)
    {
        this->waveletSynthesis(this->getOutputFineMatrix(), width, height);
    }

    this->output_fine_img = this->getImageFromDWTMatrix(this->getOutputFineMatrix());
}

/**
 * @brief Sauvegarde une image
 * @param img l'image à sauvegarder
 * @param fileName le nom de l'image
 */
void WorkSpace::saveImage(QImage img, QString fileName)
{
    img.save(fileName, 0, 100);
}

/**
 * @brief Transforme une matrice de coefficients fins en QImage
 * @param mat la matrice de coefficients à convertir
 * @return QImage
 */
QImage WorkSpace::getImageFromFineMatrix(float** mat)
{
    QImage img(this->getWidth(), this->getHeight(), QImage::Format_ARGB32);
    this->setSelectedBlock(LOWRES,0);
    this->setImageFromMatrix_SC_in_block(mat,img);
    return img;
}

/**
 * @brief Transforme une matrice de DWT en QImage
 * @param mat la matrice de coefficients à convertir
 * @return QImage
 */
QImage WorkSpace::getImageFromDWTMatrix(float** mat)
{
    QImage img(this->getWidth(), this->getHeight(), QImage::Format_ARGB32);
    this->setSelectedBlock(LOWRES,0);
    this->setImageFromMatrix_WC_in_block(mat,img);
    this->setSelectedBlock(LOWRES,this->getCurrentAnalysisLevel());
    this->setImageFromMatrix_SC_in_block(mat,img);
    return img;
}

/**
 * @brief Transforme une matrice de DWT en QImage, en accord avec le zoom sur l'image fine
 * @param mat la matrice de coefficients à convertir
 * @param zoom_block le bloc visible au niveau fin
 * @return QImage
 */

QImage WorkSpace::getZoomedImageFromDWTMatrix(float** mat,struct block zoom_block)
{
    unsigned int bw = 1 + zoom_block.bottom_right_x - zoom_block.top_left_x;
    unsigned int bh = 1 + zoom_block.bottom_right_y - zoom_block.top_left_y;
    QImage img(bw, bh, QImage::Format_ARGB32);

    // coefs d'ondelette
    for(unsigned int l=1;  l <= getCurrentAnalysisLevel(); ++l)
    {
        bw /= 2;
        bh /= 2;

        this->setSelectedZoomedBlock(DIAGONAL, l, zoom_block);
        this->setImageFromMatrix_WC_in_block(mat,img, bw-(int)this->getSelectedBlock().top_left_x, bh-(int)this->getSelectedBlock().top_left_y);

        this->setSelectedZoomedBlock(HORIZONTAL, l, zoom_block);
        this->setImageFromMatrix_WC_in_block(mat,img, -(int)this->getSelectedBlock().top_left_x, bh-(int)this->getSelectedBlock().top_left_y);

        this->setSelectedZoomedBlock(VERTICAL, l, zoom_block);
        this->setImageFromMatrix_WC_in_block(mat,img, bw-(int)this->getSelectedBlock().top_left_x, -(int)this->getSelectedBlock().top_left_y);
    }

    // coefs d'echelle
    this->setSelectedZoomedBlock(LOWRES, this->getCurrentAnalysisLevel(), zoom_block);
    this->setImageFromMatrix_SC_in_block(mat,img, -(int)this->getSelectedBlock().top_left_x, -(int)this->getSelectedBlock().top_left_y);

    // retour
    return img;
}

/**
 * @brief traduit le bloc sélectionné d'une matrice de coefficients d'échelle en QImage (simple conversion de réel vers niveau de gris)
 * @param mat la matrice de coefficients à convertir
 * @param img la QImage destination
 * @param shift_x décallage du bloc destination sur l'axe x
 * @param shift_y décallage du bloc destination sur l'axe y
 */
void WorkSpace::setImageFromMatrix_SC_in_block(float** mat,QImage& img, int shift_x, int shift_y)
{

    for(unsigned int i = getSelectedBlock().top_left_x; i <= getSelectedBlock().bottom_right_x; i++)
    {
        for(unsigned int j = getSelectedBlock().top_left_y; j <= getSelectedBlock().bottom_right_y; j++)
        {
            int v = mat[i][j];
            img.setPixel(i+shift_x, j+shift_y, qRgb(v,v,v));
        }
    }
}

/**
 * @brief traduit le bloc sélectionné d'une matrice de coefficients d'ondelette en QImage (simple conversion de réel vers niveau de gris)
 * @param mat la matrice de coefficients à convertir
 * @param img la QImage destination
 * @param shift_x décallage du bloc destination sur l'axe x
 * @param shift_y décallage du bloc destination sur l'axe y
 */
void WorkSpace::setImageFromMatrix_WC_in_block(float** mat,QImage& img, int shift_x, int shift_y){

    for(unsigned int i = getSelectedBlock().top_left_x; i <= getSelectedBlock().bottom_right_x; i++)
    {
        for(unsigned int j = getSelectedBlock().top_left_y; j <= getSelectedBlock().bottom_right_y; j++)
        {
            //            int v = std::fabs(mat[i][j]); // abs value, 0 is black, +/-255 is white
            //            int v = (mat[i][j]+255.f)/2.f; // -255 is black, 0 is grey,  +255 is white
            //            img.setPixel(i, j, qRgb(v,v,v));
            int r = std::max<float>(0,2.0f*mat[i][j]);
            int b = std::max<float>(0,-2.0f*mat[i][j]);
            img.setPixel(i+shift_x, j+shift_y, qRgb(r,0,b));
        }
    }
}

/**
 * @brief Réinitialise le WorkSpace en partant de l'image résultat
 */
void WorkSpace::swap()
{
    for(unsigned int i = 0; i < this->getWidth(); i++)
    {
        for(unsigned int j = 0; j < this->getHeight(); j++)
        {
            input_fine_matrix[i][j] = output_fine_matrix[i][j];
            input_DWT_matrix[i][j] = output_fine_matrix[i][j];
            output_DWT_matrix[i][j] = output_fine_matrix[i][j];
        }
    }
	this->input_fine_img = this->getImageFromFineMatrix(this->getInputFineMatrix());
    this->input_DWT_img = this->getImageFromDWTMatrix(this->getInputDWTMatrix());
    this->output_DWT_img = this->getImageFromDWTMatrix(this->getOutputDWTMatrix());

    this->current_analysis_level = 0;

    this->selected_block.top_left_x = 0;
    this->selected_block.top_left_y = 0;
    this->selected_block.bottom_right_x = this->getWidth()-1;
    this->selected_block.bottom_right_y = this->getHeight()-1;
}

/**
 * @brief calule le bloc sélectionné
 * @param choice le bloc désiré (LOWRES,DIAGONAL,VERTICAL, ou HORIZONTAL)
 * @param analysis_level le niveau d'analyse désiré
 */
void WorkSpace::setSelectedBlock(block_choice choice, unsigned int analysis_level)
{
    switch(choice)
    {
        case LOWRES:
            this->selected_block.top_left_x = 0;
            this->selected_block.top_left_y = 0;
            this->selected_block.bottom_right_x = this->getWidth()/pow(2, analysis_level)-1;
            this->selected_block.bottom_right_y = this->getHeight()/pow(2, analysis_level)-1;
            break;
        case DIAGONAL:
            this->selected_block.top_left_x = this->getWidth()/pow(2, analysis_level);
            this->selected_block.top_left_y = this->getHeight()/pow(2, analysis_level);
            this->selected_block.bottom_right_x = this->getWidth()/pow(2, analysis_level-1)-1;
            this->selected_block.bottom_right_y = this->getHeight()/pow(2, analysis_level-1)-1;
            break;
        case VERTICAL:
            this->selected_block.top_left_x = this->getWidth()/pow(2, analysis_level);
            this->selected_block.top_left_y = 0;
            this->selected_block.bottom_right_x = this->getWidth()/pow(2, analysis_level-1)-1;
            this->selected_block.bottom_right_y = this->getHeight()/pow(2, analysis_level)-1;
            break;
        case HORIZONTAL:
            this->selected_block.top_left_x = 0;
            this->selected_block.top_left_y = this->getHeight()/pow(2, analysis_level);
            this->selected_block.bottom_right_x = this->getWidth()/pow(2, analysis_level)-1;
            this->selected_block.bottom_right_y = this->getHeight()/pow(2, analysis_level-1)-1;
            break;
        default:
            break;
    }
}

/**
 * @brief calule le bloc sélectionné
 * @param choice le bloc désiré (LOWRES,DIAGONAL,VERTICAL, ou HORIZONTAL)
 * @param analysis_level le niveau d'analyse désiré
 * @param fineZoomedBlock le bloc zoomé au niveau fin
 */
void WorkSpace::setSelectedZoomedBlock(block_choice choice, unsigned int analysis_level,block fineZoomedBlock)
{
    unsigned int tlx = fineZoomedBlock.top_left_x;
    unsigned int tly = fineZoomedBlock.top_left_y;
    unsigned int bw = 1 + fineZoomedBlock.bottom_right_x - fineZoomedBlock.top_left_x;
    unsigned int bh = 1 + fineZoomedBlock.bottom_right_y - fineZoomedBlock.top_left_y;

    unsigned int factor = pow(2, analysis_level);
    tlx /= factor;
    tly /= factor;
    bw /= factor;
    bh /= factor;

    switch(choice)
    {
        case LOWRES:
            this->selected_block.top_left_x = tlx;
            this->selected_block.top_left_y = tly;
            break;
        case DIAGONAL:
            this->selected_block.top_left_x = tlx + this->getWidth()/pow(2, analysis_level);
            this->selected_block.top_left_y = tly + this->getHeight()/pow(2, analysis_level);
            break;
        case VERTICAL:
            this->selected_block.top_left_x = tlx + this->getWidth()/pow(2, analysis_level);
            this->selected_block.top_left_y = tly;
            break;
        case HORIZONTAL:
            this->selected_block.top_left_x = tlx;
            this->selected_block.top_left_y = tly + this->getHeight()/pow(2, analysis_level);
            break;
        default:
            break;
    }
    this->selected_block.bottom_right_x = this->selected_block.top_left_x + bw - 1;
    this->selected_block.bottom_right_y = this->selected_block.top_left_y + bh - 1;
}






/*
 * @brief Va zoomer sur une sous-partie de chaque bloc
 * @param zoom_block le bloc de l'image initiale sur lequel on veut zoomer
 * @return l'image calculée
 */
/* QImage WorkSpace::zoomEditor(struct block zoom_block, float** mat)
{
    // Etape 1 : allocation matrice de résultat
    unsigned int block_height = 1 + zoom_block.bottom_right_y - zoom_block.top_left_y;
    unsigned int block_width = 1 + zoom_block.bottom_right_x - zoom_block.top_left_x;

    float** result = new float* [block_width];

    for(unsigned int i = 0; i < block_width; i++)
    {
        result[i] = new float [block_height];
    }

    // Etape 2 : calcul du lowres
    this->setSelectedBlock(LOWRES, this->getCurrentAnalysisLevel());
    unsigned int bh = block_height/pow(2, this->getCurrentAnalysisLevel());
    unsigned int bw = block_width/pow(2, this->getCurrentAnalysisLevel());
    unsigned int tlx = zoom_block.top_left_x/(unsigned int)pow(2, this->getCurrentAnalysisLevel());
    unsigned int tly = zoom_block.top_left_y/(unsigned int)pow(2, this->getCurrentAnalysisLevel());
    for(unsigned int i = 0; i < bh; i++)
    {
        for(unsigned int j = 0; j < bw; j++)
        {
            result[i][j] = mat[i + this->getSelectedBlock().top_left_x + tlx][j + this->getSelectedBlock().top_left_y + tly];
        }
    }

    // Etape 3 : calcul des coefs d'ondelette
    for(unsigned int it = this->getCurrentAnalysisLevel(); it > 0; it--)
    {
        unsigned int bh = block_height/(unsigned int)pow(2, it);
        unsigned int bw = block_width/(unsigned int)pow(2, it);
        unsigned int tlx = zoom_block.top_left_x/(unsigned int)pow(2, it);
        unsigned int tly = zoom_block.top_left_y/(unsigned int)pow(2, it);

        this->setSelectedBlock(DIAGONAL, it);
        for(unsigned int i = 0; i < bh; i++)
        {
            for(unsigned int j = 0; j < bw; j++)
            {
                result[i + bw][j + bh] = mat[i + this->getSelectedBlock().top_left_x + tlx][j + this->getSelectedBlock().top_left_y + tly];
            }
        }

        this->setSelectedBlock(HORIZONTAL, it);
        for(unsigned int i = 0; i < bh; i++)
        {
            for(unsigned int j = 0; j < bw; j++)
            {
                result[i][j + bh] = mat[i + this->getSelectedBlock().top_left_x + tlx][j + this->getSelectedBlock().top_left_y + tly];
            }
        }

        this->setSelectedBlock(VERTICAL, it);
        for(unsigned int i = 0; i < bh; i++)
        {
            for(unsigned int j = 0; j < bw; j++)
            {
                result[i + bw][j] = mat[i + this->getSelectedBlock().top_left_x + tlx][j + this->getSelectedBlock().top_left_y + tly];
            }
        }
    }

    QImage img_result = this->getImageFromDWTMatrix(result);//, block_width, block_height);

    for(unsigned int j = 0; j < block_width; j++)
    {
        delete result[j];
    }

    delete result;

    return img_result;
}
*/

/**
 * @brief Renvoie l'image source
 * @return image source
 */
QImage WorkSpace::getInputFineImage()
{
	return this->input_fine_img;
}

/**
 * @brief Renvoie l'image de la transformée en ondelettes de la source
 * @return image DWT input
 */
QImage WorkSpace::getInputDWTImage()
{
    return this->input_DWT_img;
}

/**
 * @brief Renvoie l'image de la transformée en ondelettes de la sortie
 * @return image DWT output
 */
QImage WorkSpace::getOutputDWTImage()
{
    return this->output_DWT_img;
}

/**
 * @brief Renvoie l'image de sortie
 * @return image sortie
 */
QImage WorkSpace::getOutputFineImage()
{
    return this->output_fine_img;
}

/**
 * @brief Renvoie la largeur de l'image source
 * @return largeur de l'image source
 */
unsigned int WorkSpace::getWidth()
{
	return this->getInputFineImage().width();
}

/**
 * @brief Renvoie la hauteur de l'image source
 * @return hauteur de l'image source
 */
unsigned int WorkSpace::getHeight()
{
	return this->getInputFineImage().height();
}

/**
 * @brief Renvoie le niveau d'analyse actuel
 * @return nombre de fois où on a appliqué la transformation en ondelettes discrète
 */
unsigned int WorkSpace::getCurrentAnalysisLevel()
{
    return this->current_analysis_level;
}

/**
 * @brief calcule le niveau d'analyse maximal (puissance de 2 maximale qui divise les deux tailles)
 * @return niveau d'analyse max
 */
unsigned int WorkSpace::getMaxAnalysisLevel()
{
    unsigned int l = 0, w = this->getWidth(), h = this->getHeight();

    while ((w%2==0) && (h%2==0))
    {
        w/=2;
        h/=2;
        ++l;
    }

    return l;
}

/**
 * @brief Renvoie la matrice de coefficients de l'image de départ
 * @return matrice de coefficients initiale
 */
float** WorkSpace::getInputFineMatrix()
{
    return this->input_fine_matrix;
}

/**
 * @brief Renvoie la matrice de coefficients d'ondelettes
 * @return matrice de coefficients de Haar
 */
float** WorkSpace::getInputDWTMatrix()
{
    return this->input_DWT_matrix;
}

/**
 * @brief Renvoie la matrice résultant de l'application d'un filtre
 * @return matrice de coefficients après application d'un filtre
 */
float** WorkSpace::getOutputDWTMatrix()
{
    return this->output_DWT_matrix;
}

/**
 * @brief Renvoie la matrice de coefficients après transformation en ondelettes inverse
 * @return matrice de coefficients après synthèse
 */
float** WorkSpace::getOutputFineMatrix()
{
    return this->output_fine_matrix;
}

/**
 * @brief Renvoie le bloc sélectionné dans l'image
 * @return bloc sélectionné
 */
struct block WorkSpace::getSelectedBlock()
{
    return this->selected_block;
}
