/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionLoad;
    QAction *actionLoad_Specular;
    QAction *actionLoad_Diffuse;
    QWidget *Widget;
    QHBoxLayout *horizontalLayout;
    GLWidget *glwidget;
    QVBoxLayout *Configuration;
    QGroupBox *TreeOptions;
    QRadioButton *radio_brdf;
    QDoubleSpinBox *spin_f0r;
    QDoubleSpinBox *spin_f0g;
    QDoubleSpinBox *spin_f0b;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *spin_roughness;
    QLabel *label_5;
    QDoubleSpinBox *spin_metalness;
    QLabel *label_6;
    QGroupBox *RenderOptions;
    QLabel *Label_NumFaces;
    QLabel *Label_Faces;
    QLabel *Label_Vertices;
    QLabel *Label_NumVertices;
    QLabel *Label_Framerate;
    QLabel *Label_NumFramerate;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(827, 678);
        MainWindow->setMinimumSize(QSize(827, 618));
        MainWindow->setBaseSize(QSize(600, 600));
        QPalette palette;
        QBrush brush(QColor(62, 62, 62, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        MainWindow->setPalette(palette);
        MainWindow->setStyleSheet(QLatin1String("background-color: rgb(62, 62, 62);\n"
"font: 75 10pt \"MS Shell Dlg 2\";\n"
"border-color: rgb(126, 126, 126);"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionLoad_Specular = new QAction(MainWindow);
        actionLoad_Specular->setObjectName(QStringLiteral("actionLoad_Specular"));
        actionLoad_Diffuse = new QAction(MainWindow);
        actionLoad_Diffuse->setObjectName(QStringLiteral("actionLoad_Diffuse"));
        Widget = new QWidget(MainWindow);
        Widget->setObjectName(QStringLiteral("Widget"));
        Widget->setMinimumSize(QSize(827, 0));
        Widget->setBaseSize(QSize(1024, 600));
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        glwidget = new GLWidget(Widget);
        glwidget->setObjectName(QStringLiteral("glwidget"));
        glwidget->setMinimumSize(QSize(600, 600));
        glwidget->setMaximumSize(QSize(16777215, 16777215));
        glwidget->setBaseSize(QSize(600, 600));

        horizontalLayout->addWidget(glwidget);

        Configuration = new QVBoxLayout();
        Configuration->setSpacing(6);
        Configuration->setObjectName(QStringLiteral("Configuration"));
        TreeOptions = new QGroupBox(Widget);
        TreeOptions->setObjectName(QStringLiteral("TreeOptions"));
        TreeOptions->setMinimumSize(QSize(200, 0));
        TreeOptions->setMaximumSize(QSize(200, 16777215));
        QPalette palette1;
        QBrush brush1(QColor(220, 220, 220, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        TreeOptions->setPalette(palette1);
        TreeOptions->setStyleSheet(QStringLiteral("font: 75 10pt \"MS Shell Dlg 2\";"));
        radio_brdf = new QRadioButton(TreeOptions);
        radio_brdf->setObjectName(QStringLiteral("radio_brdf"));
        radio_brdf->setEnabled(true);
        radio_brdf->setGeometry(QRect(10, 20, 117, 22));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush);
        radio_brdf->setPalette(palette2);
        radio_brdf->setChecked(true);
        spin_f0r = new QDoubleSpinBox(TreeOptions);
        spin_f0r->setObjectName(QStringLiteral("spin_f0r"));
        spin_f0r->setGeometry(QRect(100, 50, 69, 27));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush);
        spin_f0r->setPalette(palette3);
        spin_f0r->setMaximum(2);
        spin_f0r->setSingleStep(0.05);
        spin_f0r->setValue(0.2);
        spin_f0g = new QDoubleSpinBox(TreeOptions);
        spin_f0g->setObjectName(QStringLiteral("spin_f0g"));
        spin_f0g->setGeometry(QRect(100, 80, 69, 27));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush);
        spin_f0g->setPalette(palette4);
        spin_f0g->setMaximum(2);
        spin_f0g->setSingleStep(0.05);
        spin_f0g->setValue(0.2);
        spin_f0b = new QDoubleSpinBox(TreeOptions);
        spin_f0b->setObjectName(QStringLiteral("spin_f0b"));
        spin_f0b->setGeometry(QRect(100, 110, 69, 27));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush);
        spin_f0b->setPalette(palette5);
        spin_f0b->setMaximum(2);
        spin_f0b->setSingleStep(0.05);
        spin_f0b->setValue(0.2);
        label = new QLabel(TreeOptions);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 50, 31, 31));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label->setPalette(palette6);
        label_2 = new QLabel(TreeOptions);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 80, 31, 31));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_2->setPalette(palette7);
        label_3 = new QLabel(TreeOptions);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 110, 31, 31));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_3->setPalette(palette8);
        spin_roughness = new QDoubleSpinBox(TreeOptions);
        spin_roughness->setObjectName(QStringLiteral("spin_roughness"));
        spin_roughness->setGeometry(QRect(100, 140, 69, 27));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette9.setBrush(QPalette::Active, QPalette::Button, brush);
        palette9.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette9.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette9.setBrush(QPalette::Active, QPalette::Base, brush);
        palette9.setBrush(QPalette::Active, QPalette::Window, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette9.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette9.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::Window, brush);
        spin_roughness->setPalette(palette9);
        spin_roughness->setDecimals(3);
        spin_roughness->setMinimum(0.025);
        spin_roughness->setMaximum(1);
        spin_roughness->setSingleStep(0.05);
        spin_roughness->setValue(0.025);
        label_5 = new QLabel(TreeOptions);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 140, 61, 31));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette10.setBrush(QPalette::Active, QPalette::Button, brush);
        palette10.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette10.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette10.setBrush(QPalette::Active, QPalette::Base, brush);
        palette10.setBrush(QPalette::Active, QPalette::Window, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette10.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette10.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_5->setPalette(palette10);
        spin_metalness = new QDoubleSpinBox(TreeOptions);
        spin_metalness->setObjectName(QStringLiteral("spin_metalness"));
        spin_metalness->setGeometry(QRect(100, 170, 69, 27));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette11.setBrush(QPalette::Active, QPalette::Button, brush);
        palette11.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette11.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush);
        palette11.setBrush(QPalette::Active, QPalette::Window, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette11.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette11.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::Window, brush);
        spin_metalness->setPalette(palette11);
        spin_metalness->setDecimals(2);
        spin_metalness->setMinimum(0.01);
        spin_metalness->setMaximum(1);
        spin_metalness->setSingleStep(0.05);
        spin_metalness->setValue(0.08);
        label_6 = new QLabel(TreeOptions);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 170, 61, 31));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette12.setBrush(QPalette::Active, QPalette::Button, brush);
        palette12.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette12.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush);
        palette12.setBrush(QPalette::Active, QPalette::Window, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette12.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette12.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_6->setPalette(palette12);

        Configuration->addWidget(TreeOptions);

        RenderOptions = new QGroupBox(Widget);
        RenderOptions->setObjectName(QStringLiteral("RenderOptions"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RenderOptions->sizePolicy().hasHeightForWidth());
        RenderOptions->setSizePolicy(sizePolicy);
        RenderOptions->setMaximumSize(QSize(200, 80));
        RenderOptions->setBaseSize(QSize(0, 100));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette13.setBrush(QPalette::Active, QPalette::Button, brush);
        palette13.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette13.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette13.setBrush(QPalette::Active, QPalette::Base, brush);
        palette13.setBrush(QPalette::Active, QPalette::Window, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette13.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette13.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::Window, brush);
        RenderOptions->setPalette(palette13);
        Label_NumFaces = new QLabel(RenderOptions);
        Label_NumFaces->setObjectName(QStringLiteral("Label_NumFaces"));
        Label_NumFaces->setGeometry(QRect(90, 10, 91, 17));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette14.setBrush(QPalette::Active, QPalette::Button, brush);
        palette14.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette14.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush);
        palette14.setBrush(QPalette::Active, QPalette::Window, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette14.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette14.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Label_NumFaces->setPalette(palette14);
        Label_Faces = new QLabel(RenderOptions);
        Label_Faces->setObjectName(QStringLiteral("Label_Faces"));
        Label_Faces->setGeometry(QRect(10, 10, 67, 17));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette15.setBrush(QPalette::Active, QPalette::Button, brush);
        palette15.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette15.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette15.setBrush(QPalette::Active, QPalette::Base, brush);
        palette15.setBrush(QPalette::Active, QPalette::Window, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette15.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette15.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette15.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette15.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette15.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Label_Faces->setPalette(palette15);
        Label_Vertices = new QLabel(RenderOptions);
        Label_Vertices->setObjectName(QStringLiteral("Label_Vertices"));
        Label_Vertices->setGeometry(QRect(10, 30, 67, 17));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette16.setBrush(QPalette::Active, QPalette::Button, brush);
        palette16.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette16.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette16.setBrush(QPalette::Active, QPalette::Base, brush);
        palette16.setBrush(QPalette::Active, QPalette::Window, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette16.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette16.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Label_Vertices->setPalette(palette16);
        Label_NumVertices = new QLabel(RenderOptions);
        Label_NumVertices->setObjectName(QStringLiteral("Label_NumVertices"));
        Label_NumVertices->setGeometry(QRect(90, 30, 91, 17));
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette17.setBrush(QPalette::Active, QPalette::Button, brush);
        palette17.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette17.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette17.setBrush(QPalette::Active, QPalette::Base, brush);
        palette17.setBrush(QPalette::Active, QPalette::Window, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette17.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette17.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette17.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette17.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette17.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette17.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette17.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette17.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette17.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Label_NumVertices->setPalette(palette17);
        Label_Framerate = new QLabel(RenderOptions);
        Label_Framerate->setObjectName(QStringLiteral("Label_Framerate"));
        Label_Framerate->setGeometry(QRect(10, 60, 71, 17));
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette18.setBrush(QPalette::Active, QPalette::Button, brush);
        palette18.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette18.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette18.setBrush(QPalette::Active, QPalette::Base, brush);
        palette18.setBrush(QPalette::Active, QPalette::Window, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette18.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette18.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette18.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette18.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette18.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette18.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette18.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette18.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette18.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Label_Framerate->setPalette(palette18);
        Label_NumFramerate = new QLabel(RenderOptions);
        Label_NumFramerate->setObjectName(QStringLiteral("Label_NumFramerate"));
        Label_NumFramerate->setGeometry(QRect(90, 60, 91, 17));
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette19.setBrush(QPalette::Active, QPalette::Button, brush);
        palette19.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette19.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette19.setBrush(QPalette::Active, QPalette::Base, brush);
        palette19.setBrush(QPalette::Active, QPalette::Window, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette19.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette19.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette19.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette19.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette19.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette19.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette19.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette19.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette19.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Label_NumFramerate->setPalette(palette19);

        Configuration->addWidget(RenderOptions);


        horizontalLayout->addLayout(Configuration);

        MainWindow->setCentralWidget(Widget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 827, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionLoad_Specular);
        menuFile->addAction(actionLoad_Diffuse);

        retranslateUi(MainWindow);
        QObject::connect(glwidget, SIGNAL(SetFaces(QString)), Label_NumFaces, SLOT(setText(QString)));
        QObject::connect(glwidget, SIGNAL(SetVertices(QString)), Label_NumVertices, SLOT(setText(QString)));
        QObject::connect(glwidget, SIGNAL(SetFramerate(QString)), Label_NumFramerate, SLOT(setText(QString)));
        QObject::connect(spin_f0b, SIGNAL(valueChanged(double)), glwidget, SLOT(SetFresnelB(double)));
        QObject::connect(spin_f0g, SIGNAL(valueChanged(double)), glwidget, SLOT(SetFresnelG(double)));
        QObject::connect(spin_f0r, SIGNAL(valueChanged(double)), glwidget, SLOT(SetFresnelR(double)));
        QObject::connect(spin_roughness, SIGNAL(valueChanged(double)), glwidget, SLOT(SetRoughness(double)));
        QObject::connect(spin_metalness, SIGNAL(valueChanged(double)), glwidget, SLOT(SetMetalness(double)));
        QObject::connect(radio_brdf, SIGNAL(clicked(bool)), glwidget, SLOT(SetBRDF(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", Q_NULLPTR));
        actionLoad->setText(QApplication::translate("MainWindow", "Load", Q_NULLPTR));
        actionLoad_Specular->setText(QApplication::translate("MainWindow", "Load Specular", Q_NULLPTR));
        actionLoad_Diffuse->setText(QApplication::translate("MainWindow", "Load Diffuse", Q_NULLPTR));
        TreeOptions->setTitle(QApplication::translate("MainWindow", "Options", Q_NULLPTR));
        radio_brdf->setText(QApplication::translate("MainWindow", "Visibility", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "F0 R", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "F0 G", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "F0 B", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Roughness", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Metalness", Q_NULLPTR));
        RenderOptions->setTitle(QString());
        Label_NumFaces->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        Label_Faces->setText(QApplication::translate("MainWindow", "Faces", Q_NULLPTR));
        Label_Vertices->setText(QApplication::translate("MainWindow", "Vertices", Q_NULLPTR));
        Label_NumVertices->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        Label_Framerate->setText(QApplication::translate("MainWindow", "Framerate", Q_NULLPTR));
        Label_NumFramerate->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
