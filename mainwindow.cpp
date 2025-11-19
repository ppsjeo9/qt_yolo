#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QPixmap>
#include <QPainter>
#include <QTableWidgetItem>
#include <QPen>
#include <QMessageBox>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_inference(nullptr)
{
    ui->setupUi(this);
    ui->tableReport->setColumnCount(6);
    ui->tableReport->setHorizontalHeaderLabels({"类别","置信度","X","Y","W","H"});
    connect(ui->btnBrowse, &QPushButton::clicked, this, &MainWindow::onSelectImage);
    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onStartDetection);
    
    // 初始化YOLO推理对象
    QString modelPath = QDir::currentPath() + "/yolo11n.onnx";
    if (!QFile::exists(modelPath)) {
        QMessageBox::warning(this, "警告", "未找到模型文件: " + modelPath);
        return;
    }
    
    try {
        m_inference = new Inference(modelPath.toStdString(), cv::Size(640, 640), "", false);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("加载模型失败: %1").arg(e.what()));
        m_inference = nullptr;
    }
}

MainWindow::~MainWindow()
{
    if (m_inference) {
        delete m_inference;
    }
    delete ui;
}

void MainWindow::updateInputPreview()
{
    if (m_inputImage.isNull()) {
        ui->labelInput->setText("无图片");
        return;
    }
    QSize target = ui->labelInput->size();
    QPixmap px = QPixmap::fromImage(m_inputImage.scaled(target, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->labelInput->setPixmap(px);
}

void MainWindow::updateOutputPreview()
{
    if (m_outputImage.isNull()) {
        ui->labelOutput->setText("等待检测");
        return;
    }
    QSize target = ui->labelOutput->size();
    QPixmap px = QPixmap::fromImage(m_outputImage.scaled(target, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->labelOutput->setPixmap(px);
}

void MainWindow::onSelectImage()
{
    QString path = QFileDialog::getOpenFileName(this, "选择图片", QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
    if (path.isEmpty()) return;
    m_imagePath = path;
    ui->lineEditImagePath->setText(path);
    QImage img(path);
    if (img.isNull()) return;
    m_inputImage = img;
    updateInputPreview();
}

// QImage转cv::Mat
cv::Mat MainWindow::QImageToCvMat(const QImage& image)
{
    cv::Mat mat;
    switch (image.format()) {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            // QImage的ARGB32实际上是BGRA格式存储
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
            break;
        }
        case QImage::Format_RGB888: {
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
            break;
        }
        case QImage::Format_Grayscale8: {
            mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            break;
        }
        default: {
            QImage converted = image.convertToFormat(QImage::Format_RGB888);
            mat = cv::Mat(converted.height(), converted.width(), CV_8UC3, (void*)converted.constBits(), converted.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
            break;
        }
    }
    return mat.clone();
}

// cv::Mat转QImage
QImage MainWindow::cvMatToQImage(const cv::Mat& mat)
{
    switch (mat.type()) {
        case CV_8UC4: {
            // BGR转RGB
            cv::Mat rgb;
            cv::cvtColor(mat, rgb, cv::COLOR_BGRA2RGBA);
            QImage image(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_ARGB32);
            return image.copy();
        }
        case CV_8UC3: {
            // BGR转RGB
            cv::Mat rgb;
            cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
            QImage image(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
            return image.copy();
        }
        case CV_8UC1: {
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
            return image.copy();
        }
        default:
            return QImage();
    }
}

void MainWindow::onStartDetection()
{
    if (m_inputImage.isNull()) {
        QMessageBox::warning(this, "警告", "请先选择图片！");
        return;
    }
    
    if (!m_inference) {
        QMessageBox::warning(this, "警告", "YOLO模型未加载！");
        return;
    }
    
    // 将QImage转换为cv::Mat
    cv::Mat inputMat = QImageToCvMat(m_inputImage);
    if (inputMat.empty()) {
        QMessageBox::warning(this, "错误", "图片转换失败！");
        return;
    }
    
    // 运行YOLO推理
    std::vector<Detection> detections = m_inference->runInference(inputMat);
    
    // 在图像上绘制检测结果
    cv::Mat outputMat = inputMat.clone();
    
    for (const auto& det : detections) {
        // 绘制边界框
        cv::rectangle(outputMat, det.box, det.color, 2);
        
        // 准备标签文本
        std::string label = det.className + " " + std::to_string(det.confidence).substr(0, 4);
        
        // 计算文本大小
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
        
        // 绘制标签背景
        cv::rectangle(outputMat,
                     cv::Point(det.box.x, det.box.y - textSize.height - 5),
                     cv::Point(det.box.x + textSize.width, det.box.y),
                     det.color, -1);
        
        // 绘制标签文本
        cv::putText(outputMat, label,
                   cv::Point(det.box.x, det.box.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    }
    
    // 将cv::Mat转换回QImage
    m_outputImage = cvMatToQImage(outputMat);
    
    // 更新检测报告表格
    ui->tableReport->setRowCount(detections.size());
    for (size_t i = 0; i < detections.size(); ++i) {
        const auto& det = detections[i];
        ui->tableReport->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(det.className)));
        ui->tableReport->setItem(i, 1, new QTableWidgetItem(QString::number(det.confidence, 'f', 2)));
        ui->tableReport->setItem(i, 2, new QTableWidgetItem(QString::number(det.box.x)));
        ui->tableReport->setItem(i, 3, new QTableWidgetItem(QString::number(det.box.y)));
        ui->tableReport->setItem(i, 4, new QTableWidgetItem(QString::number(det.box.width)));
        ui->tableReport->setItem(i, 5, new QTableWidgetItem(QString::number(det.box.height)));
    }
    
    updateOutputPreview();
}
