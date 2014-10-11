#include <QImage>
#include <stdio.h>
#include <iostream>

int main()
{
    QImage s("lenna");
    QImage t("lenna");
    std::cout << qGray(s.pixel(0,0)) << std::endl;

    int i, j, k;
    QRgb tmp, tmp2;
    int width = s.width();
    int height = s.height();

    for(k = 0; k < height; k++) // Pour chaque ligne de l'image
    {
        for(j = width; j > 1; j /= 2)
        {
            if(j == width) // Première itération
            {
                for(i = 0; i < j; i += 2)
                {
                    t.setPixel(k, i/2, (s.pixel(k,i) + s.pixel(k,i+1))/2);
                    t.setPixel(k, (i+j)/2, s.pixel(k,i) - t.pixel(k,i/2));
                }
            }
            else // Sinon pour chaque autre itération
            {
                tmp2 = t.pixel(k,0);
                for(i = 0; i < j; i += 2)
                {
                    tmp = tmp2;
                    t.setPixel(k, i/2, (tmp + t.pixel(k,i+1))/2);
                    tmp2 = t.pixel(k,(i+j)/2);
                    t.setPixel(k, (i+j)/2, tmp - t.pixel(k,i/2));
                }
            }
        }
    }

    t.save("lines.jpg", 0); // On sauvegarde le résultat

    return 0;
}
