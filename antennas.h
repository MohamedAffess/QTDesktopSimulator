#ifndef ANTENNAS_H
#define ANTENNAS_H

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
class Antennas : public QWidget
{
    Q_OBJECT

public:
    explicit Antennas(QWidget *parent = nullptr);
    ~Antennas();

private slots:
    void onAddAntennaClicked();
    void onDeleteActionTriggered();
    void onInfoActionTriggered();
    void onModifyActionTriggered();
    //    void onTableRightClicked(const QPoint &pos);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;


private:
    QTableWidget *antennasTable;
    QPushButton *addAntennaButton;
    QVBoxLayout *antennasLayout;
    QWidget *antennaInfoWidget;
    QWidget* addAntennaWidget ;
    QWidget* antennaModifyWidget;
    QMenu* antennasMenu;


    QAction *deleteAction;
    QAction *infoAction;
    QAction *modifyAction;


    void setupUI();
    void populateTable();
    void showAddAntennaForm();


    //DockWidget

    QDockWidget *dockWidget;
    QStackedWidget *dockContent;
    QVBoxLayout *dockLayout;

    void displayDockWidget(QString message);




};

#endif // ANTENNAS_H
