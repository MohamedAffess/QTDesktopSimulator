#include "meshrouters.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include<QDialogButtonBox>
#include<QFormLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QEventLoop>
#include <QTimer>
#include <QComboBox>
#include<QScrollArea>
#include <QApplication>
#include <QSlider>
#include <QDesktopWidget>
#include <QSpinBox>
#include <QCheckBox>

MeshRouters::MeshRouters(QWidget *parent) : QWidget(parent)
{
    //    setObjectName("MeshRouters");
    //    // Apply the stylesheet
    //    setAttribute(Qt::WA_StyledBackground, true);

    //        QFile styleFile(":/styles/style.qss");
    //        styleFile.open(QFile::ReadOnly);
    //        QString styleSheet = QLatin1String(styleFile.readAll());
    //        setStyleSheet(styleSheet);
    setupUI();
    populateTable();
    QTimer::singleShot(30000, this, &MeshRouters::populateTable);




}


MeshRouters::~MeshRouters(){
    delete meshRoutersTable;
    delete addRouterButton;
    delete meshRoutersLayout;
    delete dockContent;
    delete dockWidget;
    delete dockLayout;
    delete meshRouterInfoWidget;
    delete meshRouterModifyWidget;
    delete meshRoutersMenu;



}

void MeshRouters::setupUI()
{
    meshRoutersLayout = new QVBoxLayout;
    setLayout(meshRoutersLayout);

    meshRoutersTable = new QTableWidget(this);


    meshRoutersTable->setObjectName("meshRoutersTable");
    meshRoutersTable->setColumnCount(7);
    meshRoutersTable->setHorizontalHeaderLabels({"Router Type", "Hardware", "Wireless Interfaces", "Wired Interfaces",
                                                 "Wired Speed (Mbps)", "Wireless Speed (Mbps)", "Has Battery"});
    meshRoutersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    meshRoutersLayout->addWidget(meshRoutersTable);



    // Set the style sheet for the table
    QString tableStyleSheet = "QTableWidget {"
                              "    background-color: #f2f2f2;"
                              "    border: 0.5px solid #999999;"
                              "    border-radius: 4px;"
                              "    font-size: 14px;"
                              "    font-family: Arial;"


                              "}"
                              "QTableWidget QHeaderView {"
                              "    background-color: #e6e6e6;"
                              "    font-size: 14px;"
                              "    font-family: Arial Light;"
                              "    border: none;"

                              "}"
                              "QTableWidget QHeaderView::section {"
                              "    font-size: 14px;"
                              "    font-family: Arial Light;"
                              "    color: #333333;"
                              "    border: 0.5px solid #999999;"
                              "    border-radius: 4px;"

                              "    padding: 6px 10px;"
                              "    text-align: left;"
                              "    height: 30px;"


                              "}"
                              "QTableWidget QHeaderView::section:checked {"
                              "    background-color: #c2c2c2;"
                              "}"
                              "QTableWidget QHeaderView::section:pressed {"
                              "    background-color: #999999;"
                              "}"
                              "QTableWidget QTableWidgetItem {"
                              "    font-size: 16px;"
                              "    font-family: Arial Light;"
                              "    color: #333333;"
                              "}"
                              "QTableWidget::item:selected {"
                              "background-color: rgba(36,101,175,210);"
                              "    color: #ffffff;"
                              "}"
                              "QTableWidget::item:focus {"
                              "background-color: rgba(36,101,175,210);"
                              "    color: #ffffff;"
                              "}"
            ;



    meshRoutersTable->setStyleSheet(tableStyleSheet);
    addRouterButton = new QPushButton("Add Router", this);
    QIcon addIcon(":/images/addIcon.png");
    addIcon = addIcon.pixmap(QSize(128,128));  // Set the desired icon size here

    addRouterButton->setIcon(addIcon);
    addRouterButton->setStyleSheet("QPushButton {"
                                      "background-color:  rgba(36,101,175,190);"  // Set the background color as transparent
                                      "border: 1px solid #ccc;"
                                      "color: white;"  // Set the text color to white
                                      "padding: 12px 24px;"
                                      "text-align: center;"
                                      "text-decoration: none;"
                                      "display: inline-block;"
                                      "font-size: 16px;"
                                      "font-family: Arial Light;"


                                      "margin: 4px 2px;"
                                      "cursor: pointer;"
                                      "}"
                                      "QPushButton:hover {"
                                      "background-color: rgba(36,101,175,210);"  // Set a hover background color with slight gray transparency
                                      "}"
                                      "QPushButton:pressed {"
                                      "background-color: rgba(36,101,175,250);"  // Set a pressed background color with slight gray transparency
                                    "}");
    addRouterButton->setFixedWidth(200);
    meshRoutersLayout->addWidget(addRouterButton);

    connect(addRouterButton, &QPushButton::clicked, this, &MeshRouters::onAddRouterClicked);


    //actions
    meshRoutersMenu = new QMenu(this);
    QIcon infoMenuIcon(":/images/infoIcon-24.png");
    QIcon modifyMenuIcon(":/images/editIcon.png");
    QIcon deleteMenuIcon(":/images/deleteIcon.png");
    infoMenuIcon = infoMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
    modifyMenuIcon = modifyMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
    deleteMenuIcon = deleteMenuIcon.pixmap(QSize(24, 24));
    infoAction = meshRoutersMenu->addAction(infoMenuIcon,"Router Informations");
    modifyAction=meshRoutersMenu->addAction(modifyMenuIcon,"Modify Router");
    deleteAction=meshRoutersMenu->addAction(deleteMenuIcon,"Delete Router");
//    deleteAction = new QAction("Delete", this);

    connect(deleteAction, &QAction::triggered, this, &MeshRouters::onDeleteActionTriggered);

//    infoAction = new QAction("Info",this);
    connect(infoAction,&QAction::triggered,this,&MeshRouters::onInfoActionTriggered);

//    modifyAction = new QAction("Modify",this);
    connect(modifyAction,&QAction::triggered,this,&MeshRouters::onModifyActionTriggered);



    //DOCK WIDGET PART
    dockWidget = new QDockWidget( this);
    dockContent = new QStackedWidget(dockWidget);



    dockLayout = new QVBoxLayout;
    dockLayout->addWidget(dockWidget);
    dockWidget->hide();
    dockLayout->addWidget(dockContent);



    dockWidget->setWidget(dockContent);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);






}

void MeshRouters::displayDockWidget(QString message)
{
    meshRoutersLayout->addLayout(dockLayout);
    dockWidget->setWindowTitle(message);
    dockContent->show();  // Show the dock content before animating
    dockWidget->show();

}

void MeshRouters::contextMenuEvent(QContextMenuEvent *event)
{

//    meshRoutersMenu.addAction(infoAction);
//    meshRoutersMenu.addAction(modifyAction);
//    meshRoutersMenu.addAction(deleteAction);

    meshRoutersMenu->exec(event->globalPos());



}


void MeshRouters::populateTable()
{
    // Fetch data from the backend and populate the table

    // Clear existing table data
    meshRoutersTable->clearContents();
    meshRoutersTable->setRowCount(0);

    // Make a request to the backend endpoint and retrieve the data
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes")));
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check if the request was successful
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Failed to retrieve Routers Data :" << reply->errorString();
        reply->deleteLater();
        return;
    }

    // Parse the JSON response
    QByteArray response = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response); //create a Json document from the reply
    if (!jsonDoc.isArray()) {
        qDebug() << "Invalid JSON response.";
        reply->deleteLater();
        return;
    }

    // Iterate over the JSON array and extract the required fields
    QJsonArray jsonArray = jsonDoc.array();  //return the array contained in the doc

    for (const QJsonValue& jsonValue : jsonArray) {
        if (!jsonValue.isObject())
            continue;

        const QJsonObject jsonObject = jsonValue.toObject(); //convert the jsonValue into object

        //preparing the response elements
        const QString routerType = jsonObject.value("node_type").toString(); //returns the value of a specified key
        const QString hardware = jsonObject.value("node_hw_model").toString();
        const int wirelessInterfaces = jsonObject.value("wireless_interfaces_count").toInt();
        const int wiredInterfaces = jsonObject.value("wired_interfaces_count").toInt();
        const int wiredSpeed = jsonObject.value("wired_interfaces_speed").toInt();
        const int wirelessSpeed = jsonObject.value("wireless_interfaces_speed").toInt();
        const int hasBattery = jsonObject.value("node_has_battery").toInt();

        // Add a new row to the table and populate the fields
        int rowCount = meshRoutersTable->rowCount();
        meshRoutersTable->insertRow(rowCount);
        meshRoutersTable->setItem(rowCount, 0, new QTableWidgetItem(routerType));
        meshRoutersTable->setItem(rowCount, 1, new QTableWidgetItem(hardware));
        meshRoutersTable->setItem(rowCount, 2, new QTableWidgetItem(QString::number(wirelessInterfaces)));
        meshRoutersTable->setItem(rowCount, 3, new QTableWidgetItem(QString::number(wiredInterfaces)));
        meshRoutersTable->setItem(rowCount, 4, new QTableWidgetItem(QString::number(wiredSpeed)));
        meshRoutersTable->setItem(rowCount, 5, new QTableWidgetItem(QString::number(wirelessSpeed)));
        meshRoutersTable->setItem(rowCount, 6, new QTableWidgetItem(hasBattery ? "Yes" : "No"));
    }

    reply->deleteLater();

    // Refresh the table data every 5 seconds



}

