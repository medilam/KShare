TEMPLATE = app

CONFIG += c++11 console

QT -= gui

INCLUDEPATH += include/ \
    ../LIB/include/
DEPENDPATH += include/ \
    ../LIB/include/

SOURCES += \
    ../LIB/src/Receiver.cpp \
    ../LIB/src/Sender.cpp \
    src/main.cpp \

HEADERS += \
    ../LIB/include/Receiver.h \
    ../LIB/include/Sender.h \
    include/SignalHandler.h

LIBS += -lsfml-system -lsfml-network -lcrypto

LIBS += -L../LIB/lib -lkeyshare

OBJECTS_DIR = .obj
MOC_DIR = .moc
