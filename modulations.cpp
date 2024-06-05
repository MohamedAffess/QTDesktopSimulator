#include "modulations.h"
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

Modulations::Modulations(QWidget *parent) : QWidget(parent)
{
    setupUI();
    populateTable();
    QTimer::singleShot(30000, this, &Modulations::populateTable);

}
Modulations::~Modulations(){
    delete modulationTable;
    delete addModulationButton;
    delete dockWidget;
    delete dockContent;
    delete modulationLayout;
    delete dockLayout;
    delete modulationsInfoWidget;
    delete modulationModifyWidget;
    delete addModulationWidget;
    delete modulationsMenu;

}

void Modulations::setupUI()
{
    modulationLayout = new QVBoxLayout;
    setLayout(modulationLayout);

    modulationTable = new QTableWidget(this);
    modulationTable->setColumnCount(2);
    modulationTable->setHorizontalHeaderLabels({"Modulation ID", "Rate (Mbps)"});
    modulationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
                              "}"
                            ;



    modulationTable->setStyleSheet(tableStyleSheet);
    modulationLayout->addWidget(modulationTable);

    addModulationButton = new QPushButton("Add Modulation", this);
    QIcon addIcon(":/images/addIcon.png");
    addIcon = addIcon.pixmap(QSize(128,128));  // Set the desired icon size here

    addModulationButton->setIcon(addIcon);
    addModulationButton->setStyleSheet("QPushButton {"
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
    addModulationButton->setFixedWidth(200);
    modulationLayout->addWidget(addModulationButton);

    connect(addModulationButton, &QPushButton::clicked, this, &Modulations::onAddModulationClicked);

    //Actions
     modulationsMenu=new QMenu(this);
     QIcon infoMenuIcon(":/images/infoIcon-24.png");
     QIcon modifyMenuIcon(":/images/editIcon.png");
     QIcon deleteMenuIcon(":/images/deleteIcon.png");
     infoMenuIcon = infoMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
     modifyMenuIcon = modifyMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
     deleteMenuIcon = deleteMenuIcon.pixmap(QSize(24, 24));
     infoAction = modulationsMenu->addAction(infoMenuIcon,"Channel Informations");
     modifyAction=modulationsMenu->addAction(modifyMenuIcon,"Modify Channel");
     deleteAction=modulationsMenu->addAction(deleteMenuIcon,"Delete Channel");

//    deleteAction = new QAction("Delete",this);
    connect(deleteAction,&QAction::triggered,this,&Modulations::onDeleteActionTriggered);
//    infoAction=new QAction("Info",this);
    connect(infoAction,&QAction::triggered,this,&Modulations::onInfoActionTriggered);

//    modifyAction=new QAction("Modify",this);
    connect(modifyAction,&QAction::triggered,this,&Modulations::onModifyActionTriggered);


//DOCK WIDGET PART
    dockWidget = new QDockWidget(this);
    dockContent = new QStackedWidget(dockWidget);



    dockLayout = new QVBoxLayout;
    dockLayout->addWidget(dockWidget);
    dockWidget->hide();

    dockLayout->addWidget(dockContent);




    dockWidget->setWidget(dockContent);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);







}
//dockWidget functions
void Modulations::displayDockWidget(QString message)
{
    modulationLayout->addLayout(dockLayout);
    dockWidget->setWindowTitle(message);
    dockContent->show();  // Show the dock content before animating
    dockWidget->show();
}


