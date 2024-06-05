#include "nodesmanagement.h"
#include "ui_nodesmanagement.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include<QDialogButtonBox>
#include<QFormLayout>
#include <QInputDialog>
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
#include <QDockWidget>
#include <QTextEdit>
#include <QObject>
#include <QTimer>
#include <QEventLoop>
#include <QRegularExpressionValidator>
#include<QHostAddress>
#include"topology.h"

NodesManagement::NodesManagement(QWidget *parent) :QWidget(parent)

{



    setupUI();
    populateTable();


//    QTimer::singleShot(30000, this, &NodesManagement::populateTable);
    refreshTableTimer= new QTimer(this);
    refreshTableTimer->start(3000);
    connect(refreshTableTimer,&QTimer::timeout,this,&NodesManagement::refreshData);

    // Dock widget part
    dockWidget = new QDockWidget(this);
    dockContent = new QStackedWidget(dockWidget);

    dockLayout = new QVBoxLayout;
    dockLayout->addWidget(dockWidget);

    dockWidget->setWidget(dockContent);
    dockWidget->hide();

    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
}
void NodesManagement::refreshData(){
    populateTable();
}
NodesManagement::~NodesManagement()
{
    delete nodesManagementLayout;
    //    delete nodesListView;
    delete nodesManagementTable;
    delete nodesListLayout;
    delete createNodeButton;
    delete startNodeAction ;
    delete stopNodeAction;
    delete  rebootNodeAction;
    delete  showLinksAction;
    delete deleteNodeAction;
    delete nodeInfoAction;
    delete getFirmwareAction;
    delete startMonitoringAction;
    delete stopMonitoringAction;
    delete startVumeterAction;
    delete stopVumeterAction;
    delete dockLayout;
    delete dockWidget;
    delete dockContent;
    delete nodeManagementTimer;
    delete nodeInfoWidget;
    delete createNodeWidget;
    delete checkBoxItem;
    delete startMultipleNodesAction;
    delete stopMultipleNodesAction;
    delete rebootMultipleNodesAction;
    delete deleteMultipleNodesAction;
    delete startMultipleNodesMonitoringAction;
    delete stopMultipleNodesMonitoringAction;
    delete startMultipleNodesVumeterAction;
    delete stopMultipleNodesVumeterAction;
    delete nodesManagementLayout;
    delete nodesManagementMenu;
    delete nodeInterfacesWidget;
    delete nodeInterfacesAction;
    delete configureInterfaceWidget;
    //    delete  getVumeterStatusAction;
    //    delete getSSHFingerPrintsAction;

}

void NodesManagement::setupUI() {
    // Major layout
    nodesManagementLayout = new QHBoxLayout;
    setLayout(nodesManagementLayout);

    // Table layout
    nodesListLayout = new QVBoxLayout;
    nodesManagementTable = new QTableWidget;
    nodesListLayout->addWidget(nodesManagementTable);

    nodesManagementLayout->addLayout(nodesListLayout);

    // Set up table columns
    nodesManagementTable->setColumnCount(3);



    QStringList headerLabels;
    //    headerLabels << "Node Mac Address" << "Node Name";
    nodesManagementTable->setHorizontalHeaderLabels({"Node Mac Address","Node Name","Node Type"});
    nodesManagementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);




    // Set the style sheet for the table
    QString tableStyleSheet = "QTableWidget {"
                              "    background-color: #f2f2f2;"
                              "    border: 1px solid #999999;"
                              "    font-size: 14px;"
                              "    font-family: Arial;"
                              "    border: 0.5px solid #999999;"
                              "    border-radius: 4px;"




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
                              "    border: none;"
                              "    padding: 6px 10px;"
                              "    text-align: left;"
                              "    height: 30px;"
                              "    border: 0.5px solid #999999;"
                              "    border-radius: 4px;"



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
                              "}";



    nodesManagementTable->setStyleSheet(tableStyleSheet);

    // Set table properties
    nodesManagementTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    nodesManagementTable->setSelectionMode(QAbstractItemView::ExtendedSelection); // Enable multiple row selection
    nodesManagementTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Button
    createNodeButton = new QPushButton("Create Nodes", this);
    QIcon addIcon(":/images/addIcon.png");
    addIcon = addIcon.pixmap(QSize(128,128));  // Set the desired icon size here

    createNodeButton->setIcon(addIcon);
    createNodeButton->setStyleSheet("QPushButton {"
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
                                    "QToolButton:hover {"
                                    "background-color: rgba(36,101,175,210);"  // Set a hover background color with slight gray transparency
                                    "}"
                                    "QToolButton:pressed {"
                                    "background-color: rgba(36,101,175,250);"  // Set a pressed background color with slight gray transparency
                                    "}");
    createNodeButton->setFixedWidth(200);
    nodesListLayout->addWidget(createNodeButton);

    connect(createNodeButton, &QPushButton::clicked, this, &NodesManagement::showCreateNodeForm);






}




void NodesManagement::populateTable()
{
    // Make a network request to fetch data from the endpoint
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QUrl url("http://localhost:3200/wms/mesh/nodes");
    request.setUrl(url);
    QNetworkReply *reply = manager.get(request);

    // Create an event loop to wait for the network request to finish
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check if the request was successful
    if (reply->error() == QNetworkReply::NoError)
    {
        // Read the response data
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc(QJsonDocument::fromJson(responseData));

        // Check if the response is a valid JSON array
        if (jsonDoc.isArray())
        {
            QJsonArray jsonArray = jsonDoc.array();

            // Set the row count of the table to the size of the JSON array
            nodesManagementTable->setRowCount(jsonArray.size());

            // Iterate over each node object in the JSON array
            for (int i = 0; i < jsonArray.size(); ++i)
            {
                QJsonValue nodeValue = jsonArray[i];
                if (nodeValue.isObject())
                {
                    QJsonObject nodeObject = nodeValue.toObject();

                    // Extract the node_name and node_id values
                    if (nodeObject.contains("node_name") && nodeObject.contains("node_id"))
                    {
                        QString nodeName = nodeObject["node_name"].toString();
                        QString nodeId = nodeObject["node_id"].toString();
                        QString nodeType = nodeObject["node_type"].toString();

                        //                        // Create table items for the node_name and node_id values
                        //                        checkBoxItem = new QTableWidgetItem();
                        //                        checkBoxItem->setCheckState(Qt::Unchecked);

                        QTableWidgetItem *nodeNameItem = new QTableWidgetItem(nodeName);
                        QTableWidgetItem *nodeIdItem = new QTableWidgetItem(nodeId);
                        QTableWidgetItem *nodeTypeItem = new QTableWidgetItem(nodeType);

                        // Set the table items in the corresponding cells
                        //                        nodesManagementTable->setItem(i, 0, checkBoxItem);


                        nodesManagementTable->setItem(i, 0, nodeIdItem);
                        nodesManagementTable->setItem(i, 1, nodeNameItem);
                        nodesManagementTable->setItem(i,2,nodeTypeItem);
                    }
                }
            }
        }
    }
    else
    {
        // Handle the case when the request fails
        QMessageBox::warning(this, "Error", "Failed to fetch node data: " + reply->errorString());
    }

    // Clean up the network reply
    reply->deleteLater();
}

void NodesManagement::contextMenuEvent(QContextMenuEvent *event)
{
    nodesManagementMenu=new QMenu(this);

    // Actions single selection Menu
    startNodeAction = new QAction("Start Node", this);
    stopNodeAction = new QAction("Stop Node", this);
    rebootNodeAction = new QAction("Reboot Node", this);
    nodeInterfacesAction=new QAction("Display Interfaces",this);
    showLinksAction = new QAction("Node Links", this);
    addAntennasAction = new QAction("Add Antennas",this);
    deleteNodeAction = new QAction("Delete Node", this);
    nodeInfoAction = new QAction("Node Informations", this);
    getFirmwareAction = new QAction("Firmware Version", this);
    startMonitoringAction = new QAction("Start Monitoring", this);
    stopMonitoringAction = new QAction("Stop Monitoring", this);
    startVumeterAction = new QAction("Start Vumeter", this);
    stopVumeterAction = new QAction("Stop Vumeter", this);
    //    getVumeterStatusAction = new QAction("Vumeter Status", this);
    //    getSSHFingerPrintsAction = new QAction("SSH Fingerprints", this);
    QIcon infoMenuIcon(":/images/infoIcon-24.png");
    infoMenuIcon = infoMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
    QIcon deleteMenuIcon(":/images/deleteIcon.png");
    deleteMenuIcon = deleteMenuIcon.pixmap(QSize(24, 24));
    QIcon addAntennaMenuIcon(":/images/antenna.png");
    addAntennaMenuIcon = addAntennaMenuIcon.pixmap(QSize(24, 24));

    QIcon startIcon(":/images/startIcon.png");
    startIcon = startIcon.pixmap(QSize(24, 24));
    QIcon stopIcon(":/images/stopIcon.png");
    stopIcon = stopIcon.pixmap(QSize(24, 24));
    QIcon rebootIcon(":/images/restartIcon.png");
    rebootIcon = rebootIcon.pixmap(QSize(24, 24));
    QIcon firmwareIcon(":/images/firmwareIcon");
    firmwareIcon=firmwareIcon.pixmap(QSize(24,24));
    QIcon showLinksIcon(":/images/showLinksIcon.png");
    showLinksIcon=showLinksIcon.pixmap(QSize(24,24));
    QIcon interfacesIcon(":/images/interfaces.png");
    interfacesIcon=interfacesIcon.pixmap(QSize(24,24));






    nodeInfoAction->setIcon(infoMenuIcon);
    deleteNodeAction->setIcon(deleteMenuIcon);
    addAntennasAction->setIcon(addAntennaMenuIcon);
    startNodeAction->setIcon(startIcon);
    stopNodeAction->setIcon(stopIcon);
    rebootNodeAction->setIcon(rebootIcon);
    nodeInterfacesAction->setIcon(interfacesIcon);
    showLinksAction->setIcon(showLinksIcon);
    getFirmwareAction->setIcon(firmwareIcon);
    startMonitoringAction->setIcon(startIcon);
    stopMonitoringAction->setIcon(stopIcon);
    startVumeterAction->setIcon(startIcon);
    stopVumeterAction->setIcon(stopIcon);


    //Actions for multiple selection
    startMultipleNodesAction= new QAction("Start Multiple Nodes", this);
    stopMultipleNodesAction= new QAction("Stop Multiple Nodes", this);
    rebootMultipleNodesAction= new QAction("Reboot Multiple Nodes", this);
    deleteMultipleNodesAction= new QAction("Delete Multiple Nodes", this);
    startMultipleNodesMonitoringAction= new QAction("Start Multiple Nodes Monitoring", this);
    stopMultipleNodesMonitoringAction= new QAction("Stop Multiple Nodes Monitoring ", this);
    startMultipleNodesVumeterAction= new QAction("Start Multiple Nodes Vumeter", this);
    stopMultipleNodesVumeterAction= new QAction("Stop Multiple Nodes Vumeter", this);

    startMultipleNodesAction->setIcon(startIcon);
    stopMultipleNodesAction->setIcon(stopIcon);
    rebootMultipleNodesAction->setIcon(rebootIcon);
    deleteMultipleNodesAction->setIcon(deleteMenuIcon);
    startMultipleNodesMonitoringAction->setIcon(startIcon);
    stopMultipleNodesMonitoringAction->setIcon(stopIcon);
    startMultipleNodesVumeterAction->setIcon(startIcon);
    stopMultipleNodesVumeterAction->setIcon(stopIcon);
    //single selection action signal slot
    connect(startNodeAction, &QAction::triggered, this, &NodesManagement::onStartNodeActionTriggered);
    connect(stopNodeAction, &QAction::triggered, this, &NodesManagement::onStopNodeActionTriggered);
    connect(rebootNodeAction, &QAction::triggered, this, &NodesManagement::onRebootNodeActionTriggered);
    connect(nodeInterfacesAction,&QAction::triggered,this,&NodesManagement::onInterfacesActionTriggered);
    connect(showLinksAction, &QAction::triggered, this, &NodesManagement::onShowLinksActionTriggered);
    connect(addAntennasAction,&QAction::triggered,this,&NodesManagement::onAddAntennasActionTriggered);
    connect(deleteNodeAction, &QAction::triggered, this, &NodesManagement::onDeleteNodeActionTriggered);
    connect(nodeInfoAction, &QAction::triggered, this, &NodesManagement::onInfoActionTriggered);
    connect(getFirmwareAction, &QAction::triggered, this, &NodesManagement::onGetFirmwareActionTriggered);
    connect(startMonitoringAction, &QAction::triggered, this, &NodesManagement::onStartMonitoringActionTriggered);
    connect(stopMonitoringAction, &QAction::triggered, this, &NodesManagement::onStopMonitoringActionTriggered);
    connect(startVumeterAction, &QAction::triggered, this, &NodesManagement::onStartVumeterActionTriggered);
    connect(stopVumeterAction, &QAction::triggered, this, &NodesManagement::onStopVumeterActionTriggered);
    //    connect(getVumeterStatusAction, &QAction::triggered, this, &NodesManagement::onGetVumeterStatus);
    //    connect(getSSHFingerPrintsAction, &QAction::triggered, this, &NodesManagement::onGetSSHFingerPrints);


    //multi selection action signal slot
    connect(startMultipleNodesAction, &QAction::triggered, this, &NodesManagement::onStartMultipleNodesActionTriggered);
    connect(stopMultipleNodesAction, &QAction::triggered, this, &NodesManagement::onStopMultipleNodesActionTriggered);
    connect(rebootMultipleNodesAction, &QAction::triggered, this, &NodesManagement::onRebootMultipleNodesActionTriggered);
    connect(deleteMultipleNodesAction, &QAction::triggered, this, &NodesManagement::onDeleteMultipleNodesActionTriggered);
    connect(startMultipleNodesMonitoringAction, &QAction::triggered, this, &NodesManagement::onStartMultipleNodesMonitoringActionTriggered);
    connect(stopMultipleNodesMonitoringAction, &QAction::triggered, this, &NodesManagement::onStopMultipleNodesMonitoringActionTriggered);
    connect(startMultipleNodesVumeterAction, &QAction::triggered, this, &NodesManagement::onStartMultipleNodesVumeterActionTriggered);
    connect(stopMultipleNodesVumeterAction, &QAction::triggered, this, &NodesManagement::onStopMultipleNodesVumeterActionTriggered);
    // Check if at least one checkbox is selected

    // Get the selected rows
    QModelIndexList selectedRows = nodesManagementTable->selectionModel()->selectedRows();
    // Create the appropriate menu based on checkbox selection
    if(selectedRows.isEmpty()){
        return;
    }
    else if (selectedRows.size()>1)
    {
        // Actions for when at least one checkbox is selected
        nodesManagementMenu->addAction(startMultipleNodesAction);
        nodesManagementMenu->addAction(stopMultipleNodesAction);
        nodesManagementMenu->addAction(rebootMultipleNodesAction);
        nodesManagementMenu->addAction(deleteMultipleNodesAction);
        nodesManagementMenu->addAction(startMultipleNodesMonitoringAction);
        nodesManagementMenu->addAction(stopMultipleNodesMonitoringAction);

        nodesManagementMenu->addAction(startMultipleNodesVumeterAction);
        nodesManagementMenu->addAction(stopMultipleNodesVumeterAction);
        // Add other actions specific to multiple selection
    }
    else if(selectedRows.size()==1)
    {
        // Actions for no checkbox selection or single checkbox selection
        nodesManagementMenu->addAction(startNodeAction);
        nodesManagementMenu->addAction(stopNodeAction);
        nodesManagementMenu->addAction(rebootNodeAction);
        nodesManagementMenu->addAction(nodeInterfacesAction);
        nodesManagementMenu->addAction(showLinksAction);
        nodesManagementMenu->addAction(addAntennasAction);
        nodesManagementMenu->addAction(deleteNodeAction);
        nodesManagementMenu->addAction(nodeInfoAction);
        nodesManagementMenu->addAction(getFirmwareAction);
        nodesManagementMenu->addAction(startMonitoringAction);
        nodesManagementMenu->addAction(stopMonitoringAction);

        nodesManagementMenu->addAction(startVumeterAction);
        nodesManagementMenu->addAction(stopVumeterAction);
        //        menu.addAction(getVumeterStatusAction);
        //        menu.addAction(getSSHFingerPrintsAction);
    }

    // Display the menu at the event's global position
    nodesManagementMenu->exec(event->globalPos());
}