void MeshRouters::onAddRouterClicked()
{
    showAddRouterForm();
}

void MeshRouters::showAddRouterForm()
{
    // Create the widget for adding a new antenna
    addMeshRouterWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(addMeshRouterWidget);
    QFormLayout* formLayout = new QFormLayout;
    QLabel *routerTypeLabel = new QLabel("Router Type:");
    QLineEdit *routerTypeLineEdit = new QLineEdit;
    QLabel *hardwareLabel = new QLabel("Hardware:");
    QLineEdit *hardwareLineEdit = new QLineEdit;
    QLabel *wiredInterfacesCountLabel = new QLabel("Wired Interfaces Count:");
    QLineEdit *wiredInterfacesCountLineEdit = new QLineEdit;
    QLabel *wirelessInterfacesCountLabel = new QLabel("Wireless Interfaces Count:");
    QLineEdit *wirelessInterfacesCountLineEdit = new QLineEdit;
    QLabel *hasBatteryLabel = new QLabel("Has Battery:");
    QComboBox *hasBatteryComboBox = new QComboBox;
    hasBatteryComboBox->addItem("No");
    hasBatteryComboBox->addItem("Yes");
    QLabel *hasCellularSlotLabel = new QLabel("Has Cellular Slot:");
    QComboBox *hasCellularSlotComboBox = new QComboBox;
    hasCellularSlotComboBox->addItem("No");
    hasCellularSlotComboBox->addItem("Yes");
    QLabel *WLANIntegratedAntennaCountLabel = new QLabel("WLAN Integrated Antenna Count:");
    QLineEdit *WLANIntegratedAntennaCountLineEdit = new QLineEdit;
    QLabel *WLANIntegratedAntennaGainLabel= new QLabel("WLAN Integrated Antenna Gain:");
    QLineEdit *WLANIntegratedAntennaGainLineEdit = new QLineEdit;
    QLabel *WLANIntegratedAntennaVerticalAngleApertureLabel = new QLabel("WLAN Integrated Antenna Vertical Angle Aperture");
    QLineEdit *WLANIntegratedAntennaVerticalAngleApertureLineEdit = new QLineEdit;
    QLabel *WLANIntegratedAntennaHorizontalAngleApertureLabel = new QLabel("WLAN Integrated Antenna Horizontal Angle Aperture");
    QLineEdit *WLANIntegratedAntennaHorizontalAngleApertureLineEdit = new QLineEdit;
    QLabel *WLANInterfaceAntennaCountLabel = new QLabel("WLAN Interface Antenna Count:");
    QLineEdit *WLANInterfaceAntennaCountLineEdit = new QLineEdit;
    QLabel *cellularInterfaceAntennaCountLabel = new QLabel("Cellular Interface Antenna Count:");
    QLineEdit *cellularInterfaceAntennaCountLineEdit = new QLineEdit;
    QLabel *wiredInterfaceSpeedLabel = new QLabel("Wired Interface Speed:");
    QLineEdit *wiredInterfaceSpeedLineEdit=new QLineEdit;
    QLabel *wirelessInterfaceSpeedLabel = new QLabel("Wireless Interface Speed:");
    QLineEdit *wirelessInterfaceSpeedLineEdit=new QLineEdit;
    QLabel *cellularInterfaceTypeLabel= new QLabel("Cellular Interface Type");
    QLineEdit *cellularInterfaceTypeLineEdit=new QLineEdit;
    QLabel *cellularIntegratedAntennaCountLabel= new QLabel("Cellular Integrated Antenna Count");
    QLineEdit *cellularIntegratedAntennaCountLineEdit=new QLineEdit;
    QLabel *cellularIntegratedAntennaGainLabel= new QLabel("Cellular Integrated Antenna Gain");
    QLineEdit *cellularIntegratedAntennaGainLineEdit=new QLineEdit;
    QLabel *cellularUpLinkMaxLabel= new QLabel("Cellular Up Link Max");
    QLineEdit *cellularUpLinkMaxLineEdit=new QLineEdit;
    QLabel *cellularDownLinkMaxLabel= new QLabel("Cellular Down Link Max");
    QLineEdit *cellularDownLinkMaxLineEdit=new QLineEdit;

    // Adding the fields to the form layout
    //adding the fields to the form layout

    formLayout->addRow(routerTypeLabel, routerTypeLineEdit);
    formLayout->addRow(hardwareLabel, hardwareLineEdit);
    formLayout->addRow(wiredInterfacesCountLabel, wiredInterfacesCountLineEdit);
    formLayout->addRow(wirelessInterfacesCountLabel, wirelessInterfacesCountLineEdit);
    formLayout->addRow(hasBatteryLabel, hasBatteryComboBox);
    formLayout->addRow(hasCellularSlotLabel, hasCellularSlotComboBox);
    formLayout->addRow(WLANIntegratedAntennaCountLabel, WLANIntegratedAntennaCountLineEdit);
    formLayout->addRow(WLANIntegratedAntennaGainLabel, WLANIntegratedAntennaGainLineEdit);
    formLayout->addRow(WLANIntegratedAntennaVerticalAngleApertureLabel, WLANIntegratedAntennaVerticalAngleApertureLineEdit);
    formLayout->addRow(WLANIntegratedAntennaHorizontalAngleApertureLabel, WLANIntegratedAntennaHorizontalAngleApertureLineEdit);
    formLayout->addRow(WLANInterfaceAntennaCountLabel, WLANInterfaceAntennaCountLineEdit);
    formLayout->addRow(cellularInterfaceAntennaCountLabel, cellularInterfaceAntennaCountLineEdit);
    formLayout->addRow(wiredInterfaceSpeedLabel, wiredInterfaceSpeedLineEdit);
    formLayout->addRow(wirelessInterfaceSpeedLabel, wirelessInterfaceSpeedLineEdit);
    formLayout->addRow(cellularInterfaceTypeLabel, cellularInterfaceTypeLineEdit);
    formLayout->addRow(cellularIntegratedAntennaCountLabel, cellularIntegratedAntennaCountLineEdit);
    formLayout->addRow(cellularIntegratedAntennaGainLabel, cellularIntegratedAntennaGainLineEdit);
    formLayout->addRow(cellularUpLinkMaxLabel, cellularUpLinkMaxLineEdit);
    formLayout->addRow(cellularDownLinkMaxLabel, cellularDownLinkMaxLineEdit);


    // Add a scroll area
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(formLayout);
    scrollArea->setWidget(scrollAreaContent);
    layout->addWidget(scrollArea);

    QPushButton* submitButton = new QPushButton("Submit");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QIcon submitIcon(":/images/submitIcon.png");
    submitIcon = submitIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    submitButton->setIcon(submitIcon);
    QIcon cancelIcon(":/images/cancelIcon.png");
    cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    cancelButton->setIcon(cancelIcon);

    submitButton->setFixedWidth(200);
    cancelButton->setFixedWidth(200);
    submitButton->setObjectName("submitButton");
    cancelButton->setObjectName("cancelButton");


    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    // Connect the submit button to the onAddAntennaClicked() slot
    // Connect the submit button to the onAddRouterClicked() slot
    connect(submitButton, &QPushButton::clicked, [ routerTypeLineEdit, hardwareLineEdit,
            wiredInterfacesCountLineEdit, wirelessInterfacesCountLineEdit,
            hasBatteryComboBox,hasCellularSlotComboBox,WLANIntegratedAntennaCountLineEdit,
            WLANIntegratedAntennaGainLineEdit,WLANIntegratedAntennaVerticalAngleApertureLineEdit,
            WLANIntegratedAntennaHorizontalAngleApertureLineEdit,WLANInterfaceAntennaCountLineEdit,cellularInterfaceAntennaCountLineEdit,
            wiredInterfaceSpeedLineEdit,wirelessInterfaceSpeedLineEdit,cellularInterfaceTypeLineEdit,cellularUpLinkMaxLineEdit,cellularDownLinkMaxLineEdit,cellularIntegratedAntennaCountLineEdit,cellularIntegratedAntennaGainLineEdit,this]() {

        //preparing the request body elements
        QString routerType = routerTypeLineEdit->text();
        QString hardware = hardwareLineEdit->text();
        int wiredInterfacesCount = wiredInterfacesCountLineEdit->text().toInt();
        int wirelessInterfacesCount = wirelessInterfacesCountLineEdit->text().toInt();
        bool hasBattery = (hasBatteryComboBox->currentIndex() == 1);
        bool hasCellularSlot=(hasCellularSlotComboBox->currentIndex()==1);
        int WLANIntegratedAntennaCount= WLANIntegratedAntennaCountLineEdit->text().toInt();
        int WLANIntegratedAntennaGain = WLANIntegratedAntennaGainLineEdit->text().toInt();
        int WLANIntegratedAntennaVerticalAngleAperture = WLANIntegratedAntennaVerticalAngleApertureLineEdit->text().toInt();
        int WLANIntegratedAntennaHorizontalAngleAperture = WLANIntegratedAntennaHorizontalAngleApertureLineEdit->text().toInt();
        int WLANInterfaceAntennaCount=WLANInterfaceAntennaCountLineEdit->text().toInt();
        int cellularInterfaceAntennaCount=cellularInterfaceAntennaCountLineEdit->text().toInt();
        int wiredInterfaceSpeed=wiredInterfaceSpeedLineEdit->text().toInt();
        int wirelessInterfaceSpeed=wirelessInterfaceSpeedLineEdit->text().toInt();
        QString cellularInterfaceType=cellularInterfaceTypeLineEdit->text();
        int cellularUpLinkMax=cellularUpLinkMaxLineEdit->text().toInt();
        int cellularDownLinkMax=cellularDownLinkMaxLineEdit->text().toInt();
        int cellularIntegratedAntennaCount=cellularIntegratedAntennaCountLineEdit->text().toInt();
        int cellularIntegratedAntennaGain=cellularIntegratedAntennaGainLineEdit->text().toInt();


        // Create the request body JSON object
        QJsonObject requestBody;
        requestBody["node_type"] = routerType;
        requestBody["node_hw_model"] = hardware;
        requestBody["wired_interfaces_count"] = wiredInterfacesCount;
        requestBody["wireless_interfaces_count"] = wirelessInterfacesCount;
        requestBody["node_has_battery"] = hasBattery ? 1 : 0;
        requestBody["node_has_cellular_slot"]=hasCellularSlot ? 1 : 0;
        requestBody["WLAN_integrated_antenna_count"]=WLANIntegratedAntennaCount;
        requestBody["WLAN_integrated_antenna_gain"]=WLANIntegratedAntennaGain;
        requestBody["WLAN_integrated_antenna_vertical_angle_aperture"]=WLANIntegratedAntennaVerticalAngleAperture;
        requestBody["WLAN_integrated_antenna_horizontal_angle_aperture"]=WLANIntegratedAntennaHorizontalAngleAperture;
        requestBody["WLAN_interface_antenna_count"]=WLANInterfaceAntennaCount;
        requestBody["cellular_interface_antenna_count"]=cellularInterfaceAntennaCount;
        requestBody["wired_interfaces_speed"]=wiredInterfaceSpeed;
        requestBody["wireless_interfaces_speed"]=wirelessInterfaceSpeed;
        requestBody["cellular_interface_type"]=cellularInterfaceType;
        requestBody["cellular_uplink_max"]=cellularUpLinkMax;
        requestBody["cellular_downlink_max"]=cellularDownLinkMax;
        requestBody["cellular_integrated_antenna_count"]=cellularIntegratedAntennaCount;
        requestBody["cellular_integrated_antenna_gain"]=cellularIntegratedAntennaGain;


        // Add more fields to the JSON object as needed

        // Convert the JSON object to a QByteArray
        QJsonDocument jsonDoc(requestBody);
        QByteArray requestData = jsonDoc.toJson();

        // Make a POST request to the backend endpoint
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = manager.post(request, requestData);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            // Request successful
            qDebug() << "Antenna added successfully";
            // Refresh the table data
            populateTable();
        } else {
            // Request failed
            qDebug() << "Failed to add antenna:" << reply->errorString();
        }

        reply->deleteLater();
        addMeshRouterWidget->close();
        dockWidget->close();
        dockContent->close();
        meshRoutersLayout->removeItem(dockLayout);

    });

    connect(cancelButton, &QPushButton::clicked, addMeshRouterWidget, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockWidget, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockContent, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockContent, [this](){
        meshRoutersLayout->removeItem(dockLayout);


    });



    // Add the widget to the dock content
    dockContent->addWidget(addMeshRouterWidget);
    dockContent->setCurrentWidget(addMeshRouterWidget);

    // Show the dock widget

    displayDockWidget("Please provide Mesh Router information :");
}

