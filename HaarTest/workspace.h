#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QImage>

class WorkSpace
{

public:
    WorkSpace(QImage img);
    ~WorkSpace();
    static WorkSpace* getInstance(QImage img);
    void waveletsTransform(unsigned int iteration);
    void waveletsReverseTransform(int** mat);
    void zeroFilter(int** mat);
    void saveImage(int** mat);

    QImage getSourceImage();
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned int getNbIteration();
    int** getHaarMatrix();
    int** getFilterMatrix();
    int** getSynthesisMatrix();

private:
    QImage source;
    unsigned int nb_iteration;
    int** haar_matrix;
    int** filter_matrix;
    int** synthesis_matrix;
    static WorkSpace* instance; // singleton
};

#endif // WORKSPACE_H
