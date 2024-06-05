#ifndef NODESMANAGEMENT_H
#define NODESMANAGEMENT_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QContextMenuEvent>
#include <QMenu>
#include <QDockWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QListView>
#include <QStringListModel>
#include <QFormLayout>
#include <QComboBox>
#include <QScrollArea>
class NodesManagement : public QWidget
{
    Q_OBJECT

public:
    explicit NodesManagement(QWidget *parent = nullptr);
    ~NodesManagement();

private slots:

    void onCreateNodeClicked();

    //slots for single selection menu
    void onStartNodeActionTriggered();
    void onStopNodeActionTriggered();
    void onRebootNodeActionTriggered();
    void onShowLinksActionTriggered();
    void onAddAntennasActionTriggered();
    void onDeleteNodeActionTriggered();
    void onInfoActionTriggered();
    void onGetFirmwareActionTriggered();
    void onStartMonitoringActionTriggered();
    void onStopMonitoringActionTriggered();
    void onStartVumeterActionTriggered();
    void onStopVumeterActionTriggered();
    void onInterfacesActionTriggered();//
    void onInterfacesInfoPressed();//
    void onConfigureInterfacePressed();//
    void onAddLinksPressed();


//    void onGetSSHFingerPrints();
//    void onGetVumeterStatus();

    //slots for multi selection menu
    void onStartMultipleNodesActionTriggered();
    void onStopMultipleNodesActionTriggered();
    void onRebootMultipleNodesActionTriggered();
    void onDeleteMultipleNodesActionTriggered();
    void onStartMultipleNodesMonitoringActionTriggered();
    void onStopMultipleNodesMonitoringActionTriggered();
    void onStartMultipleNodesVumeterActionTriggered();
    void onStopMultipleNodesVumeterActionTriggered();



protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QHBoxLayout *nodesManagementLayout;

    QTableWidget *nodesManagementTable;
    QTableWidgetItem *checkBoxItem;
    QVBoxLayout *nodesListLayout;
    QPushButton *createNodeButton;
    QWidget *nodeLinksWidget;
    QWidget *nodeInfoWidget;
    QWidget *createNodeWidget;
    QWidget* nodeInterfacesWidget;
     QWidget* interfaceInfoWidget;
      QWidget* configureInterfaceWidget ;
     QTimer *nodeManagementTimer ;
     QMenu* nodesManagementMenu;
     QTimer* refreshTableTimer;




    //unchecked menu
    QAction *startNodeAction;
    QAction *stopNodeAction;
    QAction *rebootNodeAction;
    QAction *showLinksAction;
    QAction *deleteNodeAction;
    QAction *addAntennasAction;
    QAction *nodeInfoAction;
    QAction *getFirmwareAction;
    QAction *startMonitoringAction;
    QAction *stopMonitoringAction;
    QAction *startVumeterAction;
    QAction *stopVumeterAction;
    QAction* nodeInterfacesAction;//
//    QAction *getVumeterStatusAction;
//    QAction *getSSHFingerPrintsAction;

    //checked menu
    QAction *startMultipleNodesAction;
    QAction *stopMultipleNodesAction;
    QAction *rebootMultipleNodesAction;
    QAction *deleteMultipleNodesAction;
    QAction *startMultipleNodesMonitoringAction;
    QAction *stopMultipleNodesMonitoringAction;
    QAction *startMultipleNodesVumeterAction;
    QAction *stopMultipleNodesVumeterAction;




    void setupUI();
    void populateTable();
    void showCreateNodeForm();
    void onDeleteLinkClicked(const QString& linkId);
    void refreshData();



    //DockWidget

    QDockWidget *dockWidget;  // Dock widget to hold other widgets
    QStackedWidget *dockContent;
    QVBoxLayout *dockLayout;

    void displayDockWidget(QString message);  // Method to toggle the visibility of the dock widget


//create node widget

//    QVBoxLayout * createNodeLayout;
//    QFormLayout * createNodeFormLayout;
//    QScrollArea * createNodeFormScrollArea;
//    QWidget* createNodeFormScrollAreaContent;
//QPushButton* createNodeFormSubmitButton;
//QPushButton* createNodeFormCancelButton ;
//QHBoxLayout* createNodeFormButtonLayout;



};

#endif // NODESMANAGEMENT_H
