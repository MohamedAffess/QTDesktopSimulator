#include "topology.h"
#include <QDebug>
#include <QUrl>
#include <QEventLoop>
#include <QLabel>
#include <cmath>
#include <QtMath>
#include <QMessageBox>

#include <QWheelEvent> // Add this line to include the QWheelEvent header

Topology::Topology(QWidget *parent) : QWidget(parent)
{
    setupUI();
    retrieveData();




}

Topology::~Topology()
{
    delete topologyLayout;
    delete drawLayout;
    delete topologyView;
    delete topologyScene;
    delete refreshButton;
    delete linkItem;
    delete linkLabel;
    delete nodeItem;
    delete nodeNameItem;
    delete nodeTypeItem;

    //    delete nodeItem;
    //    delete nodeNameItem;
    //    delete nodeTypeItem;
    //    delete linkItem;
    //    delete wiredLinkItem;
    //    delete wiredLabelItem;
    //    delete wirelessLinkItem;
    //    delete wifiLabelItem;
    //    delete olsrLinkItem;
    //    delete olsrLabelItem;



}
qreal Topology::generateRandomNumber(qreal min, qreal max)
{
    qreal randomNumber = ((qreal)qrand() / RAND_MAX) * (max - min) + min;
    return randomNumber;
}
//QPointF Topology::generateArbitraryCoordinates(const QMap<QString, QPair<QPointF, QPair<QString, QString>>>& nodes, qreal minDistance)
//{
//    qreal minX, maxX, minY, maxY;

//    if (nodes.size() > 10) {
//        minX = topologyView->viewport()->rect().left();
//        maxX = topologyView->viewport()->rect().right();
//        minY = topologyView->viewport()->rect().top();
//        maxY = topologyView->viewport()->rect().bottom();
//    } else {
//        minX = topologyView->viewport()->rect().left() + 100;   // Adjust the padding as needed
//        maxX = topologyView->viewport()->rect().right() - 100;  // Adjust the padding as needed
//        minY = topologyView->viewport()->rect().top() + 100;    // Adjust the padding as needed
//        maxY = topologyView->viewport()->rect().bottom() - 100; // Adjust the padding as needed
//    }

//    qreal x, y;
//    bool validCoordinates = false;

//    while (!validCoordinates) {
//        x = generateRandomNumber(minX, maxX);
//        y = generateRandomNumber(minY, maxY);

//        validCoordinates = true;

//        // Check the distance from existing nodes
//        for (const auto& nodeData : nodes) {
//            const QPointF& nodePosition = nodeData.first;
//            qreal distance = QLineF(x, y, nodePosition.x(), nodePosition.y()).length();
//            if (distance < minDistance) {
//                validCoordinates = false;
//                break;
//            }
//        }
//    }

//    return QPointF(x, y);
//}

QPointF Topology::generateArbitraryCoordinates(const QMap<QString, QPair<QPointF, QPair<QString, QString>>>& nodes)
{
    qreal minX, maxX, minY, maxY;

    if (nodes.size() > 10) {
        minX = topologyView->viewport()->rect().left();
        maxX = topologyView->viewport()->rect().right();
        minY = topologyView->viewport()->rect().top();
        maxY = topologyView->viewport()->rect().bottom();
    } else {
        minX = topologyView->viewport()->rect().left() + 100;   // Adjust the padding as needed
        maxX = topologyView->viewport()->rect().right() - 100;  // Adjust the padding as needed
        minY = topologyView->viewport()->rect().top() + 100;    // Adjust the padding as needed
        maxY = topologyView->viewport()->rect().bottom() - 100; // Adjust the padding as needed
    }

    qreal x, y;
    bool validCoordinates = false;

    int nodeCount = nodes.size();
    qreal minDistance = 200 / sqrt(nodeCount);  // Calculate minDistance based on node count

    while (!validCoordinates) {
        x = generateRandomNumber(minX, maxX);
        y = generateRandomNumber(minY, maxY);

        validCoordinates = true;

        // Check the distance from existing nodes
        for (const auto& nodeData : nodes) {
            const QPointF& nodePosition = nodeData.first;
            qreal distance = QLineF(x, y, nodePosition.x(), nodePosition.y()).length();
            if (distance < minDistance) {
                validCoordinates = false;
                break;
            }
        }
    }

    return QPointF(x, y);
}

