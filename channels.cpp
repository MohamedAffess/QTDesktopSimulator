#include "channels.h"
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
#include <QGroupBox>
Channels::Channels(QWidget *parent) : QWidget(parent)
{
    setupUI();
    populateTable();
    QTimer::singleShot(30000, this, &Channels::populateTable);

}
Channels::~Channels(){
    delete channelTable;
    delete addChannelButton;
    delete dockWidget;
    delete dockContent;
    delete channelsLayout;
    delete addChannelWidget;
    delete channelInfoWidget;
    delete channelsMenu;
}
void Channels::setupUI()
{
    channelsLayout = new QVBoxLayout;
    setLayout(channelsLayout);

    channelTable = new QTableWidget(this);
    channelTable->setColumnCount(2);
    channelTable->setHorizontalHeaderLabels({"Channel ", "Frequency (MHz)"});
    channelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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



    channelTable->setStyleSheet(tableStyleSheet);
    channelsLayout->addWidget(channelTable);

    addChannelButton = new QPushButton("Add Channel", this);
    QIcon addIcon(":/images/addIcon.png");
    addIcon = addIcon.pixmap(QSize(128,128));  // Set the desired icon size here

    addChannelButton->setIcon(addIcon);
    addChannelButton->setStyleSheet("QPushButton {"
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
    addChannelButton->setFixedWidth(200);
    channelsLayout->addWidget(addChannelButton);

    connect(addChannelButton, &QPushButton::clicked, this, &Channels::onAddChannelClicked);

    //actions
     channelsMenu=new QMenu(this);
    QIcon infoMenuIcon(":/images/infoIcon-24.png");
    QIcon modifyMenuIcon(":/images/editIcon.png");
    QIcon deleteMenuIcon(":/images/deleteIcon.png");
    infoMenuIcon = infoMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
    modifyMenuIcon = modifyMenuIcon.pixmap(QSize(24, 24));  // Set the desired icon size here
    deleteMenuIcon = deleteMenuIcon.pixmap(QSize(24, 24));
    infoAction = channelsMenu->addAction(infoMenuIcon,"Channel Informations");
    modifyAction=channelsMenu->addAction(modifyMenuIcon,"Modify Channel");
    deleteAction=channelsMenu->addAction(deleteMenuIcon,"Delete Channel");

//    deleteAction=new QAction("Delete",this);
    connect(deleteAction,&QAction::triggered,this,&Channels::onDeleteActionTriggered);

//    infoAction=new QAction("Info",this);
    connect(infoAction,&QAction::triggered,this,&Channels::onInfoActionTriggered);

//    modifyAction=new QAction("Modify",this);
    connect(modifyAction,&QAction::triggered,this,&Channels::onModifyActionTriggered);


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

void Channels::displayDockWidget(QString message)
{
    channelsLayout->addLayout(dockLayout);
    dockWidget->setWindowTitle(message);
    dockContent->show();  // Show the dock content before animating
    dockWidget->show();
}


void Channels::contextMenuEvent(QContextMenuEvent *event)
{

//    channelsMenu.addAction(infoAction);
//    channelsMenu.addAction(modifyAction);
//    channelsMenu.addAction(deleteAction);
    channelsMenu->exec(event->globalPos());
}

void Channels::populateTable()
{// Fetch data from the backend and populate the table
    // Clear existing table data
    channelTable->clearContents();
    channelTable->setRowCount(0);

    // Make a request to the backend endpoint and retrieve the data
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl("http://localhost:3300/wms/survey/surveyChannels")));
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check if the request was successful
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Failed to retrieve Channels Data :" << reply->errorString();
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
        const QString channelId = jsonObject.value("channel_id").toString(); //returns the value of a specified key
        const int frequency = jsonObject.value("frequency").toInt();



        // Add a new row to the table and populate the fields
        int rowCount = channelTable->rowCount();
        channelTable->insertRow(rowCount);
        channelTable->setItem(rowCount, 0, new QTableWidgetItem(channelId));
        channelTable->setItem(rowCount, 1, new QTableWidgetItem(QString::number(frequency)));


    }

    reply->deleteLater();

    // Refresh the table data every 5 seconds



}



void Channels::onAddChannelClicked()
{
    showAddChannelForm();

}