void Modulations::contextMenuEvent(QContextMenuEvent *event){

//    modulationsMenu.addAction(infoAction);
//    modulationsMenu.addAction(modifyAction);
//    modulationsMenu.addAction(deleteAction);
    modulationsMenu->exec(event->globalPos());
}
void Modulations::populateTable()
{
    // Fetch data from the backend and populate the table
            // Clear existing table data
            modulationTable->clearContents();
            modulationTable->setRowCount(0);

            // Make a request to the backend endpoint and retrieve the data
            QNetworkAccessManager manager;
            QNetworkReply *reply = manager.get(QNetworkRequest(QUrl("http://localhost:3300/wms/survey/surveyModulations")));
            QEventLoop loop;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            // Check if the request was successful
            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << "Failed to retrieve Modulations Data :" << reply->errorString();
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
                const int modulationId = jsonObject.value("modulation_id").toInt(); //returns the value of a specified key
                const int rate = jsonObject.value("rate").toInt();



                // Add a new row to the table and populate the fields
                int rowCount = modulationTable->rowCount();
                modulationTable->insertRow(rowCount);
                modulationTable->setItem(rowCount, 0, new QTableWidgetItem(QString::number(modulationId)));
                modulationTable->setItem(rowCount, 1, new QTableWidgetItem(QString::number(rate)));


            }

            reply->deleteLater();

            // Refresh the table data every 5 seconds



}
void Modulations::onAddModulationClicked()
{

     showAddModulationForm();

}
void Modulations::onDeleteActionTriggered()
{
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this modulation?",
                                         QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::Yes)
    {
        QTableWidgetItem *currentItem = modulationTable->currentItem();
        int rowIndex = modulationTable->row(currentItem);
        QString modulationId = modulationTable->item(rowIndex, 0)->text();
        qDebug()<<"channel deleted is :"<<modulationId;
        // Assuming product ID is stored in the first column
        // Send a request to delete the product using the appropriate API endpoint
        // Example: localhost:3300/deleteProduct/:id

        // Update the table by removing the deleted row
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyModulations/" + modulationId));
        QNetworkReply *reply = manager.deleteResource(request);
        QEventLoop loop;
        connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
        loop.exec();

        if(reply->error()==QNetworkReply::NoError)
        {
            qDebug() <<"Modulation deleted succesfully";
            modulationTable->removeRow(rowIndex);

        }else{

            qDebug() << "failed to delete modulation:"<< reply->errorString();

        }
        reply->deleteLater();





    }

    }

//show add modulation form new verison
void Modulations::showAddModulationForm()
{
    // Create the widget for adding a new modulation
    addModulationWidget = new QWidget(this);

    QVBoxLayout* layout = new QVBoxLayout(addModulationWidget);
    QFormLayout* formLayout = new QFormLayout;

    QLabel* rateLabel = new QLabel("Rate:");
    QLineEdit* rateLineEdit = new QLineEdit;


    // Adding the fields to the form layout
    formLayout->addRow(rateLabel, rateLineEdit);

    // Adding a scroll area
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

//    addModulationWidget->setLayout(layout);

    // Connect the submit button to the onAddModulationClicked() slot
    connect(submitButton, &QPushButton::clicked, [rateLineEdit,this]() {
        // Read the input values from the fields
        int rate = rateLineEdit->text().toInt();

        // Prepare the request body JSON object
        QJsonObject requestBody;
        requestBody["rate"] = rate;
       // Convert the JSON object to a QByteArray
        QJsonDocument jsonDoc(requestBody);
        QByteArray requestData = jsonDoc.toJson();

        // Make a POST request to the backend endpoint
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyModulations"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply* reply = manager.post(request, requestData);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            // Request successful
            qDebug() << "Modulation added successfully";
            // Refresh the table data
            populateTable();
        } else {
            // Request failed
            qDebug() << "Failed to add Modulation:" << reply->errorString();
        }

        reply->deleteLater();
        addModulationWidget->close();
        dockWidget->close();
        dockContent->close();
        modulationLayout->removeItem(dockLayout);


    });

//    connect(submitButton,&QPushButton::clicked,dockWidget,&QWidget::close);
//    connect(submitButton,&QPushButton::clicked,dockContent,&QWidget::close);

    // Connect the cancel button to the widget's close() slot

    connect(cancelButton, &QPushButton::clicked, addModulationWidget, &QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockWidget,&QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockContent,&QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockContent,[this](){
        modulationLayout->removeItem(dockLayout);

    });



    // Set the widget as the content of the dock widget
    dockContent->addWidget(addModulationWidget);
    dockContent->setCurrentWidget(addModulationWidget);
    // Show the dock widget
    displayDockWidget("Please provide Modulation informations: ");
}

