#include "management.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QToolButton>
Management::Management(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

Management::~Management(){
    delete nodesContent;
//    delete interfacesContent;
//    delete linksContent;
//    delete antennasContent;
    delete managementLayout;
    delete nodesButton;
//    delete interfacesButton;
//    delete linksButton;
//    delete antennasButton;
    delete contentStack;





}
//void Management::setupUI()
//{
//    managementLayout = new QVBoxLayout;
//    setLayout(managementLayout);

//    QToolButton* managementMenuButton = new QToolButton(this);
//    QIcon managementMenuIcon(":/images/menu.png");
//    managementMenuIcon = managementMenuIcon.pixmap(QSize(30, 30));  // Set the desired icon size here
//    managementMenuButton->setIcon(managementMenuIcon);
//    managementMenuButton->setFixedWidth(100);
//    managementMenuButton->setPopupMode(QToolButton::InstantPopup);
//    managementMenuButton->setStyleSheet("QToolButton {"
//                                        "background-color:  rgba(36,101,175,190);"  // Set the background color as transparent
//                                        "border: 1px solid #ccc;"
//                                        "color: white;"  // Set the text color to white
//                                        "padding: 12px 24px;"
//                                        "text-align: center;"
//                                        "text-decoration: none;"
//                                        "display: inline-block;"
//                                        "font-size: 16px;"
//                                        "font-family: Arial Light;"
//                                        "icon-size: 30px 30px;"



//                                        "margin: 4px 2px;"
//                                        "cursor: pointer;"
//                                        "}"
//                                        "QToolButton:hover {"
//                                        "background-color: rgba(36,101,175,210);"  // Set a hover background color with slight gray transparency
//                                        "}"
//                                        "QToolButton:pressed {"
//                                        "background-color: rgba(36,101,175,250);"  // Set a pressed background color with slight gray transparency
//                                      "}");

//    QMenu* managementMenu = new QMenu(managementMenuButton);
//    managementMenuButton->setMenu(managementMenu);
//    managementMenu->setStyleSheet("QMenu {"
//                                "background-color: #fff;"  // Set the background color
//                                "border: 1px solid #ccc;"  // Add a border
//                                "border-radius: 4px;"  // Round the corners
//                                "padding: 4px;"  // Add padding
//                                "}"
//                                "QMenu::item {"
//                                "background-color: #fff;"  // Set the background color for each menu item
//                                "padding: 8px 20px;"  // Add padding for each menu item
//                                "}"
//                                "QMenu::item:selected {"
//                                "background-color: rgba(36,101,175,210);"  // Set the background color for the selected menu item
//                                "color: #fff;"  // Set the text color for the selected menu item
//                                "}"
//                                "QMenu::item:pressed {"
//                                "background-color: rgba(36,101,175,250);"  // Set the background color for the pressed menu item
//                                "color: #fff;"  // Set the text color for the pressed menu item
//                                "}");
//    QFont menuFont("Arial Light", 12);  // Example font family and font size
//    managementMenu->setFont(menuFont);



//    QIcon nodesMenuIcon(":/images/nodes.png");
//    QIcon interfacesMenuIcon(":/images/interfaces.png");
//    QIcon linksMenuIcon(":/images/links.png");
//    QIcon antennasMenuIcon(":/images/antennas.png");
//    nodesMenuIcon = nodesMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here
//    interfacesMenuIcon = interfacesMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here
//    linksMenuIcon = linksMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here
//    antennasMenuIcon = antennasMenuIcon.pixmap(QSize(64, 64));  // Set the desired icon size here

//    QAction* nodesAction = managementMenu->addAction(nodesMenuIcon,"Nodes");

//    QAction* interfacesAction = managementMenu->addAction(interfacesMenuIcon,"Interfaces");

//    QAction* linksAction = managementMenu->addAction(linksMenuIcon,"Links");
//    QAction* antennasAction = managementMenu->addAction(antennasMenuIcon,"Antennas");


//    connect(nodesAction, &QAction::triggered, this, &Management::onNodesButtonClicked);
//    connect(interfacesAction, &QAction::triggered, this, &Management::onInterfacesButtonClicked);
//    connect(linksAction, &QAction::triggered, this, &Management::onLinksButtonClicked);
//    connect(antennasAction, &QAction::triggered, this, &Management::onAntennasButtonClicked);

//    managementLayout->addWidget(managementMenuButton);

//    contentStack = new QStackedWidget(this);
//    managementLayout->addWidget(contentStack);


//    // Set the fixed width for the buttons
//    nodesContent = new NodesManagement;
//    interfacesContent = new InterfacesManagement;
//    linksContent = new LinksManagement;
//    antennasContent = new AntennasManagement;

//    contentStack->addWidget(nodesContent);
//    contentStack->addWidget(interfacesContent);
//    contentStack->addWidget(linksContent);
//    contentStack->addWidget(antennasContent);
//}

void Management::setupUI()
{
    managementLayout = new QVBoxLayout;
    setLayout(managementLayout);

    QLabel* titleLabel = new QLabel("Topology Management", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; margin: 20px;");  // Example style for the title

    managementLayout->addWidget(titleLabel);

    contentStack = new QStackedWidget(this);
    managementLayout->addWidget(contentStack);

    // Set the content widget for node management
    nodesContent = new NodesManagement;
    contentStack->addWidget(nodesContent);
}


//void Management::onNodesButtonClicked(){
//    contentStack->setCurrentWidget(nodesContent);
//}

//void Management::onInterfacesButtonClicked(){
//    contentStack->setCurrentWidget(interfacesContent);
//}
//void  Management::onLinksButtonClicked(){
//    contentStack->setCurrentWidget(linksContent);
//}

//void Management::onAntennasButtonClicked(){
//    contentStack->setCurrentWidget(antennasContent);
//}







