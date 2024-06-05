#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include "meshrouters.h"
#include "antennas.h"
#include "modulations.h"
#include "channels.h"

class Products : public QWidget
{
    Q_OBJECT

public:
    explicit Products(QWidget *parent = nullptr);
    ~Products();

private slots:
    void onMeshRoutersButtonClicked();
    void onAntennasButtonClicked();
    void onModulationsButtonClicked();
    void onChannelsButtonClicked();

private:
    QVBoxLayout *productsLayout;
    QPushButton *meshRoutersButton;
    QPushButton *antennasButton;
    QPushButton *modulationsButton;
    QPushButton *channelsButton;
    QStackedWidget *contentStack;
    QLabel* titleLabel;

    MeshRouters *meshRoutersContent;
    Antennas *antennasContent;
    Modulations *modulationsContent;
    Channels *channelsContent;

    void setupUI();
};

#endif // PRODUCTS_H
