#include "antennas.h"
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




#include <QSpinBox>

Antennas::Antennas(QWidget *parent) : QWidget(parent)
{
    setupUI();
    populateTable();
    QTimer::singleShot(2000, this, &Antennas::populateTable);

}
Antennas::~Antennas(){
    delete antennasTable;
    delete addAntennaButton;
    delete antennasLayout;
    delete antennaInfoWidget;
    delete antennaModifyWidget;
    delete dockLayout;
    delete dockWidget;
    delete dockContent;
    delete antennasMenu;
}

void Antennas::setupUI()
{
    antennasLayout = new QVBoxLayout;
    setLayout(antennasLayout);

    antennasTable = new QTableWidget(this);
    antennasTable->setColumnCount(4);
    antennasTable->setHorizontalHeaderLabels({"Antenna Model", "Gain (dBi)", "Vertical Angle Aperture (degree)", "Horizontal Angle Aperture (degree)"});
    antennasTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set the style sheet for the table
    QString tableStyleSheet = "QTableWidget {"
                              "    background-color: #f2f2f2;"
                              "    border: 1px solid #999999;"
                              "    font-size: 14px;"
                              "    border: 0.5px solid #999999;"
                              "    border-radius: 4px;"

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
                              "}"
                            ;



    antennasTable->setStyleSheet(tableStyleSheet);

    antennasLayout->addWidget(antennasTable);

    addAntennaButton = new QPushButton("Add Antenna", this);
    QIcon addIcon(":/images/addIcon.png");
    addIcon = addIcon.pixmap(QSize(128,128));  // Set the desired icon size here

    addAntennaButton->setIcon(addIcon);
    addAntennaButton->setStyleSheet("QPushButton {"
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
    addAntennaButton->setFixedWidth(200);
    antennasLayout->addWidget(addAntennaButton);

    connect(addAntennaButton, &QPushButton::clicked, this, &Antennas::onAddAntennaClicked);

    //actions
    antennasMenu=new QMenu(this);
    QIcon infoMenuIcon(":/images/infoIcon-24.png");
    QIcon modifyMenuIcon(":/images/editIcon.png");
    QIcon deleteMenuIcon(":/images/deleteIcon.png");
    infoMenuIcon = infoMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
    modifyMenuIcon = modifyMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
    deleteMenuIcon = deleteMenuIcon.pixmap(QSize(24, 24));
    infoAction = antennasMenu->addAction(infoMenuIcon,"Antenna Informations");
    modifyAction=antennasMenu->addAction(modifyMenuIcon,"Modify Antenna");
    deleteAction=antennasMenu->addAction(deleteMenuIcon,"Delete Antenna");

//    deleteAction = new QAction("Delete", this);
    connect(deleteAction, &QAction::triggered, this, &Antennas::onDeleteActionTriggered);

//    infoAction=new QAction("Info", this);
    connect(infoAction,&QAction::triggered,this,&Antennas::onInfoActionTriggered);

//    modifyAction=new QAction("Modify",this);

    connect(modifyAction,&QAction::triggered,this,&Antennas::onModifyActionTriggered);


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
void Antennas::displayDockWidget(QString message)
{
    antennasLayout->addLayout(dockLayout);
    dockWidget->setWindowTitle(message);
    dockContent->show();  // Show the dock content before animating
    dockWidget->show();
}

void Antennas::contextMenuEvent(QContextMenuEvent *event)
{

//    antennasMenu.addAction(infoAction);
//    antennasMenu.addAction(modifyAction);
//    antennasMenu.addAction(deleteAction);
    antennasMenu->exec(event->globalPos());
}
void Antennas::populateTable()
{
    // Fetch data from the backend and populate the table
    // Clear existing table data
    antennasTable->clearContents();
    antennasTable->setRowCount(0);

    // Make a request to the backend endpoint and retrieve the data
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl("http://localhost:3300/wms/survey/surveyAntennas")));
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check if the request was successful
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Failed to retrieve Antennas Data :" << reply->errorString();
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
        const QString modelID = jsonObject.value("model_id").toString(); //returns the value of a specified key
        const int gain = jsonObject.value("gain").toInt();
        const int verticalAngleAperture = jsonObject.value("vertical_angle_aperture").toInt();
        const int horizontalAngleAperture = jsonObject.value("horizontal_angle_aperture").toInt();


        // Add a new row to the table and populate the fields
        int rowCount = antennasTable->rowCount();
        antennasTable->insertRow(rowCount);
        antennasTable->setItem(rowCount, 0, new QTableWidgetItem(modelID));
        antennasTable->setItem(rowCount, 1, new QTableWidgetItem(QString::number(gain)));
        antennasTable->setItem(rowCount, 2, new QTableWidgetItem(QString::number(verticalAngleAperture)));
        antennasTable->setItem(rowCount, 3, new QTableWidgetItem(QString::number(horizontalAngleAperture)));

    }

    reply->deleteLater();

    // Refresh the table data every 5 seconds



}
void Antennas::showAddAntennaForm()
{
    // Create the widget for adding a new antenna
    addAntennaWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(addAntennaWidget);
    QFormLayout* formLayout = new QFormLayout;

    QLabel* modelIdLabel = new QLabel("Model:");
    QLineEdit* modelIdLineEdit = new QLineEdit;
    QLabel* gainLabel = new QLabel("Gain:");
    QLineEdit* gainLineEdit = new QLineEdit;
    QLabel* verticalAngleApertureLabel = new QLabel("Vertical Angle Aperture:");
    QLineEdit* verticalAngleApertureLineEdit = new QLineEdit;
    QLabel* horizontalAngleApertureLabel = new QLabel("Horizontal Angle Aperture:");
    QLineEdit* horizontalAngleApertureLineEdit = new QLineEdit;

    // Adding the fields to the form layout
    formLayout->addRow(modelIdLabel, modelIdLineEdit);
    formLayout->addRow(gainLabel, gainLineEdit);
    formLayout->addRow(verticalAngleApertureLabel, verticalAngleApertureLineEdit);
    formLayout->addRow(horizontalAngleApertureLabel, horizontalAngleApertureLineEdit);

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
    submitButton->setObjectName("submitButton");
    cancelButton->setObjectName("cancelButton");
    submitButton->setFixedWidth(200);
    cancelButton->setFixedWidth(200);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    // Connect the submit button to the onAddAntennaClicked() slot
    connect(submitButton, &QPushButton::clicked, this, [this, modelIdLineEdit, gainLineEdit,
                                                        verticalAngleApertureLineEdit, horizontalAngleApertureLineEdit]() {

        // Preparing the request body elements
        QString modelId = modelIdLineEdit->text();
        int gain = gainLineEdit->text().toInt();
        int verticalAngleAperture = verticalAngleApertureLineEdit->text().toInt();
        int horizontalAngleAperture = horizontalAngleApertureLineEdit->text().toInt();

        // Create the request body JSON object
        QJsonObject requestBody;
        requestBody["model_id"] = modelId;
        requestBody["gain"] = gain;
        requestBody["vertical_angle_aperture"] = verticalAngleAperture;
        requestBody["horizontal_angle_aperture"] = horizontalAngleAperture;

        // Add more fields to the JSON object as needed

        // Convert the JSON object to a QByteArray
        QJsonDocument jsonDoc(requestBody);
        QByteArray requestData = jsonDoc.toJson();

        // Make a POST request to the backend endpoint
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyAntennas"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply* reply = manager.post(request, requestData);
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
        addAntennaWidget->close();
        dockWidget->close();
        dockContent->close();
        antennasLayout->removeItem(dockLayout);

    });

    connect(cancelButton, &QPushButton::clicked, addAntennaWidget, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockWidget, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockContent, &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, dockContent,[this](){
        antennasLayout->removeItem(dockLayout);

    });


    // Add the widget to the dock content
    dockContent->addWidget(addAntennaWidget);
    dockContent->setCurrentWidget(addAntennaWidget);
    // Show the dock widget
    displayDockWidget("Please provide Antenna informations: ");
}
void Antennas::onAddAntennaClicked()
{
    //     showAddAntennaForm("Add Antenna", { "Antenna Type:", "Gain:", "Vertical Angle:", "Horizontal Angle:" });
    showAddAntennaForm();
}
void Antennas::onDeleteActionTriggered()
{
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this antenna?",
                                         QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::Yes)
    {
        QTableWidgetItem *currentItem = antennasTable->currentItem();
        int rowIndex = antennasTable->row(currentItem);
        QString modelId = antennasTable->item(rowIndex, 0)->text();
        qDebug()<<"router deleted is :"<<modelId;
        // Assuming product ID is stored in the first column
        // Send a request to delete the product using the appropriate API endpoint
        // Example: localhost:3300/deleteProduct/:id

        // Update the table by removing the deleted row
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyAntennas/" + modelId));
        QNetworkReply *reply = manager.deleteResource(request);
        QEventLoop loop;
        connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
        loop.exec();

        if(reply->error()==QNetworkReply::NoError)
        {
            qDebug() <<"Antenna deleted succesfully";
            antennasTable->removeRow(rowIndex);

        }else{

            qDebug() << "failed to delete antenna:"<< reply->errorString();

        }
        reply->deleteLater();





    }

}


