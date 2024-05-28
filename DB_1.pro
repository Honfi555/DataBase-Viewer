QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++23

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Database/connected_level.cpp \
    Database/database.cpp \
    Database/orm.cpp \
    IODataBase/iodatabase.cpp \
    Widgets/autorisation.cpp \
    Widgets/dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    Widgets/search.cpp \
    Widgets/searchtable.cpp \
    stylesheet.cpp

HEADERS += \
    Database/connected_level.h \
    Database/database.h \
    Database/orm.h \
    IODataBase/iodatabase.h \
    IODataBase/printer.h \
    Widgets/autorisation.h \
    Widgets/dialog.h \
    mainwindow.h \
    Widgets/search.h \
    Widgets/searchtable.h \
    stylesheet.h \
    types.h

FORMS += \
    Widgets/autorisation.ui \
    Widgets/dialog.ui \
    mainwindow.ui \
    Widgets/search.ui \
    Widgets/searchtable.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/icons.qrc
