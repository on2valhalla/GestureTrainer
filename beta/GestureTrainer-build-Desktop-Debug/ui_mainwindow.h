/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun Apr 28 21:53:03 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label_Camera;
    QPushButton *pushButton_Camera;
    QPushButton *pushButton_OpenImage;
    QTabWidget *tabWidget;
    QWidget *tabStart;
    QLabel *label_2;
    QWidget *tabBackground;
    QLabel *label_Color5;
    QLabel *label_MinValue;
    QLabel *label_MaxValue;
    QLabel *label_MinHue;
    QLabel *label_Color1;
    QLabel *label_MaxSat;
    QLabel *label_MinSat;
    QSlider *verticalSlider_MaxSat;
    QLabel *label_Color6;
    QLabel *label_Color2;
    QSlider *verticalSlider_MaxValue;
    QLabel *label_Max;
    QLabel *label_Color4;
    QSlider *verticalSlider_MinSat;
    QLabel *label_Color3;
    QSlider *verticalSlider_MinValue;
    QFrame *line;
    QSlider *verticalSlider_MinHue;
    QLabel *label_MaxHue;
    QLabel *label_Min;
    QSlider *verticalSlider_MaxHue;
    QFrame *line_3;
    QPushButton *pushButton_Process;
    QPushButton *pushButton_Histogram;
    QCheckBox *check_Invert;
    QCheckBox *check_Erode;
    QCheckBox *check_Dilate;
    QCheckBox *check_Blur;
    QWidget *tabMeasure;
    QLabel *label_HandDisplay;
    QTextBrowser *textBrowser;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1066, 540);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label_Camera = new QLabel(centralWidget);
        label_Camera->setObjectName(QString::fromUtf8("label_Camera"));
        label_Camera->setGeometry(QRect(570, 10, 481, 431));
        label_Camera->setFrameShape(QFrame::Box);
        label_Camera->setFrameShadow(QFrame::Raised);
        label_Camera->setLineWidth(2);
        pushButton_Camera = new QPushButton(centralWidget);
        pushButton_Camera->setObjectName(QString::fromUtf8("pushButton_Camera"));
        pushButton_Camera->setGeometry(QRect(590, 450, 221, 41));
        pushButton_OpenImage = new QPushButton(centralWidget);
        pushButton_OpenImage->setObjectName(QString::fromUtf8("pushButton_OpenImage"));
        pushButton_OpenImage->setGeometry(QRect(820, 450, 221, 41));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 531, 471));
        tabWidget->setTabShape(QTabWidget::Triangular);
        tabStart = new QWidget();
        tabStart->setObjectName(QString::fromUtf8("tabStart"));
        label_2 = new QLabel(tabStart);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 20, 461, 151));
        label_2->setTextFormat(Qt::RichText);
        label_2->setWordWrap(true);
        tabWidget->addTab(tabStart, QString());
        tabBackground = new QWidget();
        tabBackground->setObjectName(QString::fromUtf8("tabBackground"));
        label_Color5 = new QLabel(tabBackground);
        label_Color5->setObjectName(QString::fromUtf8("label_Color5"));
        label_Color5->setGeometry(QRect(230, 390, 71, 21));
        label_MinValue = new QLabel(tabBackground);
        label_MinValue->setObjectName(QString::fromUtf8("label_MinValue"));
        label_MinValue->setGeometry(QRect(120, 370, 51, 21));
        label_MaxValue = new QLabel(tabBackground);
        label_MaxValue->setObjectName(QString::fromUtf8("label_MaxValue"));
        label_MaxValue->setGeometry(QRect(300, 370, 51, 21));
        label_MinHue = new QLabel(tabBackground);
        label_MinHue->setObjectName(QString::fromUtf8("label_MinHue"));
        label_MinHue->setGeometry(QRect(10, 370, 41, 21));
        label_Color1 = new QLabel(tabBackground);
        label_Color1->setObjectName(QString::fromUtf8("label_Color1"));
        label_Color1->setGeometry(QRect(0, 390, 41, 21));
        label_MaxSat = new QLabel(tabBackground);
        label_MaxSat->setObjectName(QString::fromUtf8("label_MaxSat"));
        label_MaxSat->setGeometry(QRect(240, 370, 51, 21));
        label_MinSat = new QLabel(tabBackground);
        label_MinSat->setObjectName(QString::fromUtf8("label_MinSat"));
        label_MinSat->setGeometry(QRect(60, 370, 51, 21));
        verticalSlider_MaxSat = new QSlider(tabBackground);
        verticalSlider_MaxSat->setObjectName(QString::fromUtf8("verticalSlider_MaxSat"));
        verticalSlider_MaxSat->setGeometry(QRect(260, 10, 21, 361));
        verticalSlider_MaxSat->setMaximum(255);
        verticalSlider_MaxSat->setValue(136);
        verticalSlider_MaxSat->setOrientation(Qt::Vertical);
        label_Color6 = new QLabel(tabBackground);
        label_Color6->setObjectName(QString::fromUtf8("label_Color6"));
        label_Color6->setGeometry(QRect(310, 390, 51, 21));
        label_Color2 = new QLabel(tabBackground);
        label_Color2->setObjectName(QString::fromUtf8("label_Color2"));
        label_Color2->setGeometry(QRect(50, 390, 71, 21));
        verticalSlider_MaxValue = new QSlider(tabBackground);
        verticalSlider_MaxValue->setObjectName(QString::fromUtf8("verticalSlider_MaxValue"));
        verticalSlider_MaxValue->setGeometry(QRect(320, 10, 21, 361));
        verticalSlider_MaxValue->setMaximum(255);
        verticalSlider_MaxValue->setValue(136);
        verticalSlider_MaxValue->setOrientation(Qt::Vertical);
        label_Max = new QLabel(tabBackground);
        label_Max->setObjectName(QString::fromUtf8("label_Max"));
        label_Max->setGeometry(QRect(240, 420, 62, 16));
        label_Color4 = new QLabel(tabBackground);
        label_Color4->setObjectName(QString::fromUtf8("label_Color4"));
        label_Color4->setGeometry(QRect(180, 390, 41, 21));
        verticalSlider_MinSat = new QSlider(tabBackground);
        verticalSlider_MinSat->setObjectName(QString::fromUtf8("verticalSlider_MinSat"));
        verticalSlider_MinSat->setGeometry(QRect(80, 10, 21, 361));
        verticalSlider_MinSat->setMaximum(255);
        verticalSlider_MinSat->setValue(136);
        verticalSlider_MinSat->setOrientation(Qt::Vertical);
        label_Color3 = new QLabel(tabBackground);
        label_Color3->setObjectName(QString::fromUtf8("label_Color3"));
        label_Color3->setGeometry(QRect(120, 390, 51, 21));
        verticalSlider_MinValue = new QSlider(tabBackground);
        verticalSlider_MinValue->setObjectName(QString::fromUtf8("verticalSlider_MinValue"));
        verticalSlider_MinValue->setGeometry(QRect(140, 10, 21, 361));
        verticalSlider_MinValue->setMaximum(255);
        verticalSlider_MinValue->setValue(136);
        verticalSlider_MinValue->setOrientation(Qt::Vertical);
        line = new QFrame(tabBackground);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(170, 0, 20, 431));
        line->setLineWidth(3);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        verticalSlider_MinHue = new QSlider(tabBackground);
        verticalSlider_MinHue->setObjectName(QString::fromUtf8("verticalSlider_MinHue"));
        verticalSlider_MinHue->setGeometry(QRect(20, 10, 21, 361));
        verticalSlider_MinHue->setMaximum(180);
        verticalSlider_MinHue->setValue(136);
        verticalSlider_MinHue->setOrientation(Qt::Vertical);
        label_MaxHue = new QLabel(tabBackground);
        label_MaxHue->setObjectName(QString::fromUtf8("label_MaxHue"));
        label_MaxHue->setGeometry(QRect(190, 370, 41, 21));
        label_Min = new QLabel(tabBackground);
        label_Min->setObjectName(QString::fromUtf8("label_Min"));
        label_Min->setGeometry(QRect(60, 420, 62, 16));
        verticalSlider_MaxHue = new QSlider(tabBackground);
        verticalSlider_MaxHue->setObjectName(QString::fromUtf8("verticalSlider_MaxHue"));
        verticalSlider_MaxHue->setGeometry(QRect(200, 10, 21, 361));
        verticalSlider_MaxHue->setMaximum(180);
        verticalSlider_MaxHue->setValue(136);
        verticalSlider_MaxHue->setOrientation(Qt::Vertical);
        line_3 = new QFrame(tabBackground);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(360, 0, 20, 431));
        line_3->setLineWidth(3);
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        pushButton_Process = new QPushButton(tabBackground);
        pushButton_Process->setObjectName(QString::fromUtf8("pushButton_Process"));
        pushButton_Process->setGeometry(QRect(380, 0, 131, 51));
        pushButton_Histogram = new QPushButton(tabBackground);
        pushButton_Histogram->setObjectName(QString::fromUtf8("pushButton_Histogram"));
        pushButton_Histogram->setGeometry(QRect(380, 50, 131, 51));
        check_Invert = new QCheckBox(tabBackground);
        check_Invert->setObjectName(QString::fromUtf8("check_Invert"));
        check_Invert->setGeometry(QRect(400, 120, 87, 20));
        check_Erode = new QCheckBox(tabBackground);
        check_Erode->setObjectName(QString::fromUtf8("check_Erode"));
        check_Erode->setGeometry(QRect(400, 150, 87, 20));
        check_Erode->setChecked(true);
        check_Dilate = new QCheckBox(tabBackground);
        check_Dilate->setObjectName(QString::fromUtf8("check_Dilate"));
        check_Dilate->setGeometry(QRect(400, 180, 87, 20));
        check_Dilate->setChecked(true);
        check_Blur = new QCheckBox(tabBackground);
        check_Blur->setObjectName(QString::fromUtf8("check_Blur"));
        check_Blur->setGeometry(QRect(400, 210, 87, 20));
        check_Blur->setChecked(true);
        tabWidget->addTab(tabBackground, QString());
        tabMeasure = new QWidget();
        tabMeasure->setObjectName(QString::fromUtf8("tabMeasure"));
        label_HandDisplay = new QLabel(tabMeasure);
        label_HandDisplay->setObjectName(QString::fromUtf8("label_HandDisplay"));
        label_HandDisplay->setGeometry(QRect(300, 10, 200, 200));
        label_HandDisplay->setFrameShape(QFrame::Box);
        label_HandDisplay->setFrameShadow(QFrame::Raised);
        label_HandDisplay->setLineWidth(2);
        textBrowser = new QTextBrowser(tabMeasure);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 10, 256, 411));
        tabWidget->addTab(tabMeasure, QString());
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label_Camera->setText(QString());
        pushButton_Camera->setText(QApplication::translate("MainWindow", "Show Camera", 0, QApplication::UnicodeUTF8));
        pushButton_OpenImage->setText(QApplication::translate("MainWindow", "Open Image", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Welcome!<br/></p><p>If this is your first time, click on the background tab and select your filter. Otherwise, you know what to do!</p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabStart), QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        label_Color5->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">Saturation</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MinValue->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MaxValue->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MinHue->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Color1->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">Hue</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MaxSat->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MinSat->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Color6->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">Value</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Color2->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">Saturation</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Max->setText(QApplication::translate("MainWindow", "Max HSV", 0, QApplication::UnicodeUTF8));
        label_Color4->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">Hue</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Color3->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">Value</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MaxHue->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Min->setText(QApplication::translate("MainWindow", "Min HSV", 0, QApplication::UnicodeUTF8));
        pushButton_Process->setText(QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
        pushButton_Histogram->setText(QApplication::translate("MainWindow", "Show Histogram", 0, QApplication::UnicodeUTF8));
        check_Invert->setText(QApplication::translate("MainWindow", "Invert", 0, QApplication::UnicodeUTF8));
        check_Erode->setText(QApplication::translate("MainWindow", "Erode", 0, QApplication::UnicodeUTF8));
        check_Dilate->setText(QApplication::translate("MainWindow", "Dilate", 0, QApplication::UnicodeUTF8));
        check_Blur->setText(QApplication::translate("MainWindow", "Blur", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabBackground), QApplication::translate("MainWindow", "Background", 0, QApplication::UnicodeUTF8));
        label_HandDisplay->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabMeasure), QApplication::translate("MainWindow", "Measure Hand", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