void NodesManagement::displayDockWidget(QString message){
    nodesManagementLayout->addLayout(dockLayout);
    dockWidget->setWindowTitle(message);
    dockContent->show();
    dockWidget->show();



}


void NodesManagement::onCreateNodeClicked()
{
    nodesManagementLayout->removeItem(dockLayout);

    // Create a widget for the create node form
    createNodeWidget = new QWidget(this);
    QVBoxLayout* createNodeLayout = new QVBoxLayout(createNodeWidget);

    QFormLayout* formLayout = new QFormLayout;

    // Create labels and input fields for node name and node type
    QLabel* nameLabel = new QLabel("Number of nodes:", createNodeWidget);
    QLineEdit* numberOfNodesEdit = new QLineEdit(createNodeWidget);
    QLabel* typeLabel = new QLabel("Node Type:", createNodeWidget);
    QComboBox* nodeTypeComboBox = new QComboBox(createNodeWidget);
    formLayout->addRow(nameLabel,numberOfNodesEdit);
    formLayout->addRow(typeLabel,nodeTypeComboBox);

    // Adding a scroll area
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(formLayout);
    scrollArea->setWidget(scrollAreaContent);
    createNodeLayout->addWidget(scrollArea);


    // Fetch router types from the backend
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QUrl url("http://localhost:3300/wms/survey/luceorProductsNodes");
    request.setUrl(url);
    QNetworkReply* reply = manager.get(request);

    // Create an event loop to wait for the network request to finish
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc(QJsonDocument::fromJson(responseData));

        if (jsonDoc.isArray())
        {
            QJsonArray jsonArray = jsonDoc.array();

            // Iterate over each router type object in the JSON array
            for (const QJsonValue& routerTypeValue : jsonArray)
            {
                if (routerTypeValue.isObject())
                {
                    QJsonObject routerTypeObject = routerTypeValue.toObject();
                    if (routerTypeObject.contains("node_type"))
                    {
                        QString routerType = routerTypeObject["node_type"].toString();
                        nodeTypeComboBox->addItem(routerType);
                    }
                }
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to fetch router types: " + reply->errorString());
    }

    // Add labels and input fields to the layout


    // Create a submit button
    QPushButton* submitButton = new QPushButton("Submit", createNodeWidget);
    QPushButton* cancelButton = new QPushButton("Cancel",createNodeWidget);
    QIcon submitIcon(":/images/submitIcon.png");
    submitIcon = submitIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    submitButton->setIcon(submitIcon);
    QIcon cancelIcon(":/images/cancelIcon.png");
    cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    cancelButton->setIcon(cancelIcon);
    submitButton->setObjectName("submitButton");
    cancelButton->setObjectName("cancelButton");
    submitButton->setFixedWidth(200);
    cancelButton->setFixedWidth(200);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(cancelButton);

    createNodeLayout->addLayout(buttonLayout);

    // Connect the submit button's clicked signal to a slot
    connect(submitButton, &QPushButton::clicked, [numberOfNodesEdit, nodeTypeComboBox, this]() {
        // Get the values from the input fields
        int numberOfNodes = numberOfNodesEdit->text().toInt();
        QString nodeType = nodeTypeComboBox->currentText();


        // Create a JSON object for the request body
        QJsonObject requestBody;
        requestBody["number_of_nodes"] = numberOfNodes;
        requestBody["node_type"] = nodeType;

        // Convert the JSON object to a byte array
        QJsonDocument jsonDoc(requestBody);
        QByteArray requestData = jsonDoc.toJson();

        // Make a POST request to create the node
        QNetworkAccessManager manager;
        QNetworkRequest request;
        QUrl url("http://localhost:3200/wms/mesh/nodes/createNodes");
        request.setUrl(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply* reply = manager.post(request, requestData);

        // Create an event loop to wait for the network request to finish
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError)
        {
            QMessageBox::information(this, "Success",QString::number(numberOfNodes)+ " nodes ("+nodeType+") created successfully!");
            populateTable();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to create node: " + reply->errorString());
        }
        reply->deleteLater();
        createNodeWidget->close();
        dockWidget->close();
        dockContent->close();
        nodesManagementLayout->removeItem(dockLayout);




    });


    connect(cancelButton, &QPushButton::clicked, createNodeWidget, &QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockWidget,&QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockContent,&QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockContent,[this](){

        nodesManagementLayout->removeItem(dockLayout);

    });





    // Add the create node widget to the dock content
    dockContent->addWidget(createNodeWidget);
    dockContent->setCurrentWidget(createNodeWidget);

    // Show the dock widget
    displayDockWidget("Create Node :");




}


// Function to refresh the node information

void NodesManagement::showCreateNodeForm(){
    onCreateNodeClicked();

}



void NodesManagement::onInfoActionTriggered()
{
    nodesManagementLayout->removeItem(dockLayout);


    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column
    nodeManagementTimer = new QTimer(this);

    // Create the widget to display node information
    nodeInfoWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(nodeInfoWidget);
    QFormLayout *formLayout = new QFormLayout;

    QLabel *nodeIdLabel = new QLabel("Node ID:");
    QLabel *nodeIdValueLabel = new QLabel(nodeId);
    QLabel *nodeNameLabel = new QLabel("Node Name:");
    QLabel *nodeNameValueLabel = new QLabel();
    QLabel *nodeTypeLabel = new QLabel("Node Type:");
    QLabel *nodeTypeValueLabel = new QLabel();
    QLabel *nodeSnLabel = new QLabel("Node Serial Number:");
    QLabel *nodeSnValueLabel = new QLabel();
    QLabel *nodeHwModelLabel = new QLabel("Node Hardware Model:");
    QLabel *nodeHwModelValueLabel = new QLabel();
    QLabel *nodeIpAdminLabel = new QLabel("Node Admin IP:");
    QLabel *nodeIpAdminValueLabel = new QLabel();
    QLabel *nodeSysCPUUsageLabel = new QLabel("System CPU Usage:");
    QLabel *nodeSysCPUUsageValueLabel = new QLabel();
    QLabel *nodeSysLoad1mLabel = new QLabel("System Load (1m):");
    QLabel *nodeSysLoad1mValueLabel = new QLabel();
    QLabel *nodeSysLoad5mLabel = new QLabel("System Load (5m):");
    QLabel *nodeSysLoad5mValueLabel = new QLabel();
    QLabel *nodeSysLoad15mLabel = new QLabel("System Load (15m):");
    QLabel *nodeSysLoad15mValueLabel = new QLabel();
    QLabel *nodeSysUpTime_sLabel = new QLabel("System Up Time:");
    QLabel *nodeSysUpTime_sValueLabel = new QLabel  ();
    QLabel *routingModeLabel = new QLabel("Routing Mode:");
    QLabel *routingModeValueLabel = new QLabel();
    QLabel *nodeMonitoringStatusLabel = new QLabel("Monitoring Status:");
    QLabel *nodeMonitoringStatusValueLabel = new QLabel();
    QLabel *nodeVumeterStatusLabel = new QLabel("Vumeter Status:");
    QLabel *nodeVumeterStatusValueLabel = new QLabel();
    QLabel *nodeStatusLabel = new QLabel("Node Status:");
    QLabel *nodeStatusValueLabel = new QLabel();
    QLabel *nodeHasBatteryLabel = new QLabel("Has Battery:");
    QLabel *nodeHasBatteryValueLabel = new QLabel();

    formLayout->addRow(nodeIdLabel, nodeIdValueLabel);
    formLayout->addRow(nodeNameLabel, nodeNameValueLabel);
    formLayout->addRow(nodeTypeLabel, nodeTypeValueLabel);
    formLayout->addRow(nodeSnLabel, nodeSnValueLabel);
    formLayout->addRow(nodeHwModelLabel, nodeHwModelValueLabel);
    formLayout->addRow(nodeIpAdminLabel, nodeIpAdminValueLabel);
    formLayout->addRow(nodeSysCPUUsageLabel, nodeSysCPUUsageValueLabel);
    formLayout->addRow(nodeSysLoad1mLabel, nodeSysLoad1mValueLabel);
    formLayout->addRow(nodeSysLoad5mLabel, nodeSysLoad5mValueLabel);
    formLayout->addRow(nodeSysLoad15mLabel, nodeSysLoad15mValueLabel);
    formLayout->addRow(nodeSysUpTime_sLabel, nodeSysUpTime_sValueLabel);
    formLayout->addRow(nodeMonitoringStatusLabel, nodeMonitoringStatusValueLabel);
    formLayout->addRow(nodeMonitoringStatusLabel, nodeMonitoringStatusValueLabel);
    formLayout->addRow(nodeVumeterStatusLabel, nodeVumeterStatusValueLabel);
    formLayout->addRow(nodeStatusLabel, nodeStatusValueLabel);
    formLayout->addRow(nodeHasBatteryLabel, nodeHasBatteryValueLabel);




    // Add the formLayout and the closeButton to the layout
    // Add the nodeInfoWidget to a scroll area to handle large content
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    QWidget *scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(formLayout);
    scrollArea->setWidget(scrollAreaContent);
    layout->addWidget(scrollArea);
    // ... Add the remaining labels and their corresponding value labels ...

    // Create the close button
    QPushButton *closeButton = new QPushButton("Close");

    QIcon cancelIcon(":/images/closeIcon.png");
    cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    closeButton->setIcon(cancelIcon);

    closeButton->setObjectName("cancelButton");

    closeButton->setFixedWidth(200);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);



    // Create a QTimer to send periodic GET requests

    connect(nodeManagementTimer, &QTimer::timeout, [=]()
    {

        // Send a GET request to retrieve the information of the selected node
        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId));
        QNetworkReply* reply = manager->get(getRequest);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError)
        {
            // Request successful
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject nodeInfo = jsonResponse.object();

            // Update the values of the labels with the new data
            nodeIdValueLabel->setText(nodeInfo.value("node_id").toString());
            nodeNameValueLabel->setText(nodeInfo.value("node_name").toString());
            nodeTypeValueLabel->setText(nodeInfo.value("node_type").toString());
            nodeSnValueLabel->setText(nodeInfo.value("node_sn").toString());
            nodeHwModelValueLabel->setText(nodeInfo.value("node_hw_model").toString());
            nodeIpAdminValueLabel->setText(nodeInfo.value("node_ip_admin").toString());
            nodeSysCPUUsageValueLabel->setText(QString::number(nodeInfo.value("node_sysCPUUsage").toDouble()));
            nodeSysLoad1mValueLabel->setText(QString::number(nodeInfo.value("node_sysLoad1m").toDouble()));
            nodeSysLoad5mValueLabel->setText(QString::number(nodeInfo.value("node_sysLoad5m").toDouble()));
            nodeSysLoad15mValueLabel->setText(QString::number(nodeInfo.value("node_sysLoad15m").toDouble()));


            nodeSysUpTime_sValueLabel->setText(QString::number(nodeInfo.value("node_sysUpTime_s").toInt()));
            if (!nodeInfo.value("routing_mode").isNull())
            {
                routingModeValueLabel->setText(nodeInfo.value("routing_mode").toString());
                formLayout->addRow(routingModeLabel, routingModeValueLabel);

            }
            if (nodeInfo.value("node_monitoring_status").toInt()== 1)
            {
                nodeMonitoringStatusValueLabel->setText("ON");
            }else if(nodeInfo.value("node_monitoring_status").toInt()== 0){
                nodeMonitoringStatusValueLabel->setText("OFF");

            }

            if(nodeInfo.value("node_vumeter_status").toInt()==1){
                nodeVumeterStatusValueLabel->setText("ON");

            }else if (nodeInfo.value("node_vumeter_status").toInt()==0){
                nodeVumeterStatusValueLabel->setText("OFF");
            }


            if(nodeInfo.value("node_status").toInt()==1){
                nodeStatusValueLabel->setText("ON");
            }else if (nodeInfo.value("node_status").toInt()==0){
                nodeStatusValueLabel->setText("OFF");

            }

            if(nodeInfo.value("node_has_battery").toInt()==1){
                nodeHasBatteryValueLabel->setText("Yes");
            }else if (nodeInfo.value("node_has_battery").toInt()==0){
                nodeHasBatteryValueLabel->setText("No");
            }


            if (nodeInfo.isEmpty() || nodeInfo.value("status").toString() == "deleted")
            {
                // Stop the timer
                nodeManagementTimer->stop();

                // Show a message indicating that the node was not found or deleted
                QMessageBox::information(this, "Node Not Found", "The selected node was not found or has been deleted.");

                // Close the dock widget
                dockWidget->close();
                return; // Stop the request and return immediately
            }




        }
        else
        {
            // Request failed
            QMessageBox::critical(this, "Error","The selected node was not found or has been deleted \n" + reply->errorString());
            nodeManagementTimer->stop();
            dockWidget->close();
            return; // Stop the request and return immediately



        }

        reply->deleteLater();


    });
    dockContent->addWidget(nodeInfoWidget);
    dockContent->setCurrentWidget(nodeInfoWidget);
    displayDockWidget("Node Informations: ");

    // Start the timer with a 1-second interval
    nodeManagementTimer->start(1000);
    // Connect the close button's clicked signal to close the widget
    connect(closeButton, &QPushButton::clicked, nodeInfoWidget, &QWidget::close);
    connect(closeButton, &QPushButton::clicked, dockWidget, &QWidget::close);
    connect(closeButton, &QPushButton::clicked, dockContent, &QWidget::close);
    connect(closeButton, &QPushButton::clicked, dockContent, [this]()
    { nodesManagementLayout->removeItem(dockLayout); });
    connect(closeButton,&QPushButton::clicked,[=](){
        nodeManagementTimer->stop();
        delete nodeManagementTimer;

    });






}

