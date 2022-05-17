TEMPLATE = app
QT += core gui
TARGET = keyshare-gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include/ \
    ../LIB/include/
DEPENDPATH += include/ \
    ../LIB/include/

SOURCES += \
    ../LIB/src/Receiver.cpp \
    ../LIB/src/Sender.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/droparea.cpp \
    src/ReceiverThread.cpp \
    src/SenderThread.cpp \
    src/LogsEdit.cpp

HEADERS += \
    ../LIB/include/Receiver.h \
    ../LIB/include/Sender.h \
    include/mainwindow.h \
    include/droparea.h \
    include/mainwindow.h \
    include/ReceiverThread.h \
    include/SenderThread.h \
    include/LogsEdit.h

TRANSLATIONS += \
    lang/language_fr.ts

FORMS += mainwindow.ui

RESOURCES += resources.qrc

LIBS += -lsfml-network -lsfml-system -lcrypto

LIBS += -L../LIB/lib -lkeyshare

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .qrc
UI_DIR = .ui
