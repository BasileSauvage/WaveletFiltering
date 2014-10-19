#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QImage>

class WorkSpace
{

public:
    WorkSpace(QImage img);
    ~WorkSpace();
    static WorkSpace* getInstance(QImage img);
    void waveletsTransform(int** mat, int iteration);
    void saveImage(int** mat, int iteration);
    unsigned int getWidth();
    unsigned int getHeight();
    QImage getSourceImage();

private:
    unsigned int width;
    unsigned int height;
    QImage source;
    static WorkSpace* instance; // singleton
};

#endif // WORKSPACE_H