void MeshRouters::onDeleteActionTriggered()
{
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this router?",
                                         QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::Yes)
    {
        QTableWidgetItem *currentItem = meshRoutersTable->currentItem();
        int rowIndex = meshRoutersTable->row(currentItem);
        QString routerType = meshRoutersTable->item(rowIndex, 0)->text();
        qDebug()<<"router deleted is :"<<routerType;
        // Assuming product ID is stored in the first column
        // Send a request to delete the product using the appropriate API endpoint
        // Example: localhost:3300/deleteProduct/:id

        // Update the table by removing the deleted row
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes/" + routerType));
        QNetworkReply *reply = manager.deleteResource(request);
        QEventLoop loop;
        connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
        loop.exec();

        if(reply->error()==QNetworkReply::NoError)
        {
            qDebug() <<"Router deleted succesfully";
            meshRoutersTable->removeRow(rowIndex);

        }else{

            qDebug() << "failed to delete router:"<< reply->errorString();

        }
        reply->deleteLater();





    }

}


void MeshRouters::onInfoActionTriggered()
{
    QTableWidgetItem* currentItem = meshRoutersTable->currentItem();
    int rowIndex = meshRoutersTable->row(currentItem);
    QString routerType = meshRoutersTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

    // Send a GET request to retrieve the information of the selected node
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes/" + routerType));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject nodeInfo = jsonResponse.object();

        // Extract the fields from the JSON response and display the information
        // Extract other fields as needed

        QString nodeType = nodeInfo.value("node_type").toString();
        QString nodeHwModel = nodeInfo.value("node_hw_model").toString();
        int wiredInterfacesCount = nodeInfo.value("wired_interfaces_count").toInt();
        int wirelessInterfacesCount = nodeInfo.value("wireless_interfaces_count").toInt();
        int nodeHasBattery = nodeInfo.value("node_has_battery").toInt();
        int nodeHasCellularSlot = nodeInfo.value("node_has_cellular_slot").toInt();
        int wlanIntegratedAntennaCount = nodeInfo.value("WLAN_integrated_antenna_count").toInt();
        int wlanIntegratedAntennaGain = nodeInfo.value("WLAN_integrated_antenna_gain").toInt();
        int wlanIntegratedAntennaVerticalAngleAperture = nodeInfo.value("WLAN_integrated_antenna_vertical_angle_aperture").toInt();
        int wlanIntegratedAntennaHorizontalAngleAperture = nodeInfo.value("WLAN_integrated_antenna_horizontal_angle_aperture").toInt();
        int wlanInterfaceAntennaCount = nodeInfo.value("WLAN_interface_antenna_count").toInt();
        int cellularInterfaceAntennaCount = nodeInfo.value("cellular_interface_antenna_count").toInt();
        int wiredInterfacesSpeed = nodeInfo.value("wired_interfaces_speed").toInt();
        int wirelessInterfacesSpeed = nodeInfo.value("wireless_interfaces_speed").toInt();
        QString cellularInterfaceType = nodeInfo.value("cellular_interface_type").toString();
        int cellularUplinkMax = nodeInfo.value("cellular_uplink_max").toInt();
        int cellularDownlinkMax = nodeInfo.value("cellular_downlink_max").toInt();
        int cellularIntegratedAntennaCount = nodeInfo.value("cellular_integrated_antenna_count").toInt();
        int cellularIntegratedAntennaGain = nodeInfo.value("cellular_integrated_antenna_gain").toInt();

        // Create the custom widget
        meshRouterInfoWidget= new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(meshRouterInfoWidget);
        QFormLayout* formLayout = new QFormLayout;

        formLayout->addRow("Node Type:", new QLabel(nodeType));
        formLayout->addRow("Hardware Model:", new QLabel(nodeHwModel));
        formLayout->addRow("Wired Interfaces Count:", new QLabel(QString::number(wiredInterfacesCount)));
        formLayout->addRow("Wireless Interfaces Count:", new QLabel(QString::number(wirelessInterfacesCount)));
        formLayout->addRow("Node Has Battery:", new QLabel(QString::number(nodeHasBattery)));
        formLayout->addRow("Node Has Cellular Slot:", new QLabel(QString::number(nodeHasCellularSlot)));
        formLayout->addRow("WLAN Integrated Antenna Count:", new QLabel(QString::number(wlanIntegratedAntennaCount)));
        formLayout->addRow("WLAN Integrated Antenna Gain:", new QLabel(QString::number(wlanIntegratedAntennaGain) + " dBi"));
        formLayout->addRow("WLAN Integrated Antenna Vertical Angle Aperture:", new QLabel(QString::number(wlanIntegratedAntennaVerticalAngleAperture) + " °"));
        formLayout->addRow("WLAN Integrated Antenna Horizontal Angle Aperture:", new QLabel(QString::number(wlanIntegratedAntennaHorizontalAngleAperture) + " °"));
        formLayout->addRow("WLAN Interface Antenna Count:", new QLabel(QString::number(wlanInterfaceAntennaCount)));
        formLayout->addRow("Cellular Interface Antenna Count:", new QLabel(QString::number(cellularInterfaceAntennaCount)));
        formLayout->addRow("Wired Interfaces Speed:", new QLabel(QString::number(wiredInterfacesSpeed) + " Mbps"));
        formLayout->addRow("Wireless Interfaces Speed:", new QLabel(QString::number(wirelessInterfacesSpeed) + " Mbps"));
        formLayout->addRow("Cellular Interface Type:", new QLabel(cellularInterfaceType));
        formLayout->addRow("Cellular Uplink Max:", new QLabel(QString::number(cellularUplinkMax) + " Mbps"));
        formLayout->addRow("Cellular Downlink Max:", new QLabel(QString::number(cellularDownlinkMax) + " Mbps"));
        formLayout->addRow("Cellular Integrated Antenna Count:", new QLabel(QString::number(cellularIntegratedAntennaCount)));
        formLayout->addRow("Cellular Integrated Antenna Gain:", new QLabel(QString::number(cellularIntegratedAntennaGain) + " dBi"));
        QScrollArea* scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget* scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);


        QPushButton* closeButton = new QPushButton("Close");
        closeButton->setObjectName("cancelButton");
        closeButton->setFixedWidth(200);

        QIcon closeIcon(":/images/closeIcon.png");
        closeIcon = closeIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        closeButton->setIcon(closeIcon);

        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);
        connect(closeButton, &QPushButton::clicked, meshRouterInfoWidget, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockWidget, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockContent, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockContent, [this](){
            meshRoutersLayout->removeItem(dockLayout);


        });
        // Add the widget to the dock content
        dockContent->addWidget(meshRouterInfoWidget);
        dockContent->setCurrentWidget(meshRouterInfoWidget);

        // Show the dock widget
        displayDockWidget("Router Informations: ");

    }
    else
    {
        // Request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + reply->errorString());
    }

    reply->deleteLater();
}


