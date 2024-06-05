#ifndef TRAFIC_H
#define TRAFIC_H
#include <QDockWidget>
#include <QStackedWidget>

#include <QWidget>
#include<QComboBox>
#include<QHBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QChartView>
#include <QPushButton>
#include <QFormLayout>
class Trafic : public QWidget
{
    Q_OBJECT

public:
    explicit Trafic(QWidget *parent = nullptr);
    ~Trafic();


private slots:
    void onInterfaceClicked(const QString& nodeId, const QString& interfaceId,const QString& interfaceName);
private:
    QHBoxLayout* traficLayout;
    QVBoxLayout* nodesInterfacesListLayout;
    QVBoxLayout* graphicLayout;
    QFormLayout* interfaceListFormLayout=nullptr;
    QVector<QComboBox*> nodeComboBoxes; // Store the combo boxes for each node
    QtCharts::QLineSeries* series;
    QtCharts::QChart* chart ;
    QtCharts::QDateTimeAxis* xAxis ;
    QtCharts::QValueAxis* yAxis ;
    QtCharts::QChartView* chartView;
    QTimer* refreshChartDataTimer ;

    QFormLayout* formLayout ;

    QScrollArea* scrollArea ;
    QWidget* scrollContent;
    QPushButton* refreshButton;

    QString currentNodeId;
    QString currentInterfaceId;
    QString currentInterfaceName;


    void setupUI();
    void populateNodeInterfacesList();
    void refreshData();
    void refreshInterfacesData();

    double convertBytesToMbps(double value);







};

#endif // TRAFIC_H
