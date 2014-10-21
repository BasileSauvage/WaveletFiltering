#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QImage>

class WorkSpace
{

public:
    WorkSpace(QImage img);
    ~WorkSpace();
    static WorkSpace* getInstance(QImage img);
    void waveletsTransform(int** input_mat, int iteration);
    void saveImage(int** mat, int iteration);
    QImage getSourceImage();
    unsigned int getWidth();
    unsigned int getHeight();
    int** getMatrix(unsigned int i);

private:
    QImage source;
    QList<int**> matrix_list;
    static WorkSpace* instance; // singleton
};

#endif // WORKSPACE_H