void MeshRouters::onModifyActionTriggered()
{

    QTableWidgetItem* currentItem = meshRoutersTable->currentItem();
    int rowIndex = meshRoutersTable->row(currentItem);
    QString routerType = meshRoutersTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

    // Send a GET request to retrieve the information of the selected node
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes/" + routerType));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject nodeInfo = jsonResponse.object();

        // Create a QWidget to display the modify dialog
        meshRouterModifyWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(meshRouterModifyWidget);
        QFormLayout *formLayout = new QFormLayout;


        // Create labels and line edit fields for each node information

        QLabel *nodeTypeLabel = new QLabel("Node Type:");
        QLineEdit *nodeTypeLineEdit = new QLineEdit(nodeInfo.value("node_type").toString());
        formLayout->addRow(nodeTypeLabel,nodeTypeLineEdit);

        QLabel* nodeHWModelLabel = new QLabel("Node HW Model:");
        QLineEdit* nodeHWModelLineEdit = new QLineEdit(nodeInfo.value("node_hw_model").toString());
        //        layout.addWidget(nodeHWModelLabel);
        //        layout.addWidget(nodeHWModelLineEdit);
        formLayout->addRow(nodeHWModelLabel,nodeHWModelLineEdit);

        QLabel* wiredInterfacesCountLabel = new QLabel("Wired Interfaces Count:");
        QSpinBox * wiredInterfacesCountSpinBox = new QSpinBox(meshRouterModifyWidget);
        wiredInterfacesCountSpinBox->setValue(nodeInfo.value("wired_interfaces_count").toInt());

        //        layout.addWidget(wiredInterfacesCountLabel);
        //        layout.addWidget(wiredInterfacesCountSpinBox);
        formLayout->addRow(wiredInterfacesCountLabel,wiredInterfacesCountSpinBox);


        QLabel* wirelessInterfacesCountLabel = new QLabel("Wireless Interfaces Count:");
        QSpinBox* wirelessInterfacesCountSpinBox = new QSpinBox(meshRouterModifyWidget);
        wirelessInterfacesCountSpinBox->setValue(nodeInfo["wireless_interfaces_count"].toInt());
        //        layout.addWidget(wirelessInterfacesCountLabel);
        //        layout.addWidget(wirelessInterfacesCountSpinBox);
        formLayout->addRow(wirelessInterfacesCountLabel,wirelessInterfacesCountSpinBox);


        QLabel* nodeHasBatteryLabel = new QLabel("Node Has Battery:");
        QCheckBox* nodeHasBatteryCheckBox = new QCheckBox(meshRouterModifyWidget);
        nodeHasBatteryCheckBox->setChecked(nodeInfo["node_has_battery"].toBool());
        //        layout.addWidget(nodeHasBatteryLabel);
        //        layout.addWidget(nodeHasBatteryCheckBox);
        formLayout->addRow(nodeHasBatteryLabel,nodeHasBatteryCheckBox);

        QLabel* nodeHasCellularSlotLabel=new QLabel("Node Has Cellular Slot:");
        QCheckBox* nodeHasCellularSlotCheckBox = new QCheckBox(meshRouterModifyWidget);
        nodeHasCellularSlotCheckBox->setChecked(nodeInfo["node_has_cellular_slot"].toBool());
        //        layout.addWidget(nodeHasCellularSlotLabel);
        //        layout.addWidget(nodeHasCellularSlotCheckBox);
        formLayout->addRow(nodeHasCellularSlotLabel,nodeHasCellularSlotCheckBox);


        QLabel* WLANIntegratedAntennaCountLabel= new QLabel("WLAN Integrated Antenna Count:");
        QSpinBox* WLANIntegratedAntennaCountSpinBox = new QSpinBox(meshRouterModifyWidget);
        WLANIntegratedAntennaCountSpinBox->setValue(nodeInfo["WLAN_integrated_antenna_count"].toInt());
        //        layout.addWidget(WLANIntegratedAntennaCountLabel);
        //        layout.addWidget(WLANIntegratedAntennaCountSpinBox);
        formLayout->addRow(WLANIntegratedAntennaCountLabel,WLANIntegratedAntennaCountSpinBox);

        QLabel* WLANIntegratedAntennaGainLabel = new QLabel("WLAN Integrated Antenna Gain:");
        QSpinBox* WLANIntegratedAntennaGainSpinBox = new QSpinBox(meshRouterModifyWidget);
        WLANIntegratedAntennaGainSpinBox->setValue(nodeInfo["WLAN_integrated_antenna_gain"].toInt());
        WLANIntegratedAntennaGainSpinBox->setRange(0,10000);
        //        layout.addWidget(WLANIntegratedAntennaGainLabel);
        //        layout.addWidget(WLANIntegratedAntennaGainSpinBox);
        formLayout->addRow(WLANIntegratedAntennaGainLabel,WLANIntegratedAntennaGainSpinBox);


        QLabel* WLANIntegratedAntennaVerticalAngleApertureLabel = new QLabel("WLAN Integrated Antenna Vertical Angle Aperture:");
        QSpinBox* WLANIntegratedAntennaVerticalAngleApertureSpinBox = new QSpinBox(meshRouterModifyWidget);
        WLANIntegratedAntennaVerticalAngleApertureSpinBox->setValue(nodeInfo["WLAN_integrated_antenna_vertical_angle_aperture"].toInt());
        WLANIntegratedAntennaVerticalAngleApertureSpinBox->setRange(0,360);

        //        layout.addWidget(WLANIntegratedAntennaVerticalAngleApertureLabel);
        //        layout.addWidget(WLANIntegratedAntennaVerticalAngleApertureSpinBox);
        formLayout->addRow(WLANIntegratedAntennaVerticalAngleApertureLabel,WLANIntegratedAntennaVerticalAngleApertureSpinBox);


        QLabel* WLANIntegratedAntennaHorizontalAngleApertureLabel = new QLabel("WLAN Integrated Antenna Horizontal Angle Aperture:");
        QSpinBox* WLANIntegratedAntennaHorizontalAngleApertureSpinBox = new QSpinBox(meshRouterModifyWidget);
        WLANIntegratedAntennaHorizontalAngleApertureSpinBox->setValue(nodeInfo["WLAN_integrated_antenna_horizontal_angle_aperture"].toInt());
        WLANIntegratedAntennaHorizontalAngleApertureSpinBox->setRange(0,360);
        //        layout.addWidget(WLANIntegratedAntennaHorizontalAngleApertureLabel);
        //        layout.addWidget(WLANIntegratedAntennaHorizontalAngleApertureSpinBox);
        formLayout->addRow(WLANIntegratedAntennaHorizontalAngleApertureLabel,WLANIntegratedAntennaHorizontalAngleApertureSpinBox);


        QLabel* WLANInterfaceAntennaCountLabel = new QLabel("WLAN Interface Antenna Count:");
        QSpinBox* WLANInterfaceAntennaCountSpinBox = new QSpinBox(meshRouterModifyWidget);
        WLANInterfaceAntennaCountSpinBox->setValue(nodeInfo["WLAN_interface_antenna_count"].toInt());
        //        layout.addWidget(WLANInterfaceAntennaCountLabel);
        //        layout.addWidget(WLANInterfaceAntennaCountSpinBox);
        formLayout->addRow(WLANInterfaceAntennaCountLabel,WLANInterfaceAntennaCountSpinBox);


        QLabel* cellularInterfaceAntennaCountLabel = new QLabel("Cellular Interface Antenna Count:");
        QSpinBox* cellularInterfaceAntennaCountSpinBox = new QSpinBox(meshRouterModifyWidget);
        cellularInterfaceAntennaCountSpinBox->setValue(nodeInfo["cellular_interface_antenna_count"].toInt());
        //        layout.addWidget(cellularInterfaceAntennaCountLabel);
        //        layout.addWidget(cellularInterfaceAntennaCountSpinBox);
        formLayout->addRow(cellularInterfaceAntennaCountLabel,cellularInterfaceAntennaCountSpinBox);


        QLabel* wiredInterfacesSpeedLabel = new QLabel("Wired Interfaces Speed:");
        QSpinBox* wiredInterfacesSpeedSpinBox = new QSpinBox(meshRouterModifyWidget);
        wiredInterfacesSpeedSpinBox->setValue(nodeInfo["wired_interfaces_speed"].toInt());
        wiredInterfacesSpeedSpinBox->setRange(0,99999999);
        //        layout.addWidget(wiredInterfacesSpeedLabel);
        //        layout.addWidget(wiredInterfacesSpeedSpinBox);
        formLayout->addRow(wiredInterfacesSpeedLabel,wiredInterfacesSpeedSpinBox);


        QLabel* wirelessInterfacesSpeedLabel = new QLabel("Wireless Interfaces Speed:");
        QSpinBox* wirelessInterfacesSpeedSpinBox = new QSpinBox(meshRouterModifyWidget);
        wirelessInterfacesSpeedSpinBox->setValue(nodeInfo["wireless_interfaces_speed"].toInt());
        wirelessInterfacesSpeedSpinBox->setRange(0,99999999);
        //        layout.addWidget(wirelessInterfacesSpeedLabel);
        //        layout.addWidget(wirelessInterfacesSpeedSpinBox);
        formLayout->addRow(wirelessInterfacesSpeedLabel,wirelessInterfacesSpeedSpinBox);


        QLabel* cellularInterfaceTypeLabel = new QLabel("Cellular Interface Type:");
        QLineEdit *cellularInterfaceTypeLineEdit = new QLineEdit(nodeInfo.value("cellular_interface_type").toString());

        //        QComboBox* cellularInterfaceTypeComboBox = new QComboBox(&modifyDialog);
        //        cellularInterfaceTypeComboBox->addItem("Type 1");
        //        cellularInterfaceTypeComboBox->addItem("Type 2");
        //        cellularInterfaceTypeComboBox->setCurrentIndex(nodeInfo["cellular_interface_type"].toInt());
        //        layout.addWidget(cellularInterfaceTypeLabel);
        //        layout.addWidget(cellularInterfaceTypeComboBox);
        formLayout->addRow(cellularInterfaceTypeLabel,cellularInterfaceTypeLineEdit);


        QLabel* cellularUplinkMaxLabel = new QLabel("Cellular Uplink Max:");
        QSpinBox* cellularUplinkMaxSpinBox = new QSpinBox(meshRouterModifyWidget);
        cellularUplinkMaxSpinBox->setValue(nodeInfo["cellular_uplink_max"].toInt());
        cellularUplinkMaxSpinBox->setRange(0,9999999);
        //        layout.addWidget(cellularUplinkMaxLabel);
        //        layout.addWidget(cellularUplinkMaxSpinBox);
        formLayout->addRow(cellularUplinkMaxLabel,cellularUplinkMaxSpinBox);


        QLabel* cellularDownlinkMaxLabel = new QLabel("Cellular Downlink Max:");
        QSpinBox* cellularDownlinkMaxSpinBox = new QSpinBox(meshRouterModifyWidget);
        cellularDownlinkMaxSpinBox->setValue(nodeInfo["cellular_downlink_max"].toInt());
        cellularDownlinkMaxSpinBox->setRange(0,99999999);
        //        layout.addWidget(cellularDownlinkMaxLabel);
        //        layout.addWidget(cellularDownlinkMaxSpinBox);
        formLayout->addRow(cellularDownlinkMaxLabel,cellularDownlinkMaxSpinBox);


        QLabel* cellularIntegratedAntennaCountLabel = new QLabel("Cellular Integrated Antenna Count:");
        QSpinBox* cellularIntegratedAntennaCountSpinBox = new QSpinBox(meshRouterModifyWidget);
        cellularIntegratedAntennaCountSpinBox->setValue(nodeInfo["cellular_integrated_antenna_count"].toInt());
        //        layout.addWidget(cellularIntegratedAntennaCountLabel);
        //        layout.addWidget(cellularIntegratedAntennaCountSpinBox);
        formLayout->addRow(cellularIntegratedAntennaCountLabel,cellularIntegratedAntennaCountSpinBox);



        QLabel* cellularIntegratedAntennaGainLabel = new QLabel("Cellular Integrated Antenna Gain:");
        QSpinBox* cellularIntegratedAntennaGainSpinBox = new QSpinBox(meshRouterModifyWidget);
        cellularIntegratedAntennaGainSpinBox->setValue(nodeInfo["cellular_integrated_antenna_gain"].toInt());
        cellularIntegratedAntennaGainSpinBox->setRange(0,99999999);
        //        layout.addWidget(cellularIntegratedAntennaGainLabel);
        //        layout.addWidget(cellularIntegratedAntennaGainSpinBox);
        formLayout->addRow(cellularIntegratedAntennaGainLabel,cellularIntegratedAntennaGainSpinBox);

        // Add the form layout to the scroll area
        QScrollArea* scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget* scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);



        // Create a QPushButton for the Save button
        QPushButton* saveButton = new QPushButton("Save", meshRouterModifyWidget);
        QPushButton* cancelButton = new QPushButton("Cancel");
        QIcon submitIcon(":/images/submitIcon.png");
        submitIcon = submitIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        saveButton->setIcon(submitIcon);
        QIcon cancelIcon(":/images/cancelIcon.png");
        cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        cancelButton->setIcon(cancelIcon);

        saveButton->setObjectName("submitButton");
        cancelButton->setObjectName("cancelButton");
        saveButton->setFixedWidth(200);
        cancelButton->setFixedWidth(200);
        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(cancelButton);
        //        layout->addLayout(formLayout);
        layout->addLayout(buttonLayout);


        connect(cancelButton, &QPushButton::clicked, meshRouterModifyWidget, &QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockWidget,&QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockContent,&QWidget::close);
        connect(cancelButton, &QPushButton::clicked, dockContent, [this](){
            meshRoutersLayout->removeItem(dockLayout);


        });


        // Connect the Save button to the onSaveButtonClicked slot
        connect(saveButton, &QPushButton::clicked, this, [this,nodeInfo, nodeTypeLineEdit,nodeHWModelLineEdit,wiredInterfacesCountSpinBox,wirelessInterfacesCountSpinBox,
                nodeHasBatteryCheckBox,nodeHasCellularSlotCheckBox,WLANIntegratedAntennaCountSpinBox,WLANIntegratedAntennaGainSpinBox,WLANIntegratedAntennaVerticalAngleApertureSpinBox,WLANIntegratedAntennaHorizontalAngleApertureSpinBox,
                WLANInterfaceAntennaCountSpinBox,cellularInterfaceAntennaCountSpinBox,wiredInterfacesSpeedSpinBox,wirelessInterfacesSpeedSpinBox
                ,cellularInterfaceTypeLineEdit,cellularUplinkMaxSpinBox,cellularDownlinkMaxSpinBox,cellularIntegratedAntennaCountSpinBox,cellularIntegratedAntennaGainSpinBox]() {
            // Extract the modified fields from the modify dialog's line edit fields
            // Example: QString modifiedName = nameLineEdit->text();
            QString modifiedNodeType = nodeTypeLineEdit->text();
            QString modifiedNodeHWModel = nodeHWModelLineEdit->text();
            int modifiedWiredInterfacesCount = wiredInterfacesCountSpinBox->value();
            int modifiedWirelessInterfacesCount = wirelessInterfacesCountSpinBox->value();
            int modifiedNodeHasBattery = nodeHasBatteryCheckBox->isChecked() ? 1 : 0;
            int modifiedNodeHasCellularSlot = nodeHasCellularSlotCheckBox->isChecked() ? 1 : 0;
            int modifiedWLANIntegratedAntennaCount = WLANIntegratedAntennaCountSpinBox->value();
            int modifiedWLANIntegratedAntennaGain = WLANIntegratedAntennaGainSpinBox->value();
            int modifiedWLANIntegratedAntennaVerticalAngleAperture = WLANIntegratedAntennaVerticalAngleApertureSpinBox->value();
            int modifiedWLANIntegratedAntennaHorizontalAngleAperture = WLANIntegratedAntennaHorizontalAngleApertureSpinBox->value();
            int modifiedWLANInterfaceAntennaCount = WLANInterfaceAntennaCountSpinBox->value();
            int modifiedCellularInterfaceAntennaCount = cellularInterfaceAntennaCountSpinBox->value();
            int modifiedWiredInterfacesSpeed = wiredInterfacesSpeedSpinBox->value();
            int modifiedWirelessInterfacesSpeed = wirelessInterfacesSpeedSpinBox->value();
            QString modifiedCellularInterfaceType = cellularInterfaceTypeLineEdit->text();
            int modifiedCellularUplinkMax = cellularUplinkMaxSpinBox->value();
            int modifiedCellularDownlinkMax = cellularDownlinkMaxSpinBox->value();
            int modifiedCellularIntegratedAntennaCount = cellularIntegratedAntennaCountSpinBox->value();
            int modifiedCellularIntegratedAntennaGain = cellularIntegratedAntennaGainSpinBox->value();




            // Construct the modified fields in a QJsonObject
            QJsonObject modifiedFields;
            // Example: modifiedFields["name"] = modifiedName;
            modifiedFields["node_type"] = modifiedNodeType;
            modifiedFields["node_hw_model"] = modifiedNodeHWModel;
            modifiedFields["wired_interfaces_count"] = modifiedWiredInterfacesCount;
            modifiedFields["wireless_interfaces_count"] = modifiedWirelessInterfacesCount;
            modifiedFields["node_has_battery"] = modifiedNodeHasBattery;
            modifiedFields["node_has_cellular_slot"] = modifiedNodeHasCellularSlot;
            modifiedFields["WLAN_integrated_antenna_count"] = modifiedWLANIntegratedAntennaCount;
            modifiedFields["WLAN_integrated_antenna_gain"] = modifiedWLANIntegratedAntennaGain;
            modifiedFields["WLAN_integrated_antenna_vertical_angle_aperture"] = modifiedWLANIntegratedAntennaVerticalAngleAperture;
            modifiedFields["WLAN_integrated_antenna_horizontal_angle_aperture"] = modifiedWLANIntegratedAntennaHorizontalAngleAperture;
            modifiedFields["WLAN_interface_antenna_count"] = modifiedWLANInterfaceAntennaCount;
            modifiedFields["cellular_interface_antenna_count"] = modifiedCellularInterfaceAntennaCount;
            modifiedFields["wired_interfaces_speed"] = modifiedWiredInterfacesSpeed;
            modifiedFields["wireless_interfaces_speed"] = modifiedWirelessInterfacesSpeed;
            modifiedFields["cellular_interface_type"] = modifiedCellularInterfaceType;
            modifiedFields["cellular_uplink_max"] = modifiedCellularUplinkMax;
            modifiedFields["cellular_downlink_max"] = modifiedCellularDownlinkMax;
            modifiedFields["cellular_integrated_antenna_count"] = modifiedCellularIntegratedAntennaCount;
            modifiedFields["cellular_integrated_antenna_gain"] = modifiedCellularIntegratedAntennaGain;



            // Create the request body JSON object
            QJsonObject requestBody;
            // Add the modified fields to the request body
            requestBody= modifiedFields;

            qDebug()<<"body:"<<requestBody;

            // Convert the JSON object to a QByteArray
            QJsonDocument jsonDoc(requestBody);
            QByteArray requestData = jsonDoc.toJson();

            // Send a PUT request to update the node information
            QNetworkAccessManager manager;
            QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes/" + nodeInfo.value("node_type").toString()));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply* reply = manager.put(request, requestData);
            QEventLoop loop;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            if (reply->error() == QNetworkReply::NoError) {
                // Request successful
                QMessageBox::information(this, "Success", "Antenna information modified successfully!");
                dockContent->removeWidget(meshRouterModifyWidget); // Remove the modify widget from the dock
                meshRouterModifyWidget->deleteLater(); // Delete the modify widget
            } else {
                // Request failed
                QMessageBox::critical(this, "Error", "Failed to modify antenna information: " + reply->errorString());
                dockContent->removeWidget(meshRouterModifyWidget); // Remove the modify widget from the dock

                meshRouterModifyWidget->deleteLater();
            }

            reply->deleteLater();
            reply->deleteLater();
            meshRouterModifyWidget->close();
            dockWidget->close();
            dockContent->close();
            meshRoutersLayout->removeItem(dockLayout);



        });

        // Set the layout and add the modify widget to the dock content
        meshRouterModifyWidget->setLayout(layout);
        dockContent->addWidget(meshRouterModifyWidget);
        dockContent->setCurrentWidget(meshRouterModifyWidget);

        // Show the dock widget
        displayDockWidget("Edit Mesh Router informations: ");
    }
    else
    {
        // Request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + reply->errorString());
    }

    reply->deleteLater();
}


