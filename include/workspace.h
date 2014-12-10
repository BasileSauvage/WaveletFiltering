#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QImage>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

struct block
{
    unsigned int top_left_x;
    unsigned int top_left_y;
    unsigned int bottom_right_x;
    unsigned int bottom_right_y;
};

enum block_choice{DIAGONAL, HORIZONTAL, VERTICAL, LOWRES};

/**
  * @class WorkSpace
  * @brief Structure de données
  * @author Julien Vogel
  * @version 1.0
  * @date 19 octobre 2014
  */
class WorkSpace
{

public:
    static void newInstance(QImage img);
    static WorkSpace* getInstance();
    void copyMatrix(float** src, float** dest);
    void waveletTransform(unsigned int target_level);
    void waveletAnalysis(float** mat, unsigned int width, unsigned int height);
    void waveletSynthesis(float** mat, unsigned int width, unsigned int height);
    void updateOutputFineFromDWT();
    void filterVanishCoarseDetails();
    void filterRandomCoarseDetails();
    void saveImage(QImage img, QString fileName);
    void swap();
    void setSelectedBlock(block_choice choice, unsigned int analysis_level);
    void setSelectedZoomedBlock(block_choice choice, unsigned int analysis_level,block fineZoomedBlock);
//    QImage zoomEditor(struct block zoom_block, float** mat);

    QImage getSourceImage();
    QImage getInputDWTImage();
    QImage getOutputDWTImage();
    QImage getOutputFineImage();
    QImage getImageFromFineMatrix(float** mat);
    QImage getImageFromDWTMatrix(float** mat);
    QImage getZoomedImageFromDWTMatrix(float** mat,struct block zoom_block);
    void setImageFromMatrix_SC_in_block(float** mat,QImage& img, int shift_x=0, int shift_y=0);
    void setImageFromMatrix_WC_in_block(float** mat,QImage& img, int shift_x=0, int shift_y=0);
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned int getCurrentAnalysisLevel();
    unsigned int getMaxAnalysisLevel();
    float** getInputFineMatrix();
    float** getInputDWTMatrix();
    float** getOutputDWTMatrix();
    float** getOutputFineMatrix();
    struct block getSelectedBlock();

private:
    WorkSpace(QImage img); // constructeur privé à cause du singleton
    ~WorkSpace();
    QImage source;
    QImage input_DWT_img;
    QImage output_DWT_img;
    QImage output_fine_img;
    unsigned int current_analysis_level;
    float** input_fine_matrix;
    float** input_DWT_matrix;
    float** output_DWT_matrix;
    float** output_fine_matrix;
    struct block selected_block;
    static WorkSpace* instance; // singleton
};

#endif // WORKSPACE_H