#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QtGlobal>
#include <QSettings>

#include <sailfishapp.h>

#include <stdio.h>
#include <stdlib.h>
#include <QScopedPointer>
#include "authorization.h"

#include "models/playlistmodel.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    // QByteArray localMsg = msg.toLocal8Bit();
 QByteArray localMsg = msg.toLocal8Bit();
    QFile fileOut("/home/defaultuser/YaSailMusicLog.txt"); // Связываем объект с файлом
    if(fileOut.open(QIODevice::Append | QIODevice::Text))
    { // Если файл успешно открыт для записи в текстовом режиме
        fileOut.write(localMsg);
        fileOut.flush();
        fileOut.close();
    }

}


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
     qInstallMessageHandler(myMessageOutput);
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("org.ilyavysotsky"));
    application->setApplicationName(QStringLiteral("yasailmusic"));

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    QSettings settings;

    qmlRegisterType<PlaylistModel>("org.ilyavysotsky.yasailmusic",1,0,"PlaylistModel");
    Authorization* auth = new Authorization();
    view->rootContext()->setContextProperty("application", application.data());
    view->rootContext()->setContextProperty("auth", auth);
    view->setSource(SailfishApp::pathTo("qml/YaSailMusic.qml"));
    view->show();

    return application->exec();
}
