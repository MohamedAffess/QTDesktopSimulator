#include "products.h"

#include <QToolButton>
Products::Products(QWidget *parent) : QWidget(parent)
{
//    setObjectName("Products");
//    // Apply the stylesheet
//    setAttribute(Qt::WA_StyledBackground, true);

//        QFile styleFile(":/styles/style.qss");
//        styleFile.open(QFile::ReadOnly);
//        QString styleSheet = QLatin1String(styleFile.readAll());
//        setStyleSheet(styleSheet);
    setupUI();
}
Products::~Products(){
    delete meshRoutersContent;
    delete antennasContent;
    delete modulationsContent;
    delete channelsContent;
    delete contentStack;
    delete meshRoutersButton;
    delete modulationsButton;
    delete channelsButton;
    delete antennasButton;
    delete productsLayout;
    delete contentStack;

}

void Products::setupUI()
{
    productsLayout = new QVBoxLayout;
    setLayout(productsLayout);
    titleLabel = new QLabel(this);
        titleLabel->setStyleSheet("font-size: 18px; font-weight: semi-bold; margin: 20px; font-family: 'Arial Light';");  // Example style for the title



    // Create the hidden menu

    QToolButton* productsMenuButton = new QToolButton(this);
    QHBoxLayout* contentHeader= new QHBoxLayout(this);
    contentHeader->addWidget(productsMenuButton);
    contentHeader->addWidget(titleLabel);


    QIcon productsMenuIcon(":/images/menu.png");
    productsMenuIcon = productsMenuIcon.pixmap(QSize(30, 30));  // Set the desired icon size here
    productsMenuButton->setIcon(productsMenuIcon);
    productsMenuButton->setPopupMode(QToolButton::InstantPopup);
    productsMenuButton->setStyleSheet("QToolButton {"
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
    QMenu* productsMenu = new QMenu(this);
    productsMenuButton->setMenu(productsMenu);
    productsMenuButton->setText("Products List");

    productsMenuButton->setFixedWidth(100);

    productsMenu->setStyleSheet("QMenu {"
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
    productsMenu->setFont(menuFont);
    // Set the menu icon

    productsMenu->setTitle("Products");

    QIcon meshRoutersMenuIcon(":/images/meshRouter.png");
    QIcon antennasMenuIcon(":/images/antenna.png");
    QIcon channelMenuIcon(":/images/channel.png");
    QIcon modulationMenuIcon(":/images/modulation.png");
    meshRoutersMenuIcon = meshRoutersMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here
    antennasMenuIcon = antennasMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here
    channelMenuIcon = channelMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here
    modulationMenuIcon = modulationMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here



    QAction* meshRoutersAction = productsMenu->addAction(meshRoutersMenuIcon,"Mesh Routers");
    QAction* antennasAction = productsMenu->addAction(antennasMenuIcon,"Antennas");
    QAction* modulationsAction = productsMenu->addAction(channelMenuIcon,"Modulations");
    QAction* channelsAction = productsMenu->addAction(modulationMenuIcon,"Channels");

    // Connect the menu actions to their respective slots
    connect(meshRoutersAction, &QAction::triggered, this, &Products::onMeshRoutersButtonClicked);
    connect(antennasAction, &QAction::triggered, this, &Products::onAntennasButtonClicked);
    connect(modulationsAction, &QAction::triggered, this, &Products::onModulationsButtonClicked);
    connect(channelsAction, &QAction::triggered, this, &Products::onChannelsButtonClicked);

    // Create the button for the hidden menu
    productsLayout->addLayout(contentHeader);



    contentStack = new QStackedWidget(this);
    productsLayout->addWidget(contentStack);



    meshRoutersContent = new MeshRouters;
    antennasContent = new Antennas;
    modulationsContent = new Modulations;
    channelsContent = new Channels;

    contentStack->addWidget(meshRoutersContent);
    titleLabel->setText("LUCEOR Mesh Routers");

    contentStack->addWidget(antennasContent);
    contentStack->addWidget(modulationsContent);
    contentStack->addWidget(channelsContent);
}

void Products::onMeshRoutersButtonClicked()
{
    titleLabel->setText("LUCEOR Mesh Routers");
    contentStack->setCurrentWidget(meshRoutersContent);
}

void Products::onAntennasButtonClicked()
{
    titleLabel->setText("LUCEOR Antennas");
    contentStack->setCurrentWidget(antennasContent);
}

void Products::onModulationsButtonClicked()
{
    titleLabel->setText("Modulations");
    contentStack->setCurrentWidget(modulationsContent);
}

void Products::onChannelsButtonClicked()
{
    titleLabel->setText("Channels");
    contentStack->setCurrentWidget(channelsContent);
}