// showAddForm new version
void Channels::showAddChannelForm()
{
    // Create the widget for adding a new channel
    addChannelWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(addChannelWidget);
    QFormLayout* formLayout = new QFormLayout;

    QLabel* channelIdLabel = new QLabel("Channel:");
    QLineEdit* channelIdLineEdit = new QLineEdit;
    QLabel* frequencyLabel = new QLabel("Frequency:");
    QLineEdit* frequencyLineEdit = new QLineEdit;

    // Add the fields to the form layout
    formLayout->addRow(channelIdLabel, channelIdLineEdit);
    formLayout->addRow(frequencyLabel, frequencyLineEdit);

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

    // Connect the submit button to the onAddChannelClicked() slot
    connect(submitButton, &QPushButton::clicked, this, [this, channelIdLineEdit, frequencyLineEdit]() {
        // Prepare the request body elements
        QString channelId = channelIdLineEdit->text();
        int frequency = frequencyLineEdit->text().toInt();

        // Create the request body JSON object
        QJsonObject requestBody;
        requestBody["channel_id"] = channelId;
        requestBody["frequency"] = frequency;

        // Add more fields to the JSON object as needed

        // Convert the JSON object to a QByteArray
        QJsonDocument jsonDoc(requestBody);
        QByteArray requestData = jsonDoc.toJson();

        // Make a POST request to the backend endpoint
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyChannels"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply* reply = manager.post(request, requestData);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            // Request successful
            qDebug() << "Channel added successfully";
            // Refresh the table data
            populateTable();
        } else {
            // Request failed
            qDebug() << "Failed to add Channel:" << reply->errorString();
        }

        reply->deleteLater();
        addChannelWidget->close();
        dockWidget->close();
        dockContent->close();
        channelsLayout->removeItem(dockLayout);


    });

    connect(cancelButton, &QPushButton::clicked, addChannelWidget, &QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockWidget,&QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockContent,&QWidget::close);
    connect(cancelButton,&QPushButton::clicked,dockContent,[this](){
        channelsLayout->removeItem(dockLayout);


    });




    // Add the widget to the dock content
    dockContent->addWidget(addChannelWidget);
    dockContent->setCurrentWidget(addChannelWidget);
    // Show the dock widget
    displayDockWidget("Please provide Channel informations: ");
}
//on info action new version
void Channels::onInfoActionTriggered()
{
    QTableWidgetItem* currentItem = channelTable->currentItem();
    int rowIndex = channelTable->row(currentItem);
    QString channelId = channelTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

    // Send a GET request to retrieve the information of the selected channel
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyChannels/" + channelId));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject channelInfo = jsonResponse.object();

        // Extract the fields from the JSON response and display the information
        QString channelId = channelInfo.value("channel_id").toString();
        int frequency = channelInfo.value("frequency").toInt();

        // Create the widget to display channel information
        channelInfoWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(channelInfoWidget);
        QFormLayout* formLayout = new QFormLayout;



        QLabel* channelIdLabel = new QLabel("Channel:");
        QLabel* channelIdValueLabel = new QLabel(channelId);
        QLabel* frequencyLabel = new QLabel("Frequency:");
        QLabel* frequencyValueLabel = new QLabel(QString::number(frequency) + " MHz");
        formLayout->addRow(channelIdLabel,channelIdValueLabel);
        formLayout->addRow(frequencyLabel,frequencyValueLabel);

        QScrollArea* scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget* scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);


        QPushButton *closeButton = new QPushButton("Close");

        QIcon cancelIcon(":/images/closeIcon.png");
        cancelIcon = cancelIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
        closeButton->setIcon(cancelIcon);
        closeButton->setObjectName("cancelButton");
        closeButton->setFixedWidth(200);
        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);
        connect(closeButton,&QPushButton::clicked,channelInfoWidget,&QWidget::close);

        connect(closeButton,&QPushButton::clicked,dockWidget,&QWidget::close);
        connect(closeButton,&QPushButton::clicked,dockContent,&QWidget::close);
        connect(closeButton,&QPushButton::clicked,dockContent,[this](){
            channelsLayout->removeItem(dockLayout);


        });


        // Add the widget to the dock content
        dockContent->addWidget(channelInfoWidget);
        dockContent->setCurrentWidget(channelInfoWidget);

        // Show the dock widget
        displayDockWidget("Channel informations: ");


    }
    else
    {
        // Request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve Channel information: " + reply->errorString());
    }

    reply->deleteLater();

}



