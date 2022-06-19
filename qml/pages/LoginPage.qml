import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0


import "../components/"

Page {
    id: loginPage

    Notification {
        id: loginNotification
        category: "YaSailMusic"
    }

    Banner{
        id: banner
        z: 1000
    }

    Column {
        width: parent.width

        Label {
            id: loginLabel
            text: qsTr("Login:")
            color: Theme.primaryColor
        }

        TextField {
            id: loginField
            width: parent.width
            color: Theme.primaryColor

            EnterKey.enabled: false
        }

        Label {
            id: passwordLabel
            text: qsTr("Password:")
            color: Theme.primaryColor
        }

        TextField {
            id: passwordField
            width: parent.width
            visible: true
            color: Theme.primaryColor

            EnterKey.enabled: false
            echoMode: TextInput.Password
        }

        Label {
            id: codeLabel
            text: qsTr("Code:")
            visible: false
            color: Theme.primaryColor
        }

        TextField {
            id: codeField
            width: parent.width
            color: Theme.primaryColor
            visible: false
            EnterKey.enabled: false
            text: ""
        }
    }

    Button {
        id: enterButton
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: qsTr("Login")
        enabled: passwordField.text != "" && loginField.text != ""
        onClicked: {
            if(loginField.text.indexOf("@") === -1) {
                loginField.text = loginField.text.trim()+"@yandex.ru"
            }

            auth.doAuth(loginField.text, passwordField.text, codeField.text)
        }
    }

    Connections {
        target: auth
        onAuthorized: {
            pageContainer.replace(Qt.resolvedUrl("MainPage.qml"))
            loginNotification.previewBody = qsTr("Logged into Yandex Music")
            loginNotification.publish()
        }
        onError: {
            banner.notify(qsTr("Login fail!"))
        }
    }
}

