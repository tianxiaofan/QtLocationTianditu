
QT += location concurrent sql location-private positioning-private
CONFIG += c++11

DEFINES += MyMapPlugin_Static

SOURCES += \
        $$PWD/tidataworker.cpp \
        $$PWD/tigeotilefetcher.cpp \
        $$PWD/timapengine.cpp \
        $$PWD/timapprovider.cpp \
        $$PWD/timapurlengine.cpp \
        $$PWD/tiqgeoserviceproviderfactory.cpp \
        $$PWD/tiqgeotiledmappingmanagerengine.cpp \
        $$PWD/tiqgeotiledmapreply.cpp \
        $$PWD/tiqgeocodereply.cpp \
        $$PWD/tiqgeocodingmanagerengine.cpp \
        $$PWD/tiqgeofiletilecache.cpp \
        $$PWD/titianditumapprovider.cpp

HEADERS += \
        $$PWD/TiMapTile.h \
        $$PWD/tidataworker.h \
        $$PWD/tigeotilefetcher.h \
        $$PWD/timapengine.h \
        $$PWD/timapprovider.h \
        $$PWD/timapurlengine.h \
        $$PWD/tiqgeoserviceproviderfactory.h \
        $$PWD/tiqgeotiledmappingmanagerengine.h \
        $$PWD/tiqgeotiledmapreply.h \
        $$PWD/tiqgeocodereply.h \
        $$PWD/tiqgeocodingmanagerengine.h \
        $$PWD/tiqgeofiletilecache.h \
        $$PWD/titianditumapprovider.h

OTHER_FILES += \
    $$PWD/timap_plugin.json

INCLUDEPATH += \
    $$PWD
