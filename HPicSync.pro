#-------------------------------------------------
#
# Project created by QtCreator 2011-05-04T15:09:39
#
#-------------------------------------------------

QT       += core network gui

TARGET = HPicSync
TEMPLATE = app


SOURCES += main.cpp\
    hpsoptionwidget.cpp \
    hpsdirmanager.cpp \
    hpicsync.cpp \
    hpsfindserver.cpp \
    hpslistviewdelegate.cpp \
    hpsoption.cpp \
    hpsauthentication.cpp \
    hpsimageloader.cpp \
    hpspasswordwidget.cpp \
    hpsselectaddresswidget.cpp \
    hpstcpmodul.cpp \
    hpstreecombobox.cpp

HEADERS  += \
    hpsoptionwidget.h \
    hpsdirmanager.h \
    hpicsync.h \
    hpsdirknoten.h \
    hpsfindserver.h \
    hpslistviewdelegate.h \
    hpsoption.h \
    hpsauthentication.h \
    hpsimageloader.h \
    hpspasswordwidget.h \
    hpsselectaddresswidget.h \
    hpstcpmodul.h \
    hpstreecombobox.h


RESOURCES += \
    res.qrc

OTHER_FILES += \
    knopfRot.png \
    knopfGruen.png