//onInfoActionTriggered new version
void Modulations::onModifyActionTriggered()
{
    QTableWidgetItem* currentItem = modulationTable->currentItem();
    int rowIndex = modulationTable->row(currentItem);
    QString modulationId = modulationTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

    // Send a GET request to retrieve the information of the selected node
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyModulations/" + modulationId));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject modulationInfo = jsonResponse.object();

        // Create a QWidget to display the modify dialog
        modulationModifyWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(modulationModifyWidget);
        QFormLayout* formLayout = new QFormLayout;

        // Create labels and line edit fields for each node information
        QLabel* modulationIdLabel = new QLabel("Modulation Id:");
        QLabel* modulationIdValueLabel = new QLabel(QString::number(modulationInfo.value("modulation_id").toInt()));
        formLayout->addRow(modulationIdLabel, modulationIdValueLabel);

        QLabel* rateLabel = new QLabel("Rate:");
        QSpinBox* rateSpinBox = new QSpinBox(modulationModifyWidget);
        rateSpinBox->setValue(modulationInfo.value("rate").toInt());
        rateSpinBox->setRange(0, 999999999);
        formLayout->addRow(rateLabel, rateSpinBox);

        // Add the other fields as needed

        // Create a QPushButton for the Save button
        QPushButton* saveButton = new QPushButton("Save", modulationModifyWidget);

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



        layout->addLayout(formLayout);
        layout->addLayout(buttonLayout);
//        layout->addWidget(saveButton);
//        layout->addWidget(cancelButton);

        connect(cancelButton, &QPushButton::clicked, modulationModifyWidget, &QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockWidget,&QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockContent,&QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockContent,[this](){
            modulationLayout->removeItem(dockLayout);


        });

        // Connect the Save button to the onSaveButtonClicked slot
        connect(saveButton, &QPushButton::clicked, this, [this, modulationInfo, rateSpinBox]() {
            // Extract the modified fields from the modify dialog's line edit fields
            // Example: QString modifiedName = nameLineEdit->text();
            int modifiedRate = rateSpinBox->value();

            // Construct the modified fields in a QJsonObject
            QJsonObject modifiedFields;
            // Example: modifiedFields["name"] = modifiedName;
            modifiedFields["rate"] = modifiedRate;

            // Create the request body JSON object
            QJsonObject requestBody;
            // Add the modified fields to the request body
            requestBody = modifiedFields;

            qDebug() << "body:" << requestBody;

            // Convert the JSON object to a QByteArray
            QJsonDocument jsonDoc(requestBody);
            QByteArray requestData = jsonDoc.toJson();

            // Send a PUT request to update the node information
            QNetworkAccessManager manager;
            QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyModulations/" + QString::number(modulationInfo.value("modulation_id").toInt())));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply* reply = manager.put(request, requestData);
            QEventLoop loop;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            if (reply->error() == QNetworkReply::NoError)
            {
                // Request successful
                QMessageBox::information(this, "Success", "Modulation information modified successfully!");
                dockContent->removeWidget(modulationModifyWidget); // Remove the modify widget from the dock
                modulationModifyWidget->deleteLater(); // Delete the modify widget
            }
            else
            {
                // Request failed
                QMessageBox::critical(this, "Error", "Failed to modify Modulation information: " + reply->errorString());
                dockContent->removeWidget(modulationModifyWidget); // Remove the modify widget from the dock
                modulationModifyWidget->deleteLater(); // Delete the modify widget
            }

            reply->deleteLater();
            modulationModifyWidget->close();
            dockWidget->close();
            dockContent->close();
            modulationLayout->removeItem(dockLayout);

        });



        // Set the layout and add the modify widget to the dock content
        modulationModifyWidget->setLayout(layout);
        dockContent->addWidget(modulationModifyWidget);
        dockContent->setCurrentWidget(modulationModifyWidget);
        // Show the dock widget
        displayDockWidget("Edit Modulation informations: ");
    }
    else
    {
        // Request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve Modulation information: " + reply->errorString());
    }

    reply->deleteLater();
}

