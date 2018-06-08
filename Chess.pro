TEMPLATE = app

CONFIG += c++11
CONFIG += console

QT += qml quick widgets

SOURCES += src/main.cpp \
    src/logic.cpp \
    src/moveslist.cpp \
    src/database.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

HEADERS += \
    src/logic.h \
    src/moveslist.h \
    src/database.h

deployment.files += myfile.sqlite
deployment.path = /assets
INSTALLS += deployment

DISTFILES += \
    chess.db \
    chess.db
