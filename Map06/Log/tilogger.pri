!contains(DEFINES,LOGGER){
DEFINES += LOGGER
CONFIG += c++11

HEADERS += \
    $$PWD/tilogger.h

SOURCES += \
    $$PWD/tilogger.cpp

INCLUDEPATH += $$PWD
}

