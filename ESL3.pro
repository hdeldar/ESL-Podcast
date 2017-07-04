#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T05:30:21
#
#-------------------------------------------------

QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ESL3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    SqliteDataProvider.cpp \
    PodcastModel.cpp \
    GlossaryModel.cpp \
    ESLFileDataProvider.cpp \
    ESLDbDataProvider.cpp \
    CategoryModel.cpp \
    BlogModel.cpp \
    BinFileWindow.cpp \
    QPodcastTableViewDelegate.cpp

HEADERS  += mainwindow.h \
    SqliteDataProvider.h \
    PodcastModel.h \
    IESL.h \
    IDbDataProvider.h \
    GlossaryModel.h \
    ESLFileDataProvider.h \
    ESLDbDataProvider.h \
    CategoryModel.h \
    BlogModel.h \
    BinFileWindow.h \
    QPodcastTableViewDelegate.h \
    Icon.h

FORMS    += mainwindow.ui

RESOURCES += \
    qrc.qrc

win32 {
   RC_FILE = ESL3.rc
}

