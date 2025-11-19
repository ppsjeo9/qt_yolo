/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupInput;
    QHBoxLayout *horizontalLayoutInput;
    QLineEdit *lineEditImagePath;
    QPushButton *btnBrowse;
    QPushButton *btnStart;
    QSplitter *splitter;
    QGroupBox *groupInputImage;
    QVBoxLayout *verticalLayoutInputImage;
    QLabel *labelInput;
    QGroupBox *groupOutputImage;
    QVBoxLayout *verticalLayoutOutputImage;
    QLabel *labelOutput;
    QGroupBox *groupReport;
    QVBoxLayout *verticalLayoutReport;
    QTableWidget *tableReport;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        groupInput = new QGroupBox(centralwidget);
        groupInput->setObjectName("groupInput");
        horizontalLayoutInput = new QHBoxLayout(groupInput);
        horizontalLayoutInput->setObjectName("horizontalLayoutInput");
        lineEditImagePath = new QLineEdit(groupInput);
        lineEditImagePath->setObjectName("lineEditImagePath");

        horizontalLayoutInput->addWidget(lineEditImagePath);

        btnBrowse = new QPushButton(groupInput);
        btnBrowse->setObjectName("btnBrowse");

        horizontalLayoutInput->addWidget(btnBrowse);

        btnStart = new QPushButton(groupInput);
        btnStart->setObjectName("btnStart");

        horizontalLayoutInput->addWidget(btnStart);


        verticalLayout->addWidget(groupInput);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        groupInputImage = new QGroupBox(splitter);
        groupInputImage->setObjectName("groupInputImage");
        verticalLayoutInputImage = new QVBoxLayout(groupInputImage);
        verticalLayoutInputImage->setObjectName("verticalLayoutInputImage");
        labelInput = new QLabel(groupInputImage);
        labelInput->setObjectName("labelInput");
        labelInput->setMinimumSize(QSize(300, 300));
        labelInput->setFrameShape(QFrame::Box);
        labelInput->setAlignment(Qt::AlignCenter);

        verticalLayoutInputImage->addWidget(labelInput);

        splitter->addWidget(groupInputImage);
        groupOutputImage = new QGroupBox(splitter);
        groupOutputImage->setObjectName("groupOutputImage");
        verticalLayoutOutputImage = new QVBoxLayout(groupOutputImage);
        verticalLayoutOutputImage->setObjectName("verticalLayoutOutputImage");
        labelOutput = new QLabel(groupOutputImage);
        labelOutput->setObjectName("labelOutput");
        labelOutput->setMinimumSize(QSize(300, 300));
        labelOutput->setFrameShape(QFrame::Box);
        labelOutput->setAlignment(Qt::AlignCenter);

        verticalLayoutOutputImage->addWidget(labelOutput);

        splitter->addWidget(groupOutputImage);

        verticalLayout->addWidget(splitter);

        groupReport = new QGroupBox(centralwidget);
        groupReport->setObjectName("groupReport");
        verticalLayoutReport = new QVBoxLayout(groupReport);
        verticalLayoutReport->setObjectName("verticalLayoutReport");
        tableReport = new QTableWidget(groupReport);
        tableReport->setObjectName("tableReport");
        tableReport->setRowCount(0);
        tableReport->setColumnCount(6);

        verticalLayoutReport->addWidget(tableReport);


        verticalLayout->addWidget(groupReport);

        verticalLayout->setStretch(1, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupInput->setTitle(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245", nullptr));
        lineEditImagePath->setPlaceholderText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\345\233\276\347\211\207\350\267\257\345\276\204", nullptr));
        btnBrowse->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\345\233\276\347\211\207", nullptr));
        btnStart->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\243\200\346\265\213", nullptr));
        groupInputImage->setTitle(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\345\233\276\345\203\217", nullptr));
        labelInput->setText(QCoreApplication::translate("MainWindow", "\346\227\240\345\233\276\347\211\207", nullptr));
        groupOutputImage->setTitle(QCoreApplication::translate("MainWindow", "\350\276\223\345\207\272\347\273\223\346\236\234", nullptr));
        labelOutput->setText(QCoreApplication::translate("MainWindow", "\347\255\211\345\276\205\346\243\200\346\265\213", nullptr));
        groupReport->setTitle(QCoreApplication::translate("MainWindow", "\346\243\200\346\265\213\346\212\245\345\221\212", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
