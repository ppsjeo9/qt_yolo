#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QPixmap>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QVector>
#include <QtConcurrent/QtConcurrent>

namespace {
QString defaultInputLabelText(int index)
{
    return index == 0 ? QStringLiteral("无图片1") : QStringLiteral("无图片2");
}

QString defaultOutputLabelText(int index)
{
    return index == 0 ? QStringLiteral("等待检测1") : QStringLiteral("等待检测2");
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableReport->setColumnCount(7);
    ui->tableReport->setHorizontalHeaderLabels({"图片","类别","置信度","X","Y","W","H"});

    connect(ui->btnBrowse1, &QPushButton::clicked, this, &MainWindow::onSelectImage1);
    connect(ui->btnBrowse2, &QPushButton::clicked, this, &MainWindow::onSelectImage2);
    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onStartDetection);

    for (int i = 0; i < kImageSlots; ++i) {
        connect(&m_detectionWatchers[i], &QFutureWatcher<DetectionJobResult>::finished,
                this, [this, i]() { handleDetectionFinished(i); });
        updateInputPreview(i);
        updateOutputPreview(i);
    }

    m_modelPath = QDir::currentPath() + "/yolo11n.onnx";
    initializeInferenceWorkers();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeInferenceWorkers()
{
    if (!QFile::exists(m_modelPath)) {
        QMessageBox::warning(this, "警告", "未找到模型文件: " + m_modelPath);
        m_modelLoaded = false;
        return;
    }

    try {
        for (int i = 0; i < kImageSlots; ++i) {
            m_inferenceWorkers[i] = std::make_unique<Inference>(m_modelPath.toStdString(),
                                                                cv::Size(640, 640),
                                                                "",
                                                                false);
        }
        m_modelLoaded = true;
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "错误", QString("加载模型失败: %1").arg(e.what()));
        m_modelLoaded = false;
    }
}

