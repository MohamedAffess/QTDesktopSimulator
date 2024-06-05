QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    antennas.cpp \
    antennasmanagement.cpp \
    channels.cpp \
    interfacesmanagement.cpp \
    linksmanagement.cpp \
    main.cpp \
    management.cpp \
    meshrouters.cpp \
    modulations.cpp \
    nodesmanagement.cpp \
    products.cpp \
    simulation.cpp \
    simulatorsuite.cpp \
    snrmeasures.cpp \
    topology.cpp \
    trafic.cpp

HEADERS += \
    antennas.h \
    antennasmanagement.h \
    channels.h \
    interfacesmanagement.h \
    linksmanagement.h \
    management.h \
    meshrouters.h \
    modulations.h \
    nodesmanagement.h \
    products.h \
    simulation.h \
    simulatorsuite.h \
    snrmeasures.h \
    topology.h \
    trafic.h

FORMS += \
    antennas.ui \
    antennasmanagement.ui \
    channels.ui \
    interfacesmanagement.ui \
    linksmanagement.ui \
    management.ui \
    meshrouters.ui \
    modulations.ui \
    nodesmanagement.ui \
    products.ui \
    simulation.ui \
    simulatorsuite.ui \
    snrmeasures.ui \
    topology.ui \
    trafic.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
