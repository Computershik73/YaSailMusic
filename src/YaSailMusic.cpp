#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif



#include <QGuiApplication>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QtGlobal>

#include </home/okabe2011/SailfishOS/mersdk/targets/SailfishOS-3.4.0.24-armv7hl.default/usr/include/sailfishapp/sailfishapp.h>

//#include "filesaver.h"
#include <QScopedPointer>
#include "mediaplayerwrapper.h"
//#include "settingswrapper.h"
//#include "yamusicsdk/src/yamusicsdk.h"

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

    QScopedPointer<MediaPlayerWrapper> player(new MediaPlayerWrapper(view.data()));
    view->rootContext()->setContextProperty("player", player.data());

  //  QScopedPointer<SettingsWrapper> settings(new SettingsWrapper(view.data()));
   // view->rootContext()->setContextProperty("settings", settings.data());

   // QScopedPointer<VkSDK> vksdk(new VkSDK(view.data()));
   // view->rootContext()->setContextProperty("vksdk", vksdk.data());


    return SailfishApp::main(argc, argv);
}
