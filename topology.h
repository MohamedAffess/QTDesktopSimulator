#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPointF>
#include <QVector>
#include <QPainter>
#include <QVBoxLayout>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QGraphicsTextItem>
#include<QGraphicsEllipseItem>
#include <QPushButton>
#include<QEvent>

class Topology : public QWidget
{
    Q_OBJECT

public:
    explicit Topology(QWidget *parent = nullptr);
    ~Topology();

private slots:
    void onToggleLabelsClicked();



private:
    QVBoxLayout *topologyLayout;
    QVBoxLayout* drawLayout;
    QHBoxLayout* buttonsLayout;
    QGraphicsView* topologyView;
    QGraphicsScene* topologyScene;
    QPushButton *refreshButton;
    QPushButton* toggleLabelsButton;



     QGraphicsLineItem* linkItem ;
     QGraphicsTextItem* linkLabel;
     QGraphicsPixmapItem* nodeItem;
     QGraphicsTextItem* nodeNameItem ;
    QGraphicsTextItem* nodeTypeItem ;
    QGraphicsPixmapItem* selectedNodeItem;
    QGraphicsLineItem* selectedLinkItem;
    //graphical items
//    QGraphicsPixmapItem* nodeItem;
//    QGraphicsTextItem* nodeNameItem;
//    QGraphicsTextItem* nodeTypeItem ;
//    QGraphicsItem* linkItem ;
//    QGraphicsLineItem* wiredLinkItem ;
//    QGraphicsTextItem* wiredLabelItem ;
//    QGraphicsLineItem* wirelessLinkItem ;
//    QGraphicsTextItem* wifiLabelItem ;
//    QGraphicsLineItem* olsrLinkItem;
//    QGraphicsTextItem* olsrLabelItem ;




    bool showLabels;




//    nodes.insert(nodeId, qMakePair(nodePosition, qMakePair(nodeName, nodeType)));

//DATA STORING :

    QMap<QString, QPair<QGraphicsTextItem*, QGraphicsTextItem*>> nodeLabels;

    QMap<QString , QPair<QPointF, QPair<QString, QString>>> nodes;  //QMap<node_id,pair(nodePosition, pair(nodeName, nodeType))>
//    QMap<QPair<QString, QString>, QPointF> interfacePositionMap;//QMap<Pair(nodeId, interfaceId), interfacePosition>;
//    QMap<QString, QString> interfaceNodeMap;

    struct LinkData {
        QString interfaceFrom;
        QString interfaceTo;
        QString linkPhysicType;
        int linkType;
    };
    QVector<QPair<QPair<QString, QString>, LinkData>> links; //QVector<Pair(Pair(nodeFrom, nodeTo), LinkData{ interfaceFrom, interfaceTo, linkPhysicType, linkType })>;
//    QVector<QPair<QString, QString>> interfaces; //interface_id, node_id

//    QVector<QPair<QPair<QString, QString>, QPair<QString, QString>>> olsrLinks;


    void retrieveData();

    void drawTopology();
    void setupUI();

    bool eventFilter(QObject* object, QEvent* event);





public:
    qreal generateRandomNumber(qreal min, qreal max);
    QPointF generateArbitraryCoordinates(const QMap<QString, QPair<QPointF, QPair<QString, QString>>>& nodes);
    void toggleLabels();



};

#endif // TOPOLOGY_H