//on mofidy new version
void Channels::onModifyActionTriggered(){

    QTableWidgetItem* currentItem = channelTable->currentItem();
    int rowIndex = channelTable->row(currentItem);
    QString channelId = channelTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

    // Send a GET request to retrieve the information of the selected node
    QNetworkAccessManager manager;
    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyChannels/" + channelId));
    QNetworkReply* reply = manager.get(getRequest);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        // Request successful
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject channelInfo = jsonResponse.object();

        // Create a QDialog to display the modify dialog

        channelModifyWidget= new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(channelModifyWidget);
        QFormLayout *formLayout = new QFormLayout;


        // Create labels and line edit fields for each node information

        QLabel *channelIdLabel = new QLabel("Channel:");
        QLabel *channelIdValueLabel = new QLabel(channelInfo.value("channel_id").toString());
        formLayout->addRow(channelIdLabel,channelIdValueLabel);

        QLabel* frequencyLabel = new QLabel("Frequency:");
        QSpinBox * frequencySpinBox = new QSpinBox(channelModifyWidget);
        frequencySpinBox->setValue(channelInfo.value("frequency").toInt());
        frequencySpinBox->setRange(0,99999999);

        formLayout->addRow(frequencyLabel,frequencySpinBox);



        // Add a scroll area
        QScrollArea* scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        QWidget* scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(formLayout);
        scrollArea->setWidget(scrollAreaContent);
        layout->addWidget(scrollArea);





        // Add the other fields as needed

        // Create a QPushButton for the Save button
        QPushButton* saveButton = new QPushButton("Save",channelModifyWidget);

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


        connect(cancelButton, &QPushButton::clicked, channelModifyWidget, &QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockWidget,&QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockContent,&QWidget::close);
        connect(cancelButton,&QPushButton::clicked,dockContent,[this](){
            channelsLayout->removeItem(dockLayout);

        });


        // Connect the Save button to the onSaveButtonClicked slot
        connect(saveButton, &QPushButton::clicked, this, [this,channelInfo,frequencySpinBox]() {
            // Extract the modified fields from the modify dialog's line edit fields
            // Example: QString modifiedName = nameLineEdit->text();
            int modifiedFrequency=frequencySpinBox->value();




            // Construct the modified fields in a QJsonObject
            QJsonObject modifiedFields;
            modifiedFields["frequency"] = modifiedFrequency;




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
            QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyChannels/" + channelInfo.value("channel_id").toString()));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply* reply = manager.put(request, requestData);
            QEventLoop loop;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            if (reply->error() == QNetworkReply::NoError)
            {
                // Request successful
                QMessageBox::information(this, "Success", "Channel information modified successfully!");
                dockContent->removeWidget(channelModifyWidget);
                channelModifyWidget->deleteLater();// Close the dialog
                populateTable();


            }
            else
            {
                // Request failed
                QMessageBox::critical(this, "Error", "Failed to modify Channels information: " + reply->errorString());
                dockContent->removeWidget(channelModifyWidget); // Remove the modify widget from the dock
                channelModifyWidget->deleteLater(); // Delete the modify widget
            }

            reply->deleteLater();
            channelModifyWidget->close();
            dockWidget->close();
            dockContent->close();
            channelsLayout->removeItem(dockLayout);


        });

        // Set the layout and add the modify widget to the dock content
        channelModifyWidget->setLayout(layout);
        dockContent->addWidget(channelModifyWidget);
        dockContent->setCurrentWidget(channelModifyWidget);
        // Show the dock widget
        displayDockWidget("Edit Channel informations: ");

    }
    else
    {
        // Request failed
        QMessageBox::critical(this, "Error", "Failed to retrieve Channel information: " + reply->errorString());
    }

    reply->deleteLater();





}


void Channels::onDeleteActionTriggered(){

    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this channel?",
                                         QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::Yes)
    {
        QTableWidgetItem *currentItem = channelTable->currentItem();
        int rowIndex = channelTable->row(currentItem);
        QString channelId = channelTable->item(rowIndex, 0)->text();
        qDebug()<<"channel deleted is :"<<channelId;
        // Assuming product ID is stored in the first column
        // Send a request to delete the product using the appropriate API endpoint
        // Example: localhost:3300/deleteProduct/:id

        // Update the table by removing the deleted row
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyChannels/" + channelId));
        QNetworkReply *reply = manager.deleteResource(request);
        QEventLoop loop;
        connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
        loop.exec();

        if(reply->error()==QNetworkReply::NoError)
        {
            qDebug() <<"Channel deleted succesfully";
            channelTable->removeRow(rowIndex);

        }else{

            qDebug() << "failed to delete Channel:"<< reply->errorString();

        }
        reply->deleteLater();





    }


}





