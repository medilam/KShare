TEMPLATE = lib

CONFIG += c++11 staticlib

TARGET = keyshare
DESTDIR = ./lib

INCLUDEPATH += include/

SOURCES += \
    src/Receiver.cpp \
    src/Sender.cpp \

HEADERS += \
    include/Receiver.h \
    include/Sender.h \

LIBS += -lsfml-network -lsfml-system -lcrypto

OBJECTS_DIR = .obj
MOC_DIR = .moc
