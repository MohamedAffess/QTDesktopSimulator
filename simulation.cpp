#include "simulation.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QToolButton>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>

Simulation::Simulation(QWidget *parent) : QWidget(parent)
{
    setupUI();

}

Simulation::~Simulation(){
    delete simulationLayout;
    delete topologyButton;
    delete TraficButton;
    delete SnrMeasuresButton;
    delete simulationMenuButton;
    delete contentHeader;
    delete contentStack;
    delete titleLabel;
    delete simulationMenu;
    delete topologyAction;
    delete traficAction;
    delete snrMeasuresAction;
    delete topologyContent;
    delete traficContent;
    delete snrMeasuresContent;
    delete MQTTDataTimer;

}


void Simulation::setupUI()
{



    simulationLayout=new QVBoxLayout;
    setLayout(simulationLayout);

    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: semi-bold; margin: 20px; font-family: 'Arial Light'; ");  // Example style for the title


     publishToMQTTButton = new QPushButton("Push Simulation Data", this);
     publishToMQTTButton->setToolTip("Click to push simulation data to the MQTT Broker");

    publishToMQTTButton->setFixedWidth(200);
    publishToMQTTButton->setFixedHeight(30);
    QIcon productsIcon(":/images/stopIcon.png");
    productsIcon = productsIcon.pixmap(QSize(80, 80));  // Set the desired icon size here
    publishToMQTTButton->setIcon(productsIcon);



    simulationMenuButton = new QToolButton(this);



    contentHeader= new QHBoxLayout(this);
    contentHeader->addWidget(simulationMenuButton);
    contentHeader->addWidget(titleLabel);
    contentHeader->addWidget(publishToMQTTButton);







    connect(publishToMQTTButton, &QPushButton::clicked, this, &Simulation::toggleTimer);
    MQTTDataTimer = new QTimer(this);
    connect(MQTTDataTimer, &QTimer::timeout, this, &Simulation::onPublishDataToMQTTButtonClicked);



    QIcon simulationMenuIcon(":/images/menu.png");
    simulationMenuIcon = simulationMenuIcon.pixmap(QSize(30, 30));  // Set the desired icon size here
    simulationMenuButton->setIcon(simulationMenuIcon);
    simulationMenuButton->setFixedWidth(100);
    simulationMenuButton->setPopupMode(QToolButton::InstantPopup);
    simulationMenuButton->setStyleSheet("QToolButton {"
                                        "background-color:  rgba(36,101,175,190);"  // Set the background color as transparent
                                        "border: 1px solid #ccc;"
                                        "color: white;"  // Set the text color to white
                                        "padding: 12px 24px;"
                                        "text-align: center;"
                                        "text-decoration: none;"
                                        "display: inline-block;"
                                        "font-size: 16px;"
                                        "font-family: Arial Light;"
                                        "icon-size: 30px 30px;"



                                        "margin: 4px 2px;"
                                        "cursor: pointer;"
                                        "}"
                                        "QToolButton:hover {"
                                        "background-color: rgba(36,101,175,210);"  // Set a hover background color with slight gray transparency
                                        "}"
                                        "QToolButton:pressed {"
                                        "background-color: rgba(36,101,175,250);"  // Set a pressed background color with slight gray transparency
                                        "}");



    simulationMenu = new QMenu(simulationMenuButton);

    simulationMenuButton->setMenu(simulationMenu);
    simulationMenu->setStyleSheet("QMenu {"
                                  "background-color: #fff;"  // Set the background color
                                  "border: 1px solid #ccc;"  // Add a border
                                  "border-radius: 4px;"  // Round the corners
                                  "padding: 4px;"  // Add padding
                                  "}"
                                  "QMenu::item {"
                                  "background-color: #fff;"  // Set the background color for each menu item
                                  "padding: 8px 20px;"  // Add padding for each menu item
                                  "}"
                                  "QMenu::item:selected {"
                                  "background-color: rgba(36,101,175,210);"  // Set the background color for the selected menu item
                                  "color: #fff;"  // Set the text color for the selected menu item
                                  "}"
                                  "QMenu::item:pressed {"
                                  "background-color: rgba(36,101,175,250);"  // Set the background color for the pressed menu item
                                  "color: #fff;"  // Set the text color for the pressed menu item
                                  "}");
    QFont menuFont("Arial Light", 12);  // Example font family and font size
    simulationMenu->setFont(menuFont);

    //download icons for simulation menu

    QIcon topologyMenuIcon(":/images/topology.png");
    QIcon traficMenuIcon(":/images/trafic.png");
    QIcon snrMeasuresMenuIcon(":/images/snr.png");

    topologyMenuIcon = topologyMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here
    traficMenuIcon = traficMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here
    snrMeasuresMenuIcon = snrMeasuresMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here


    topologyAction = simulationMenu->addAction(topologyMenuIcon,"Topology Viewer");

    traficAction = simulationMenu->addAction(traficMenuIcon,"Trafic Measures");

    snrMeasuresAction = simulationMenu->addAction(snrMeasuresMenuIcon,"SNR Measures");



    connect(topologyAction, &QAction::triggered, this, &Simulation::onTopologyButtonClicked);
    connect(traficAction, &QAction::triggered, this, &Simulation::onTraficButtonClicked);
    connect(snrMeasuresAction, &QAction::triggered, this, &Simulation::onSnrMeasuresButtonClicked);


    simulationLayout->addLayout(contentHeader);

    contentStack = new QStackedWidget(this);
    simulationLayout->addWidget(contentStack);


    // Set the fixed width for the buttons
    topologyContent = new Topology;
    traficContent = new Trafic;
    snrMeasuresContent = new SnrMeasures;


    contentStack->addWidget(topologyContent);
    titleLabel->setText("Topology View");

    contentStack->addWidget(traficContent);
    contentStack->addWidget(snrMeasuresContent);




}
void Simulation::toggleTimer()
{
    if (!MQTTDataTimer->isActive()) {
        // Timer is not running, start the timer
        // ... You can add any necessary setup code here before starting the timer ...

        // Start the timer with the desired interval (in milliseconds)
        MQTTDataTimer->start(3000); // Adjust the interval as needed (e.g., 1000ms = 1 second)
        publishToMQTTButton->setIcon(QIcon(":/images/startIcon.png"));


    } else {
        // Timer is running, stop the timer
        MQTTDataTimer->stop();
        publishToMQTTButton->setIcon(QIcon(":/images/stopIcon.png"));


    }
}


void Simulation::onPublishDataToMQTTButtonClicked()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // Connect to the finished signal to handle the response
    QObject::connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            // Successful response
            QByteArray data = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(data);
            QJsonObject jsonObject = jsonResponse.object();
            QString message = jsonObject.value("message").toString();
        } else {
            // Error handling
            QString error = reply->errorString();
            QMessageBox::critical(nullptr, "Error", error);
        }

        // Clean up the network reply and manager
        reply->deleteLater();
        manager->deleteLater();
    });

    // Send a GET request to the specified URL
    QUrl url("http://localhost:3200/wms/simData/MQTTSimulationData");
    QNetworkRequest request(url);

    manager->get(request);
}



void Simulation::onTopologyButtonClicked()
{
    titleLabel->setText("Topology View");
    contentStack->setCurrentWidget(topologyContent);

}

void Simulation::onTraficButtonClicked()
{
    titleLabel->setText("Trafic View");

    contentStack->setCurrentWidget(traficContent);
}

void Simulation::onSnrMeasuresButtonClicked()
{
    titleLabel->setText("SNR Measures");

    contentStack->setCurrentWidget(snrMeasuresContent);
}
