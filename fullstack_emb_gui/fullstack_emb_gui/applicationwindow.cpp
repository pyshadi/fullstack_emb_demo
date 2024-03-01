#include "applicationwindow.h"
#include "ui_applicationwindow.h"
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QtCharts/QLineSeries>

ApplicationWindow::ApplicationWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ApplicationWindow),
    serialPort(new QSerialPort(this)),
    chart(new QChart()) // Only initialize serialPort and chart here
{
    ui->setupUi(this);

    // Setup the chart properties
    chart->setTitle("Real-time Temperature Data");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);

    // Setup serial port
    serialPort->setPortName("COM5"); // Adjust to your actual port
    serialPort->setBaudRate(QSerialPort::Baud115200);
    connect(serialPort, &QSerialPort::readyRead, this, &ApplicationWindow::readSerialData);


    if (!serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open port COM5, error:" << serialPort->errorString();
    }

    // No need to initialize 'series' or 'seriesMap' here, they'll be handled dynamically
}

void ApplicationWindow::readSerialData() {
    QByteArray data = serialPort->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    // Check if the document contains an array
    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            QString address = obj["address"].toString();
            double temperature = obj["temperature"].toDouble();

            // Check if the series already exists, if not create a new one
            if (!seriesMap.contains(address)) {
                QLineSeries *newSeries = new QLineSeries();
                newSeries->setName(address); // Optional: Set the name to the sensor address
                chart->addSeries(newSeries);
                chart->createDefaultAxes(); // Recreate axes to accommodate the new series
                seriesMap.insert(address, newSeries);
            }

            // Add the data point to the appropriate series
            seriesMap[address]->append(timeIndex, temperature);
        }
        timeIndex++; // Increment the time index for the next read
    }

    // Optionally, if your chart's x-axis is not automatically updating its range
    chart->axes(Qt::Horizontal).first()->setRange(0, timeIndex);
    chart->axes(Qt::Vertical).first()->setRange(0, 50); // Adjust vertical range as needed
}




ApplicationWindow::~ApplicationWindow() {
    if (serialPort->isOpen())
        serialPort->close();
    delete ui;
}
