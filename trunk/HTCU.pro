#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T09:24:09
#
#-------------------------------------------------

QT              += core gui
QT              += sql
#QT              += qt3support

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET           =  HTCU
TEMPLATE         =  app

target.path=/mnt/ssh
INSTALLS += target

MOC_DIR          = temp/moc
RCC_DIR          = temp/rcc
UI_DIR           = temp/ui
OBJECTS_DIR      = temp/obj
DESTDIR          = bin

QMAKE_CXXFLAGS  +=  -Wno-unused-parameter
QMAKE_CXXFLAGS  +=  -Wno-unused-variable
QMAKE_CXXFLAGS  +=  -Wno-psabi

include(src/gui/gui.pri)
include(src/app/app.pri)
include(src/util/util.pri)
include(src/comm/comm.pri)

#include(src/gui/dialog/dialog.pri)


include(test/test.pri) ## Modules Test


INCLUDEPATH	+= $$PWD/src/app
INCLUDEPATH	+= $$PWD/src/gui
INCLUDEPATH	+= $$PWD/src/comm
INCLUDEPATH     += $$PWD/src/util
INCLUDEPATH     += $$PWD/test

OTHER_FILES +=