void NodesManagement::onAddAntennasActionTriggered()
{
    nodesManagementLayout->removeItem(dockLayout);

    // Get the selected node ID from the first column, first row

    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString selectedNodeId = nodesManagementTable->item(rowIndex, 0)->text();


    // Create the form widget to add antennas
    QWidget* addAntennasWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(addAntennasWidget);
    QFormLayout* formLayout = new QFormLayout;

    // Create the antenna type combo box
    QLabel* antennaTypeLabel=new QLabel("Antenna Types");
    QComboBox* antennaTypeComboBox = new QComboBox;
    formLayout->addRow(antennaTypeLabel,antennaTypeComboBox);
    // TODO: Fill the combo box with model IDs of antennas using the API endpoint: localhost:3300/wms/survey/surveyAntennas
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyAntennas"));
    QNetworkReply* reply =manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
       {
           // Request successful, handle the response
           QByteArray responseData = reply->readAll();
           QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
           QJsonArray antennasArray = jsonResponse.array();

           // Extract the model IDs and add them to the combo box
           for (const QJsonValue& antennaValue : antennasArray)
           {
               QJsonObject antennaObject = antennaValue.toObject();
               QString modelId = antennaObject.value("model_id").toString();
               antennaTypeComboBox->addItem(modelId);
           }
       }
       else
       {
           // Request failed, handle the error
           QMessageBox::critical(this, "Error", "Failed to retrieve antenna model IDs.");
           reply->deleteLater();
           addAntennasWidget->close();
           return;
       }

       reply->deleteLater();

    // Create the submit and cancel buttons
    QPushButton* submitButton = new QPushButton("Submit");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QIcon submitIcon(":/images/submitIcon.png");
    submitIcon = submitIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    submitButton->setIcon(submitIcon);
    QIcon cancelIcon(":/images/cancelIcon.png");
    cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    cancelButton->setIcon(cancelIcon);
    submitButton->setObjectName("submitButton");
    cancelButton->setObjectName("cancelButton");
    submitButton->setFixedWidth(200);
    cancelButton->setFixedWidth(200);


    QHBoxLayout *buttonLayout = new QHBoxLayout;

    // Add the antenna type combo box and buttons to the layout

    // Create a scroll area and set the layout as its widget
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    QWidget *scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(formLayout);
    scrollArea->setWidget(scrollAreaContent);
    layout->addWidget(scrollArea);

    buttonLayout->addWidget(submitButton);

    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    // Connect the submit button's clicked signal to send a POST request to add the antenna
    connect(submitButton, &QPushButton::clicked, [=]() {
        // Get the selected model ID from the combo box
        QString selectedModelId = antennaTypeComboBox->currentText();

        // Send a POST request to add the antenna
        QNetworkAccessManager manager;
        QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/antennas/" + selectedNodeId));
        qDebug()<<selectedNodeId;
        postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // Create the request body
        QJsonObject requestBody;
        requestBody["model_id"] = selectedModelId;
        QJsonDocument requestBodyJson(requestBody);

        QNetworkReply* reply = manager.post(postRequest, requestBodyJson.toJson());
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError)
        {
            // Request successful, handle the response
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = jsonResponse.object();
            QString message = responseObject.value("message").toString();
            QMessageBox::information(this, "Success", message);
        }
        else
        {
            // Request failed, handle the error
            QMessageBox::critical(this, "Error", "Failed to add antennas.");
        }

        reply->deleteLater();
        addAntennasWidget->close();
        dockWidget->close();
        dockContent->close();
        nodesManagementLayout->removeItem(dockLayout);

    });

    // Connect the cancel button's clicked signal to close the widget
    connect(cancelButton, &QPushButton::clicked, addAntennasWidget, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockWidget, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockContent, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockContent, [this]()
    { nodesManagementLayout->removeItem(dockLayout); });
    // Add the form widget to the dock content area
    dockContent->addWidget(addAntennasWidget);
    dockContent->setCurrentWidget(addAntennasWidget);
    displayDockWidget("Add Antennas");
}


void NodesManagement::onDeleteNodeActionTriggered() {
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this router?", QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::Yes) {
        QTableWidgetItem* currentItem = nodesManagementTable->currentItem();
        int rowIndex = nodesManagementTable->row(currentItem);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        qDebug() << "Node deleted is: " << nodeId;

        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3200/wms/mesh/nodes/deleteNode/" + nodeId));
        QNetworkReply* reply = manager.deleteResource(request);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonResponse = jsonDoc.object();
            QString message = jsonResponse["message"].toString();

            if (message == "Node deleted successfully") {
                qDebug() << "Router deleted successfully";
                nodesManagementTable->removeRow(rowIndex);
                populateTable();
                QMessageBox::information(this, "Success", "The selected node has been deleted successfully");
            } else {
                qDebug() << "Failed to delete router: Unexpected response";
                QMessageBox::critical(this, "Error", "Unexpected response from the server");
            }
        } else {
            QString errorMessage = reply->errorString();
            qDebug() << "Failed to delete router: " << errorMessage;

            switch (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) {
            case 404:
                QMessageBox::warning(this, "Error", "Node not found");
                break;
            case 400:
                QMessageBox::warning(this, "Error", "Please turn off the node first!");
                break;
            default:
                QMessageBox::critical(this, "Error", "Failed to delete the router");
                break;
            }
        }

        reply->deleteLater();
    }
}

void NodesManagement::onStartNodeActionTriggered()
{
    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column

    QNetworkAccessManager manager;

    // Send a GET request to retrieve node information
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId));
    QNetworkReply* getReply = manager.get(getRequest);
    QEventLoop getLoop;
    connect(getReply, &QNetworkReply::finished, &getLoop, &QEventLoop::quit);
    getLoop.exec();

    if (getReply->error() == QNetworkReply::NoError)
    {
        // GET request successful
        QByteArray responseData = getReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject nodeInfo = jsonResponse.object();

        QString nodeType = nodeInfo.value("node_type").toString();
        QString nodeName = nodeInfo.value("node_name").toString();

        // Send a PUT request to start the node
        QNetworkRequest putRequest(QUrl("http://localhost:3200/wms/mesh/nodes/startNode/" + nodeId));
        QNetworkReply* putReply = manager.put(putRequest, QByteArray());
        QEventLoop putLoop;
        connect(putReply, &QNetworkReply::finished, &putLoop, &QEventLoop::quit);
        putLoop.exec();

        if (putReply->error() == QNetworkReply::NoError)
        {
            // PUT request successful
            QByteArray putResponseData = putReply->readAll();
            QJsonDocument putJsonResponse = QJsonDocument::fromJson(putResponseData);
            QJsonObject responseObj = putJsonResponse.object();

            int status = responseObj.value("status").toInt();
            QString message = responseObj.value("message").toString();

            if (status == 200)
            {
                // Node started successfully
                QMessageBox::information(this, "Node Started", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") started successfully");
            }
            else if (status == 400)
            {
                // Node already started
                QMessageBox::warning(this, "Node Already Started", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") is already started");
            }
            else if (status == 404)
            {
                // Node does not exist
                QMessageBox::warning(this, "Node Not Found", "Node " + nodeId + " does not exist");
            }
            else
            {
                // Unknown status
                QMessageBox::warning(this, "Error", "Unknown response from the server");
            }
        }
        else
        {
            // PUT request failed
            QMessageBox::critical(this, "Error", "Failed to start the node: " + putReply->errorString());
        }

        putReply->deleteLater();
    }
    else
    {
        // GET request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + getReply->errorString());
    }

    getReply->deleteLater();
}


void NodesManagement::onStopNodeActionTriggered()
{
    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column

    QNetworkAccessManager manager;

    // Send a GET request to retrieve node information
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId));
    QNetworkReply* getReply = manager.get(getRequest);
    QEventLoop getLoop;
    connect(getReply, &QNetworkReply::finished, &getLoop, &QEventLoop::quit);
    getLoop.exec();

    if (getReply->error() == QNetworkReply::NoError)
    {
        // GET request successful
        QByteArray responseData = getReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject nodeInfo = jsonResponse.object();

        QString nodeType = nodeInfo.value("node_type").toString();
        QString nodeName = nodeInfo.value("node_name").toString();

        // Send a PUT request to stop the node
        QNetworkRequest putRequest(QUrl("http://localhost:3200/wms/mesh/nodes/stopNode/" + nodeId));
        QNetworkReply* putReply = manager.put(putRequest, QByteArray());
        QEventLoop putLoop;
        connect(putReply, &QNetworkReply::finished, &putLoop, &QEventLoop::quit);
        putLoop.exec();

        if (putReply->error() == QNetworkReply::NoError)
        {
            // PUT request successful
            QByteArray putResponseData = putReply->readAll();
            QJsonDocument putJsonResponse = QJsonDocument::fromJson(putResponseData);
            QJsonObject responseObj = putJsonResponse.object();

            int status = responseObj.value("status").toInt();
            QString message = responseObj.value("message").toString();

            if (status == 200)
            {
                // Node stopped successfully
                QMessageBox::information(this, "Node Stopped", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") stopped successfully");
            }
            else if (status == 400)
            {
                // Node already stopped
                QMessageBox::warning(this, "Node Already Stopped", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") is already stopped");
            }
            else if (status == 404)
            {
                // Node does not exist
                QMessageBox::warning(this, "Node Not Found", "Node " + nodeId + " does not exist");
            }
            else
            {
                // Unknown status
                QMessageBox::warning(this, "Error", "Unknown response from the server");
            }
        }
        else
        {
            // PUT request failed
            QMessageBox::critical(this, "Error", "Failed to stop the node: " + putReply->errorString());
        }

        putReply->deleteLater();
    }
    else
    {
        // GET request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + getReply->errorString());
    }

    getReply->deleteLater();
}


void NodesManagement::onRebootNodeActionTriggered()
{
    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column

    QNetworkAccessManager manager;

    // Send a POST request to reboot the node
    QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId + "/reboot"));
    QNetworkReply* postReply = manager.post(postRequest, QByteArray());

    // Show loading message
    QMessageBox loadingMessage;
    loadingMessage.setIcon(QMessageBox::Information);
    loadingMessage.setText("Rebooting node " + nodeId + "...");
    loadingMessage.setStandardButtons(QMessageBox::NoButton);
    loadingMessage.show();

    QEventLoop loop;
    connect(postReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check the response
    if (postReply->error() == QNetworkReply::NoError)
    {
        // POST request successful
        QByteArray postResponseData = postReply->readAll();
        QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
        QJsonObject responseObj = postJsonResponse.object();

        QString message = responseObj.value("message").toString();

        if (message == "node does not exist")
        {
            // Node does not exist
            loadingMessage.close();
            QMessageBox::warning(this, "Node Not Found", "Node " + nodeId + " does not exist");
        }
        else if (message == "node isn't booted already")
        {
            // Node isn't booted already
            loadingMessage.close();
            QMessageBox::warning(this, "Node Not Booted", "Node " + nodeId + " is not booted already");
        }
        else if (message == "rebooted succesfully")
        {
            loadingMessage.close();

            QMessageBox::information(this,"Success","rebooted succesfully");


        }
        else
        {
            // Unknown response message
            loadingMessage.close();
            QMessageBox::warning(this, "Error", "Unknown response from the server");
            postReply->deleteLater();
            return;
        }
    }
    else
    {
        // POST request failed
        loadingMessage.close();
        QMessageBox::critical(this, "Error", "Failed to reboot the node: " + postReply->errorString());
        postReply->deleteLater();
        return;
    }

    // Reboot initiated successfully, wait for the backend to finish rebooting
    connect(postReply, &QNetworkReply::finished, this, [this, &loadingMessage, postReply, nodeId]() {
        loadingMessage.close();
        QMessageBox::information(this, "Node Rebooted", "Node " + nodeId + " has finished rebooting");
        postReply->deleteLater();
    });
}







