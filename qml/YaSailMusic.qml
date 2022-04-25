import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow {
    initialPage: {
        if (settings.accessToken()) {

            if (yamussdk.checkToken(settings.accessToken())) {
                yamussdk.setAccessTocken(settings.accessToken())
                yamussdk.setUserId(settings.userId())
            return Qt.createComponent(Qt.resolvedUrl("pages/MainPage.qml"))
            } else {
            settings.removeAccessToken()
            settings.removeUserId()
            return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
            }
        } else {
            return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
        }
    }


    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
}
