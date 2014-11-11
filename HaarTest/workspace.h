#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QImage>

struct block
{
    unsigned int top_left_x;
    unsigned int top_left_y;
    unsigned int bottom_right_x;
    unsigned int bottom_right_y;
};

enum block_choice{DIAGONAL, HORIZONTAL, VERTICAL, LOWRES};

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
    void swap();
    void setSelectedBlock(block_choice choice, unsigned int iteration);
    QImage zoomEditor(struct block zoom_block, float** mat);

    QImage getSourceImage();
    QImage getImageFromMatrix(float** mat);
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned int getNbIteration();
    float** getHaarMatrix();
    float** getFilterMatrix();
    float** getSynthesisMatrix();
    struct block getSelectedBlock();

private:
    QImage source;
    unsigned int nb_iteration;
    float** haar_matrix;
    float** filter_matrix;
    float** synthesis_matrix;
    struct block selected_block;
    static WorkSpace* instance; // singleton
};

#endif // WORKSPACE_H
