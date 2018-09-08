#-------------------------------------------------
#
# Project created by QtCreator 2018-09-03T05:43:07
#
#-------------------------------------------------

QT       -= gui

TARGET = PixyUSB2
TEMPLATE = lib

DEFINES += LIBPIXYUSB2_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/libpixyusb2.cpp \
    src/usblink.cpp \
    src/util.cpp \
    ../common/src/chirp.cpp

HEADERS += \
        libpixyusb2_global.h \ 
    include/libpixyusb2.h \
    include/usblink.h \
    include/util.h \
    libpixyusb2_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DEFINES += LIBPIXYUSB2_LIBRARY

#INC = -I/usr/include/libusb-1.0 -I../include -I../../../common/inc -I../inc -I../../arduino/libraries/Pixy2
INCLUDEPATH += include
INCLUDEPATH += ../common/inc
INCLUDEPATH += ../arduino/libraries/Pixy2

#CONFIG(debug, debug|release) {
#    DESTDIR = build/debug
#} else {
#    DESTDIR = build/release
#}
DESTDIR = lib

macx {
    #ICON = pixy.icns
    DEFINES += __MACOS__

    LIBS += -L/usr/local/opt/libusb/lib -lusb-1.0
    INCLUDEPATH += /usr/local/include/libusb-1.0
}
