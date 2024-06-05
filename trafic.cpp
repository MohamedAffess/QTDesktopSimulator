#include "trafic.h"
#include "ui_trafic.h"
#include <QLabel>

#include <QWidget>
#include <QComboBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QLabel>
#include <QFormLayout>
#include <QScrollArea>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QChartView>
#include <QTimer>
#include <QDateTime>

Trafic::Trafic(QWidget *parent) :
    QWidget(parent)
{
    setupUI();
     refreshChartDataTimer = new QTimer(this);
    connect(refreshChartDataTimer, &QTimer::timeout, this, &Trafic::refreshData);






}

Trafic::~Trafic()
{
    delete traficLayout;
    delete graphicLayout;
    // Create the chart
    delete nodesInterfacesListLayout;
    delete series;
    delete chart;
    delete xAxis;
    delete yAxis;
    delete chartView;
    delete refreshChartDataTimer;
    delete scrollArea;
    delete scrollContent;
    delete refreshButton;



}

void Trafic::setupUI(){
    traficLayout = new QHBoxLayout(this);
    setLayout(traficLayout);

    nodesInterfacesListLayout = new QVBoxLayout();
    traficLayout->addLayout(nodesInterfacesListLayout);


    populateNodeInterfacesList();

    graphicLayout=new QVBoxLayout();
    traficLayout->addLayout(graphicLayout);
    chartView = new QtCharts::QChartView();
    graphicLayout->addWidget(chartView);

    // Create a scroll area to contain the form layout
    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
     scrollContent = new QWidget;
    scrollContent->setLayout(interfaceListFormLayout);
    scrollArea->setWidget(scrollContent);

    // Set the scroll area as the central widget of the main window
    nodesInterfacesListLayout->addWidget(scrollArea);

    refreshButton = new QPushButton("Refresh", this);
    nodesInterfacesListLayout->addWidget(refreshButton);
    connect(refreshButton, &QPushButton::clicked, this, &Trafic::refreshInterfacesData);




}


void Trafic::populateNodeInterfacesList()
{
    // Clear previous data
    nodeComboBoxes.clear();

    if (!interfaceListFormLayout)
       {
           interfaceListFormLayout = new QFormLayout;
       }
       else
       {
           // Clear existing layout content
           QLayoutItem* child;
           while ((child = interfaceListFormLayout->takeAt(0)) != nullptr)
           {
               QWidget* widget = child->widget();
               if (widget)
               {
                   delete widget;
               }
               delete child;
           }
       }

    // Get all the nodes
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes"));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonArray nodesArray = jsonResponse.array();


        // Iterate through the nodes
        for (const QJsonValue& nodeValue : nodesArray)
        {
            QJsonObject nodeObject = nodeValue.toObject();

            // Extract node information
            QString nodeName = nodeObject.value("node_name").toString();
            QString nodeType = nodeObject.value("node_type").toString();
            QString nodeId = nodeObject.value("node_id").toString();

            // Create a label for the node
            QLabel* label = new QLabel(QString("%1 (%2 - %3)").arg(nodeName).arg(nodeType).arg(nodeId));
            label->setStyleSheet("font-weight: bold;");

            // Create a combo box for the node
            QComboBox* comboBox = new QComboBox(this);

            // Get the interfaces for the node
            QNetworkAccessManager manager2;
            QNetworkRequest interfacesRequest(QUrl("http://localhost:3200/wms/mesh/interfaces"));
            QNetworkReply* interfacesReply = manager2.get(interfacesRequest);
            QEventLoop interfacesLoop;
            connect(interfacesReply, &QNetworkReply::finished, &interfacesLoop, &QEventLoop::quit);
            interfacesLoop.exec();

            if (interfacesReply->error() == QNetworkReply::NoError)
            {
                // Request successful
                QByteArray interfacesData = interfacesReply->readAll();
                QJsonDocument interfacesJson = QJsonDocument::fromJson(interfacesData);
                QJsonArray interfacesArray = interfacesJson.array();

                // Iterate through the interfaces and add the ones with matching node_id to the combo box
                for (const QJsonValue& interfaceValue : interfacesArray)
                {
                    QJsonObject interfaceObject = interfaceValue.toObject();
                    QString interfaceId = interfaceObject.value("interface_id").toString();
                    QString interfaceNodeId = interfaceObject.value("node_id").toString();
                    QString interfaceName = interfaceObject.value("interface_name").toString();


                    if (interfaceNodeId == nodeId)
                    {
                        comboBox->addItem(QString("%1 (%2)").arg(interfaceId, interfaceName));
                    }
                }
            }

            interfacesReply->deleteLater();

            // Connect the combo box's currentIndexChanged signal to the onInterfaceClicked slot
            connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, nodeId, comboBox]() {
                QString selectedItemText = comboBox->currentText();
                QString interfaceId = selectedItemText.split(' ').first();
                QString interfaceName= selectedItemText.split(' ').last();

                // Call the onInterfaceClicked slot with the node_id and interface_id

                onInterfaceClicked(nodeId, interfaceId,interfaceName);
            });

            // Add the label and combo box to the form layout
            interfaceListFormLayout->addRow(label, comboBox);

            // Add the combo box to the list
            nodeComboBoxes.append(comboBox);
        }


    }

    reply->deleteLater();
}

