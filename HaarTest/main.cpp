#include <QImage>
#include <stdio.h>
#include <iostream>

int main()
{
    QImage t("lenna"); // Image sur laquelle on travaille
    int i, j, k; // Itérateurs
    //QGray tmp, tmp2; // Pour les calculs
    int tmp, tmp2;
    int gray; // Pour la conversion
    int width = t.width();
    int height = t.height();

    /*for(int i=0;i<256;++i)
        t.setColor(i, qRgb(i,i,i));*/

    for(k = 0; k < height; k++) // Pour chaque ligne de l'image
    {
        for(j = width; j > 1; j /= 2)
        {
            tmp2 = qGray(t.pixel(0, k));
            for(i = 0; i < j; i += 2)
            {
                tmp = tmp2;

                gray = tmp/2 + qGray(t.pixel(i+1, k))/2;
                //std::cout << i/2 << " " << k << " :" << std::endl;
                t.setPixel(i/2, k, gray); // Coefficient d'approximation
                //std::cout << tmp << " " << qGray(t.pixel(i+1, k)) << " moyenne " << qGray(t.pixel(i/2, k)) << std::endl;

                tmp2 = qGray(t.pixel((i+j)/2, k));
                //std::cout << (i+j)/2 << " " << k << " :" << std::endl;

                if(tmp < qGray(t.pixel(i/2, k))) // Cas où le résultat est négatif
                {
                    t.setPixel((i+j)/2, k, 0); // On ramène à 0
                    //std::cout << tmp << " " << qGray(t.pixel(i/2, k)) << " difference " << qGray(t.pixel((i+j)/2, k)) << std::endl;
                }
                else // Cas général
                {
                    gray = tmp - qGray(t.pixel(i/2, k));
                    t.setPixel((i+j)/2, k, gray); // Coefficient de détail
                    //std::cout << tmp << " " << qGray(t.pixel(i/2, k)) << " difference " << qGray(t.pixel((i+j)/2, k)) << std::endl;
                }
            }
        }
    }

    t.save("lines.jpg", 0);

    for(k = 0; k < width; k++) // Pour chaque colonne de l'image
    {
        for(j = height; j > 1; j /= 2)
        {
            tmp2 = qGray(t.pixel(k, 0));
            for(i = 0; i < j; i += 2)
            {
                tmp = tmp2;

                gray = tmp/2 + qGray(t.pixel(k, i+1))/2;
                //std::cout << k << " " << i/2 << " :" << std::endl;
                t.setPixel(k, i/2, gray); // Coefficient d'approximation
                //std::cout << tmp << " " << qGray(t.pixel(k, i+1)) << " moyenne " << qGray(t.pixel(k, i/2)) << std::endl;

                tmp2 = qGray(t.pixel(k, (i+j)/2));
                //std::cout << k << " " << (i+j)/2 << " :" << std::endl;

                if(tmp < qGray(t.pixel(k, i/2))) // Cas où le résultat est négatif
                {
                    t.setPixel(k, (i+j)/2, 0); // On ramène à 0
                    //std::cout << tmp << " " << qGray(t.pixel(k, i/2)) << " difference " << qGray(t.pixel(k, (i+j)/2)) << std::endl;
                }
                else // Cas général
                {
                    gray = tmp - qGray(t.pixel(k, i/2));
                    t.setPixel(k, (i+j)/2, gray); // Coefficient de détail
                    //std::cout << tmp << " " << qGray(t.pixel(k, i/2)) << " difference " << qGray(t.pixel(k, (i+j)/2)) << std::endl;
                }
            }
        }
    }

    t.save("final.jpg", 0); // On sauvegarde le résultat*/

    return 0;
}
