#ifndef MANAGEMENT_H
#define MANAGEMENT_H



#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include "nodesmanagement.h"
#include "interfacesmanagement.h"
#include "linksmanagement.h"
#include "antennasmanagement.h"


class Management : public QWidget
{
    Q_OBJECT

public:
    explicit Management(QWidget *parent = nullptr);
    ~Management();


private slots:
//    void onNodesButtonClicked();
//    void onInterfacesButtonClicked();
//    void onLinksButtonClicked();
//    void onAntennasButtonClicked();

private:
    QVBoxLayout *managementLayout;
    QPushButton *nodesButton;
//    QPushButton *interfacesButton;
//    QPushButton *linksButton;
//    QPushButton *antennasButton;
    QStackedWidget *contentStack;


    NodesManagement *nodesContent;
//    InterfacesManagement *interfacesContent;
//    LinksManagement *linksContent;
//    AntennasManagement *antennasContent;
    void setupUI();



};

#endif // MANAGEMENT_H
