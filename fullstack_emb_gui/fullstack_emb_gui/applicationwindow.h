#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
namespace Ui { class ApplicationWindow; }
QT_END_NAMESPACE



class ApplicationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ApplicationWindow(QWidget *parent = nullptr);
    ~ApplicationWindow();

private slots:
    void readSerialData();


private:
    Ui::ApplicationWindow *ui;
    QSerialPort *serialPort;
    QChart *chart;
    QMap<QString, QLineSeries*> seriesMap; // Maps sensor addresses to series
    int timeIndex = 0; // Used as x-axis value
};

#endif // APPLICATIONWINDOW_H