//onInfoActionTriggered new version
void Modulations::onInfoActionTriggered()
{
    QTableWidgetItem* currentItem = modulationTable->currentItem();
    int rowIndex = modulationTable->row(currentItem);
    QString modulationId = modulationTable->item(rowIndex, 0)->text(); // Assuming router ID is stored in the first column

    // Send a GET request to retrieve the information of the selected modulation
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyModulations/" + modulationId));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject modulationInfo = jsonResponse.object();

        // Extract the fields from the JSON response
        int modulationId = modulationInfo.value("modulation_id").toInt();
        int rate = modulationInfo.value("rate").toInt();
        // Extract other fields as needed

        // Create a widget to display the information
         modulationsInfoWidget = new QWidget(this);
         QFormLayout* formLayout = new QFormLayout;

        QVBoxLayout* layout = new QVBoxLayout(modulationsInfoWidget);

        QLabel* idLabel = new QLabel("Modulation ID: ");
        QLabel*  idValueLabel = new QLabel(QString::number(modulationId));
        QLabel* rateLabel = new QLabel("Rate: ");
        QLabel* rateValueLabel= new QLabel(QString::number(rate) + " Mbps");

        formLayout->addRow(idLabel,idValueLabel);
        formLayout->addRow(rateLabel,rateValueLabel);
        // Add other labels for additional fields



        // Add a scroll area
        QScrollArea* scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget* scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);

        QPushButton* cancelButton = new QPushButton("Close");


        QIcon cancelIcon(":/images/closeIcon.png");
        cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        cancelButton->setIcon(cancelIcon);

        cancelButton->setObjectName("cancelButton");

        cancelButton->setFixedWidth(200);

        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(cancelButton);
        layout->addLayout(buttonLayout);
        connect(cancelButton,&QPushButton::clicked,modulationsInfoWidget,&QWidget::close);

        connect(cancelButton,&QPushButton::clicked,dockWidget,&QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockContent,&QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockContent,[this](){
            modulationLayout->removeItem(dockLayout);

        });




        // Add other labels to the layout for additional fields

        modulationsInfoWidget->setLayout(layout);

             // Add the new infoWidget to dockContent
             dockContent->addWidget(modulationsInfoWidget);
        dockContent->setCurrentWidget(modulationsInfoWidget);
        // Show the dock widget
        displayDockWidget("Modulation informations: ");
    }
    else
    {
        // Request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve Modulation information: " + reply->errorString());
    }

    reply->deleteLater();
}





// onModifyAction old version
//void Modulations::onModifyActionTriggered(){

//    QTableWidgetItem* currentItem = modulationTable->currentItem();
//    int rowIndex = modulationTable->row(currentItem);
//    QString modulationId = modulationTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column
//    qDebug()<<"id"<<modulationId;
//    // Send a GET request to retrieve the information of the selected node
//    QNetworkAccessManager manager;
//    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyModulations/" + modulationId));
//    QNetworkReply* reply = manager.get(getRequest);
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

//    loop.exec();

//    if (reply->error() == QNetworkReply::NoError)
//    {
//        // Request successful
//        QByteArray responseData = reply->readAll();
//        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
//        QJsonObject modulationInfo = jsonResponse.object();

//        // Create a QDialog to display the modify dialog
//        QDialog modifyDialog(this);
//        QVBoxLayout layout(&modifyDialog);
//        QFormLayout *formLayout = new QFormLayout;


//        // Create labels and line edit fields for each node information

//        QLabel *modulationIdLabel = new QLabel("Modulation Id:");
//        QLabel *modulationIdValueLabel = new QLabel(modulationInfo.value("modulation_id").toString());
//        formLayout->addRow(modulationIdLabel,modulationIdValueLabel);

//        QLabel* rateLabel = new QLabel("Rate:");
//        QSpinBox * rateSpinBox = new QSpinBox(&modifyDialog);
//        rateSpinBox->setValue(modulationInfo.value("rate").toInt());
//        rateSpinBox->setRange(0,999999999);
//        formLayout->addRow(rateLabel,rateSpinBox);




//        QScrollArea scrollArea;
//        scrollArea.setWidgetResizable(true);
//        QWidget *scrollAreaContent = new QWidget;
//        scrollAreaContent->setLayout(formLayout);
//        scrollArea.setWidget(scrollAreaContent);
//        layout.addWidget(&scrollArea);



//        // Add the other fields as needed

//        // Create a QPushButton for the Save button
//        QPushButton* saveButton = new QPushButton("Save", &modifyDialog);
//        layout.addWidget(saveButton);

