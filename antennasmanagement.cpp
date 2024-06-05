#include "antennasmanagement.h"
#include "ui_antennasmanagement.h"
#include <QLabel>

AntennasManagement::AntennasManagement(QWidget *parent) :
    QWidget(parent)
{
    // Create a QLabel widget
            QLabel *label = new QLabel("antennas content", this);
            label->setAlignment(Qt::AlignCenter);  // Align text to center

            // Set the size and position of the QLabel widget
            label->setGeometry(50, 50, 200, 50);

            // Set the font properties of the QLabel widget
            QFont font;
            font.setPointSize(16);
            font.setBold(true);
            label->setFont(font);
}

AntennasManagement::~AntennasManagement()
{
}
