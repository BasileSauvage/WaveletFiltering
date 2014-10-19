#include "workspace.h"

int main()
{
    QImage img("lenna"); // Image sur laquelle on travaille
    WorkSpace *ws = WorkSpace::getInstance(img);
    unsigned int width = img.width();
    unsigned int height = img.height();
    int** mat; // Passer en float

    mat = (int**) malloc(sizeof(int*)*width);
    for(unsigned int i = 0; i < width; i++)
    {
        mat[i] = (int*) malloc(sizeof(int)*height);
    }

    for(unsigned int j = 0; j < height; j++) // On remplit la matrice d'entiers M avec les niveaux de gris de l'image
    {
        for(unsigned int i = 0; i < width; i++)
        {
            mat[i][j] = qGray(img.pixel(i,j));
            //mat[i][j] = qGray(t.color(img.pixelIndex(i,j)));
        }
    }

    for(int i = 1; i < 4; i++)
    {
        ws->waveletsTransform(mat, i);
        ws->saveImage(mat, i);
    }

    free(mat);

    return 0;
}

// TODO : Convertir en float
// TODO : Affichage résultat semblable à celui de TER