void Antennas::onInfoActionTriggered()
{
    QTableWidgetItem* currentItem = antennasTable->currentItem();
    int rowIndex = antennasTable->row(currentItem);
    QString modelId = antennasTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

    // Send a GET request to retrieve the information of the selected node
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyAntennas/" + modelId));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject antennaInfo = jsonResponse.object();

        // Extract the fields from the JSON response and display the information
        // Extract other fields as needed

        QString modelId = antennaInfo.value("model_id").toString();
        int gain = antennaInfo.value("gain").toInt();
        int verticalAngleAperture = antennaInfo.value("vertical_angle_aperture").toInt();
        int horizontalAngleAperture = antennaInfo.value("horizontal_angle_aperture").toInt();

        antennaInfoWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(antennaInfoWidget);
        QFormLayout* formLayout = new QFormLayout;

        QLabel* modelIdLabel = new QLabel("Model:");
        QLabel* modelIdValueLabel = new QLabel(modelId);
        QLabel* gainLabel = new QLabel("Gain:");
        QLabel* gainValueLabel = new QLabel(QString::number(gain) + " dBi");
        QLabel* verticalAngleApertureLabel = new QLabel("Vertical Angle Aperture:");
        QLabel* verticalAngleApertureValueLabel = new QLabel(QString::number(verticalAngleAperture) + " °");
        QLabel* horizontalAngleApertureLabel = new QLabel("Horizontal Angle Aperture:");
        QLabel* horizontalAngleApertureValueLabel = new QLabel(QString::number(horizontalAngleAperture) + " °");
        formLayout->addRow(modelIdLabel, modelIdValueLabel);
        formLayout->addRow(gainLabel, gainValueLabel);
        formLayout->addRow(verticalAngleApertureLabel, verticalAngleApertureValueLabel);
        formLayout->addRow(horizontalAngleApertureLabel, horizontalAngleApertureValueLabel);

        QScrollArea* scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget* scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);

        QPushButton* closeButton = new QPushButton("Close");

        QIcon cancelIcon(":/images/closeIcon.png");
        cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        closeButton->setIcon(cancelIcon);
        closeButton->setObjectName("cancelButton");
        closeButton->setFixedWidth(200);

        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);
        connect(closeButton, &QPushButton::clicked, antennaInfoWidget, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockWidget, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockContent, &QWidget::close);
        connect(closeButton, &QPushButton::clicked, dockContent,[this](){
            antennasLayout->removeItem(dockLayout);

        });


        // Add the widget to the dock content
        dockContent->addWidget(antennaInfoWidget);
        dockContent->setCurrentWidget(antennaInfoWidget);

        // Show the dock widget
        displayDockWidget("Antenna Informations: ");

        connect(antennaInfoWidget, &QObject::destroyed, [=]() {
            delete scrollAreaContent;
            delete scrollArea;
            delete formLayout;
            delete modelIdValueLabel;
            delete gainValueLabel;
            delete verticalAngleApertureValueLabel;
            delete horizontalAngleApertureValueLabel;
            delete modelIdLabel;
            delete gainLabel;
            delete verticalAngleApertureLabel;
            delete horizontalAngleApertureLabel;
            delete closeButton;
            delete buttonLayout;
        });
    }
    else
    {
        // Request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve Antenna information: " + reply->errorString());
    }

    reply->deleteLater();
}

