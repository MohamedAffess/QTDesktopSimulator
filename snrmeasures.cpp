#include "snrmeasures.h"
#include "ui_snrmeasures.h"
#include <QLabel>
SnrMeasures::SnrMeasures(QWidget *parent) :
    QWidget(parent)
{

    QLabel *label = new QLabel("SNR content", this);

    // Create a QLabel widget
            label->setAlignment(Qt::AlignCenter);  // Align text to center

            // Set the size and position of the QLabel widget
            label->setGeometry(50, 50, 200, 50);

            // Set the font properties of the QLabel widget
            QFont font;
            font.setPointSize(16);
            font.setBold(true);
            label->setFont(font);

}

SnrMeasures::~SnrMeasures()
{

}