void NodesManagement::onShowLinksActionTriggered()
{
    nodesManagementLayout->removeItem(dockLayout);

    QTableWidgetItem* currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString selectedNodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID
    QString selectedNodeName= nodesManagementTable->item(rowIndex,1)->text();
    // Send a GET request to retrieve the links information for the selected node
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + selectedNodeId + "/links"));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonArray linksArray = jsonResponse.array();

        // Create the widget to display node links information
        nodeLinksWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(nodeLinksWidget);
        QFormLayout* formLayout = new QFormLayout;


        QString titleText = selectedNodeName+ " (" + selectedNodeId + ")" + " links:";
        QLabel* titleLabel = new QLabel(titleText);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setFont(QFont("Arial", 12, QFont::Bold));
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);


        formLayout->addRow(titleLabel);
        formLayout->addRow(line);


        for (const QJsonValue& linkValue : linksArray)
        {
            QJsonObject linkObject = linkValue.toObject();

            QString linkId = linkObject.value("link_id").toString();
            QString nodeFromId = linkObject.value("node_id_from").toString();
            QString interfaceFromId = linkObject.value("interface_from").toString();
            QString interfaceToId = linkObject.value("interface_to").toString();
            int linkType = linkObject.value("link_type").toInt();
            int linkValidity = linkObject.value("link_validity").toInt();

            // Get node from information
            QNetworkRequest getNodeFromRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeFromId));
            QNetworkReply* nodeFromReply = manager.get(getNodeFromRequest);
            QEventLoop nodeFromLoop;
            connect(nodeFromReply, &QNetworkReply::finished, &nodeFromLoop, &QEventLoop::quit);
            nodeFromLoop.exec();

            QString nodeFromName;
            QString nodeFromType;

            if (nodeFromReply->error() == QNetworkReply::NoError)
            {
                QByteArray nodeFromResponseData = nodeFromReply->readAll();
                QJsonDocument nodeFromJsonResponse = QJsonDocument::fromJson(nodeFromResponseData);
                QJsonObject nodeFromObject = nodeFromJsonResponse.object();
                nodeFromName = nodeFromObject.value("node_name").toString();
                nodeFromType = nodeFromObject.value("node_type").toString();
            }

            nodeFromReply->deleteLater();

            // Get node to information
            QNetworkRequest getNodeToRequest(QUrl("http://localhost:3200/wms/mesh/interfaces/" + interfaceToId));
            QNetworkReply* nodeToReply = manager.get(getNodeToRequest);
            QEventLoop nodeToLoop;
            connect(nodeToReply, &QNetworkReply::finished, &nodeToLoop, &QEventLoop::quit);
            nodeToLoop.exec();

            QString nodeToName;
            QString nodeToType;

            if (nodeToReply->error() == QNetworkReply::NoError)
            {
                QByteArray nodeToResponseData = nodeToReply->readAll();
                QJsonDocument nodeToJsonResponse = QJsonDocument::fromJson(nodeToResponseData);
                QJsonObject nodeToObject = nodeToJsonResponse.object();
                QString nodeId = nodeToObject.value("node_id").toString();

                QNetworkRequest getNodeRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId));
                QNetworkReply* getNodeReply = manager.get(getNodeRequest);
                QEventLoop getNodeLoop;
                connect(getNodeReply, &QNetworkReply::finished, &getNodeLoop, &QEventLoop::quit);
                getNodeLoop.exec();

                if (getNodeReply->error() == QNetworkReply::NoError)
                {
                    QByteArray getNodeResponseData = getNodeReply->readAll();
                    QJsonDocument getNodeJsonResponse = QJsonDocument::fromJson(getNodeResponseData);
                    QJsonObject getNodeObject = getNodeJsonResponse.object();
                    nodeToName = getNodeObject.value("node_name").toString();
                    nodeToType = getNodeObject.value("node_type").toString();
                }

                getNodeReply->deleteLater();
            }

            nodeToReply->deleteLater();

            // Get interface from information
            QNetworkRequest getInterfaceFromRequest(QUrl("http://localhost:3200/wms/mesh/interfaces/" + interfaceFromId));
            QNetworkReply* interfaceFromReply = manager.get(getInterfaceFromRequest);
            QEventLoop interfaceFromLoop;
            connect(interfaceFromReply, &QNetworkReply::finished, &interfaceFromLoop, &QEventLoop::quit);
            interfaceFromLoop.exec();

            QString interfaceFromName;

            if (interfaceFromReply->error() == QNetworkReply::NoError)
            {
                QByteArray interfaceFromResponseData = interfaceFromReply->readAll();
                QJsonDocument interfaceFromJsonResponse = QJsonDocument::fromJson(interfaceFromResponseData);
                QJsonObject interfaceFromObject = interfaceFromJsonResponse.object();
                interfaceFromName = interfaceFromObject.value("interface_name").toString();
            }

            interfaceFromReply->deleteLater();

            // Get interface to information
            QNetworkRequest getInterfaceToRequest(QUrl("http://localhost:3200/wms/mesh/interfaces/" + interfaceToId));
            QNetworkReply* interfaceToReply = manager.get(getInterfaceToRequest);
            QEventLoop interfaceToLoop;
            connect(interfaceToReply, &QNetworkReply::finished, &interfaceToLoop, &QEventLoop::quit);
            interfaceToLoop.exec();

            QString interfaceToName;

            if (interfaceToReply->error() == QNetworkReply::NoError)
            {
                QByteArray interfaceToResponseData = interfaceToReply->readAll();
                QJsonDocument interfaceToJsonResponse = QJsonDocument::fromJson(interfaceToResponseData);
                QJsonObject interfaceToObject = interfaceToJsonResponse.object();
                interfaceToName = interfaceToObject.value("interface_name").toString();
            }

            interfaceToReply->deleteLater();

            // Create labels to display the information


            //            QLabel* nodeFromLabel = new QLabel("Node From: ");
            //            QLabel* nodeFromValueLabel= new QLabel(nodeFromName+ " (" + nodeFromType + ")");
            QLabel* nodeToLabel = new QLabel("Node Linked To: ");
            QLabel*nodeToValueLabel= new QLabel(nodeToName + " (" + nodeToType + ")");
            QLabel* interfaceFromLabel = new QLabel("Interface Linked From: ");
            QLabel* interfaceFromValueLabel = new QLabel(interfaceFromId + " (" + interfaceFromName + ")");
            QLabel* interfaceToLabel = new QLabel("Interface Linked To: ");
            QLabel * interfaceToValueLabel= new QLabel(interfaceToId + " (" + interfaceToName + ")");

            // Create a label for link information
            QString linkTypeString = (linkType == 1) ? "Wired" : "Wireless";
            QString linkValidityString = (linkValidity == 1) ? "Valid" : "Invalid";

            QLabel *linkIdLabel=new QLabel("Link ID: ");
            QLabel *linkIdValueLabel = new QLabel(linkId);
            QLabel* linkTypeLabel = new QLabel("Link Type: ");
            QLabel* linkTypeValueLabel = new QLabel(linkTypeString);
            QLabel* linkValidityLabel= new QLabel("Link Validity: ");
            QLabel* linkValidityValueLabel= new QLabel(linkValidityString);


            // Add labels to the layout

            formLayout->addRow(linkIdLabel,linkIdValueLabel);
            formLayout->addRow(linkTypeLabel,linkTypeValueLabel);
            formLayout->addRow(linkValidityLabel,linkValidityValueLabel);
            //            formLayout->addRow(nodeFromLabel,nodeFromValueLabel);
            formLayout->addRow(nodeToLabel,nodeToValueLabel);
            formLayout->addRow(interfaceFromLabel,interfaceFromValueLabel);
            formLayout->addRow(interfaceToLabel,interfaceToValueLabel);

            // Add a separator line between each link information
            QFrame* line = new QFrame();
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            // Add delete button
            QPushButton* deleteButton = new QPushButton("Delete");
            QIcon deleteIcon(":/images/deleteIcon.png");
            deleteIcon = deleteIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
            deleteButton ->setIcon(deleteIcon);
            deleteButton ->setObjectName("cancelButton");
            deleteButton ->setFixedWidth(200);
            formLayout->addRow(deleteButton);
            connect(deleteButton, &QPushButton::clicked, [=]() {
                QNetworkAccessManager manager;
                QNetworkRequest deleteRequest(QUrl("http://localhost:3200/wms/mesh/links/" + linkId));

                QNetworkReply* deleteReply = manager.deleteResource(deleteRequest);
                QEventLoop deleteLoop;
                connect(deleteReply, &QNetworkReply::finished, &deleteLoop, &QEventLoop::quit);
                deleteLoop.exec();

                if (deleteReply->error() == QNetworkReply::NoError) {
                    // Deletion successful, refresh the GUI
                     QMessageBox::information(this, "Success", "Link deleted successfully");
                    deleteReply->deleteLater();
                    nodeLinksWidget->close();
                    onShowLinksActionTriggered();
                } else {
                    // Deletion failed, handle the error
                    deleteReply->deleteLater();
                    QMessageBox::critical(this, "Error", "Failed to delete the link.");
                }
            });



            formLayout->addRow(line);
        }

        // Create a scroll area and set the layout as its widget
        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget *scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);




        QPushButton *closeButton = new QPushButton("Close");

        QIcon cancelIcon(":/images/closeIcon.png");
        cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        closeButton->setIcon(cancelIcon);
        closeButton->setObjectName("cancelButton");
        closeButton->setFixedWidth(200);
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);
        // Connect the close button's clicked signal to close the widget
        connect(closeButton, &QPushButton::clicked, nodeLinksWidget, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockWidget, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockContent, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockContent, [this]()
        { nodesManagementLayout->removeItem(dockLayout); });



        // Add the dock widget to the dock content area
        dockContent->addWidget(nodeLinksWidget);
        dockContent->setCurrentWidget(nodeLinksWidget);
        displayDockWidget("Node Links");
    }

    reply->deleteLater();
}



//TOMORROW

void NodesManagement::onGetFirmwareActionTriggered() {

    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column
    qDebug()<<nodeId;

    // Send a GET request to the backend endpoint
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("http://localhost:3200/wms/mesh/nodes/"+nodeId+"/firmwareVersion"));
    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    // Connect the finished signal to process the response
    if (reply->error() == QNetworkReply::NoError) {
        // Read the response data
        QByteArray responseData = reply->readAll();

        // Parse the JSON response
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonDoc.object();

        // Extract the firmware version
        QString firmwareVersion = jsonObject["node_fw_version"].toString();
        qDebug()<<firmwareVersion;
        // Create a QDialog to display the firmware version
        QDialog dialog(this);
        dialog.setWindowTitle("Firmware Version");
        QVBoxLayout* layout = new QVBoxLayout(&dialog);
        QLabel* firmwareLabel = new QLabel(firmwareVersion, &dialog);
        layout->addWidget(firmwareLabel);

        // Add a close button
        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
        layout->addWidget(buttonBox);

        // Connect the close button's clicked signal to close the dialog
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::close);

        // Set the dialog's layout and display it
        dialog.setLayout(layout);
        dialog.exec();
    } else {
        // Handle the error case
        QMessageBox::critical(this, "Error", "Failed to retrieve firmware version: " + reply->errorString());
    }

    // Clean up resources
    reply->deleteLater();

}

void NodesManagement::onStartMonitoringActionTriggered() {
    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column

    QNetworkAccessManager manager ;

    // Send a GET request to retrieve node information
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId));
    QNetworkReply* getReply = manager.get(getRequest);
    QEventLoop getLoop;
    connect(getReply, &QNetworkReply::finished, &getLoop, &QEventLoop::quit);
    getLoop.exec();

    if (getReply->error() == QNetworkReply::NoError) {
        // GET request successful
        QByteArray responseData = getReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject nodeInfo = jsonResponse.object();

        QString nodeType = nodeInfo.value("node_type").toString();
        QString nodeName = nodeInfo.value("node_name").toString();

        // Send a POST request to start the node monitoring
        QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId + "/startMonitoring"));
        QNetworkReply* postReply = manager.post(postRequest, QByteArray());
        QEventLoop postLoop;
        connect(postReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
        postLoop.exec();

        if (postReply->error() == QNetworkReply::NoError) {
            // POST request successful
            QByteArray postResponseData = postReply->readAll();
            QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
            QJsonObject responseObj = postJsonResponse.object();

            int status = responseObj.value("status").toInt();
            QString message = responseObj.value("message").toString();

            if (status == 404) {
                // Node does not exist
                QMessageBox::warning(this, "Node Not Found", "Node with MAC Address (" + nodeId + ") does not exist");
            }
            else if (status == 200) {
                // Node monitoring started successfully
                QMessageBox::information(this, "Node Monitoring Started", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") monitoring is started");
            }
            else if (status == 202) {
                // Node already started
                QMessageBox::warning(this, "Node Monitoring Already Started", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") monitoring has already started");
            }
            else if (status == 409) {
                // Node is off
                QMessageBox::warning(this, "Node is Off", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") should be turned on in order to start monitoring");
            }
            else {
                QMessageBox::warning(this, "Error", "Unknown response from the server");
            }
        }
        else {
            QMessageBox::warning(this, "Error", "Failed to send startMonitoring request: " + postReply->errorString());
        }

        postReply->deleteLater();
    }
    else {
        // GET request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + getReply->errorString());
    }

    getReply->deleteLater();
}

