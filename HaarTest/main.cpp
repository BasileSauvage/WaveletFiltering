#include <QImage>

void waveletsTransform(int** mat, int width, int height)
{
    /* Le vecteur de calcul aura la taille de la plus grande valeur entre width et height pour éviter d'allouer 2 vecteurs */
    int vec[(width > height) ? width : height]; // Passer en float

    /* Opérations sur les lignes */
    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width/2 ; i++)
        {
            vec[i] = (mat[2*i][j] + mat[2*i+1][j])/2;
            vec[i+width/2] = (mat[2*i][j] - mat[2*i+1][j])/2;
        }
        for(int i = 0; i < width; i++)
        {
            mat[i][j] = vec[i];
        }
    }

    /* Opérations sur les colonnes */
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height/2 ; j++)
        {
            vec[j] = (mat[i][2*j] + mat[i][2*j+1])/2;
            vec[j+height/2] = (mat[i][2*j] - mat[i][2*j+1])/2;
        }
        for(int j = 0; j < height; j++)
        {
            mat[i][j] = vec[j];
        }
    }
}

void saveImage(int** mat, QImage img, int iteration)
{

    /* On modifie l'image à partir de la matrice d'entrée */
    for(int j = 0; j < img.height(); j++)
    {
        for(int i = 0; i < img.width(); i++)
        {
            if(mat[i][j] < 0) img.setPixel(i, j, -mat[i][j]);
            else img.setPixel(i, j, mat[i][j]);
        }
    }

    /* Et on la sauvegarde */
    img.save("final" + QString::number(iteration) + ".jpg", 0);
}

int main()
{
    QImage img("lenna"); // Image sur laquelle on travaille
    int width = img.width();
    int height = img.height();
    int** mat; // Passer en float

    mat = (int**) malloc(sizeof(int*)*width);
    for(int i = 0; i < width; i++)
    {
        mat[i] = (int*) malloc(sizeof(int)*height);
    }

    for(int j = 0; j < height; j++) // On remplit la matrice d'entiers M avec les niveaux de gris de l'image
    {
        for(int i = 0; i < width; i++)
        {
            mat[i][j] = qGray(img.pixel(i,j));
            //mat[i][j] = qGray(t.color(img.pixelIndex(i,j)));
        }
    }

    for(int i = 1; i < 4; i++, width/=2, height/=2)
    {
        waveletsTransform(mat, width, height);
        saveImage(mat, img, i);
    }

    free(mat);

    return 0;
}

// TODO : Convertir en float
// TODO : Affichage résultat semblable à celui de TER
