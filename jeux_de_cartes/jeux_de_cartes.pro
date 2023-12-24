QT       += core gui widgets multimedia svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    carte.cpp \
    deck.cpp \
    main.cpp \
    mainwindow.cpp \
    second.cpp

HEADERS += \
    carte.h \
    deck.h \
    mainwindow.h \
    second.h

FORMS += \
    mainwindow.ui \
    second.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    cartes.qrc \
    pics.qrc \
    sounds.qrc

