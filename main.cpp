#include "simulatorsuite.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set the application name and organization
        app.setApplicationName("LUCEOR Simulator");
        app.setOrganizationName("LUCEOR");
        app.setApplicationDisplayName("LUCEOR Simulator");
        QIcon appIcon(":/images/app_icon.png");
            app.setWindowIcon(appIcon);


        // Set the application version
        app.setApplicationVersion("1.0.0");
    QFile styleFile(":/styles/style.qss");  // The ":/" prefix refers to the resource file
    styleFile.open(QFile::ReadOnly);
    QFont font("Arial Light",10); // Example: Arial font with size 12

        // Set the font for the application
        app.setFont(font);
    QString styleSheet = QLatin1String(styleFile.readAll());
    app.setStyleSheet(styleSheet);



    SimulatorSuite w;
    w.show();
    return app.exec();
}