//        // Connect the Save button to the onSaveButtonClicked slot
//        connect(saveButton, &QPushButton::clicked, this, [this,&modifyDialog,modulationInfo,rateSpinBox]() {
//            // Extract the modified fields from the modify dialog's line edit fields
//            // Example: QString modifiedName = nameLineEdit->text();
//            int modifiedRate=rateSpinBox->value();




//            // Construct the modified fields in a QJsonObject
//            QJsonObject modifiedFields;
//            // Example: modifiedFields["name"] = modifiedName;
//            modifiedFields["rate"] = modifiedRate;




//            // Create the request body JSON object
//            QJsonObject requestBody;
//            // Add the modified fields to the request body
//            requestBody= modifiedFields;

//            qDebug()<<"body:"<<requestBody;

//            // Convert the JSON object to a QByteArray
//            QJsonDocument jsonDoc(requestBody);
//            QByteArray requestData = jsonDoc.toJson();

//            // Send a PUT request to update the node information
//            QNetworkAccessManager manager;


//            QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyModulations/" + QString::number(modulationInfo.value("modulation_id").toInt())));
//            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//            QNetworkReply* reply = manager.put(request, requestData);
//            QEventLoop loop;
//            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//            loop.exec();

//            if (reply->error() == QNetworkReply::NoError)
//            {
//                // Request successful
//                QMessageBox::information(this, "Success", "Modulation information modified successfully!");
//                modifyDialog.close(); // Close the dialog

//            }
//            else
//            {
//                // Request failed
//                QMessageBox::critical(this, "Error", "Failed to modify Modulation information: " + reply->errorString());
//                modifyDialog.close(); // Close the dialog

//            }

//            reply->deleteLater();


//        });

//        // Set the layout and show the dialog
//        modifyDialog.setLayout(&layout);
//        modifyDialog.exec();


//    }
//    else
//    {
//        // Request failed
//        QMessageBox::critical(this, "Error", "Failed to retrieve Modulation information: " + reply->errorString());
//    }

//    reply->deleteLater();




//}

//ON INFO ACTION TRIGGERED OLD VERSION
//void Modulations::onInfoActionTriggered(){


//    QTableWidgetItem* currentItem = modulationTable->currentItem();
//    int rowIndex = modulationTable->row(currentItem);
//    QString modulationId = modulationTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

//    // Send a GET request to retrieve the information of the selected node
//    QNetworkAccessManager manager;
//    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyModulations/" + modulationId));
//    QNetworkReply* reply = manager.get(getRequest);
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();

//    if (reply->error() == QNetworkReply::NoError)
//    {
//        // Request successful
//        QByteArray responseData = reply->readAll();
//        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
//        QJsonObject modulationInfo = jsonResponse.object();

//        // Extract the fields from the JSON response and display the information
//        // Extract other fields as needed

//        int modulationId = modulationInfo.value("modulation_id").toInt();
//        int rate = modulationInfo.value("rate").toInt();



//        // Display the information using QMessageBox or custom dialog

//        QMessageBox::information(this, "Modulation Information",
//                                 "Modulation ID: " + QString::number(modulationId) + "\n"
//                                                       "Rate: " + QString::number(rate) + " Mbps\n"
//                                 );


//    }
//    else
//    {
//        // Request failed
//        QMessageBox::critical(this, "Error", "Failed to retrieve Modulation information: " + reply->errorString());
//    }

//    reply->deleteLater();





//}

// SHOW add modulation form old version

//void Modulations::showAddModulationForm()
//{
//    // Create the dialog for adding a new router
//    QDialog addModulationDialog;
//    addModulationDialog.setWindowTitle("Add Modulation"); // Set the dialog title

//    QVBoxLayout layout(&addModulationDialog);
//    QFormLayout *formLayout = new QFormLayout;


//    QLabel *rateLabel = new QLabel("Rate:");
//    QLineEdit *rateLineEdit = new QLineEdit;


//    //adding the fields to the form layout

//    formLayout->addRow(rateLabel, rateLineEdit);

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

//    addModulationDialog.setLayout(&layout);


//    // Connect the submit button to the onAddRouterClicked() slot
//    connect(submitButton, &QPushButton::clicked, [&addModulationDialog, rateLineEdit,this]() {

//        //preparing the request body elements
//        int rate = rateLineEdit->text().toInt();



//        // Create the request body JSON object
//        QJsonObject requestBody;
//        requestBody["rate"] = rate;