void MainWindow::updateInputPreview(int index)
{
    QLabel *label = (index == 0) ? ui->labelInput1 : ui->labelInput2;
    label->setPixmap(QPixmap());

    if (m_inputImages[index].isNull()) {
        label->setText(defaultInputLabelText(index));
        return;
    }

    QSize target = label->size();
    QPixmap px = QPixmap::fromImage(m_inputImages[index].scaled(target, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    label->setPixmap(px);
}

void MainWindow::updateOutputPreview(int index)
{
    QLabel *label = (index == 0) ? ui->labelOutput1 : ui->labelOutput2;
    label->setPixmap(QPixmap());

    if (m_outputImages[index].isNull()) {
        label->setText(defaultOutputLabelText(index));
        return;
    }

    QSize target = label->size();
    QPixmap px = QPixmap::fromImage(m_outputImages[index].scaled(target, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    label->setPixmap(px);
}

void MainWindow::selectImage(int index)
{
    QString dialogTitle = index == 0 ? "选择图片1" : "选择图片2";
    QString path = QFileDialog::getOpenFileName(this, dialogTitle, QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
    if (path.isEmpty()) {
        return;
    }

    QImage img(path);
    if (img.isNull()) {
        QMessageBox::warning(this, "错误", "无法加载所选图片。");
        return;
    }

    m_imagePaths[index] = path;
    m_inputImages[index] = img;

    if (index == 0) {
        ui->lineEditImagePath1->setText(path);
    } else {
        ui->lineEditImagePath2->setText(path);
    }

    updateInputPreview(index);
}

void MainWindow::onSelectImage1()
{
    selectImage(0);
}

void MainWindow::onSelectImage2()
{
    selectImage(1);
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

MainWindow::DetectionJobResult MainWindow::runDetectionJob(int index, const QImage &image)
{
    DetectionJobResult result;
    result.slotIndex = index;

    cv::Mat inputMat = QImageToCvMat(image);
    if (inputMat.empty()) {
        result.errorMessage = "图片转换失败";
        return result;
    }

    auto *worker = m_inferenceWorkers[index].get();
    if (!worker) {
        result.errorMessage = "推理器未初始化";
        return result;
    }

    try {
        std::vector<Detection> detections = worker->runInference(inputMat);
        cv::Mat outputMat = inputMat.clone();

        int boxThickness = std::max(2, std::min(outputMat.cols, outputMat.rows) / 200);
        for (const auto& det : detections) {
            cv::rectangle(outputMat, det.box, det.color, boxThickness, cv::LINE_AA);

            std::string label = det.className + " " + QString::number(det.confidence, 'f', 2).toStdString();

            int baseline = 0;
            cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);

            cv::rectangle(outputMat,
                         cv::Point(det.box.x, det.box.y - textSize.height - 5),
                         cv::Point(det.box.x + textSize.width, det.box.y),
                         det.color, cv::FILLED);
            cv::rectangle(outputMat,
                         cv::Point(det.box.x, det.box.y - textSize.height - 5),
                         cv::Point(det.box.x + textSize.width, det.box.y),
                         cv::Scalar(0, 0, 0), 1, cv::LINE_AA);

            cv::putText(outputMat, label,
                       cv::Point(det.box.x, det.box.y - 5),
                       cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        }

        result.paintedImage = cvMatToQImage(outputMat);
        result.detections = std::move(detections);
    } catch (const std::exception &e) {
        result.errorMessage = e.what();
    }

    return result;
}

void MainWindow::handleDetectionFinished(int index)
{
    if (!m_detectionWatchers[index].isFinished()) {
        return;
    }

    auto result = m_detectionWatchers[index].future().result();
    if (!result.errorMessage.isEmpty()) {
        QMessageBox::warning(this, "检测失败", QString("图片%1: %2").arg(index + 1).arg(result.errorMessage));
    } else {
        m_outputImages[index] = result.paintedImage;
        m_latestDetections[index] = result.detections;
        updateOutputPreview(index);
    }

    if (m_pendingDetections > 0) {
        --m_pendingDetections;
    }

    if (m_pendingDetections == 0) {
        ui->btnStart->setEnabled(true);
        updateReportTable();
    }
}

QString MainWindow::imageDisplayName(int index) const
{
    QFileInfo info(m_imagePaths[index]);
    if (info.exists() && !info.fileName().isEmpty()) {
        return info.fileName();
    }
    return QString("图片%1").arg(index + 1);
}

void MainWindow::updateReportTable()
{
    int totalRows = 0;
    for (const auto &vec : m_latestDetections) {
        totalRows += static_cast<int>(vec.size());
    }

    ui->tableReport->setRowCount(totalRows);
    int row = 0;
    for (int i = 0; i < kImageSlots; ++i) {
        const QString imageName = imageDisplayName(i);
        for (const auto &det : m_latestDetections[i]) {
            ui->tableReport->setItem(row, 0, new QTableWidgetItem(imageName));
            ui->tableReport->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(det.className)));
            ui->tableReport->setItem(row, 2, new QTableWidgetItem(QString::number(det.confidence, 'f', 2)));
            ui->tableReport->setItem(row, 3, new QTableWidgetItem(QString::number(det.box.x)));
            ui->tableReport->setItem(row, 4, new QTableWidgetItem(QString::number(det.box.y)));
            ui->tableReport->setItem(row, 5, new QTableWidgetItem(QString::number(det.box.width)));
            ui->tableReport->setItem(row, 6, new QTableWidgetItem(QString::number(det.box.height)));
            ++row;
        }
    }
}

void MainWindow::onStartDetection()
{
    if (!m_modelLoaded) {
        QMessageBox::warning(this, "警告", "YOLO模型未加载！");
        return;
    }

    if (m_pendingDetections > 0) {
        QMessageBox::information(this, "提示", "检测正在进行，请稍候。");
        return;
    }

    QVector<int> slotsToProcess;
    for (int i = 0; i < kImageSlots; ++i) {
        if (!m_inputImages[i].isNull()) {
            slotsToProcess.append(i);
        } else {
            m_outputImages[i] = QImage();
            m_latestDetections[i].clear();
            updateOutputPreview(i);
        }
    }

    if (slotsToProcess.isEmpty()) {
        QMessageBox::warning(this, "警告", "请至少选择一张图片！");
        return;
    }

    ui->btnStart->setEnabled(false);
    ui->tableReport->setRowCount(0);

    m_pendingDetections = slotsToProcess.size();
    for (int index : slotsToProcess) {
        auto future = QtConcurrent::run([this, index]() {
            return runDetectionJob(index, m_inputImages[index]);
        });
        m_detectionWatchers[index].setFuture(future);
    }
}
