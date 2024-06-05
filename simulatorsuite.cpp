#include "simulatorsuite.h"
#include "ui_simulatorsuite.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScopedPointer>


SimulatorSuite::SimulatorSuite(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SimulatorSuite)
{
//    setObjectName("SimulatorSuite");
//    // Apply the stylesheet
//        QFile styleFile(":/styles/style.qss");
//        styleFile.open(QFile::ReadOnly);
//        QString styleSheet = QLatin1String(styleFile.readAll());
//        setStyleSheet(styleSheet);
    ui->setupUi(this);
    setupUI();
    displayContent(simulationContent);

}

SimulatorSuite::~SimulatorSuite()
{
    delete ui;
    delete productsContent;
    delete managementContent;
    delete simulationContent;
    delete productButton;
    delete managementButton;
    delete simulationButton;
    delete currentContentWidget;
}

void SimulatorSuite::setupUI(){
    //Main Layout
    mainLayout = new QVBoxLayout;
    centralWidget =new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Set minimum and maximum window size
        setMinimumSize(1325, 1000);  // Set the minimum width and height for the window
        setMaximumSize(1325, 1000); // Set the maximum width and height for the window


    //Header Layout and label
    headerLayout = new QHBoxLayout;
    titleLabel = new QLabel("Luceor Simulator");
    QFont titleFont = titleLabel->font();
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    // Set the style sheet for the title label
    titleLabel->setStyleSheet("QLabel {"
                             "font-family: Arial Light;"
                              "font-size: 45px;"
                              "border: 1px solid #ccc;"
                              "font-weight: 200;"
                              "border-radius: 8px;"
                              "padding: 10px;"
                              "}");

    // Add a margin to the title label
    titleLabel->setContentsMargins(10, 0, 10, 0);


    logoLabel = new QLabel;
    QPixmap logoPixmap(":/images/logo-luceor.png");  // Use the resource path
    QSize logoSize(300, 300);  // Set the desired size
    QPixmap resizedPixmap = logoPixmap.scaled(logoSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(resizedPixmap);

    // Set the style sheet for the logo label
    logoLabel->setStyleSheet("QLabel {"
                             "border: 1px solid #ccc;"
                             "border-radius: 8px;"
                             "padding: 10px;"
                             "}");

    // Set the size policy of the logo label to expand and fill the available space
//    logoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    headerLayout->addWidget(logoLabel, 0, Qt::AlignLeft);  // Align the logo to the left

    headerLayout->addWidget(titleLabel,0,Qt::AlignCenter);
    headerLayout->setContentsMargins(10, 10, 10, 10);  // Adjust margins as desired





    // Create the navigation bar layout and buttons

   navLayout = new QHBoxLayout;
    productButton = new QPushButton("Routers and Tools");
    QIcon productsIcon(":/images/ProductsIcon.png");
    productsIcon = productsIcon.pixmap(QSize(80, 80));  // Set the desired icon size here
    productButton->setIcon(productsIcon);

    managementButton = new QPushButton("Topology Management");
    QIcon managementIcon(":/images/ManagementIcon.png");
    managementIcon = managementIcon.pixmap(QSize(80, 80));  // Set the desired icon size here
    managementButton->setIcon(managementIcon);
    simulationButton = new QPushButton("Viewer");
    QIcon simulationIcon(":/images/SimulationIcon.png");
    simulationIcon = simulationIcon.pixmap(QSize(80, 80));  // Set the desired icon size here
    simulationButton->setIcon(simulationIcon);
    QFrame* navFrame = new QFrame;
    navFrame->setFrameShape(QFrame::StyledPanel);
    navFrame->setFrameShadow(QFrame::Raised);
    navFrame->setStyleSheet("QFrame {"
                            "border: 1px solid #ccc;"
                            "border-radius: 8px;"
                            "padding: 5px;"
                            "}"
                );



    navLayout->addWidget(productButton);
    navLayout->addWidget(managementButton);
    navLayout->addWidget(simulationButton);
    navFrame->setLayout(navLayout);

    int buttonWidth = 350; // Set the desired width for the buttons

    productButton->setFixedWidth(buttonWidth);
    managementButton->setFixedWidth(buttonWidth);
    simulationButton->setFixedWidth(buttonWidth);

//    navLayout->addLayout(buttonsLayout);
//    navLayout->addStretch();

    //connect the button signal to their slots

    connect(productButton,&QPushButton::clicked,this,&SimulatorSuite::onProductButtonClicked);
    connect(managementButton,&QPushButton::clicked,this,&SimulatorSuite::onManagementButtonClicked);
    connect(simulationButton,&QPushButton::clicked,this,&SimulatorSuite::onSimulationButtonClicked);

    // Add the header layout and navigation layout to the main layout


    mainLayout->addLayout(headerLayout);
//    mainLayout->addLayout(navLayout);
    mainLayout->addWidget(navFrame);



    // Set the style sheet for the buttons
    productButton->setStyleSheet("QPushButton {"
                                 "background-color:  rgba(36,101,175,190);"  // Set the background color as transparent
                                 "border: 1px solid #ccc;"
                                 "color: white;"  // Set the text color to white
                                 "padding: 12px 24px;"
                                 "text-align: center;"
                                 "text-decoration: none;"
                                 "display: inline-block;"
                                 "font-size: 18px;"
                                 "font-family: Arial Light;"
                                "icon-size: 48px 48px;"
                                 "margin: 4px 2px;"
                                 "cursor: pointer;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color: rgba(36,101,175,210);"  // Set a hover background color with slight gray transparency
                                 "}"
                                 "QPushButton:pressed {"
                                 "background-color: rgba(36,101,175,250);"  // Set a pressed background color with slight gray transparency
                               "}");

    managementButton->setStyleSheet("QPushButton {"
                                    "background-color:  rgba(36,101,175,190);"  // Set the background color as transparent
                                    "border: 1px solid #ccc;"
                                    "color: white;"  // Set the text color to white
                                    "padding: 12px 24px;"
                                    "text-align: center;"
                                    "text-decoration: none;"
                                    "display: inline-block;"
                                    "font-size: 18px;"
                                    "font-family: Arial Light;"
                                    "icon-size: 48px 48px;"


                                    "margin: 4px 2px;"
                                    "cursor: pointer;"
                                    "}"
                                    "QPushButton:hover {"
                                    "background-color: rgba(36,101,175,210);"  // Set a hover background color with slight gray transparency
                                    "}"
                                    "QPushButton:pressed {"
                                    "background-color: rgba(36,101,175,250);"  // Set a pressed background color with slight gray transparency
                                   "}");

    simulationButton->setStyleSheet("QPushButton {"
                                    "background-color:  rgba(36,101,175,190);"  // Set the background color as transparent
                                    "border: 1px solid #ccc;"
                                    "color: white;"  // Set the text color to white
                                    "padding: 12px 24px;"
                                    "text-align: center;"
                                    "text-decoration: none;"
                                    "display: inline-block;"
                                    "font-size: 18px;"
                                    "font-family: Arial Light;"
                                    "icon-size: 48px 48px;"


                                    "margin: 4px 2px;"
                                    "cursor: pointer;"
                                    "}"
                                    "QPushButton:hover {"
                                    "background-color: rgba(36,101,175,210);"  // Set a hover background color with slight gray transparency
                                    "}"
                                    "QPushButton:pressed {"
                                    "background-color: rgba(36,101,175,250);"  // Set a pressed background color with slight gray transparency
                                    "}");



    //create content widgets

    productsContent = new Products;
    managementContent = new Management;
    simulationContent = new Simulation;




}

void SimulatorSuite::displayContent(QWidget *contentWidget){
    // Check if the content widget is already being displayed
    if (currentContentWidget == contentWidget)
        return;

        // Clear layout
        if (currentContentWidget != nullptr) {
            centralWidget->layout()->removeWidget(currentContentWidget);
            currentContentWidget->setParent(nullptr);  // Détacher le widget du layout pour éviter les erreurs de suppression


        }

        // Set the new content widget
        currentContentWidget = contentWidget;
        centralWidget->layout()->addWidget(currentContentWidget);
}

void SimulatorSuite::onProductButtonClicked()
{


    displayContent(productsContent);

}

void SimulatorSuite::onManagementButtonClicked()
{
    displayContent(managementContent);
}

void SimulatorSuite::onSimulationButtonClicked()
{
    displayContent(simulationContent);
}