//old version
//void MeshRouters::onInfoActionTriggered()
//{
//    QTableWidgetItem* currentItem = routerTable->currentItem();
//    int rowIndex = routerTable->row(currentItem);
//    QString routerType = routerTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

//    // Send a GET request to retrieve the information of the selected node
//    QNetworkAccessManager manager;
//    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes/" + routerType));
//    QNetworkReply* reply = manager.get(getRequest);
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();

//    if (reply->error() == QNetworkReply::NoError)
//    {
//        // Request successful
//        QByteArray responseData = reply->readAll();
//        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
//        QJsonObject nodeInfo = jsonResponse.object();

//        // Extract the fields from the JSON response and display the information
//        // Extract other fields as needed

//        QString nodeType = nodeInfo.value("node_type").toString();
//        QString nodeHwModel = nodeInfo.value("node_hw_model").toString();
//        int wiredInterfacesCount = nodeInfo.value("wired_interfaces_count").toInt();
//        int wirelessInterfacesCount = nodeInfo.value("wireless_interfaces_count").toInt();
//        int nodeHasBattery = nodeInfo.value("node_has_battery").toInt();
//        int nodeHasCellularSlot = nodeInfo.value("node_has_cellular_slot").toInt();
//        int wlanIntegratedAntennaCount = nodeInfo.value("WLAN_integrated_antenna_count").toInt();
//        int wlanIntegratedAntennaGain = nodeInfo.value("WLAN_integrated_antenna_gain").toInt();
//        int wlanIntegratedAntennaVerticalAngleAperture = nodeInfo.value("WLAN_integrated_antenna_vertical_angle_aperture").toInt();
//        int wlanIntegratedAntennaHorizontalAngleAperture = nodeInfo.value("WLAN_integrated_antenna_horizontal_angle_aperture").toInt();
//        int wlanInterfaceAntennaCount = nodeInfo.value("WLAN_interface_antenna_count").toInt();
//        int cellularInterfaceAntennaCount = nodeInfo.value("cellular_interface_antenna_count").toInt();
//        int wiredInterfacesSpeed = nodeInfo.value("wired_interfaces_speed").toInt();
//        int wirelessInterfacesSpeed = nodeInfo.value("wireless_interfaces_speed").toInt();
//        QString cellularInterfaceType = nodeInfo.value("cellular_interface_type").toString();
//        int cellularUplinkMax = nodeInfo.value("cellular_uplink_max").toInt();
//        int cellularDownlinkMax = nodeInfo.value("cellular_downlink_max").toInt();
//        int cellularIntegratedAntennaCount = nodeInfo.value("cellular_integrated_antenna_count").toInt();
//        int cellularIntegratedAntennaGain = nodeInfo.value("cellular_integrated_antenna_gain").toInt();