void NodesManagement::onStopMonitoringActionTriggered() {
    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column

    QNetworkAccessManager manager ;

    // Send a GET request to retrieve node information
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId));
    QNetworkReply* getReply = manager.get(getRequest);
    QEventLoop getLoop;
    connect(getReply, &QNetworkReply::finished, &getLoop, &QEventLoop::quit);
    getLoop.exec();

    if (getReply->error() == QNetworkReply::NoError) {
        // GET request successful
        QByteArray responseData = getReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject nodeInfo = jsonResponse.object();

        QString nodeType = nodeInfo.value("node_type").toString();
        QString nodeName = nodeInfo.value("node_name").toString();

        // Send a POST request to stop node monitoring
        QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId + "/stopMonitoring"));
        QNetworkReply* postReply = manager.post(postRequest, QByteArray());
        QEventLoop postLoop;
        connect(postReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
        postLoop.exec();

        if (postReply->error() == QNetworkReply::NoError) {
            // POST request successful
            QByteArray postResponseData = postReply->readAll();
            QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
            QJsonObject responseObj = postJsonResponse.object();

            int status = responseObj.value("status").toInt();
            QString message = responseObj.value("message").toString();

            if (status == 404) {
                // Node does not exist
                QMessageBox::warning(this, "Node Not Found", "Node with MAC Address (" + nodeId + ") does not exist");
            }
            else if (status == 200) {
                // Node monitoring stopped successfully
                QMessageBox::information(this, "Node Monitoring Stopped", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") monitoring is stopped");
            }
            else if (status == 202) {
                // Node already not monitored
                QMessageBox::warning(this, "Node Monitoring Already Stopped", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") monitoring has already stopped");
            }
            else if (status == 409) {
                // Node is off
                QMessageBox::warning(this, "Node is Off", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") should be turned on in order to stop monitoring");
            }
            else {
                QMessageBox::warning(this, "Error", "Unknown response from the server");
            }
        }
        else {
            QMessageBox::warning(this, "Error", "Failed to send stopMonitoring request: " + postReply->errorString());
        }

        postReply->deleteLater();
    }
    else {
        // GET request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + getReply->errorString());
    }

    getReply->deleteLater();
}
void NodesManagement::onStartVumeterActionTriggered() {
    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column

    QNetworkAccessManager manager;

    // Send a GET request to retrieve node information
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId));
    QNetworkReply* getReply = manager.get(getRequest);
    QEventLoop getLoop;
    connect(getReply, &QNetworkReply::finished, &getLoop, &QEventLoop::quit);
    getLoop.exec();

    if (getReply->error() == QNetworkReply::NoError) {
        // GET request successful
        QByteArray responseData = getReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject nodeInfo = jsonResponse.object();

        QString nodeType = nodeInfo.value("node_type").toString();
        QString nodeName = nodeInfo.value("node_name").toString();

        // Send a POST request to start Vumeter
        QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId + "/startVumeter"));
        QNetworkReply* postReply = manager.post(postRequest, QByteArray());
        QEventLoop postLoop;
        connect(postReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
        postLoop.exec();

        if (postReply->error() == QNetworkReply::NoError) {
            // POST request successful
            QByteArray postResponseData = postReply->readAll();
            QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
            QJsonObject responseObj = postJsonResponse.object();

            int status = responseObj.value("status").toInt();
            QString message = responseObj.value("message").toString();

            if (status == 404) {
                // Node does not exist
                QMessageBox::warning(this, "Node Not Found", "Node with MAC Address (" + nodeId + ") does not exist");
            }
            else if (status == 200) {
                // Vumeter started successfully
                QMessageBox::information(this, "Vumeter Started", "Vumeter for Node " + nodeId + " (" + nodeType + " - " + nodeName + ") is started");
            }
            else if (status == 202) {
                // Vumeter already started
                QMessageBox::warning(this, "Vumeter Already Started", "Vumeter for Node " + nodeId + " (" + nodeType + " - " + nodeName + ") has already started");
            }
            else if (status == 409) {
                // Monitoring must be started
                QMessageBox::warning(this, "Monitoring Not Started", "Monitoring for Node " + nodeId + " (" + nodeType + " - " + nodeName + ") must be started before starting the Vumeter");
            }
            else {
                QMessageBox::warning(this, "Error", "Unknown response from the server");
            }
        }
        else {
            QMessageBox::warning(this, "Error", "Failed to send startVumeter request: " + postReply->errorString());
        }

        postReply->deleteLater();
    }
    else {
        // GET request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + getReply->errorString());
    }

    getReply->deleteLater();
}


void NodesManagement::onStopVumeterActionTriggered() {
    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString nodeId = nodesManagementTable->item(rowIndex, 0)->text(); // Assuming node ID is stored in the first column

    QNetworkAccessManager manager ;

    // Send a GET request to retrieve node information
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId));
    QNetworkReply* getReply = manager.get(getRequest);
    QEventLoop getLoop;
    connect(getReply, &QNetworkReply::finished, &getLoop, &QEventLoop::quit);
    getLoop.exec();

    if (getReply->error() == QNetworkReply::NoError) {
        // GET request successful
        QByteArray responseData = getReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject nodeInfo = jsonResponse.object();

        QString nodeType = nodeInfo.value("node_type").toString();
        QString nodeName = nodeInfo.value("node_name").toString();

        // Send a POST request to stop node monitoring
        QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + nodeId + "/stopVumeter"));
        QNetworkReply* postReply = manager.post(postRequest, QByteArray());
        QEventLoop postLoop;
        connect(postReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
        postLoop.exec();

        if (postReply->error() == QNetworkReply::NoError) {
            // POST request successful
            QByteArray postResponseData = postReply->readAll();
            QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
            QJsonObject responseObj = postJsonResponse.object();

            int status = responseObj.value("status").toInt();
            QString message = responseObj.value("message").toString();

            if (status == 404) {
                // Node does not exist
                QMessageBox::warning(this, "Node Not Found", "Node with MAC Address (" + nodeId + ") does not exist");
            }
            else if (status == 200) {
                // Node monitoring stopped successfully
                QMessageBox::information(this, "Node Vumeter Stopped", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") Vumeter is stopped");
            }
            else if (status == 202) {
                // Node already not monitored
                QMessageBox::warning(this, "Node Vumeter Already Stopped", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") Vumeter has already stopped");
            }
            else if (status == 409) {
                // Node is off
                QMessageBox::warning(this, "Node is Off", "Node " + nodeId + " (" + nodeType + " - " + nodeName + ") should be turned on in order to stop Vumeter");
            }
            else {
                QMessageBox::warning(this, "Error", "Unknown response from the server");
            }
        }
        else {
            QMessageBox::warning(this, "Error", "Failed to send stop vumeter request: " + postReply->errorString());
        }

        postReply->deleteLater();
    }
    else {
        // GET request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve node information: " + getReply->errorString());
    }

    getReply->deleteLater();
}
//void NodesManagement::onGetSSHFingerPrints(){

//}
//void NodesManagement::onGetVumeterStatus(){
//    QTableWidgetItem *currentItem = nodesManagementTable->currentItem();
//    int rowIndex = nodesManagementTable->row(currentItem);
//    QString nodeId = nodesManagementTable->item(rowIndex, 1)->text(); // Assuming node ID is stored in the first column
//    qDebug()<<nodeId;

//    // Send a GET request to the backend endpoint
//    QNetworkAccessManager manager;
//    QNetworkRequest request(QUrl("http://localhost:3200/wms/mesh/nodes/"+nodeId+"/vumeterStatus"));
//    QNetworkReply* reply = manager.get(request);
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();
//    // Connect the finished signal to process the response
//        if (reply->error() == QNetworkReply::NoError) {
//            // Read the response data
//            QByteArray responseData = reply->readAll();

//            // Parse the JSON response
//            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
//            QJsonObject jsonObject = jsonDoc.object();

//            // Extract the firmware version
//            bool vumeterStatus = jsonObject["vumeterStatus"].toBool();
//            qDebug()<<vumeterStatus;
//            // Create a QDialog to display the firmware version
//            QDialog dialog(this);
//            dialog.setWindowTitle("Vumeter Status");
//            QVBoxLayout* layout = new QVBoxLayout(&dialog);
//            QString status;
//            if(vumeterStatus==true){
//               status="Vumeter is ON";

//            }else{
//                status="Vumeter is OFF";

//            }
//            QLabel* vumeterStatusLabel = new QLabel(status, &dialog);
//            layout->addWidget(vumeterStatusLabel);

//            // Add a close button
//            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
//            layout->addWidget(buttonBox);

//            // Connect the close button's clicked signal to close the dialog
//            connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::close);

//            // Set the dialog's layout and display it
//            dialog.setLayout(layout);
//            dialog.exec();
//        } else {
//            // Handle the error case
//            QMessageBox::critical(this, "Error", "Failed to retrieve firmware version: " + reply->errorString());
//        }

//        // Clean up resources
//        reply->deleteLater();



//}





//multiple window

