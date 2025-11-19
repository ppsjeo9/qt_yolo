#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QRect>
#include "inference.h"
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString m_imagePath;
    QImage m_inputImage;
    QImage m_outputImage;
    Inference* m_inference;  // YOLO推理对象
    void updateInputPreview();
    void updateOutputPreview();
    // QImage与cv::Mat之间的转换函数
    static QImage cvMatToQImage(const cv::Mat& mat);
    static cv::Mat QImageToCvMat(const QImage& image);
private slots:
    void onSelectImage();
    void onStartDetection();
};
#endif // MAINWINDOW_H
