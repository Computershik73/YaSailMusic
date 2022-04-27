# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = YaSailMusic

CONFIG += sailfishapp

CONFIG += sailfishapp_i18n

QT += multimedia core gui

SOURCES += \
    src/apirequest.cpp \
    src/authorization.cpp \
    src/YaSailMusic.cpp \
    src/models/playlistmodel.cpp

DISTFILES += \
    YaSailMusic.desktop \
    YaSailMusic.png \
    qml/YaSailMusic.qml \
    qml/components/Banner.qml \
    qml/cover/CoverPage.qml \
    qml/pages/LoginPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/MediaButton.qml \
    qml/img/YaSailMusic.png \
    qml/pages/PlaylistPage.qml \
    rpm/YaSailMusic.spec
    translations/*.ts

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/YandexMusic-de.ts

HEADERS += \
    src/apirequest.h \
    src/authorization.h \
    src/models/playlistmodel.h