//        // Add more fields to the JSON object as needed

//        // Convert the JSON object to a QByteArray
//        QJsonDocument jsonDoc(requestBody);
//        QByteArray requestData = jsonDoc.toJson();

//        // Make a POST request to the backend endpoint
//        QNetworkAccessManager manager;
//        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyModulations"));
//        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//        QNetworkReply *reply = manager.post(request, requestData);
//        QEventLoop loop;
//        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//        loop.exec();

//        if (reply->error() == QNetworkReply::NoError) {
//            // Request successful
//            qDebug() << "Modulation added successfully";
//            // Refresh the table data
//            populateTable();
//        } else {
//            // Request failed
//            qDebug() << "Failed to add Modulation:" << reply->errorString();
//        }

//        reply->deleteLater();
//        addModulationDialog.accept();
//    });
//    connect(cancelButton, &QPushButton::clicked, &addModulationDialog, &QDialog::close);
//    addModulationDialog.exec();


//}




//void Modulations::createDockWidget()
//{
//    dockWidget = new QDockWidget("Dock Widget", this);
//    dockContent = new QWidget(dockWidget);
//    dockButton = new QPushButton("Toggle Dock", dockContent);

//    QVBoxLayout *dockLayout = new QVBoxLayout(dockContent);
//    dockLayout->addWidget(dockButton);


//    dockWidget->setWidget(dockContent);
//    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);


//    // Connect the button click to the slot
//    connect(dockButton, &QPushButton::clicked, this, &Modulations::onDockButtonClicked);

//    dockAnimation = new QPropertyAnimation(dockWidget, "geometry");
//    dockAnimation->setDuration(500);  // Animation duration in milliseconds
//    dockAnimation->setEasingCurve(QEasingCurve::InOutQuad);

//    connect(dockAnimation, &QPropertyAnimation::finished, this, &Modulations::onDockWidgetAnimationFinished);
//}










//void Modulations::onTableRightClicked(const QPoint &pos)
//{
//Q_UNUSED(pos);
//    QTableWidgetItem *currentItem = modulationTable->itemAt(pos);
//    if (currentItem)
//    {
//        QMenu *contextMenu = new QMenu(this);
//        QAction *deleteAction = contextMenu->addAction("Delete");
//        connect(deleteAction, &QAction::triggered, this, &Modulations::onDeleteActionTriggered);
//        contextMenu->exec(QCursor::pos());
//        delete contextMenu;
//    }
//}

//void Modulations::onInfoActionTriggered()
//{
//    QTableWidgetItem *currentItem = modulationTable->currentItem();
//    if (currentItem)
//    {
//        int rowIndex = currentItem->row();
//        QString modulationId = modulationTable->item(rowIndex, 0)->text(); // Assuming modulation ID is stored in the first column

//        // Retrieve the information for the selected modulation
//        QNetworkAccessManager manager;
//        QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyModulations/" + modulationId));
//        QNetworkReply *reply = manager.get(getRequest);
//        QEventLoop loop;
//        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//        loop.exec();

//        if (reply->error() == QNetworkReply::NoError)
//        {
//            QByteArray responseData = reply->readAll();
//            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
//            QJsonObject modulationInfo = jsonResponse.object();

//            // Create a dock widget to display the modulation information
//            QDockWidget *dockWidget = new QDockWidget("Modulation Information", this);

//            // Create a text widget to show the information
//            QLabel *label = new QLabel(dockWidget);
//                        label->setAlignment(Qt::AlignTop | Qt::AlignLeft);

//                        // Set the modulation information in the label
//                        QString modulationInfoText = "Modulation ID: " + modulationId + "<br>";
//                        modulationInfoText += "Rate: " + QString::number(modulationInfo.value("rate").toInt()) + " Mbps<br>";
//                        // Add other information as needed
//                        // ...

//                        label->setText(modulationInfoText);

//                        // Set the label as the central widget of the dock widget
//                        dockWidget->setWidget(label);


//            // Add the dock widget to the layout of the Modulations widget
//            layout()->addWidget(dockWidget);

//        }
//        else
//        {
//            QMessageBox::critical(this, "Error", "Failed to retrieve Modulation information: " + reply->errorString());
//        }

//        reply->deleteLater();
//    }
//}
