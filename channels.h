#ifndef CHANNELS_H
#define CHANNELS_H

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
class Channels : public QWidget
{
    Q_OBJECT

public:
    explicit Channels(QWidget *parent = nullptr);
    ~Channels();

private slots:
    void onAddChannelClicked();
    void onDeleteActionTriggered();
    void onInfoActionTriggered();
    void onModifyActionTriggered();

    //    void onTableRightClicked(const QPoint &pos);


protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QTableWidget *channelTable;
    QPushButton *addChannelButton;
    QVBoxLayout *channelsLayout;
    QWidget *addChannelWidget;
    QWidget *channelInfoWidget;
    QWidget* channelModifyWidget;
    QMenu* channelsMenu;


    QAction *deleteAction;
    QAction *infoAction;
    QAction *modifyAction;

    void setupUI();
    void populateTable();
    void showAddChannelForm();

    //dockWidget
    QDockWidget *dockWidget;  // Dock widget to hold other widgets
    QStackedWidget *dockContent;
    QVBoxLayout *dockLayout;

    void displayDockWidget(QString message);  // Method to toggle the visibility of the dock widget





};

#endif // CHANNELS_H