void Antennas::onModifyActionTriggered() {
    QTableWidgetItem* currentItem = antennasTable->currentItem();
    int rowIndex = antennasTable->row(currentItem);
    QString modelId = antennasTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

    // Send a GET request to retrieve the information of the selected node
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyAntennas/" + modelId));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject antennaInfo = jsonResponse.object();

        // Create a QWidget to display the modify widget
        antennaModifyWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(antennaModifyWidget);
        QFormLayout* formLayout = new QFormLayout;

        // Create labels and line edit fields for each node information
        QLabel* modelIdLabel = new QLabel("Model:");
        QLabel* modelIdValueLabel = new QLabel(antennaInfo.value("model_id").toString());
        formLayout->addRow(modelIdLabel, modelIdValueLabel);

        QLabel* gainLabel = new QLabel("Gain:");
        QSpinBox* gainSpinBox = new QSpinBox(antennaModifyWidget);
        gainSpinBox->setValue(antennaInfo.value("gain").toInt());
        gainSpinBox->setRange(0, 999999999);
        formLayout->addRow(gainLabel, gainSpinBox);

        QLabel* verticalAngleApertureLabel = new QLabel("Vertical Angle Aperture:");
        QSpinBox* verticalAngleApertureSpinBox = new QSpinBox(antennaModifyWidget);
        verticalAngleApertureSpinBox->setValue(antennaInfo.value("vertical_angle_aperture").toInt());
        verticalAngleApertureSpinBox->setRange(0, 999999999);
        formLayout->addRow(verticalAngleApertureLabel, verticalAngleApertureSpinBox);

        QLabel* horizontalAngleApertureLabel = new QLabel("Horizontal Angle Aperture:");
        QSpinBox* horizontalAngleApertureSpinBox = new QSpinBox(antennaModifyWidget);
        horizontalAngleApertureSpinBox->setValue(antennaInfo["horizontal_angle_aperture"].toInt());
        horizontalAngleApertureSpinBox->setRange(0, 99999999);
        formLayout->addRow(horizontalAngleApertureLabel, horizontalAngleApertureSpinBox);

        // Add the form layout to the scroll area
        QScrollArea* scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget* scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);

        // Create a QPushButton for the Save button
        QPushButton* saveButton = new QPushButton("Save", antennaModifyWidget);
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


        connect(cancelButton, &QPushButton::clicked, antennaModifyWidget, &QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockWidget,&QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockContent,&QWidget::close);
        connect(cancelButton, &QPushButton::clicked, dockContent,[this](){
            antennasLayout->removeItem(dockLayout);

        });



        // Connect the Save button to the onSaveButtonClicked slot
        connect(saveButton, &QPushButton::clicked, this, [this, antennaInfo, gainSpinBox, verticalAngleApertureSpinBox, horizontalAngleApertureSpinBox]() {
            // Extract the modified fields from the modify widget's spin boxes
            int modifiedGain = gainSpinBox->value();
            int modifiedVerticalAngleAperture = verticalAngleApertureSpinBox->value();
            int modifiedHorizontalAngleAperture = horizontalAngleApertureSpinBox->value();

            // Construct the modified fields in a QJsonObject
            QJsonObject modifiedFields;
            modifiedFields["gain"] = modifiedGain;
            modifiedFields["vertical_angle_aperture"] = modifiedVerticalAngleAperture;
            modifiedFields["horizontal_angle_aperture"] = modifiedHorizontalAngleAperture;

            // Create the request body JSON object
            QJsonObject requestBody = modifiedFields;

            // Convert the JSON object to a QByteArray
            QJsonDocument jsonDoc(requestBody);
            QByteArray requestData = jsonDoc.toJson();

            // Send a PUT request to update the antenna information
            QNetworkAccessManager manager;
            QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyAntennas/" + antennaInfo.value("model_id").toString()));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply* reply = manager.put(request, requestData);
            QEventLoop loop;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            if (reply->error() == QNetworkReply::NoError) {
                // Request successful
                QMessageBox::information(this, "Success", "Antenna information modified successfully!");
                dockContent->removeWidget(antennaModifyWidget);
                antennaModifyWidget->close(); // Close the widget
            } else {
                // Request failed
                QMessageBox::critical(this, "Error", "Failed to modify antenna information: " + reply->errorString());
                dockContent->removeWidget(antennaModifyWidget);

                antennaModifyWidget->close(); // Close the widget
            }

            reply->deleteLater();
            reply->deleteLater();
            reply->deleteLater();
            antennaModifyWidget->close();
            dockWidget->close();
            dockContent->close();
            antennasLayout->removeItem(dockLayout);

        });

        // Set the layout and add the modify widget to the dock content
