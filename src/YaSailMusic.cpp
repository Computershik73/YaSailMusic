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
#include <stdio.h>
 #include <stdlib.h>
#include <QScopedPointer>
#include "mediaplayerwrapper.h"
#include "settingswrapper.h"
#include "../yamussdk.h"


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);
    Q_UNUSED(msg);
    QByteArray localMsg = msg.toLocal8Bit();
    //switch (type) {
    /*case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }*/

    QFile fileOut("/home/nemo/yamus.txt");
        if(fileOut.open(QIODevice::Append | QIODevice::Text))
        {
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
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("org.ilyavysotsky"));
    application->setApplicationName(QStringLiteral("yasailmusic"));
     qInstallMessageHandler(myMessageOutput);
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    QScopedPointer<SettingsWrapper> settings(new SettingsWrapper(view.data()));
    view->rootContext()->setContextProperty("settings", settings.data());

    QScopedPointer<MediaPlayerWrapper> player(new MediaPlayerWrapper(view.data()));
    view->rootContext()->setContextProperty("player", player.data());

  //  QScopedPointer<SettingsWrapper> settings(new SettingsWrapper(view.data()));
   // view->rootContext()->setContextProperty("settings", settings.data());

    QScopedPointer<yamussdk> Yamussdk(new yamussdk(view.data()));

    view->rootContext()->setContextProperty("yamussdk", Yamussdk.data());
   view->setSource(SailfishApp::pathTo("qml/YaSailMusic.qml"));
   view->show();

   return application->exec();
}