void Topology::retrieveData()
{
    // Clear previous elements
    nodes.clear();
    links.clear();
    //    interfaceNodeMap.clear();
    //    interfacePositionMap.clear();
    //    interfaces.clear();

    QNetworkAccessManager networkAccessManager;
    QNetworkRequest request(QUrl("http://localhost:3200/wms/mesh/topology"));

    QNetworkReply *reply = networkAccessManager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray jsonData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = jsonDoc.object();

        QJsonArray nodeList = jsonObject["nodes_list"].toArray();



        for (const QJsonValue& nodeValue : nodeList) {
            QJsonObject nodeObject = nodeValue.toObject();
            QString nodeId = nodeObject["node_id"].toString();
            QPointF nodePosition = generateArbitraryCoordinates(nodes);
            QString nodeName = nodeObject["node_name"].toString();
            QString nodeType = nodeObject["node_type"].toString();
            nodes.insert(nodeId, qMakePair(nodePosition, qMakePair(nodeName, nodeType)));
        }

        //        QJsonArray interfaceList = jsonObject["interfaces_list"].toArray();
        //        for (const QJsonValue& interfaceValue : interfaceList) {
        //            QJsonObject interfaceObject = interfaceValue.toObject();
        //            QString interfaceId = interfaceObject["interface_id"].toString();
        //            QString nodeId = interfaceObject["node_id"].toString();
        //            interfaceNodeMap.insert(interfaceId, nodeId);
        //}
        ////            QPointF interfacePosition = generateArbitraryCoordinates(nodes, 80);
        //            QPointF interfacePosition(0, 0);
        //            interfacePositionMap.insert(qMakePair(nodeId, interfaceId), interfacePosition);
        //        }

        QJsonArray linkList = jsonObject["links_list"].toArray();
        for (const QJsonValue& linkValue : linkList) {
            QJsonObject linkObject = linkValue.toObject();
            QString interfaceFrom = linkObject["interface_from"].toString();
            QString interfaceTo = linkObject["interface_to"].toString();
            QString linkPhysicType = linkObject["link_physic_type"].toString();
            int linkType = linkObject["link_type"].toInt();

            QString nodeFrom = linkObject["node_id_from"].toString();
            QString nodeTo = linkObject["node_id_to"].toString();

            if (!nodeFrom.isEmpty() && !nodeTo.isEmpty()) {
                links.append(qMakePair(qMakePair(nodeFrom, nodeTo), LinkData{ interfaceFrom, interfaceTo, linkPhysicType, linkType }));
            }

        }


        drawTopology();
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to retrieve information " + reply->errorString());
    }

    reply->deleteLater();
}