//        antennaModifyWidget->setLayout(layout);
        dockContent->addWidget(antennaModifyWidget);
        dockContent->setCurrentWidget(antennaModifyWidget);

        // Show the dock widget
        displayDockWidget("Edit Antenna Informations :");
    } else {
        // Request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve antenna information: " + reply->errorString());
    }

    reply->deleteLater();
}




//old version
//void Antennas::onInfoActionTriggered(){
//    QTableWidgetItem* currentItem = antennasTable->currentItem();
//    int rowIndex = antennasTable->row(currentItem);
//    QString modelId = antennasTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

//    // Send a GET request to retrieve the information of the selected node
//    QNetworkAccessManager manager;
//    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyAntennas/" + modelId));
//    QNetworkReply* reply = manager.get(getRequest);
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();

//    if (reply->error() == QNetworkReply::NoError)
//    {
//        // Request successful
//        QByteArray responseData = reply->readAll();
//        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
//        QJsonObject antennaInfo = jsonResponse.object();

//        // Extract the fields from the JSON response and display the information
//        // Extract other fields as needed

//        QString modelId = antennaInfo.value("model_id").toString();
//        int gain = antennaInfo.value("gain").toInt();
//        int verticalAngleAperture = antennaInfo.value("vertical_angle_aperture").toInt();
//        int horizontalAngleAperture = antennaInfo.value("horizontal_angle_aperture").toInt();



