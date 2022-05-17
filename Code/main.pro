TEMPLATE = subdirs
CONFIG += ordered \
    qt
SUBDIRS = LIB/keyshare-lib.pro CLI/keyshare-cli.pro GUI/keyshare-gui.pro
TARGET = GUI/keyshare-gui

QT += widgets