double Trafic::convertBytesToMbps(double valueInBytes) {
    double valueInBits = valueInBytes * 8; // Convert bytes to bits
    double valueInMbps = valueInBits / (1000 * 1000); // Convert bits to megabits
    return valueInMbps;
}



void Trafic::onInterfaceClicked(const QString& nodeId, const QString& interfaceId, const QString& interfaceName)
{
    currentNodeId = nodeId;
    currentInterfaceId = interfaceId;
    currentInterfaceName = interfaceName;

    // Send request to get the traffic data for the selected interface
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(QString("http://localhost:3200/wms/mesh/nodes/%1/allTotalBytes").arg(nodeId)));
    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonArray dataPointsArray = jsonResponse.array();

        // Filter the data points based on the selected interface_id
        QJsonArray filteredDataPoints;
        for (const QJsonValue& dataPointValue : dataPointsArray)
        {
            QJsonObject dataPointObject = dataPointValue.toObject();
            QString dataPointInterfaceId = dataPointObject.value("interface_id").toString();

            if (dataPointInterfaceId == interfaceId)
            {
                filteredDataPoints.append(dataPointValue);
            }
        }

        // Create the series
        series = new QtCharts::QLineSeries();

        // Extract the values from the filtered data points
        for (const QJsonValue& filteredValue : filteredDataPoints)
        {
            QJsonObject filteredObject = filteredValue.toObject();
            double value = filteredObject.value("value").toDouble();
            double valueInMbps = convertBytesToMbps(value);
            double epochDate = filteredObject.value("date").toDouble();
            qreal actualDate = static_cast<qreal>(epochDate * 1000);

            // Add the data point to the series
            series->append(actualDate, valueInMbps);
        }

        // Create the chart and set the series
        chart = new QtCharts::QChart();
        chart->addSeries(series);

        // Create the axes
        xAxis = new QtCharts::QDateTimeAxis();
        yAxis = new QtCharts::QValueAxis();
        xAxis->setFormat("dd/MM/yyyy");
        xAxis->setTitleText("Date");
        yAxis->setTitleText("Traffic Value (Mbps)");

        chart->setAxisX(xAxis, series);
        chart->setAxisY(yAxis, series);
        chart->setTitle("Traffic Data of interface " + interfaceId + interfaceName);

        // Set the chart properties
        chartView->setRenderHint(QPainter::Antialiasing);

        // Set the updated chart to the chart view
        chartView->setChart(chart);

        refreshChartDataTimer->start(2000); // Refresh every 8 seconds (adjust the interval as needed)
    }

    reply->deleteLater();
}

void Trafic::refreshData()
{
    if (!currentNodeId.isEmpty() && !currentInterfaceId.isEmpty())
    {
        onInterfaceClicked(currentNodeId, currentInterfaceId,currentInterfaceName);
    }
    else
    {
        qDebug() << "Error: Empty node or interface ID";
    }
}
void Trafic::refreshInterfacesData()
{
    populateNodeInterfacesList();
}
