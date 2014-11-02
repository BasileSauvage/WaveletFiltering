#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QImage>

class WorkSpace
{

public:
    WorkSpace(QImage img);
    ~WorkSpace();
    static void newInstance(QImage img);
    static WorkSpace* getInstance();
    void waveletsTransform(unsigned int iteration);
    void waveletsReverseTransform(float** mat);
    void zeroFilter(float** mat);
    void saveImage(QImage img, QString fileName);

    QImage getSourceImage();
    QImage getImageFromMatrix(float** mat);
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned int getNbIteration();
    float** getHaarMatrix();
    float** getFilterMatrix();
    float** getSynthesisMatrix();

private:
    QImage source;
    unsigned int nb_iteration;
    float** haar_matrix;
    float** filter_matrix;
    float** synthesis_matrix;
    static WorkSpace* instance; // singleton
};

#endif // WORKSPACE_H
