#ifndef MODULATIONS_H
#define MODULATIONS_H

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
class Modulations : public QWidget
{
    Q_OBJECT

public:
    explicit Modulations(QWidget *parent = nullptr);
    ~Modulations();

private slots:
    void onAddModulationClicked();
    void onDeleteActionTriggered();
    void onInfoActionTriggered();
    void onModifyActionTriggered();
    //    void onTableRightClicked(const QPoint &pos);



protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QTableWidget *modulationTable;
    QPushButton *addModulationButton;
    QVBoxLayout *modulationLayout;
    QWidget *addModulationWidget;
    QWidget *modulationModifyWidget;
    QWidget *modulationsInfoWidget;
    QMenu* modulationsMenu;


    QAction *deleteAction;
    QAction *infoAction;
    QAction *modifyAction;

    void setupUI();
    void populateTable();
    void showAddModulationForm();


    //DockWidget

    QDockWidget *dockWidget;  // Dock widget to hold other widgets
    QStackedWidget *dockContent;
    QVBoxLayout *dockLayout;

    void displayDockWidget(QString message);  // Method to toggle the visibility of the dock widget


};

#endif // MODULATIONS_H
