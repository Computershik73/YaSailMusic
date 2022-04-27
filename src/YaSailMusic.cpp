#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QtGlobal>

#include <sailfishapp.h>

#include <stdio.h>
#include <stdlib.h>
#include <QScopedPointer>
#include "settingswrapper.h"
#include "../yamussdk.h"

#include "models/playlistmodel.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/YandexMusic.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("org.ilyavysotsky"));
    application->setApplicationName(QStringLiteral("yasailmusic"));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qmlRegisterType<PlaylistModel>("org.ilyavysotsky.yasailmusic",1,0,"PlaylistModel");

    QScopedPointer<SettingsWrapper> settings(new SettingsWrapper(view.data()));
    view->rootContext()->setContextProperty("settings", settings.data());

    QScopedPointer<yamussdk> Yamussdk(new yamussdk(view.data()));

    view->rootContext()->setContextProperty("yamussdk", Yamussdk.data());
    view->setSource(SailfishApp::pathTo("qml/YaSailMusic.qml"));
    view->show();

    return application->exec();
}
