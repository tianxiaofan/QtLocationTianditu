
QT += location concurrent sql location-private positioning-private
CONFIG += c++11

DEFINES += MyMapPlugin_Static

SOURCES += \
        $$PWD/tibingmapprovider.cpp \
        $$PWD/tiesrimapprovider.cpp \
        $$PWD/tigeotilefetcher.cpp \
        $$PWD/tigooglemapprovider.cpp \
        $$PWD/timapcacheworker.cpp \
        $$PWD/timapengine.cpp \
        $$PWD/timapjsonengine.cpp \
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
        $$PWD/tibingmapprovider.h \
        $$PWD/tiesrimapprovider.h \
        $$PWD/tigeotilefetcher.h \
        $$PWD/tigooglemapprovider.h \
        $$PWD/timapcacheworker.h \
        $$PWD/timapengine.h \
        $$PWD/timapenginedata.h \
        $$PWD/timapjsonengine.h \
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