//on show add form old version
//void Channels::showAddChannelForm()
//{
//    // Create the dialog for adding a new router
//    QDialog addChannelDialog;
//    addChannelDialog.setWindowTitle("Add Channel"); // Set the dialog title

//    QVBoxLayout layout(&addChannelDialog);
//    QFormLayout *formLayout = new QFormLayout;

//    QLabel *channelIdLabel = new QLabel("Channel:");
//    QLineEdit *channelIdLineEdit = new QLineEdit;
//    QLabel *frequencyLabel = new QLabel("Frequency:");
//    QLineEdit *frequencyLineEdit = new QLineEdit;


//    //adding the fields to the form layout

//    formLayout->addRow(channelIdLabel, channelIdLineEdit);
//    formLayout->addRow(frequencyLabel, frequencyLineEdit);

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

//    addChannelDialog.setLayout(&layout);


//    // Connect the submit button to the onAddRouterClicked() slot
//    connect(submitButton, &QPushButton::clicked, [&addChannelDialog, channelIdLineEdit, frequencyLineEdit,this]() {

//        //preparing the request body elements
//        QString channelId = channelIdLineEdit->text();
//        int frequency = frequencyLineEdit->text().toInt();



//        // Create the request body JSON object
//        QJsonObject requestBody;
//        requestBody["channel_id"] = channelId;
//        requestBody["frequency"] = frequency;


//        // Add more fields to the JSON object as needed

//        // Convert the JSON object to a QByteArray
//        QJsonDocument jsonDoc(requestBody);
//        QByteArray requestData = jsonDoc.toJson();

//        // Make a POST request to the backend endpoint
//        QNetworkAccessManager manager;
//        QNetworkRequest request(QUrl("http://localhost:3300/wms/survey/surveyChannels"));
//        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//        QNetworkReply *reply = manager.post(request, requestData);
//        QEventLoop loop;
//        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//        loop.exec();

//        if (reply->error() == QNetworkReply::NoError) {
//            // Request successful
//            qDebug() << "Channel added successfully";
//            // Refresh the table data
//            populateTable();
//        } else {
//            // Request failed
//            qDebug() << "Failed to add Channel:" << reply->errorString();
//        }

//        reply->deleteLater();
//        addChannelDialog.accept();
//    });
//    connect(cancelButton, &QPushButton::clicked, &addChannelDialog, &QDialog::close);
//    addChannelDialog.exec();








//}

//onInfoAction old version
//void Channels::onInfoActionTriggered(){

//    QTableWidgetItem* currentItem = channelTable->currentItem();
//    int rowIndex = channelTable->row(currentItem);
//    QString channelId = channelTable->item(rowIndex, 0)->text(); // Assuming router type is stored in the first column

//    // Send a GET request to retrieve the information of the selected node
//    QNetworkAccessManager manager;
//    QNetworkRequest getRequest(QUrl("http://localhost:3300/wms/survey/surveyChannels/" + channelId));
//    QNetworkReply* reply = manager.get(getRequest);
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();

//    if (reply->error() == QNetworkReply::NoError)
//    {
//        // Request successful
//        QByteArray responseData = reply->readAll();
//        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
//        QJsonObject channelInfo = jsonResponse.object();

//        // Extract the fields from the JSON response and display the information
//        // Extract other fields as needed

//        QString channelId = channelInfo.value("channel_id").toString();
//        int frequency = channelInfo.value("frequency").toInt();



//        // Display the information using QMessageBox or custom dialog

//        QMessageBox::information(this, "Channel Information",
//                                 "Channel: " + channelId + "\n"
//                                                       "Frequency: " + QString::number(frequency) + " MHz\n"
//                                 );


//    }
//    else
//    {
//        // Request failed
//        QMessageBox::critical(this, "Error", "Failed to retrieve Channel information: " + reply->errorString());
//    }

//    reply->deleteLater();







//}


//void Channels::onTableRightClicked(const QPoint &pos)
//{
//Q_UNUSED(pos);
//    QTableWidgetItem *currentItem = channelTable->itemAt(pos);
//    if (currentItem)
//    {
//        QMenu *contextMenu = new QMenu(this);
//        QAction *deleteAction = contextMenu->addAction("Delete");
//        connect(deleteAction, &QAction::triggered, this, &Channels::onDeleteActionTriggered);
//        contextMenu->exec(QCursor::pos());
//        delete contextMenu;
//    }
//}
