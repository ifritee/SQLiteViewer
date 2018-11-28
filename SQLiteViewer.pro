#-------------------------------------------------
#
# Project created by QtCreator 2017-10-20T16:28:50
#
#-------------------------------------------------

QT       += core gui sql

include(xlsx/qtxlsx.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SQLiteViewer

TEMPLATE = app


SOURCES += \
    src/main.cpp\
    frm/WMainWindow.cpp \
    frm/DAbout.cpp \
    frm/DCreateDB.cpp \
    src/common/SLogger.cpp \
    frm/WDBInquiry.cpp \
    frm/DAddTable.cpp \
    frm/DEditTable.cpp \
    src/common/SQLConstants.cpp \
    src/common/SQLHighLighter.cpp \
    src/common/SCompleterTextEdit.cpp \
    frm/DInsertQuery.cpp \
    src/WPerfomance.cpp \
    src/WViewDBTable.cpp \
    src/CSVConverter.cpp \
    frm/DCreateCSVTable.cpp

HEADERS  += \
    frm/WMainWindow.h \
    frm/DAbout.h \
    frm/DCreateDB.h \
    src/common/SLogger.h \
    frm/WDBInquiry.h \
    frm/DAddTable.h \
    frm/DEditTable.h \
    src/common/SQLConstants.h \
    src/common/SQLHighLighter.h \
    src/common/SCompleterTextEdit.h \
    frm/DInsertQuery.h \
    src/WPerfomance.h \
    src/WViewDBTable.h \
    src/CSVConverter.h \
    frm/DCreateCSVTable.h

FORMS    += \
    frm/WMainWindow.ui \
    frm/DAbout.ui \
    frm/DCreateDB.ui \
    frm/WDBInquiry.ui \
    frm/DAddTable.ui \
    frm/DInsertQuery.ui \
    frm/DCreateCSVTable.ui

INCLUDEPATH += \
    src \
    src/common \
    src/routine \
    src/ui \
    frm \
    res

RESOURCES += \
    res/Icons.qrc
