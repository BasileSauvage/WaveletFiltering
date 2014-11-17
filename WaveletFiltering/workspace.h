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
    static void newInstance(QImage img);
    static WorkSpace* getInstance();
    void copyMatrix(float** src, float** dest);
    void waveletTransform(bool apply_to_output, unsigned int target_level);
    void waveletAnalysis(float** mat, unsigned int width, unsigned int height);
    void waveletSynthesis(float** mat, unsigned int width, unsigned int height);
    void updateOutputFineFromDWT();
    void zeroFilter();
    void saveImage(QImage img, QString fileName);
    void swap();
    void setSelectedBlock(block_choice choice, unsigned int analysis_level);
    QImage zoomEditor(struct block zoom_block, float** mat);

    QImage getSourceImage();
    QImage getImageFromMatrix(float** mat);
    QImage getImageFromMatrix(float** mat, unsigned int mat_width, unsigned int mat_height);
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned int getCurrentAnalysisLevel();
    float** getInputFineMatrix();
    float** getInputDWTMatrix();
    float** getOutputDWTMatrix();
    float** getOutputFineMatrix();
    struct block getSelectedBlock();

private:
    WorkSpace(QImage img); // constructeur privé à cause du singleton
    ~WorkSpace();
    QImage source;
    unsigned int current_analysis_level;
    float** input_fine_matrix;
    float** input_DWT_matrix;
    float** output_DWT_matrix;
    float** output_fine_matrix;
    struct block selected_block;
    static WorkSpace* instance; // singleton
};

#endif // WORKSPACE_H