//        // Display the information using QMessageBox or custom dialog

//        QMessageBox::information(this, "Node Information",
//                                 "Node Type: " + nodeType + "\n"
//                                                            "Hardware Model: " + nodeHwModel + "\n"
//                                                                                               "Wired Interfaces Count: " + QString::number(wiredInterfacesCount) + "\n"
//                                                                                                                                                                    "Wireless Interfaces Count: " + QString::number(wirelessInterfacesCount) + "\n"
//                                                                                                                                                                                                                                               "Node Has Battery: " + QString::number(nodeHasBattery) + "\n"
//                                                                                                                                                                                                                                                                                                        "Node Has Cellular Slot: " + QString::number(nodeHasCellularSlot) + "\n"
//                                                                                                                                                                                                                                                                                                                                                                            "WLAN Integrated Antenna Count: " + QString::number(wlanIntegratedAntennaCount) + "\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                              "WLAN Integrated Antenna Gain: " + QString::number(wlanIntegratedAntennaGain) + " dBi\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              "WLAN Integrated Antenna Vertical Angle Aperture: " + QString::number(wlanIntegratedAntennaVerticalAngleAperture) + " °\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  "WLAN Integrated Antenna Horizontal Angle Aperture: " + QString::number(wlanIntegratedAntennaHorizontalAngleAperture) + " °\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          "WLAN Interface Antenna Count: " + QString::number(wlanInterfaceAntennaCount) + "\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          "Cellular Interface Antenna Count: " + QString::number(cellularInterfaceAntennaCount) + "\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  "Wired Interfaces Speed: " + QString::number(wiredInterfacesSpeed) + " Mbps\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       "Wireless Interfaces Speed: " + QString::number(wirelessInterfacesSpeed) + " Mbps\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  "Cellular Interface Type: " + cellularInterfaceType + "\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        "Cellular Uplink Max: " + QString::number(cellularUplinkMax) + " Mbps\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       "Cellular Downlink Max: " + QString::number(cellularDownlinkMax) + " Mbps\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          "Cellular Integrated Antenna Count: " + QString::number(cellularIntegratedAntennaCount) + "\n"
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    "Cellular Integrated Antenna Gain: " + QString::number(cellularIntegratedAntennaGain) + " dBi\n"
//                                 );


//    }
//    else
//    {
//        // Request failed
//        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + reply->errorString());
//    }

//    reply->deleteLater();

//}

//void MeshRouters::showAddRouterForm()
//{
//    // Create the dialog for adding a new router
//    QDialog addRouterDialog;

//    addRouterDialog.setWindowTitle("Add Mesh Router"); // Set the dialog title

//    QVBoxLayout layout(&addRouterDialog);
//    QFormLayout *formLayout = new QFormLayout;

