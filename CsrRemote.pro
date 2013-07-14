TEMPLATE = app
CONFIG += console
CONFIG -= qt

unix:!symbian: LIBS += -lusb-1.0

SOURCES += main.cpp \
    remote.cpp \
    usbprogrammer.cpp \
    usbdriver.cpp \
    stopwatch.cpp \
    devicemanager.cpp \
    programmer.cpp

HEADERS += \
    remote.h \
    usbprogrammer.h \
    usbdriver.h \
    stopwatch.h \
    devicemanager.h \
    bc_def.h \
    programmer.h

