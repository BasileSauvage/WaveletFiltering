#include <QImage>
#include <QDebug>

int main()
{
    QImage t("lenna"); // Image sur laquelle on travaille
    int i, j; // Itérateurs
    int width = t.width();
    int height = t.height();
    int V[width];
    int M[width][height];
    int w;

    for(j = 0; j < height; j++) // On remplit la matrice d'entiers M avec les niveaux de gris de l'image
    {
        for(i = 0; i < width; i++)
        {
            M[i][j] = qGray(t.pixel(i,j));
            //M[i][j] = qGray(t.color(t.pixelIndex(i,j)));
        }
    }

    for(j = 0; j < height; j++) // Opérations sur les lignes
    {
        w = width;
        //do
        //{
            for(i = 0; i < w/2 ; i++)
            {
                V[i] = (M[2*i][j] + M[2*i+1][j])/2;
                V[i+w/2] = (M[2*i][j] - M[2*i+1][j])/2;
            }
            for(i = 0; i < width; i++)
            {
                M[i][j] = V[i];
            }
            //w = w/2;
        //} while(w != 1);
    }

    for(i = 0; i < width; i++) // Opérations sur les colonnes
    {
        w = height;
        //do
        //{
            for(j = 0; j < w/2 ; j++)
            {
                V[j] = (M[i][2*j] + M[i][2*j+1])/2;
                V[j+w/2] = (M[i][2*j] - M[i][2*j+1])/2;
            }
            for(j = 0; j < height; j++)
            {
                M[i][j] = V[j];
            }
            //w = w/2;
        //} while(w != 1);
    }

    for(j = 0; j < height; j++) // On enlève les valeurs négatives
    {
        for(i = 0; i < width; i++)
        {
            if(M[i][j] < 0)
                M[i][j] = 0;
        }
    }

    for(j = 0; j < height; j++) // On modifie l'image de départ avec notre matrice M transformée
    {
        for(i = 0; i < width; i++)
        {
            t.setPixel(i, j, M[i][j]);
        }
    }

    t.save("final.jpg", 0); // Et on la sauvegarde

    return 0;
}