void Topology::drawTopology()
{
    // Clear the scene and view
    topologyScene->clear();
    topologyView->clearMask();

    // Set the scene for the QGraphicsView

    // Calculate the size of nodes and labels based on the number of nodes
    int nodeSizeFactor = qMax(nodes.size(), 1);  // Avoid division by zero
    QSize nodeSize(40 , 40);
    QFont nodeNameFont("Arial", 13 - qMin(nodeSizeFactor, 6), QFont::Normal);
    QFont linkLabelFont("Arial", 13 - qMin(nodeSizeFactor, 6), QFont::Normal);

    // Draw nodes
    QPixmap nodePixmap(":/images/node_icon.png");
    QPixmap scaledNodePixmap = nodePixmap.scaled(nodeSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    for (const QString& nodeId : nodes.keys()) {
        QPair<QPointF, QPair<QString, QString>> nodeData = nodes.value(nodeId);
        QPointF nodePosition = nodeData.first;
        QString nodeName = nodeData.second.first;
        QString nodeType = nodeData.second.second;

        nodeItem = new QGraphicsPixmapItem();
        nodeItem->setPixmap(scaledNodePixmap);
        nodeItem->setPos(nodePosition);
        topologyScene->addItem(nodeItem);

        nodeNameItem = new QGraphicsTextItem(nodeName);
        nodeNameItem->setFont(nodeNameFont);
        nodeNameItem->setPos(nodePosition.x(), nodePosition.y() - nodeSize.height() +5);
        topologyScene->addItem(nodeNameItem);

        nodeTypeItem = new QGraphicsTextItem(nodeType);
        nodeTypeItem->setFont(nodeNameFont);
        nodeTypeItem->setPos(nodePosition.x(), nodePosition.y() + nodeSize.height()+10);
        topologyScene->addItem(nodeTypeItem);
        nodeNameItem->setVisible(true);
        nodeTypeItem->setVisible(showLabels);
    }

    // Draw links
    QMap<QGraphicsLineItem*, bool> linkLabelVisibility;

    for (const QPair<QPair<QString, QString>, LinkData>& link : links) {
        QPair<QString, QString> nodePair = link.first;
        QString nodeFrom = nodePair.first;
        QString nodeTo = nodePair.second;
        LinkData linkData = link.second;

        QPointF nodeFromPosition = nodes.value(nodeFrom).first;
        QPointF nodeToPosition = nodes.value(nodeTo).first;

        QPointF startPoint = nodeFromPosition + QPointF(nodeSize.width() / 2, nodeSize.height() / 2);
        QPointF endPoint = nodeToPosition + QPointF(nodeSize.width() / 2, nodeSize.height() / 2);

        linkItem = new QGraphicsLineItem(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y());
        linkLabel = new QGraphicsTextItem;
        linkLabel->setVisible(showLabels);


        if (linkData.linkPhysicType == "Wired") {
            QPen wiredPen(Qt::gray, 2);
            linkItem->setPen(wiredPen);
            linkLabel->setPlainText("Wired");
            linkLabel->setDefaultTextColor(Qt::gray);
            linkLabel->setFont(linkLabelFont);

            QPointF wiredLabelPosition = (startPoint + endPoint) / 2;
            wiredLabelPosition.setY(wiredLabelPosition.y()+10);
            linkLabel->setPos(wiredLabelPosition);



        }
        else if (linkData.linkPhysicType == "Wireless") {
            if (linkData.linkType == 2) {
                QVector<qreal> dashPattern;
                dashPattern << 6 << 4;
                QPen wirelessPen(Qt::blue, 2);
                wirelessPen.setDashPattern(dashPattern);
                linkItem->setPen(wirelessPen);
                linkLabel->setPlainText("WIFI");
                linkLabel->setDefaultTextColor(Qt::blue);
                linkLabel->setFont(linkLabelFont);
                QPointF labelPosition = (startPoint + endPoint) / 2;
                labelPosition.setY(labelPosition.y() - 10); // Adjust Y position

                linkLabel->setPos(labelPosition);
            }
            else if (linkData.linkType == 3) {
                QVector<qreal> dashPattern;
                dashPattern << 2 << 2;
                QPen olsrPen(Qt::green, 2);
                olsrPen.setDashPattern(dashPattern);
                linkItem->setPen(olsrPen);
                linkLabel->setPlainText("OLSR");
                linkLabel->setDefaultTextColor(Qt::green);
                linkLabel->setFont(linkLabelFont);
                QPointF labelPosition = (startPoint + endPoint) / 2;
                labelPosition.setY(labelPosition.y() - 10); // Adjust Y position

                linkLabel->setPos(labelPosition);
            }
        }

        topologyScene->addItem(linkItem);
        topologyScene->addItem(linkLabel);
    }

    // Update the layout to reflect the changes
    update();
}
bool Topology::eventFilter(QObject* object, QEvent* event)
{
    if (object == topologyView && event->type() == QEvent::Wheel) {
        // Handle mouse wheel event for zooming
        QWheelEvent* wheelEvent = dynamic_cast<QWheelEvent*>(event);
        if (wheelEvent) {
            QPoint delta = wheelEvent->angleDelta();
            qreal scaleFactor = qPow(1.2, delta.y() / 120.0);  // Adjust the zoom factor as desired

            // Apply the zoom factor
            topologyView->scale(scaleFactor, scaleFactor);
            return true;  // Event handled
        }
    }

    return false;  // Event not handled, pass it to the base class
}

void Topology::onToggleLabelsClicked()
{
    showLabels = !showLabels;
    drawTopology();
}
void Topology::setupUI()
{
    topologyLayout = new QVBoxLayout(this);
    setLayout(topologyLayout);

    topologyView = new QGraphicsView;
    topologyView->setRenderHint(QPainter::Antialiasing);
    topologyView->setInteractive(true);
    topologyView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    topologyView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        topologyView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        topologyView->installEventFilter(this);  // Install event filter to handle scroll events

    topologyScene = new QGraphicsScene;
    topologyLayout->addWidget(topologyView);
    topologyView->setScene(topologyScene);

    showLabels = true; // Show labels by default
    toggleLabelsButton = new QPushButton("Hide/Show Labels", this);


    buttonsLayout= new QHBoxLayout;
    refreshButton = new QPushButton("Refresh",this);
    buttonsLayout->addWidget(refreshButton);
    buttonsLayout->addWidget(toggleLabelsButton);
    topologyLayout->addLayout(buttonsLayout);
    // Connect the clicked signal of the refreshButton to the drawTopology() function
    connect(refreshButton, &QPushButton::clicked, this, &Topology::retrieveData);
    connect(toggleLabelsButton, &QPushButton::clicked, this, &Topology::onToggleLabelsClicked);

    refreshButton->setStyleSheet(
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

    refreshButton->setFixedWidth(100);
    QIcon returnIcon(":/images/returnIcon.png");
    returnIcon = returnIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    refreshButton->setIcon(returnIcon);

    toggleLabelsButton->setStyleSheet(
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

    toggleLabelsButton->setFixedWidth(150);
    QIcon toggleIcon(":/images/toggleButton.png");
    toggleIcon = toggleIcon.pixmap(QSize(32, 32));  // Set the desired icon size here
    toggleLabelsButton->setIcon(toggleIcon);










}



//void Topology::drawTopology()
//{
//    // Clear the scene and view
//    topologyScene->clear();
//    topologyView->clearMask();

//    // Set the scene for the QGraphicsView

//    // Calculate node size and label size based on node count
//    int nodeCount = nodes.size();
//    qreal nodeSize = 100.0 - nodeCount * 2.0;     // Adjust the calculation as needed
//    qreal labelSize = 8.0 + nodeCount * 0.5;       // Adjust the calculation as needed

//    // Draw nodes
//    QPixmap nodePixmap(":/images/node_icon.png");
//    for (const QString& nodeId : nodes.keys()) {
//        QPair<QPointF, QPair<QString, QString>> nodeData = nodes.value(nodeId);
//        QPointF nodePosition = nodeData.first;
//        QString nodeName = nodeData.second.first;
//        QString nodeType = nodeData.second.second;

//        // Adjust the node size
//        qreal halfNodeSize = nodeSize / 2.0;
//        QRectF nodeRect(-halfNodeSize, -halfNodeSize, nodeSize, nodeSize);

//        // Create and add the node item
//        nodeItem = new QGraphicsPixmapItem(nodePixmap);
//        nodeItem->setPos(nodePosition);
//        nodeItem->setOffset(-nodePixmap.width() / 2, -nodePixmap.height() / 2);
//        nodeItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
//        nodeItem->setTransformOriginPoint(0, 0);
//        nodeItem->setScale(nodeSize / nodePixmap.width());
//        topologyScene->addItem(nodeItem);


//        // Create and add the node name label
//        nodeNameItem = new QGraphicsTextItem(nodeName);
//        nodeNameItem->setPos(nodePosition.x(), nodePosition.y() - halfNodeSize - labelSize - 2.0);  // Adjusted position
//        nodeNameItem->setDefaultTextColor(Qt::black);
//        nodeNameItem->setFont(QFont("Arial", labelSize, QFont::Bold));
//        nodeNameItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
//        topologyScene->addItem(nodeNameItem);

//        // Create and add the node type label
//        nodeTypeItem = new QGraphicsTextItem(nodeType);
//        nodeTypeItem->setPos(nodePosition.x(), nodePosition.y() + halfNodeSize + 2.0);  // Adjusted position
//        nodeTypeItem->setDefaultTextColor(Qt::black);
//        nodeTypeItem->setFont(QFont("Arial", labelSize, QFont::Bold));
//        nodeTypeItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
//        topologyScene->addItem(nodeTypeItem);
//    }

//    // Draw links
//    for (const QPair<QPair<QString, QString>, LinkData>& link : links) {
//        QPair<QString, QString> nodePair = link.first;
//        QString nodeFrom = nodePair.first;
//        QString nodeTo = nodePair.second;
//        LinkData linkData = link.second;

//        QPointF nodeFromPosition = nodes.value(nodeFrom).first;
//        QPointF nodeToPosition = nodes.value(nodeTo).first;

//        // Calculate the link start and end points based on the node size
//        QPointF startPoint = nodeFromPosition + QPointF(0.0, -nodeSize / 2.0); // Adjusted point
//        QPointF endPoint = nodeToPosition + QPointF(0.0, -nodeSize / 2.0);     // Adjusted point

//        linkItem = new QGraphicsLineItem(startPoint.x(), startPoint.y(), endPoint.x(),endPoint.y());
//        linkLabel = new QGraphicsTextItem();

//        if (linkData.linkPhysicType == "Wired") {
//            QPen wiredPen(Qt::gray, 1);
//            linkItem->setPen(wiredPen);
//            linkLabel->setPlainText("Wired");
//            linkLabel->setDefaultTextColor(Qt::gray);
//            QPointF wiredLabelPosition = (startPoint + endPoint) / 2;
//            linkLabel->setPos(wiredLabelPosition);
//        }
//        else if (linkData.linkPhysicType == "Wireless") {
//            if (linkData.linkType == 2) {
//                QVector<qreal> dashPattern;
//                dashPattern << 6 << 4;
//                QPen wirelessPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
//                wirelessPen.setDashPattern(dashPattern);
//                linkItem->setPen(wirelessPen);
//                linkLabel->setPlainText("WIFI");
//                linkLabel->setDefaultTextColor(Qt::blue);
//                linkLabel->setFont(QFont("Arial", labelSize, QFont::Bold));
//                QPointF labelPosition = (startPoint + endPoint) / 2;
//                linkLabel->setPos(labelPosition);
//            }
//            else if (linkData.linkType == 3) {
//                QVector<qreal> dashPattern;
//                dashPattern << 2 << 2;
//                QPen olsrPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
//                olsrPen.setDashPattern(dashPattern);
//                linkItem->setPen(olsrPen);
//                linkLabel->setPlainText("OLSR");
//                linkLabel->setDefaultTextColor(Qt::green);
//                linkLabel->setFont(QFont("Arial", labelSize, QFont::Bold));
//                QPointF labelPosition = (startPoint + endPoint) / 2;
//                linkLabel->setPos(labelPosition);
//            }
//        }

//        topologyScene->addItem(linkItem);
//        topologyScene->addItem(linkLabel);
//    }

//    // Update the layout to reflect the changes
//    update();
//}

//QPointF startPoint = nodeFromPosition + QPointF(0.0, -nodeSize / 2.0); // Adjusted point
//QPointF endPoint = nodeToPosition + QPointF(0.0, -nodeSize / 2.0);     // Adjusted point

//void Topology::drawTopology()
//{
//    // Clear previous elements
//    nodes.clear();
//    links.clear();
//    interfacePositionMap.clear();
//    interfaces.clear();
////    olsrLinks.clear();

//    topologyScene->clear();
//    topologyView->clearMask();

//    QNetworkAccessManager networkAccessManager;
//    QNetworkRequest request(QUrl("http://localhost:3200/wms/mesh/topology"));

//    QNetworkReply *reply = networkAccessManager.get(request);
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();

//    if (reply->error() == QNetworkReply::NoError) {
//        QByteArray jsonData = reply->readAll();
//        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
//        QJsonObject jsonObject = jsonDoc.object();

//        QJsonArray nodeList = jsonObject["nodes_list"].toArray();
//        for (const QJsonValue& nodeValue : nodeList) {
//            QJsonObject nodeObject = nodeValue.toObject();
//            QString nodeId = nodeObject["node_id"].toString();
//            QPointF nodePosition = generateArbitraryCoordinates(nodes, 120); // NODE POSITIONS (can be handled by a move function )
//            QString nodeName = nodeObject["node_name"].toString();
//            QString nodeType = nodeObject["node_type"].toString();
//            nodes.insert(nodeId, qMakePair(nodePosition, qMakePair(nodeName, nodeType)));
//        }


//        QJsonArray interfaceList = jsonObject["interfaces_list"].toArray();
//        QMap<QString, QString> interfaceNodeMap;
//        for (const QJsonValue& interfaceValue : interfaceList) {
//            QJsonObject interfaceObject = interfaceValue.toObject();
//            QString interfaceId = interfaceObject["interface_id"].toString();
//            QString nodeId = interfaceObject["node_id"].toString();
//            interfaceNodeMap.insert(interfaceId, nodeId);

//            QPointF interfacePosition = generateArbitraryCoordinates(nodes, 80);
//            interfacePositionMap.insert(qMakePair(nodeId, interfaceId), interfacePosition);// INTERFACES POSITIONS (can be handled by a move function )
//        }

//        QJsonArray linkList = jsonObject["links_list"].toArray();
//        for (const QJsonValue& linkValue : linkList) {
//            QJsonObject linkObject = linkValue.toObject();
//            QString interfaceFrom = linkObject["interface_from"].toString();
//            QString interfaceTo = linkObject["interface_to"].toString();
//            QString linkPhysicType = linkObject["link_physic_type"].toString();
//            int linkType = linkObject["link_type"].toInt();

//            QString nodeFrom = interfaceNodeMap.value(interfaceFrom);
//            QString nodeTo = interfaceNodeMap.value(interfaceTo);

//            if (!nodeFrom.isEmpty() && !nodeTo.isEmpty()) {
//                links.append(qMakePair(qMakePair(nodeFrom, nodeTo), LinkData{ interfaceFrom, interfaceTo, linkPhysicType, linkType }));
//            }
//        }

//        // Set the scene for the QGraphicsView
//        topologyView->setScene(topologyScene);


//        // Example: Drawing nodes as icons
//        for (const QString& nodeId : nodes.keys()) {
//            QPair<QPointF, QPair<QString, QString>> nodeData = nodes.value(nodeId);
//            QPointF nodePosition = nodeData.first;  //HANDLE THIS POSITION BY ANOTHER FUNCTION TO DRAG AND MOVE THE NODE
//            QString nodeName = nodeData.second.first;
//            QString nodeType = nodeData.second.second;

//            // Create a QGraphicsPixmapItem for each node using an icon or image
//            QGraphicsPixmapItem* nodeItem=nodeItem = new QGraphicsPixmapItem(); ;
//             //THIS ITEM can be moved by another function
//            QPixmap nodePixmap(":/images/node_icon.png"); // Replace with the path to your icon or image
//            // Set the desired size of the node icon
//            QSize nodeSize(50, 50); // Adjust the size as needed
//            QPixmap scaledNodePixmap = nodePixmap.scaled(nodeSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//            nodeItem->setPixmap(scaledNodePixmap);
//            nodeItem->setPos(nodePosition);
//            topologyScene->addItem(nodeItem);

//            // Add node name and type as labels
//           QGraphicsTextItem*nodeNameItem = new QGraphicsTextItem(nodeName); //THIS ITEM
//            nodeNameItem->setPos(nodePosition.x(), nodePosition.y() - 20); //this
//            topologyScene->addItem(nodeNameItem);


//           QGraphicsTextItem* nodeTypeItem = new QGraphicsTextItem(nodeType);
//            nodeTypeItem->setPos(nodePosition.x(), nodePosition.y() + 60);
//            topologyScene->addItem(nodeTypeItem);

//        }

//        // Example: Drawing links with different styles based on link_physic_type and link_type
//        for (const QPair<QPair<QString, QString>, LinkData>& link : links) {
//            QPair<QString, QString> nodePair = link.first;
//            QString nodeFrom = nodePair.first;
//            QString nodeTo = nodePair.second;
//            LinkData linkData = link.second;

//            // Retrieve the positions of the nodes
//            QPointF nodeFromPosition = nodes.value(nodeFrom).first;  //HANDLE THIS POSITION BY ANOTHER FUNCTION TO DRAG AND MOVE THE NODE
//            QPointF nodeToPosition = nodes.value(nodeTo).first;  //HANDLE THIS POSITION BY ANOTHER FUNCTION TO DRAG AND MOVE THE NODE


//           //HANDLE THIS POSITION BY ANOTHER FUNCTION TO DRAG AND MOVE THE NODE
//              //HANDLE THIS POSITION BY ANOTHER FUNCTION TO DRAG AND MOVE THE NODE

//            // Calculate the start and end points for the link line
//            QPointF startPoint =nodeFromPosition + QPointF(40, 40);
//            QPointF endPoint = nodeToPosition + QPointF(40, 40);

//            // Create a QGraphicsItem for the link with the determined style
//            QGraphicsItem* linkItem = nullptr; //this item must be moved by the moving function
//            if (linkData.linkPhysicType == "Wired") {
//                   // Straight line for wired links with gray color
//                 QGraphicsLineItem*  wiredLinkItem = new QGraphicsLineItem(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y());//this item must be moved by the moving function
//                   QPen wiredPen(Qt::gray, 1);
//                   wiredLinkItem->setPen(wiredPen);
//                   linkItem = wiredLinkItem;

//                   // Add label for wired link
//                  QGraphicsTextItem* wiredLabelItem = new QGraphicsTextItem("Wired");//this item must be moved by the moving function
//                   wiredLabelItem->setDefaultTextColor(Qt::gray);//this item must be moved by the moving function
//                   QPointF wiredLabelPosition = (startPoint + endPoint) / 2;
//                   wiredLabelItem->setPos(wiredLabelPosition);
//                   topologyScene->addItem(wiredLabelItem);
//               } else if (linkData.linkPhysicType == "Wireless") {
//                   if (linkData.linkType == 2) {
//                       // Custom line style for WIFI (Wireless) links with blue color and bold
//                       QVector<qreal> dashPattern;
//                       dashPattern << 6 << 4; // Adjust the dash pattern as desired
//                       QPen wirelessPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
//                       wirelessPen.setDashPattern(dashPattern);
//                   QGraphicsLineItem*    wirelessLinkItem = new QGraphicsLineItem(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y());//this item must be moved by the moving function
//                       wirelessLinkItem->setPen(wirelessPen);
//                       linkItem = wirelessLinkItem;//this item must be moved by the moving function

//                       // Add label for WIFI link
//                   QGraphicsTextItem*   wifiLabelItem = new QGraphicsTextItem("WiFi");//this item must be moved by the moving function
//                       wifiLabelItem->setDefaultTextColor(Qt::blue);
//                       wifiLabelItem->setFont(QFont("Arial", 8, QFont::Bold));
//                       QPointF labelPosition = (startPoint + endPoint) / 2;
//                       wifiLabelItem->setPos(labelPosition);
//                       topologyScene->addItem(wifiLabelItem);
//                   } else if (linkData.linkType == 3) {
//                       // Custom line style for OLSR links with green color and bold
//                       QVector<qreal> dashPattern;
//                       dashPattern << 2 << 2; // Adjust the dash pattern as desired
//                       QPen olsrPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
//                       olsrPen.setDashPattern(dashPattern);
//                      QGraphicsLineItem* olsrLinkItem = new QGraphicsLineItem(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y());//this item must be moved by the moving function
//                       olsrLinkItem->setPen(olsrPen);
//                       linkItem = olsrLinkItem;//this item must be moved by the moving function

//                       // Add label for OLSR link
//               QGraphicsTextItem*        olsrLabelItem = new QGraphicsTextItem("OLSR");
//                       olsrLabelItem->setDefaultTextColor(Qt::green);
//                       olsrLabelItem->setFont(QFont("Arial", 8, QFont::Bold));
//                       QPointF labelPosition = (startPoint + endPoint) / 2;
//                       olsrLabelItem->setPos(labelPosition);
//                       topologyScene->addItem(olsrLabelItem);
//                   }
//               }


//            if (linkItem) {
//                topologyScene->addItem(linkItem);

////                // Debug line to print the link data
////                qDebug() << "Link Data:"
////                         << "Interface From:" << linkData.interfaceFrom
////                         << "Interface To:" << linkData.interfaceTo
////                         << "Link Physic Type:" << linkData.linkPhysicType
////                         << "Link Type:" << linkData.linkType;
//            }
//        }

//        // Update the layout to reflect the changes
//        update();
//    }
//    else {
//        QMessageBox::critical(this, "Error", "Failed to retrieve informations " + reply->errorString());
//    }

//    reply->deleteLater();
//}

