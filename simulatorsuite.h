#ifndef SIMULATORSUITE_H
#define SIMULATORSUITE_H

#include <QMainWindow>
#include <QPushButton>
#include "products.h"
#include "management.h"
#include "simulation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SimulatorSuite; }
QT_END_NAMESPACE


class SimulatorSuite : public QMainWindow
{
    Q_OBJECT

public:
    SimulatorSuite(QWidget *parent = nullptr);
    ~SimulatorSuite();
private slots:
    void onProductButtonClicked();
    void onManagementButtonClicked();
    void onSimulationButtonClicked();


private:
    Ui::SimulatorSuite *ui;
    void setupUI();
    void displayContent(QWidget *contentWidget);

    //main
    QVBoxLayout *mainLayout;
    QWidget *centralWidget;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;
    QLabel *logoLabel;
    QHBoxLayout *navLayout ;



    //Buttons
    QPushButton *productButton;
    QPushButton *managementButton;
    QPushButton *simulationButton;

    //Content

    Products *productsContent;
    Management *managementContent;
    Simulation *simulationContent;

//current content
     QWidget *currentContentWidget = nullptr;

};
#endif // SIMULATORSUITE_H