//    QLabel *routerTypeLabel = new QLabel("Router Type:");
//    QLineEdit *routerTypeLineEdit = new QLineEdit;
//    QLabel *hardwareLabel = new QLabel("Hardware:");
//    QLineEdit *hardwareLineEdit = new QLineEdit;
//    QLabel *wiredInterfacesCountLabel = new QLabel("Wired Interfaces Count:");
//    QLineEdit *wiredInterfacesCountLineEdit = new QLineEdit;
//    QLabel *wirelessInterfacesCountLabel = new QLabel("Wireless Interfaces Count:");
//    QLineEdit *wirelessInterfacesCountLineEdit = new QLineEdit;
//    QLabel *hasBatteryLabel = new QLabel("Has Battery:");
//    QComboBox *hasBatteryComboBox = new QComboBox;
//    hasBatteryComboBox->addItem("No");
//    hasBatteryComboBox->addItem("Yes");
//    QLabel *hasCellularSlotLabel = new QLabel("Has Cellular Slot:");
//    QComboBox *hasCellularSlotComboBox = new QComboBox;
//    hasCellularSlotComboBox->addItem("No");
//    hasCellularSlotComboBox->addItem("Yes");
//    QLabel *WLANIntegratedAntennaCountLabel = new QLabel("WLAN Integrated Antenna Count:");
//    QLineEdit *WLANIntegratedAntennaCountLineEdit = new QLineEdit;
//    QLabel *WLANIntegratedAntennaGainLabel= new QLabel("WLAN Integrated Antenna Gain:");
//    QLineEdit *WLANIntegratedAntennaGainLineEdit = new QLineEdit;
//    QLabel *WLANIntegratedAntennaVerticalAngleApertureLabel = new QLabel("WLAN Integrated Antenna Vertical Angle Aperture");
//    QLineEdit *WLANIntegratedAntennaVerticalAngleApertureLineEdit = new QLineEdit;
//    QLabel *WLANIntegratedAntennaHorizontalAngleApertureLabel = new QLabel("WLAN Integrated Antenna Horizontal Angle Aperture");
//    QLineEdit *WLANIntegratedAntennaHorizontalAngleApertureLineEdit = new QLineEdit;
//    QLabel *WLANInterfaceAntennaCountLabel = new QLabel("WLAN Interface Antenna Count:");
//    QLineEdit *WLANInterfaceAntennaCountLineEdit = new QLineEdit;
//    QLabel *cellularInterfaceAntennaCountLabel = new QLabel("Cellular Interface Antenna Count:");
//    QLineEdit *cellularInterfaceAntennaCountLineEdit = new QLineEdit;
//    QLabel *wiredInterfaceSpeedLabel = new QLabel("Wired Interface Speed:");
//    QLineEdit *wiredInterfaceSpeedLineEdit=new QLineEdit;
//    QLabel *wirelessInterfaceSpeedLabel = new QLabel("Wireless Interface Speed:");
//    QLineEdit *wirelessInterfaceSpeedLineEdit=new QLineEdit;
//    QLabel *cellularInterfaceTypeLabel= new QLabel("Cellular Interface Type");
//    QLineEdit *cellularInterfaceTypeLineEdit=new QLineEdit;
//    QLabel *cellularIntegratedAntennaCountLabel= new QLabel("Cellular Integrated Antenna Count");
//    QLineEdit *cellularIntegratedAntennaCountLineEdit=new QLineEdit;
//    QLabel *cellularIntegratedAntennaGainLabel= new QLabel("Cellular Integrated Antenna Gain");
//    QLineEdit *cellularIntegratedAntennaGainLineEdit=new QLineEdit;
//    QLabel *cellularUpLinkMaxLabel= new QLabel("Cellular Up Link Max");
//    QLineEdit *cellularUpLinkMaxLineEdit=new QLineEdit;
//    QLabel *cellularDownLinkMaxLabel= new QLabel("Cellular Down Link Max");
//    QLineEdit *cellularDownLinkMaxLineEdit=new QLineEdit;

//    //adding the fields to the form layout

//    formLayout->addRow(routerTypeLabel, routerTypeLineEdit);
//    formLayout->addRow(hardwareLabel, hardwareLineEdit);
//    formLayout->addRow(wiredInterfacesCountLabel, wiredInterfacesCountLineEdit);
//    formLayout->addRow(wirelessInterfacesCountLabel, wirelessInterfacesCountLineEdit);
//    formLayout->addRow(hasBatteryLabel, hasBatteryComboBox);
//    formLayout->addRow(hasCellularSlotLabel, hasCellularSlotComboBox);
//    formLayout->addRow(WLANIntegratedAntennaCountLabel, WLANIntegratedAntennaCountLineEdit);
//    formLayout->addRow(WLANIntegratedAntennaGainLabel, WLANIntegratedAntennaGainLineEdit);
//    formLayout->addRow(WLANIntegratedAntennaVerticalAngleApertureLabel, WLANIntegratedAntennaVerticalAngleApertureLineEdit);
//    formLayout->addRow(WLANIntegratedAntennaHorizontalAngleApertureLabel, WLANIntegratedAntennaHorizontalAngleApertureLineEdit);
//    formLayout->addRow(WLANInterfaceAntennaCountLabel, WLANInterfaceAntennaCountLineEdit);
//    formLayout->addRow(cellularInterfaceAntennaCountLabel, cellularInterfaceAntennaCountLineEdit);
//    formLayout->addRow(wiredInterfaceSpeedLabel, wiredInterfaceSpeedLineEdit);
//    formLayout->addRow(wirelessInterfaceSpeedLabel, wirelessInterfaceSpeedLineEdit);
//    formLayout->addRow(cellularInterfaceTypeLabel, cellularInterfaceTypeLineEdit);
//    formLayout->addRow(cellularIntegratedAntennaCountLabel, cellularIntegratedAntennaCountLineEdit);
//    formLayout->addRow(cellularIntegratedAntennaGainLabel, cellularIntegratedAntennaGainLineEdit);
//    formLayout->addRow(cellularUpLinkMaxLabel, cellularUpLinkMaxLineEdit);
//    formLayout->addRow(cellularDownLinkMaxLabel, cellularDownLinkMaxLineEdit);

//    //adding a scroll area
//    QScrollArea scrollArea;
//    scrollArea.setWidgetResizable(true);
//    QWidget *scrollAreaContent = new QWidget;
//    scrollAreaContent->setLayout(formLayout);
//    scrollArea.setWidget(scrollAreaContent);
//    layout.addWidget(&scrollArea);



//    QPushButton *submitButton = new QPushButton("Submit");
//    QPushButton *cancelButton = new QPushButton("Cancel");

//    QHBoxLayout *buttonLayout = new QHBoxLayout;




//    buttonLayout->addWidget(submitButton);
//    buttonLayout->addWidget(cancelButton);
//    layout.addLayout(buttonLayout);

//    addRouterDialog.setLayout(&layout);


//    // Connect the submit button to the onAddRouterClicked() slot
//    connect(submitButton, &QPushButton::clicked, [&addRouterDialog, routerTypeLineEdit, hardwareLineEdit,
//            wiredInterfacesCountLineEdit, wirelessInterfacesCountLineEdit,
//            hasBatteryComboBox,hasCellularSlotComboBox,WLANIntegratedAntennaCountLineEdit,
//            WLANIntegratedAntennaGainLineEdit,WLANIntegratedAntennaVerticalAngleApertureLineEdit,
//            WLANIntegratedAntennaHorizontalAngleApertureLineEdit,WLANInterfaceAntennaCountLineEdit,cellularInterfaceAntennaCountLineEdit,
//            wiredInterfaceSpeedLineEdit,wirelessInterfaceSpeedLineEdit,cellularInterfaceTypeLineEdit,cellularUpLinkMaxLineEdit,cellularDownLinkMaxLineEdit,cellularIntegratedAntennaCountLineEdit,cellularIntegratedAntennaGainLineEdit,this]() {

//        //preparing the request body elements
//        QString routerType = routerTypeLineEdit->text();
//        QString hardware = hardwareLineEdit->text();
//        int wiredInterfacesCount = wiredInterfacesCountLineEdit->text().toInt();
//        int wirelessInterfacesCount = wirelessInterfacesCountLineEdit->text().toInt();
//        bool hasBattery = (hasBatteryComboBox->currentIndex() == 1);
//        bool hasCellularSlot=(hasCellularSlotComboBox->currentIndex()==1);
//        int WLANIntegratedAntennaCount= WLANIntegratedAntennaCountLineEdit->text().toInt();
//        int WLANIntegratedAntennaGain = WLANIntegratedAntennaGainLineEdit->text().toInt();
//        int WLANIntegratedAntennaVerticalAngleAperture = WLANIntegratedAntennaVerticalAngleApertureLineEdit->text().toInt();
//        int WLANIntegratedAntennaHorizontalAngleAperture = WLANIntegratedAntennaHorizontalAngleApertureLineEdit->text().toInt();
//        int WLANInterfaceAntennaCount=WLANInterfaceAntennaCountLineEdit->text().toInt();
//        int cellularInterfaceAntennaCount=cellularInterfaceAntennaCountLineEdit->text().toInt();
//        int wiredInterfaceSpeed=wiredInterfaceSpeedLineEdit->text().toInt();
//        int wirelessInterfaceSpeed=wirelessInterfaceSpeedLineEdit->text().toInt();
//        QString cellularInterfaceType=cellularInterfaceTypeLineEdit->text();
//        int cellularUpLinkMax=cellularUpLinkMaxLineEdit->text().toInt();
//        int cellularDownLinkMax=cellularDownLinkMaxLineEdit->text().toInt();
//        int cellularIntegratedAntennaCount=cellularIntegratedAntennaCountLineEdit->text().toInt();
//        int cellularIntegratedAntennaGain=cellularIntegratedAntennaGainLineEdit->text().toInt();


