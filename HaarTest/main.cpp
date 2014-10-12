#include <QImage>
#include <stdio.h>
#include <iostream>

int main()
{
    QImage t("lenna");
    int i, j, k;
    QRgb tmp, tmp2;
    int width = t.width();
    int height = t.height();

    for(k = 0; k < height; k++) // Pour chaque ligne de l'image
    {
        for(j = width; j > 1; j /= 2)
        {
            tmp2 = t.pixel(0,k);
            for(i = 0; i < j; i += 2)
            {
                tmp = tmp2;
                t.setPixel(i/2, k, tmp/2 + t.pixel(i+1,k)/2);
                tmp2 = t.pixel((i+j)/2,k);
                if(tmp < t.pixel(i/2,k)) t.setPixel((i+j)/2, k, 0);
                else t.setPixel((i+j)/2, k, tmp - t.pixel(i/2,k));
            }
        }
    }

    t.save("lines.jpg", 0); // On sauvegarde le résultat

    for(k = 0; k < width; k++) // Pour chaque colonne de l'image
    {
        for(j = height; j > 1; j /= 2)
        {
            tmp2 = t.pixel(k,0);
            for(i = 0; i < j; i += 2)
            {
                tmp = tmp2;
                t.setPixel(k, i/2, tmp/2 + t.pixel(k,i+1)/2);
                tmp2 = t.pixel(k,(i+j)/2);
                if(tmp < t.pixel(k,i/2)) t.setPixel(k,(i+j)/2, 0);
                else t.setPixel(k, (i+j)/2, tmp - t.pixel(k,i/2));
            }
        }
    }

    t.save("final.jpg", 0); // On sauvegarde le résultat

    return 0;
}
