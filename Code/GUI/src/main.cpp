#include "mainwindow.h"
#include <QFileInfo>
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QStyle>
#include <QDesktopWidget>
#include <QScreen>
#include <QStyleFactory>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList languages = QLocale().uiLanguages();
    if (!languages.isEmpty())
    {
        QString language = languages.front().split("-").front();
        QString qmFile = QString(":/lang/language_%1.qm").arg(language);
        if (QFileInfo::exists(qmFile))
        {
            QTranslator * translator = new QTranslator(&app);
            translator->load(QLocale(), qmFile);
            app.installTranslator(translator);

            QTranslator * qtTranslator = new QTranslator;
            qtTranslator->load(QString("qt_%1").arg(language), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
            app.installTranslator(qtTranslator);
        }
    }
    MainWindow w;
    w.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w.size(), QGuiApplication::primaryScreen()->availableGeometry()));
    w.show();
    return app.exec();
}