//        // Create the request body JSON object
//        QJsonObject requestBody;
//        requestBody["node_type"] = routerType;
//        requestBody["node_hw_model"] = hardware;
//        requestBody["wired_interfaces_count"] = wiredInterfacesCount;
//        requestBody["wireless_interfaces_count"] = wirelessInterfacesCount;
//        requestBody["node_has_battery"] = hasBattery ? 1 : 0;
//        requestBody["node_has_cellular_slot"]=hasCellularSlot ? 1 : 0;
//        requestBody["WLAN_integrated_antenna_count"]=WLANIntegratedAntennaCount;
//        requestBody["WLAN_integrated_antenna_gain"]=WLANIntegratedAntennaGain;
//        requestBody["WLAN_integrated_antenna_vertical_angle_aperture"]=WLANIntegratedAntennaVerticalAngleAperture;
//        requestBody["WLAN_integrated_antenna_horizontal_angle_aperture"]=WLANIntegratedAntennaHorizontalAngleAperture;
//        requestBody["WLAN_interface_antenna_count"]=WLANInterfaceAntennaCount;
//        requestBody["cellular_interface_antenna_count"]=cellularInterfaceAntennaCount;
//        requestBody["wired_interfaces_speed"]=wiredInterfaceSpeed;
//        requestBody["wireless_interfaces_speed"]=wirelessInterfaceSpeed;
//        requestBody["cellular_interface_type"]=cellularInterfaceType;
//        requestBody["cellular_uplink_max"]=cellularUpLinkMax;
//        requestBody["cellular_downlink_max"]=cellularDownLinkMax;
//        requestBody["cellular_integrated_antenna_count"]=cellularIntegratedAntennaCount;
//        requestBody["cellular_integrated_antenna_gain"]=cellularIntegratedAntennaGain;


//        // Add more fields to the JSON object as needed

//        // Convert the JSON object to a QByteArray
//        QJsonDocument jsonDoc(requestBody);
//        QByteArray requestData = jsonDoc.toJson();

//        // Make a POST request to the backend endpoint
//        QNetworkAccessManager manager;
//        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes"));
//        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//        QNetworkReply *reply = manager.post(request, requestData);
//        QEventLoop loop;
//        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//        loop.exec();

//        if (reply->error() == QNetworkReply::NoError) {
//            // Request successful
//            qDebug() << "Router added successfully";
//            // Refresh the table data
//            populateTable();
//        } else {
//            // Request failed
//            qDebug() << "Failed to add router:" << reply->errorString();
//        }

//        reply->deleteLater();
//        addRouterDialog.accept();
//    });
//    connect(cancelButton, &QPushButton::clicked, &addRouterDialog, &QDialog::close);
//    addRouterDialog.exec();
//}



//void MeshRouters::onSaveButtonClicked(const QJsonObject& nodeInfo)
//{
//    // Extract the modified fields from the modify dialog's line edit fields



//    QString modifiedNodeType = nodeTypeLineEdit->text();
//    QString modifiedNodeHWModel = nodeHWModelLineEdit->text();
//    int modifiedWiredInterfacesCount = wiredInterfacesCountSpinBox->value();
//    int modifiedWirelessInterfacesCount = wirelessInterfacesCountSpinBox->value();
//    int modifiedNodeHasBattery = nodeHasBatteryCheckBox->isChecked() ? 1 : 0;
//    int modifiedNodeHasCellularSlot = nodeHasCellularSlotCheckBox->isChecked() ? 1 : 0;
//    int modifiedWLANIntegratedAntennaCount = WLANIntegratedAntennaCountSpinBox->value();
//    int modifiedWLANIntegratedAntennaGain = WLANIntegratedAntennaGainSpinBox->value();
//    int modifiedWLANIntegratedAntennaVerticalAngleAperture = WLANIntegratedAntennaVerticalAngleApertureSpinBox->value();
//    int modifiedWLANIntegratedAntennaHorizontalAngleAperture = WLANIntegratedAntennaHorizontalAngleApertureSpinBox->value();
//    int modifiedWLANInterfaceAntennaCount = WLANInterfaceAntennaCountSpinBox->value();
//    int modifiedCellularInterfaceAntennaCount = cellularInterfaceAntennaCountSpinBox->value();
//    int modifiedWiredInterfacesSpeed = wiredInterfacesSpeedSpinBox->value();
//    int modifiedWirelessInterfacesSpeed = wirelessInterfacesSpeedSpinBox->value();
//    int modifiedCellularInterfaceType = cellularInterfaceTypeComboBox->currentIndex();
//    int modifiedCellularUplinkMax = cellularUplinkMaxSpinBox->value();
//    int modifiedCellularDownlinkMax = cellularDownlinkMaxSpinBox->value();
//    int modifiedCellularIntegratedAntennaCount = cellularIntegratedAntennaCountSpinBox->value();
//    int modifiedCellularIntegratedAntennaGain = cellularIntegratedAntennaGainSpinBox->value();

//    // Construct the modified fields in a QJsonObject
//    QJsonObject modifiedFields;
//    modifiedFields["node_type"] = modifiedNodeType;
//    modifiedFields["node_hw_model"] = modifiedNodeHWModel;
//    modifiedFields["wired_interfaces_count"] = modifiedWiredInterfacesCount;
//    modifiedFields["wireless_interfaces_count"] = modifiedWirelessInterfacesCount;
//    modifiedFields["node_has_battery"] = modifiedNodeHasBattery;
//    modifiedFields["node_has_cellular_slot"] = modifiedNodeHasCellularSlot;
//    modifiedFields["WLAN_integrated_antenna_count"] = modifiedWLANIntegratedAntennaCount;
//    modifiedFields["WLAN_integrated_antenna_gain"] = modifiedWLANIntegratedAntennaGain;
//    modifiedFields["WLAN_integrated_antenna_vertical_angle_aperture"] = modifiedWLANIntegratedAntennaVerticalAngleAperture;
//    modifiedFields["WLAN_integrated_antenna_horizontal_angle_aperture"] = modifiedWLANIntegratedAntennaHorizontalAngleAperture;
//    modifiedFields["WLAN_interface_antenna_count"] = modifiedWLANInterfaceAntennaCount;
//    modifiedFields["cellular_interface_antenna_count"] = modifiedCellularInterfaceAntennaCount;
//    modifiedFields["wired_interfaces_speed"] = modifiedWiredInterfacesSpeed;
//    modifiedFields["wireless_interfaces_speed"] = modifiedWirelessInterfacesSpeed;
//    modifiedFields["cellular_interface_type"] = modifiedCellularInterfaceType;
//    modifiedFields["cellular_uplink_max"] = modifiedCellularUplinkMax;
//    modifiedFields["cellular_downlink_max"] = modifiedCellularDownlinkMax;
//    modifiedFields["cellular_integrated_antenna_count"] = modifiedCellularIntegratedAntennaCount;
//    modifiedFields["cellular_integrated_antenna_gain"] = modifiedCellularIntegratedAntennaGain;

//    // Create the request body JSON object
//    QJsonObject requestBody;
//    // Add the modified fields to the request body
//    requestBody["modified_fields"] = modifiedFields;

//    // Convert the JSON object to a QByteArray
//    QJsonDocument jsonDoc(requestBody);
//    QByteArray requestData = jsonDoc.toJson();

//    // Send a PUT request to update the node information
//    QNetworkAccessManager manager;
//    QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/luceorProductsNodes/" + nodeInfo.value("node_type").toString()));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    QNetworkReply* reply = manager.put(request, requestData);
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();

//    if (reply->error() == QNetworkReply::NoError)
//    {
//        // Request successful
//        QMessageBox::information(this, "Success", "Node information modified successfully!");
//    }
//    else
//    {
//        // Request failed
//        QMessageBox::critical(this, "Error", "Failed to modify node information: " + reply->errorString());
//    }

//    reply->deleteLater();
//}


//void MeshRouters::showAddRouterForm(const QString &title, const QStringList &labels)
//{
//// Create a dialog window to display the add product form
//QDialog *dialog = new QDialog(this);
//dialog->setWindowTitle(title);

//QFormLayout *formLayout = new QFormLayout(dialog);

//QList<QLineEdit *> lineEdits;
//for (const QString &label : labels)
//{
//    QLabel *labelWidget = new QLabel(label);
//    QLineEdit *lineEdit = new QLineEdit();
//    lineEdits.append(lineEdit);

//    formLayout->addRow(labelWidget, lineEdit);
//}

//QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
//formLayout->addWidget(buttonBox);

//connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);


//if (dialog->exec() == QDialog::Accepted)
//{
//    QStringList values;
//    for (QLineEdit *lineEdit : lineEdits)
//    {
//        values.append(lineEdit->text());
//    }

//    // Send a POST request to add the product using the appropriate API endpoint
//    // Example: localhost:3300/addProduct

//    // Update the table by adding a new row with the entered values
//    int row = routerTable->rowCount();
//    routerTable->insertRow(row);
//    for (int i = 0; i < values.size(); i++)
//    {
//        routerTable->setItem(row, i, new QTableWidgetItem(values[i]));
//    }
//}


//delete dialog;
//}

//void MeshRouters::onTableRightClicked(const QPoint &pos)
//{
//Q_UNUSED(pos);
//    QTableWidgetItem *currentItem = routerTable->itemAt(pos);
//    if (currentItem)
//    {
//        QMenu *contextMenu = new QMenu(this);
//        QAction *deleteAction = contextMenu->addAction("Delete");
//        connect(deleteAction, &QAction::triggered, this, &MeshRouters::onDeleteActionTriggered);
//        contextMenu->exec(QCursor::pos());
//        delete contextMenu;
//    }
//}

//void MeshRouters::onAddRouterClicked()
//{
//    showAddRouterForm("Add Router", { "Router Type:", "Hardware:", "Wireless Interfaces:",
//                                       "Wired Interfaces:", "Wired Speed:", "Wireless Speed:", "Has Battery:" });
//}
