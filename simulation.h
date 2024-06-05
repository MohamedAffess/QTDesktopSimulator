#ifndef SIMULATION_H
#define SIMULATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include "topology.h"
#include "snrmeasures.h"
#include "trafic.h"
#include<QLabel>
#include<QToolButton>
class Simulation : public QWidget
{
    Q_OBJECT

public:
    explicit Simulation(QWidget *parent = nullptr);
    ~Simulation();

private slots :

    void onTopologyButtonClicked();
    void onTraficButtonClicked();
    void onSnrMeasuresButtonClicked();
    void onPublishDataToMQTTButtonClicked();
    void toggleTimer();


private :
    QVBoxLayout *simulationLayout;
    QPushButton *topologyButton;
    QPushButton *TraficButton;
    QPushButton *SnrMeasuresButton;
    QToolButton* simulationMenuButton;
    QPushButton* publishToMQTTButton;
    QHBoxLayout* contentHeader;
    QStackedWidget *contentStack;
    QLabel *titleLabel;
    QMenu* simulationMenu;
    QAction* topologyAction ;
    QAction* traficAction;
    QAction* snrMeasuresAction;
    Topology *topologyContent;
    Trafic *traficContent;
    SnrMeasures *snrMeasuresContent;
    QTimer* MQTTDataTimer;


    void setupUI();

};

#endif // SIMULATION_H
