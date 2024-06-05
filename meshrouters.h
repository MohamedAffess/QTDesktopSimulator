#ifndef MESHROUTERS_H
#define MESHROUTERS_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QContextMenuEvent>
#include <QMenu>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QStackedWidget>
class MeshRouters : public QWidget
{
    Q_OBJECT

public:
    explicit MeshRouters(QWidget *parent = nullptr);
    ~MeshRouters();

private slots:
    void onAddRouterClicked();
    void onDeleteActionTriggered();
    void onInfoActionTriggered();
    void onModifyActionTriggered();
//    void onSaveButtonClicked(const QJsonObject& nodeInfo);
    //void onTableRightClicked(const QPoint &pos);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;


private:
    QTableWidget *meshRoutersTable;
    QPushButton *addRouterButton;
    QVBoxLayout *meshRoutersLayout;
    QWidget *meshRouterInfoWidget;
    QWidget *meshRouterModifyWidget;
    QWidget *addMeshRouterWidget;
    QMenu *meshRoutersMenu;

    QAction *deleteAction;
    QAction *infoAction;
    QAction *modifyAction;


    void setupUI();
    void populateTable();
    void showAddRouterForm();


    QDockWidget *dockWidget;
    QStackedWidget *dockContent;
    QVBoxLayout *dockLayout;

    void displayDockWidget(QString message);


//    void showAddRouterForm(const QString &title, const QStringList &labels);
//    void populateMeshRoutersTable();

};

#endif // MESHROUTERS_H
