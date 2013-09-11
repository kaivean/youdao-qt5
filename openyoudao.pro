#-------------------------------------------------
#
# Project created by QtCreator 2013-09-08T18:37:49
#
#-------------------------------------------------

QT       += core gui widgets network  webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = openyoudao
TEMPLATE = app



SOURCES += main.cpp\
        dialog.cpp \
    tipdia.cpp \
    resultdia.cpp


HEADERS  += dialog.h \
    tipdia.h \
    resultdia.h



FORMS    += dialog.ui \
    resultdia.ui


RESOURCES += \
    qresoures.qrc

OTHER_FILES += \
    README


