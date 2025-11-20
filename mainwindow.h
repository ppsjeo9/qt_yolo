#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QRect>
#include <QFutureWatcher>
#include <array>
#include <memory>
#include <vector>
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
    static constexpr int kImageSlots = 2;
    struct DetectionJobResult {
        int slotIndex{-1};
        QImage paintedImage;
        std::vector<Detection> detections;
        QString errorMessage;
    };

    Ui::MainWindow *ui;
    QString m_modelPath;
    bool m_modelLoaded{false};
    std::array<QString, kImageSlots> m_imagePaths;
    std::array<QImage, kImageSlots> m_inputImages;
    std::array<QImage, kImageSlots> m_outputImages;
    std::array<std::vector<Detection>, kImageSlots> m_latestDetections;
    std::array<std::unique_ptr<Inference>, kImageSlots> m_inferenceWorkers;
    std::array<QFutureWatcher<DetectionJobResult>, kImageSlots> m_detectionWatchers;
    int m_pendingDetections{0};

    void updateInputPreview(int index);
    void updateOutputPreview(int index);
    void updateReportTable();
    void selectImage(int index);
    DetectionJobResult runDetectionJob(int index, const QImage &image);
    void handleDetectionFinished(int index);
    void initializeInferenceWorkers();
    QString imageDisplayName(int index) const;
    // QImage与cv::Mat之间的转换函数
    static QImage cvMatToQImage(const cv::Mat& mat);
    static cv::Mat QImageToCvMat(const QImage& image);
private slots:
    void onSelectImage1();
    void onSelectImage2();
    void onStartDetection();
};
#endif // MAINWINDOW_H