void NodesManagement::onStartMultipleNodesActionTriggered(){

    QList<QTableWidgetItem*> selectedItems = nodesManagementTable->selectedItems();
    QSet<QString> nodeIdSet;
    // Extract node IDs from selected rows

    for (QTableWidgetItem* item : selectedItems)
    {
        int rowIndex = nodesManagementTable->row(item);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        nodeIdSet.insert(nodeId); // Add node ID to the set


    }
    QStringList nodeIds = nodeIdSet.toList(); // Convert set to list


    if (nodeIds.isEmpty())
    {
        QMessageBox::warning(this, "No Nodes Selected", "Please select at least one node to start.");
        return;
    }

    QNetworkAccessManager manager;

    // Create JSON payload
    QJsonObject payload;
    payload["node_ids"] = QJsonArray::fromStringList(nodeIds);

    // Send a PUT request to start multiple nodes
    QNetworkRequest putRequest(QUrl("http://localhost:3200/wms/mesh/nodes/startNodes"));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* putReply = manager.put(putRequest, QJsonDocument(payload).toJson());
    QEventLoop postLoop;
    connect(putReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
    postLoop.exec();

    if (putReply->error() == QNetworkReply::NoError)
    {
        // POST request successful
        QByteArray responseData = putReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObj = jsonResponse.object();

        QString message = responseObj.value("message").toString();
        QJsonArray startedNodes = responseObj.value("startedNodes").toArray();

        QStringList startedNodeList;
        for (const QJsonValue& node : startedNodes)
        {
            startedNodeList.append(node.toString());
        }

        QMessageBox::information(this, "Nodes Started", message + ": " + startedNodeList.join(", "));
    }
    else
    {
        // POST request failed
        QMessageBox::critical(this, "Error", "Failed to start nodes: " + putReply->errorString());
    }

    putReply->deleteLater();
}


void NodesManagement::onStopMultipleNodesActionTriggered(){
    QList<QTableWidgetItem*> selectedItems = nodesManagementTable->selectedItems();
    QSet<QString> nodeIdSet;

    // Extract node IDs from selected rows
    for (QTableWidgetItem* item : selectedItems)
    {
        int rowIndex = nodesManagementTable->row(item);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        nodeIdSet.insert(nodeId); // Add node ID to the set
    }
    QStringList nodeIds = nodeIdSet.toList(); // Convert set to list


    if (nodeIds.isEmpty())
    {
        QMessageBox::warning(this, "No Nodes Selected", "Please select at least one node to stop.");
        return;
    }

    QNetworkAccessManager manager;

    // Create JSON payload
    QJsonObject payload;
    payload["node_ids"] = QJsonArray::fromStringList(nodeIds);

    // Send a PUT request to stop multiple nodes
    QNetworkRequest putRequest(QUrl("http://localhost:3200/wms/mesh/nodes/stopNodes"));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* putReply = manager.put(putRequest, QJsonDocument(payload).toJson());
    QEventLoop postLoop;
    connect(putReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
    postLoop.exec();

    if (putReply->error() == QNetworkReply::NoError)
    {
        // POST request successful
        QByteArray responseData = putReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObj = jsonResponse.object();

        int status = responseObj.value("status").toInt();
        QString message = responseObj.value("message").toString();
        QJsonArray stoppedNodes = responseObj.value("stoppedNodes").toArray();

        QStringList stoppedNodeList;
        for (const QJsonValue& node : stoppedNodes)
        {
            stoppedNodeList.append(node.toString());
        }

        if (status == 200)
        {
            QMessageBox::information(this, "Nodes Stopped", message + ": " + stoppedNodeList.join(", "));
        }
        else if (status == 404)
        {
            QMessageBox::warning(this, "Nodes Not Found", message);
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unknown response from the server");
        }
    }
    else
    {
        // POST request failed
        QMessageBox::critical(this, "Error", "Failed to stop nodes: " + putReply->errorString());
    }

    putReply->deleteLater();

}
void NodesManagement::onRebootMultipleNodesActionTriggered(){

    QList<QTableWidgetItem*> selectedItems = nodesManagementTable->selectedItems();
    QSet<QString> nodeIdSet;

    // Extract node IDs from selected rows
    for (QTableWidgetItem* item : selectedItems)
    {
        int rowIndex = nodesManagementTable->row(item);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        nodeIdSet.insert(nodeId);
    }
    QStringList nodeIds = nodeIdSet.toList(); // Convert set to list
    if (nodeIds.isEmpty())
    {
        QMessageBox::warning(this, "No Nodes Selected", "Please select at least one node to reboot.");
        return;
    }

    QNetworkAccessManager manager;

    // Create JSON payload
    QJsonObject payload;
    payload["node_ids"] = QJsonArray::fromStringList(nodeIds);

    // Send a POST request to reboot multiple nodes
    QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/multiReboot"));
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* postReply = manager.post(postRequest, QJsonDocument(payload).toJson());

    // Show loading message
    QMessageBox loadingMessage;
    loadingMessage.setIcon(QMessageBox::Information);
    loadingMessage.setText("Rebooting selected nodes...");
    loadingMessage.setStandardButtons(QMessageBox::NoButton);
    loadingMessage.show();

    QEventLoop loop;
    connect(postReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check the response
    if (postReply->error() == QNetworkReply::NoError)
    {
        // POST request successful
        QByteArray postResponseData = postReply->readAll();
        QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
        QJsonObject responseObj = postJsonResponse.object();

        int status = responseObj.value("status").toInt();
        QString successMessage = responseObj.value("SuccessMessage").toString();
        QStringList rebootedNodes = responseObj.value("rebootedNodes").toVariant().toStringList();
        QString failureMessage = responseObj.value("FailureMessage").toString();
        QStringList downNodes = responseObj.value("downNodes").toVariant().toStringList();

        loadingMessage.close();

        if (status == 200)
        {
            // Nodes rebooted successfully
            QString message = successMessage + rebootedNodes.join(", ");
            QMessageBox::information(this, "Nodes Rebooted", message);
        }
        else
        {
            // Nodes not rebooted
            QString message = failureMessage + downNodes.join(", ");
            QMessageBox::warning(this, "Nodes Not Rebooted", message);
        }
    }
    else
    {
        // POST request failed
        loadingMessage.close();
        QMessageBox::critical(this, "Error", "Failed to reboot the nodes: " + postReply->errorString());
    }

    postReply->deleteLater();

}
void NodesManagement::onDeleteMultipleNodesActionTriggered() {
    QList<QTableWidgetItem*> selectedItems = nodesManagementTable->selectedItems();
    QSet<QString> nodeIdSet;

    // Extract node IDs from selected rows
    for (QTableWidgetItem* item : selectedItems) {
        int rowIndex = nodesManagementTable->row(item);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        nodeIdSet.insert(nodeId);
    }
    QStringList nodeIds = nodeIdSet.toList(); // Convert set to list

    if (nodeIds.isEmpty()) {
        QMessageBox::warning(this, "No Nodes Selected", "Please select at least one node to delete.");
        return;
    }

    QJsonArray nodeIdsArray;
    for (const QString& nodeId : nodeIds) {
        nodeIdsArray.append(nodeId);
    }

    QJsonObject requestBody;
    requestBody["node_ids"] = nodeIdsArray;

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("http://localhost:3200/wms/mesh/nodes/deleteNodes"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = manager.sendCustomRequest(request, "DELETE", QJsonDocument(requestBody).toJson());
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonResponse = jsonDoc.object();
        int status = jsonResponse["status"].toInt();
        QString message = jsonResponse["message"].toString();

        if (status == 200) {
            qDebug() << "Nodes deleted successfully";
            QJsonArray deletedNodesArray = jsonResponse["deletedNodes"].toArray();
            QStringList deletedNodes;
            for (const QJsonValue& value : deletedNodesArray) {
                deletedNodes.append(value.toString());
            }
            // Update table and display success message
            populateTable();
            QMessageBox::information(this, "Success", "The selected nodes have been deleted successfully");
        } else if (status == 404) {
            qDebug() << "Nodes do not exist";
            QMessageBox::warning(this, "Error", "Nodes do not exist");
        } else {
            qDebug() << "Failed to delete nodes: Unexpected response";
            QMessageBox::critical(this, "Error", "Unexpected response from the server");
        }
    } else {
        QString errorMessage = reply->errorString();
        qDebug() << "Failed to delete nodes: " << errorMessage;

        switch (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) {
        case 400:
            QMessageBox::warning(this, "Error", "Please turn off the nodes first!");
            break;
        default:
            QMessageBox::critical(this, "Error", "Failed to delete the nodes");
            break;
        }
    }

    reply->deleteLater();
}
void NodesManagement::onStartMultipleNodesMonitoringActionTriggered() {
    QList<QTableWidgetItem*> selectedItems = nodesManagementTable->selectedItems();
    QSet<QString> nodeIdSet;

    // Extract node IDs from selected rows
    for (QTableWidgetItem* item : selectedItems) {
        int rowIndex = nodesManagementTable->row(item);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        nodeIdSet.insert(nodeId);
    }

    if (nodeIdSet.isEmpty()) {
        QMessageBox::warning(this, "No Nodes Selected", "Please select at least one node to start monitoring.");
        return;
    }

    QStringList nodeIds = nodeIdSet.toList(); // Convert set to list

    // Create JSON object with node IDs
    QJsonObject requestBody;
    requestBody["node_ids"] = QJsonArray::fromStringList(nodeIds);

    QNetworkAccessManager manager;

    // Send a POST request to start node monitoring for multiple nodes
    QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/startNodesMonitoring"));
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* postReply = manager.post(postRequest, QJsonDocument(requestBody).toJson());
    QEventLoop postLoop;
    connect(postReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
    postLoop.exec();

    if (postReply->error() == QNetworkReply::NoError) {
        // POST request successful
        QByteArray postResponseData = postReply->readAll();
        QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
        QJsonObject responseObj = postJsonResponse.object();

        int status = responseObj.value("status").toInt();
        QString message = responseObj.value("message").toString();

        if (status == 404) {
            // Node does not exist
            QMessageBox::warning(this, "Node Not Found", "One or more selected nodes do not exist");
        }
        else if (status == 200) {
            // Node monitoring started successfully
            QStringList startedNodes = responseObj.value("startedNodes").toVariant().toStringList();
            QStringList notStartedMonitoring = responseObj.value("notStartedMonitoring").toVariant().toStringList();

            QString successMessage = responseObj.value("SuccessMessage").toString();
            QString failureMessage = responseObj.value("FailureMessage").toString();

            QMessageBox::information(this, "Node Monitoring Started", successMessage + "\n\nMonitoring started for Nodes:\n" + startedNodes.join("\n") +
                                     "\n\n" + failureMessage + "\n" + notStartedMonitoring.join("\n"));
        }
        else {
            QMessageBox::warning(this, "Error", "Unknown response from the server");
        }
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to send startNodesMonitoring request: " + postReply->errorString());
    }

    postReply->deleteLater();
}
void NodesManagement::onStopMultipleNodesMonitoringActionTriggered() {
    QList<QTableWidgetItem*> selectedItems = nodesManagementTable->selectedItems();
    QSet<QString> nodeIdSet;

    // Extract node IDs from selected rows
    for (QTableWidgetItem* item : selectedItems) {
        int rowIndex = nodesManagementTable->row(item);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        nodeIdSet.insert(nodeId);
    }

    if (nodeIdSet.isEmpty()) {
        QMessageBox::warning(this, "No Nodes Selected", "Please select at least one node to stop monitoring.");
        return;
    }

    QStringList nodeIds = nodeIdSet.toList(); // Convert set to list

    // Create JSON object with node IDs
    QJsonObject requestBody;
    requestBody["node_ids"] = QJsonArray::fromStringList(nodeIds);

    QNetworkAccessManager manager;

    // Send a POST request to stop node monitoring for multiple nodes
    QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/stopNodesMonitoring"));
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* postReply = manager.post(postRequest, QJsonDocument(requestBody).toJson());
    QEventLoop postLoop;
    connect(postReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
    postLoop.exec();

    if (postReply->error() == QNetworkReply::NoError) {
        // POST request successful
        QByteArray postResponseData = postReply->readAll();
        QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
        QJsonObject responseObj = postJsonResponse.object();

        int status = responseObj.value("status").toInt();
        QString message = responseObj.value("message").toString();

        if (status == 404) {
            // Node does not exist
            QMessageBox::warning(this, "Node Not Found", "One or more selected nodes do not exist");
        }
        else if (status == 200) {
            // Node monitoring stopped successfully
            QStringList stoppedNodes = responseObj.value("stoppedNodes").toVariant().toStringList();
            QStringList shutDownNodes = responseObj.value("shutDownNodes").toVariant().toStringList();

            QString successMessage = responseObj.value("SuccessMessage").toString();
            QString failureMessage = responseObj.value("FailureMessage").toString();

            QMessageBox::information(this, "Node Monitoring Stopped", successMessage + "\n\nStopped monitoring for Nodes:\n" + stoppedNodes.join("\n") +
                                     "\n\nNodes that are turned off:\n" + failureMessage + "\n" + shutDownNodes.join("\n"));
        }
        else {
            QMessageBox::warning(this, "Error", "Unknown response from the server");
        }
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to send stopNodesMonitoring request: " + postReply->errorString());
    }

    postReply->deleteLater();
}

void NodesManagement::onStartMultipleNodesVumeterActionTriggered() {
    QList<QTableWidgetItem*> selectedItems = nodesManagementTable->selectedItems();
    QSet<QString> nodeIdSet;

    // Extract node IDs from selected rows
    for (QTableWidgetItem* item : selectedItems) {
        int rowIndex = nodesManagementTable->row(item);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        nodeIdSet.insert(nodeId);
    }

    if (nodeIdSet.isEmpty()) {
        QMessageBox::warning(this, "No Nodes Selected", "Please select at least one node to start monitoring.");
        return;
    }

    QStringList nodeIds = nodeIdSet.toList(); // Convert set to list

    // Create JSON object with node IDs
    QJsonObject requestBody;
    requestBody["node_ids"] = QJsonArray::fromStringList(nodeIds);

    QNetworkAccessManager manager;

    // Send a POST request to start node monitoring for multiple nodes
    QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/startNodesVumeter"));
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* postReply = manager.post(postRequest, QJsonDocument(requestBody).toJson());
    QEventLoop postLoop;
    connect(postReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
    postLoop.exec();

    if (postReply->error() == QNetworkReply::NoError) {
        // POST request successful
        QByteArray postResponseData = postReply->readAll();
        QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
        QJsonObject responseObj = postJsonResponse.object();

        int status = responseObj.value("status").toInt();
        QString successMessage = responseObj.value("SuccessMessage").toString();
        QString failureMessage = responseObj.value("FailureMessage").toString();
        QJsonArray vumeterStartedForNodes = responseObj.value("vumeter_started_for_nodes").toArray();
        QJsonArray vumeterNotStartedForNodes = responseObj.value("vumeter_not_started_for_nodes").toArray();

        if (status == 404) {
            // Node does not exist
            QMessageBox::warning(this, "Node Not Found", "One or more nodes do not exist");
        }
        else if (status == 200) {
            // Vumeter started successfully
            QString startedNodes;
            QString notStartedNodes;

            // Process started nodes
            for (const QJsonValue& nodeValue : vumeterStartedForNodes) {
                QString node = nodeValue.toString();
                startedNodes += node + "\n";
            }

            // Process not started nodes
            for (const QJsonValue& nodeValue : vumeterNotStartedForNodes) {
                QString node = nodeValue.toString();
                notStartedNodes += node + "\n";
            }

            QMessageBox::information(this, "Vumeter Status", "Vumeter Started for Nodes:\n" + startedNodes + "\n\n" + "Vumeter not started for Nodes :\n" + notStartedNodes);
        }
        else {
            QMessageBox::warning(this, "Error", "Unknown response from the server");
        }
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to send startNodesVumeter request: " + postReply->errorString());
    }

    postReply->deleteLater();
}
void NodesManagement::onStopMultipleNodesVumeterActionTriggered() {
    QList<QTableWidgetItem*> selectedItems = nodesManagementTable->selectedItems();
    QSet<QString> nodeIdSet;

    // Extract node IDs from selected rows
    for (QTableWidgetItem* item : selectedItems) {
        int rowIndex = nodesManagementTable->row(item);
        QString nodeId = nodesManagementTable->item(rowIndex, 0)->text();
        nodeIdSet.insert(nodeId);
    }

    if (nodeIdSet.isEmpty()) {
        QMessageBox::warning(this, "No Nodes Selected", "Please select at least one node to stop monitoring.");
        return;
    }

    QStringList nodeIds = nodeIdSet.toList(); // Convert set to list

    // Create JSON object with node IDs
    QJsonObject requestBody;
    requestBody["node_ids"] = QJsonArray::fromStringList(nodeIds);

    QNetworkAccessManager manager;

    // Send a POST request to stop node monitoring for multiple nodes
    QNetworkRequest postRequest(QUrl("http://localhost:3200/wms/mesh/nodes/stopNodesVumeter"));
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* postReply = manager.post(postRequest, QJsonDocument(requestBody).toJson());
    QEventLoop postLoop;
    connect(postReply, &QNetworkReply::finished, &postLoop, &QEventLoop::quit);
    postLoop.exec();

    if (postReply->error() == QNetworkReply::NoError) {
        // POST request successful
        QByteArray postResponseData = postReply->readAll();
        QJsonDocument postJsonResponse = QJsonDocument::fromJson(postResponseData);
        QJsonObject responseObj = postJsonResponse.object();

        int status = responseObj.value("status").toInt();
        QJsonArray vumeterStoppedForNodes = responseObj.value("stoppedNodes").toArray();
        QJsonArray shutDownNodes = responseObj.value("shutDownNodes").toArray();

        if (status == 404) {
            // Node does not exist
            QMessageBox::warning(this, "Node Not Found", "One or more nodes do not exist");
        }
        else if (status == 200) {
            // Vumeter stopped successfully
            QString stoppedNodes;
            QString shutDownedNodes;

            // Process stopped nodes
            for (const QJsonValue& nodeValue : vumeterStoppedForNodes) {
                QString node = nodeValue.toString();
                stoppedNodes += node + "\n";
            }

            // Process shut down nodes
            for (const QJsonValue& nodeValue : shutDownNodes) {
                QString node = nodeValue.toString();
                shutDownedNodes += node + "\n";
            }

            QMessageBox::information(this, "Vumeter Stopped", "Stopped Nodes Vumeters:\n" + stoppedNodes + "\n" + "Vumeter not stopped for nodes : "+ "\n\n" + shutDownedNodes);
        }
        else {
            QMessageBox::warning(this, "Error", "Unknown response from the server");
        }
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to send stopNodesVumeter request: " + postReply->errorString());
    }

    postReply->deleteLater();
}




void NodesManagement::onInterfacesActionTriggered()
{
    nodesManagementLayout->removeItem(dockLayout);

    // Get the selected node's ID from the table
    QTableWidgetItem* currentItem = nodesManagementTable->currentItem();
    int rowIndex = nodesManagementTable->row(currentItem);
    QString selectedNodeId = nodesManagementTable->item(rowIndex, 0)->text();

    // Send a GET request to retrieve the interfaces information for the selected node
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/interfaces"));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonArray interfacesArray = jsonResponse.array();

        // Create the widget to display node interfaces information
        nodeInterfacesWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(nodeInterfacesWidget);
        QFormLayout* formLayout= new QFormLayout;




        for (const QJsonValue& interfaceValue : interfacesArray)
        {
            QJsonObject interfaceObject = interfaceValue.toObject();

            QString interfaceId = interfaceObject.value("interface_id").toString();
            QString nodeId = interfaceObject.value("node_id").toString();
            int interfaceValidity = interfaceObject.value("interface_validity").toInt();

            // Skip interfaces that don't belong to the selected node
            if (nodeId != selectedNodeId)
                continue;

            // Send a GET request to retrieve the interface information
            QNetworkRequest getInterfaceRequest(QUrl("http://localhost:3200/wms/mesh/interfaces/" + interfaceId));
            QNetworkReply* interfaceReply = manager.get(getInterfaceRequest);
            QEventLoop interfaceLoop;
            connect(interfaceReply, &QNetworkReply::finished, &interfaceLoop, &QEventLoop::quit);
            interfaceLoop.exec();

            if (interfaceReply->error() == QNetworkReply::NoError)
            {
                QByteArray interfaceResponseData = interfaceReply->readAll();
                QJsonDocument interfaceJsonResponse = QJsonDocument::fromJson(interfaceResponseData);
                QJsonObject interfaceJsonObject = interfaceJsonResponse.object();

                QString interfaceName = interfaceJsonObject.value("interface_name").toString();
                QString interfaceType = interfaceJsonObject.value("interface_type").toString();

                // Create labels to display the interface information
                QLabel* interfaceIdLabel = new QLabel("Interface ID: " );
                QLabel* interfaceIdValueLabel = new QLabel(interfaceId);
                QLabel* interfaceNameLabel = new QLabel("Interface Name: ");
                QLabel* interfaceNameValueLabel=new QLabel(interfaceName);
                QLabel* interfaceTypeLabel = new QLabel("Interface Type: " + interfaceType);
                QLabel* interfaceTypeValueLabel=new QLabel(interfaceType);

                // Create QPushButton for interface configuration
                QPushButton* configButton = new QPushButton("Configure");
                configButton->setObjectName(interfaceId); // Set the object name to the interface ID for identification
                configButton->setStyleSheet(
                            "QPushButton {"
                            "background-color: rgba(0, 0, 0, 0);"
                            "border: 1px solid #ccc;"
                            "padding: 6px;"
                            "}"
                            "QPushButton:pressed {"
                            "background-color: rgba(241, 241, 241, 0.5);"
                            "}"
                            "QPushButton:released {"
                            "background-color: rgba(0, 0, 0, 0);"
                            "}"
                            );
                configButton->setFixedWidth(100);

                // Create QPushButton for adding links
                QPushButton* addLinksButton = new QPushButton("Add Links");
                addLinksButton->setStyleSheet(
                            "QPushButton {"
                            "background-color: rgba(0, 0, 0, 0);"
                            "border: 1px solid #ccc;"
                            "padding: 6px;"
                            "}"
                            "QPushButton:pressed {"
                            "background-color: rgba(241, 241, 241, 0.5);"
                            "}"
                            "QPushButton:released {"
                            "background-color: rgba(0, 0, 0, 0);"
                            "}"
                            );
                addLinksButton->setFixedWidth(100);

                addLinksButton->setObjectName(nodeId + "_" + interfaceId);


                // Create QPushButton for adding links
                QPushButton* interfaceInfoButton = new QPushButton("info");
                interfaceInfoButton->setStyleSheet(
                            "QPushButton {"
                            "background-color: rgba(0, 0, 0, 0);"
                            "border: 1px solid #ccc;"
                            "padding: 6px;"
                            "}"
                            "QPushButton:pressed {"
                            "background-color: rgba(241, 241, 241, 0.5);"
                            "}"
                            "QPushButton:released {"
                            "background-color: rgba(0, 0, 0, 0);"
                            "}"
                            );
                interfaceInfoButton->setFixedWidth(100);

                interfaceInfoButton->setObjectName(interfaceId); // Set the object name to the interface ID for identification
                QIcon configureIcon(":/images/configureIcon.png");
                configureIcon = configureIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
                configButton->setIcon(configureIcon);
                QIcon addLinksIcon(":/images/addLinks.png");
                addLinksIcon = addLinksIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
                addLinksButton->setIcon(addLinksIcon);
                QIcon infoIcon(":/images/infoIcon-24.png");
                infoIcon = infoIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
                interfaceInfoButton->setIcon(infoIcon);

                formLayout->addRow(interfaceIdLabel,interfaceIdValueLabel);
                formLayout->addRow(interfaceNameLabel,interfaceNameValueLabel);
                formLayout->addRow(interfaceTypeLabel,interfaceTypeValueLabel);
                QHBoxLayout* buttonsLayout= new QHBoxLayout;
                buttonsLayout->addWidget(configButton);
                buttonsLayout->addWidget(addLinksButton);
                buttonsLayout->addWidget(interfaceInfoButton);
                formLayout->addItem(buttonsLayout);

                QFrame* line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                formLayout->addRow(line);

                // Add the interface information labels and buttons to the layout




                // Connect the button signals to appropriate slots for further actions

                // slot for configuring the interface
                connect(configButton, &QPushButton::clicked, this, &NodesManagement::onConfigureInterfacePressed);

                // slot for adding links
                connect(addLinksButton, &QPushButton::clicked, this, &NodesManagement::onAddLinksPressed);
                //slot for getting interface info
                connect(interfaceInfoButton, &QPushButton::clicked, this, &NodesManagement::onInterfacesInfoPressed);


            }

            // Clean up the interface reply object
            interfaceReply->deleteLater();

        }

        // Set the layout for the widget and display it in the dock widget
        // Create a scroll area and set the layout as its widget
        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget *scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);
        QPushButton *closeButton = new QPushButton("Close");

        QIcon cancelIcon(":/images/closeIcon.png");
        cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        closeButton->setIcon(cancelIcon);
        closeButton->setObjectName("cancelButton");
        closeButton->setFixedWidth(200);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);


        nodeInterfacesWidget->setLayout(layout);

        // Add the dock widget to the dock content area
        dockContent->addWidget(nodeInterfacesWidget);
        dockContent->setCurrentWidget(nodeInterfacesWidget);
        QString title= "Node (" +selectedNodeId+") Interfaces";
        displayDockWidget(title);
        connect(closeButton, &QPushButton::clicked, nodeInterfacesWidget, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockWidget, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockContent, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockContent, [this]()
        { nodesManagementLayout->removeItem(dockLayout); });
    }


    // Clean up the reply object
    reply->deleteLater();
}



void NodesManagement::onConfigureInterfacePressed()
{
    nodesManagementLayout->removeItem(dockLayout);


    // Get the interface ID from the clicked button's object name
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString interfaceId = button->objectName();

    // Create the form and its fields
    QFormLayout* formLayout = new QFormLayout;
    QLineEdit* ipLineEdit = new QLineEdit;
    QLineEdit* maskLineEdit = new QLineEdit;
    QLineEdit* subnetLineEdit = new QLineEdit;
    QLineEdit* channelLineEdit = new QLineEdit;
    QLineEdit* frequencySpinBox = new QLineEdit;
    QLineEdit* antennaRadioBandWidthLineEdit= new QLineEdit;

    formLayout->addRow("Interface IP:", ipLineEdit);
    formLayout->addRow("Interface Mask:", maskLineEdit);
    formLayout->addRow("Interface Subnet:", subnetLineEdit);
    formLayout->addRow("Wireless Channel:", channelLineEdit);
    formLayout->addRow("Radio Frequency Band:", frequencySpinBox);
    formLayout->addRow("Interface Antennas Radio BandWidth",antennaRadioBandWidthLineEdit);


    // Create the submit and cancel buttons
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


    // Create a layout for the buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(cancelButton);

    // Create a vertical layout for the entire form
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(formLayout);
    layout->addLayout(buttonLayout);

    // Create a widget to hold the form and buttons
    configureInterfaceWidget = new QWidget;
    configureInterfaceWidget->setLayout(layout);

    // Create a scroll area and set the widget as its content
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(configureInterfaceWidget);

    // Add the scroll area to the dock widget
    dockContent->addWidget(scrollArea);
    dockContent->setCurrentWidget(scrollArea);
    displayDockWidget("Configure Interface");

    // Connect the buttons' signals to their respective slots
    connect(submitButton, &QPushButton::clicked, this, [=]() {
        QString interfaceIp = ipLineEdit->text();
        QString interfaceMask = maskLineEdit->text();
        QString interfaceSubnet = subnetLineEdit->text();
        int wirelessChannel = channelLineEdit->text().toInt();
        int radioFrequencyBand = frequencySpinBox->text().toInt();

        int antennaRadioBW= antennaRadioBandWidthLineEdit->text().toInt();
        QJsonObject requestBody2;
        requestBody2["radio_bw"]=antennaRadioBW;
        QJsonDocument jsonDoc2(requestBody2);
        QByteArray requestData2=jsonDoc2.toJson();
        QNetworkAccessManager manager2;
        QNetworkRequest request2(QUrl("http://localhost:3200/wms/mesh/antennas/" + interfaceId));
        request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply* reply2 = manager2.put(request2, requestData2);
        QEventLoop putLoop2;
        connect(reply2, &QNetworkReply::finished, &putLoop2, &QEventLoop::quit);
        putLoop2.exec();
        if (reply2->error() == QNetworkReply::NoError) {
            // Request was successful
            QByteArray response2 = reply2->readAll();
            QJsonObject jsonResponse2 = QJsonDocument::fromJson(response2).object();

            // Extract the necessary information from the response
            QString message = jsonResponse2["message"].toString();

            // Display a message indicating the successful configuration
            QMessageBox::information(this, "Success", message);
        } else {
            // Request failed, handle the error
            QMessageBox::warning(this, "Error", "Failed to configure the antenna. Please try again.");
        }




        // Create the request body
        QJsonObject requestBody;
        requestBody["interface_ip"] = interfaceIp;
        requestBody["interface_mask"] = interfaceMask;
        requestBody["interface_subnet"] = interfaceSubnet;
        requestBody["wireless_channel"] = wirelessChannel;
        requestBody["radio_frequency_band"] = radioFrequencyBand;
        // Validate IP address using a regular expression
        QRegularExpression ipRegex("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
        QRegularExpressionValidator ipValidator(ipRegex, this);

        int ipPos = 0;
        if (ipValidator.validate(interfaceIp, ipPos) != QValidator::Acceptable) {
            // Invalid IP address
            QMessageBox::warning(this, "Error", "Invalid IP address. Please check the value.");
            return;
        }

        // ... (similar validation for mask and subnet)

        // Ensure that IP address matches the mask and subnet
        quint32 ipAddr = QHostAddress(interfaceIp).toIPv4Address();
        quint32 maskAddr = QHostAddress(interfaceMask).toIPv4Address();
        quint32 subnetAddr = QHostAddress(interfaceSubnet).toIPv4Address();

        if ((ipAddr & maskAddr) != subnetAddr) {
            // IP address doesn't match the mask and subnet
            QMessageBox::warning(this, "Error", "The IP address doesn't match the mask and subnet. Please check the values.");
            return;
        }






        QJsonDocument jsonDoc(requestBody);
        QByteArray requestData=jsonDoc.toJson();


        // Send the PUT request with the updated interface configuration
        // Send a PUT request to start the node
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3200/wms/mesh/interfaces/" + interfaceId));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply* reply = manager.put(request, requestData);
        QEventLoop putLoop;
        connect(reply, &QNetworkReply::finished, &putLoop, &QEventLoop::quit);
        putLoop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            // Request was successful
            QByteArray response = reply->readAll();
            QJsonObject jsonResponse = QJsonDocument::fromJson(response).object();

            // Extract the necessary information from the response
            QString interfaceType = jsonResponse["interface_type"].toString();
            QString interfaceId = jsonResponse["interface_id"].toString();

            // Display a message indicating the successful configuration
            QMessageBox::information(this, "Success", QString("%1 interface of id %2 configured successfully").arg(interfaceType, interfaceId));
        } else {
            // Request failed, handle the error
            QMessageBox::warning(this, "Error", "Failed to configure the interface. Please try again.");
        }

        // Remove the configure interface widget and show the interfaces display
        dockContent->removeWidget(scrollArea);
        nodesManagementLayout->removeItem(dockLayout);
        displayDockWidget("Node Interfaces");

        // Clean up the network manager and reply objects
        reply->deleteLater();

    });

    connect(cancelButton, &QPushButton::clicked, this, [=]() {
        // Remove the configure interface widget and show the interfaces display
        dockContent->removeWidget(scrollArea);
        nodesManagementLayout->removeItem(dockLayout);

        displayDockWidget("Node Interfaces");
    });


}


void NodesManagement::onAddLinksPressed()
{    nodesManagementLayout->removeItem(dockLayout);

    // Get the addLinksButton object name
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString objectName = button->objectName();

    // Split the object name to extract the node_id and interface_id
    QStringList nameParts = objectName.split("_");
    QString nodeId = nameParts.at(0);
    QString interfaceId = nameParts.at(1);


    // Retrieve the interface type of the current interface
    QNetworkAccessManager manager;
    QNetworkRequest interfaceTypeRequest(QUrl("http://localhost:3200/wms/mesh/interfaces/" + interfaceId));
    QNetworkReply* interfaceTypeReply = manager.get(interfaceTypeRequest);
    QEventLoop interfaceTypeLoop;
    connect(interfaceTypeReply, &QNetworkReply::finished, &interfaceTypeLoop, &QEventLoop::quit);
    interfaceTypeLoop.exec();

    QString interfaceTypeFromCurrentInterface;
    if (interfaceTypeReply->error() == QNetworkReply::NoError) {
        QByteArray interfaceTypeResponse = interfaceTypeReply->readAll();
        QJsonObject interfaceTypeData = QJsonDocument::fromJson(interfaceTypeResponse).object();
        interfaceTypeFromCurrentInterface = interfaceTypeData["interface_type"].toString();
    } else {
        QMessageBox::warning(this, "Error", "Failed to retrieve interface type. Please try again.");
        interfaceTypeReply->deleteLater();
        return;
    }
    interfaceTypeReply->deleteLater();

    // Get the list of available interfaces
    QNetworkRequest availableInterfacesRequest(QUrl("http://localhost:3200/wms/mesh/interfaces"));
    QNetworkReply* availableInterfacesReply = manager.get(availableInterfacesRequest);
    QEventLoop availableInterfacesLoop;
    connect(availableInterfacesReply, &QNetworkReply::finished, &availableInterfacesLoop, &QEventLoop::quit);
    availableInterfacesLoop.exec();

    if (availableInterfacesReply->error() == QNetworkReply::NoError) {
        QByteArray response = availableInterfacesReply->readAll();
        QJsonArray interfacesArray = QJsonDocument::fromJson(response).array();

        // Filter interfaces based on criteria
        QList<QString> availableInterfaces;
        for (const QJsonValue& value : interfacesArray) {
            QJsonObject interfaceObj = value.toObject();
            QString availableNodeId = interfaceObj["node_id"].toString();
            QString availableInterfaceId = interfaceObj["interface_id"].toString();
            QString availableInterfaceName = interfaceObj["interface_name"].toString();
            QString availableInterfaceType = interfaceObj["interface_type"].toString();

            if (availableNodeId != nodeId) {
                // Request the interface type for the available interface
                QNetworkRequest interfaceRequest(QUrl("http://localhost:3200/wms/mesh/interfaces/" + availableInterfaceId));
                QNetworkReply* interfaceReply = manager.get(interfaceRequest);
                QEventLoop interfaceLoop;
                connect(interfaceReply, &QNetworkReply::finished, &interfaceLoop, &QEventLoop::quit);
                interfaceLoop.exec();

                if (interfaceReply->error() == QNetworkReply::NoError) {
                    QByteArray interfaceResponse = interfaceReply->readAll();
                    QJsonObject interfaceData = QJsonDocument::fromJson(interfaceResponse).object();
                    QString interfaceType = interfaceData["interface_type"].toString();

                    // Check if the interface type matches the current interface
                    if (interfaceType == interfaceTypeFromCurrentInterface) {
                        // Request the node information for the available interface
                        QString availableNodeID = interfaceData["node_id"].toString();
                        QNetworkRequest nodeRequest(QUrl("http://localhost:3200/wms/mesh/nodes/" + availableNodeID));
                        QNetworkReply* nodeReply = manager.get(nodeRequest);
                        QEventLoop nodeLoop;
                        connect(nodeReply, &QNetworkReply::finished, &nodeLoop, &QEventLoop::quit);
                        nodeLoop.exec();

                        if (nodeReply->error() == QNetworkReply::NoError) {
                            QByteArray nodeResponse = nodeReply->readAll();
                            QJsonObject nodeData = QJsonDocument::fromJson(nodeResponse).object();
                            QString availableNodeName = nodeData["node_name"].toString();
                            QString availableNodeType = nodeData["node_type"].toString();

                            QString availableInterfaceInfo = availableInterfaceId + " ( " + availableInterfaceName + " - " + availableInterfaceType + " ) " + " (" + availableNodeName + " - " + availableNodeType + ")";
                            availableInterfaces.append(availableInterfaceInfo);
                        }
                        nodeReply->deleteLater();
                    }
                }

                interfaceReply->deleteLater();
            }
        }

        if (!availableInterfaces.isEmpty()) {
            // Create the form and its fields
            QWidget* formWidget = new QWidget(this);
            QVBoxLayout* layout = new QVBoxLayout(formWidget);
            QFormLayout* formLayout = new QFormLayout;
            QLabel* interfaceLabel = new QLabel("Select an interface:");
            QComboBox* interfaceComboBox = new QComboBox();
            QComboBox* linkTypeComboBox = new QComboBox();
            linkTypeComboBox->addItem("WIFI");
            linkTypeComboBox->addItem("OLSR LINK");

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

            // Populate the interfaceComboBox with available interfaces
            for (const QString& interfaceId : availableInterfaces) {
                interfaceComboBox->addItem(interfaceId);
            }

            // Add form fields to the layout
            formLayout->addRow(interfaceLabel);
            formLayout->addRow(interfaceComboBox);



            if (interfaceTypeFromCurrentInterface == "Wireless") {
                // Show a comboBox to choose the wireless link type
                formLayout->addRow(linkTypeComboBox);



            }



            QScrollArea* scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            QWidget* scrollAreaContent = new QWidget;
            scrollAreaContent->setLayout(formLayout);
            scrollArea->setWidget(scrollAreaContent);
            layout->addWidget(scrollArea);

            QHBoxLayout* buttonLayout = new QHBoxLayout;
            buttonLayout->addWidget(submitButton);
            buttonLayout->addWidget(cancelButton);
            layout->addLayout(buttonLayout);





            // Connect the buttons' signals to their respective slots
            connect(submitButton, &QPushButton::clicked,this, [=]() {
                QString selectedInterfaceInfo = interfaceComboBox->currentText();
                QString selectedInterfaceId = selectedInterfaceInfo.split(" (").at(0);

                // Create the request body
                int linkType;
                if (interfaceTypeFromCurrentInterface=="Wired"){
                    linkType=2;
                }
                QJsonObject requestBody;
                if (linkTypeComboBox->currentText() == "WIFI") {
                    qDebug()<<linkTypeComboBox->currentText();
                    linkType = 2; // WIFI link type
                }
                if (linkTypeComboBox->currentText() == "OLSR LINK") {
                    qDebug()<<linkTypeComboBox->currentText();
                    linkType= 3; // OLSR LINK link type

                }
                requestBody["interface_from"] = interfaceId;
                requestBody["interface_to"] = selectedInterfaceId;
                requestBody["link_type"]= linkType;



                QJsonDocument jsonDoc(requestBody);
                QByteArray requestData = jsonDoc.toJson();

                // Send the POST request to add a link
                QNetworkAccessManager manager;
                QNetworkRequest addLinksRequest(QUrl("http://localhost:3200/wms/mesh/links/addLinks/" + nodeId));
                addLinksRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QNetworkReply* addLinksReply = manager.post(addLinksRequest, requestData);

                QEventLoop addLinksLoop;
                connect(addLinksReply, &QNetworkReply::finished, &addLinksLoop, &QEventLoop::quit);
                addLinksLoop.exec();
                int statusCode = addLinksReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                QByteArray responseData = addLinksReply->readAll();
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                QJsonObject jsonObject = jsonResponse.object();

                if (addLinksReply->error() == QNetworkReply::NoError) {
                    if (statusCode == 200) {
                        QString message = jsonObject.value("message").toString();
                        QMessageBox::information(this, "Success", message);
                    }
                } else {
                    if (statusCode == 404) {
                        QString message = jsonObject.value("message").toString();
                        QMessageBox::warning(this, "Error", message);
                    } else if (statusCode == 401) {
                        QString message = jsonObject.value("message").toString();
                        QMessageBox::warning(this, "Error", message);
                    } else if (statusCode == 400) {
                        QString message = jsonObject.value("message").toString();
                        QMessageBox::warning(this, "Error", message);
                    } else {
                        QMessageBox::warning(this, "Error", "Unknown response from server");
                    }
                    //                    QMessageBox::warning(this, "Error", "Failed to create link. Please try again.");
                }

                // Remove the addLinks widget and show the interfaces display
                dockContent->removeWidget(formWidget);
                nodesManagementLayout->removeItem(dockLayout);
                displayDockWidget("Node Interfaces");

                // Clean up the network manager and reply objects
                addLinksReply->deleteLater();
            });

            connect(cancelButton, &QPushButton::clicked, this, [=]() {
                // Remove the addLinks widget and show the interfaces display
                dockContent->removeWidget(formWidget);
                nodesManagementLayout->removeItem(dockLayout);
                displayDockWidget("Node Interfaces");
            });

            dockContent->addWidget(formWidget);
            dockContent->setCurrentWidget(formWidget);
            displayDockWidget("Add links");
        } else {
            QMessageBox::warning(this, "No Available Interfaces", "No available interfaces found.");
        }
    } else {
        QMessageBox::warning(this, "Error", "Failed to retrieve interfaces. Please try again.");
    }

    availableInterfacesReply->deleteLater();
}


void NodesManagement::onInterfacesInfoPressed()
{
    nodesManagementLayout->removeItem(dockLayout);

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString interfaceId = button->objectName();

    // Send a GET request to retrieve the interface information
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3200/wms/mesh/interfaces/" + interfaceId));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject interfaceObject = jsonResponse.object();

        // Create the widget to display interface information
        interfaceInfoWidget = new QWidget(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(interfaceInfoWidget);

        // Create a scroll area
        QScrollArea* scrollArea = new QScrollArea(interfaceInfoWidget);
        QWidget* scrollWidget = new QWidget(scrollArea);
        QFormLayout* formLayout = new QFormLayout(scrollWidget);

        // Create labels to display the interface information
        QLabel* interfaceIdTitleLabel = new QLabel("Interface ID:");
        QLabel* interfaceIdValueLabel = new QLabel(interfaceObject.value("interface_id").toString());
        QLabel* interfaceNameTitleLabel = new QLabel("Interface Name:");
        QLabel* interfaceNameValueLabel = new QLabel(interfaceObject.value("interface_name").toString());
        QLabel* interfaceTypeTitleLabel = new QLabel("Interface Type:");
        QLabel* interfaceTypeValueLabel = new QLabel(interfaceObject.value("interface_type").toString());
        QLabel* nodeIdTitleLabel = new QLabel("Node ID:");
        QLabel* nodeIdValueLabel = new QLabel(interfaceObject.value("node_id").toString());
        QLabel* interfaceIpTitleLabel = new QLabel("Interface IP:");
        QLabel* interfaceIpValueLabel = new QLabel(interfaceObject.value("interface_ip").toString());
        QLabel* interfaceMaskTitleLabel = new QLabel("Interface Mask:");
        QLabel* interfaceMaskValueLabel = new QLabel(interfaceObject.value("interface_mask").toString());
        QLabel* interfaceSubnetTitleLabel = new QLabel("Interface Subnet:");
        QLabel* interfaceSubnetValueLabel = new QLabel(interfaceObject.value("interface_subnet").toString());
        QLabel* interfaceSpeedTitleLabel = new QLabel("Interface Speed:");
        QLabel* interfaceSpeedValueLabel = new QLabel(QString::number(interfaceObject.value("interface_speed").toInt()));

        // Add the labels to the form layout
        formLayout->addRow(interfaceIdTitleLabel, interfaceIdValueLabel);
        formLayout->addRow(interfaceNameTitleLabel, interfaceNameValueLabel);
        formLayout->addRow(interfaceTypeTitleLabel, interfaceTypeValueLabel);
        formLayout->addRow(nodeIdTitleLabel, nodeIdValueLabel);
        formLayout->addRow(interfaceIpTitleLabel, interfaceIpValueLabel);
        formLayout->addRow(interfaceMaskTitleLabel, interfaceMaskValueLabel);
        formLayout->addRow(interfaceSubnetTitleLabel, interfaceSubnetValueLabel);
        formLayout->addRow(interfaceSpeedTitleLabel, interfaceSpeedValueLabel);

        // Set the form layout for the scroll widget
        scrollWidget->setLayout(formLayout);
        scrollArea->setWidget(scrollWidget);
        scrollArea->setWidgetResizable(true);

        // Create a return button
        QPushButton* returnButton = new QPushButton("Return", interfaceInfoWidget);
        returnButton->setStyleSheet(
                    "QPushButton {"
                    "background-color: rgba(0, 0, 0, 0);"
                    "border: 1px solid #ccc;"
                    "padding: 6px;"
                    "}"
                    "QPushButton:pressed {"
                    "background-color: rgba(241, 241, 241, 0.5);"
                    "}"
                    "QPushButton:released {"
                    "background-color: rgba(0, 0, 0, 0);"
                    "}"
                    );
        returnButton->setFixedWidth(90);
        QIcon returnIcon(":/images/returnIcon.png");
        returnIcon = returnIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        returnButton->setIcon(returnIcon);


        connect(returnButton, &QPushButton::clicked, [=]() {
            // Remove the interface information widget and show the interfaces display
            dockContent->removeWidget(interfaceInfoWidget);
            nodesManagementLayout->removeItem(dockLayout);

            displayDockWidget("Node Interfaces");
        });

        // Add the scroll area and return button to the main layout
        mainLayout->addWidget(scrollArea);
        mainLayout->addWidget(returnButton);

        // Set the main layout for the widget and display it in the dock widget
        interfaceInfoWidget->setLayout(mainLayout);

        // Add the dock widget to the dock content area
        dockContent->addWidget(interfaceInfoWidget);
        dockContent->setCurrentWidget(interfaceInfoWidget);
        displayDockWidget("Interface Information");
    }

    // Clean up the reply object
    reply->deleteLater();
}