//        // Display the information using QMessageBox or custom dialog

//        QMessageBox::information(this, "Antenna Information",
//                                 "Model: " + modelId + "\n"
//                                                       "Gain: " + QString::number(gain) + " dBi\n"
//                                                                                          "Vertical Angle Aperture: " + QString::number(verticalAngleAperture) + " °\n"
//                                                                                                                                                                 "Horizontal Angle Aperture: " + QString::number(horizontalAngleAperture) + " °\n"

//                                 );


//    }
//    else
//    {
//        // Request failed
//        QMessageBox::critical(this, "Error", "Failed to retrieve Antenna information: " + reply->errorString());
//    }

//    reply->deleteLater();







//}
//void Antennas::onTableRightClicked(const QPoint &pos)
//{
//Q_UNUSED(pos);
//    QTableWidgetItem *currentItem = antennasTable->itemAt(pos);
//    if (currentItem)
//    {
//        QMenu *contextMenu = new QMenu(this);
//        QAction *deleteAction = contextMenu->addAction("Delete");
//        connect(deleteAction, &QAction::triggered, this, &Antennas::onDeleteActionTriggered);
//        contextMenu->exec(QCursor::pos());
//        delete contextMenu;
//    }
//}
//void Antennas::showAddAntennaForm(const QString &title, const QStringList &labels)
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
//    int row = antennasTable->rowCount();
//    antennasTable->insertRow(row);
//    for (int i = 0; i < values.size(); i++)
//    {
//        antennasTable->setItem(row, i, new QTableWidgetItem(values[i]));
//    }
//}


//delete dialog;
//}
