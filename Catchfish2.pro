TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
LIBS += -lcurl
LIBS += -lpthread
HEADERS += \
    basen.hpp \
    basen.hpp \
    profile.h \
    whoisds.h


unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libzip

DISTFILES += \
    keywords \
    banks
