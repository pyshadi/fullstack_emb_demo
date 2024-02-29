/********************************************************************************
** Form generated from reading UI file 'applicationwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPLICATIONWINDOW_H
#define UI_APPLICATIONWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ApplicationWindow
{
public:
    QWidget *centralwidget;
    QChartView *widget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ApplicationWindow)
    {
        if (ApplicationWindow->objectName().isEmpty())
            ApplicationWindow->setObjectName("ApplicationWindow");
        ApplicationWindow->resize(800, 550);
        centralwidget = new QWidget(ApplicationWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new QChartView(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(170, 80, 601, 311));
        ApplicationWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ApplicationWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        ApplicationWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ApplicationWindow);
        statusbar->setObjectName("statusbar");
        ApplicationWindow->setStatusBar(statusbar);

        retranslateUi(ApplicationWindow);

        QMetaObject::connectSlotsByName(ApplicationWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ApplicationWindow)
    {
        ApplicationWindow->setWindowTitle(QCoreApplication::translate("ApplicationWindow", "ApplicationWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ApplicationWindow: public Ui_ApplicationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPLICATIONWINDOW_H
