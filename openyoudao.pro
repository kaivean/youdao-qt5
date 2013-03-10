#-------------------------------------------------
#
# Project created by QtCreator 2012-12-19T15:25:23
#
#-------------------------------------------------

QT       += core gui network webkit

TARGET = openyoudao
TEMPLATE = app

LIBS  += /usr/lib/i386-linux-gnu/libXtst.a



SOURCES += main.cpp\
        dialog.cpp \
    tipdia.cpp \
    resultdia.cpp

HEADERS  += dialog.h \
    tipdia.h \
    resultdia.h

FORMS    += dialog.ui \
    resultdia.ui

OTHER_FILES +=

RESOURCES += \
    qresoures.qrc
