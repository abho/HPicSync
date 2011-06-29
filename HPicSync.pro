#-------------------------------------------------
#
# Project created by QtCreator 2011-05-04T15:09:39
#
#-------------------------------------------------

QT       += core network gui sql xml

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
    hpsdbhandler.cpp \
    hpsthumbmanager.cpp \
    hpsworkerclass.cpp \
    hpshashsaver.cpp \
    hpsthumb.cpp \
    hpsoldlistdelegate.cpp \
    hpsprogressbar.cpp \
    hpsdirdialog.cpp \
    hpsdirlister.cpp \
    hpsknotdirmodel.cpp \
    hpsdomdirmodel.cpp \
    hpspopupwidget.cpp \
    hpscombobox.cpp

HEADERS  += \
    hpsoptionwidget.h \
    hpsdirmanager.h \
    hpicsync.h \
    hpsfindserver.h \
    hpslistviewdelegate.h \
    hpsoption.h \
    hpsauthentication.h \
    hpsimageloader.h \
    hpspasswordwidget.h \
    hpsselectaddresswidget.h \
    hpstcpmodul.h \
    hpsdbhandler.h \
    hpsthumbmanager.h \
    hpsworkerclass.h \
    hpshashsaver.h \
    hpsthumb.h \
    hpsoldlistdelegate.h \
    hpsprogressbar.h \
    hpsdirdialog.h \
    hpsdirlister.h \
    hpsknotdirmodel.h \
    hpsdomdirmodel.h \
    hpspopupwidget.h \
    hpscombobox.h


RESOURCES += \
    res.qrc

OTHER_FILES += \
    knopfRot.png \
    knopfGruen.png

FORMS += \
    hpsdirdialog.ui \
    hpspopupwidget.ui
