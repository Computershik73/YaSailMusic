import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.notifications 1.0

Page {
    id: loginPage

    Notification {
        id: loginNotification
        category: "YaSailMusic"
    }

    Label {
        id: label1
        text: qsTr("Login:")
        color: Theme.primaryColor
    }

    TextField {
        id: textField1
        width: parent.width
        anchors.top: label1.bottom
        color: Theme.primaryColor

        EnterKey.enabled: false
    }

    Label {
        id: label2
        anchors.top: textField1.bottom
        text: qsTr("Password:")
        color: Theme.primaryColor
    }

    TextField {
        id: textField2
        width: parent.width
        anchors.top: label2.bottom
        visible: true
        color: Theme.primaryColor

        EnterKey.enabled: false
        echoMode: TextInput.Password
    }

    Label {
        id: label3
        anchors.top: textField2.bottom
        text: qsTr("Code:")
        visible: false
        color: Theme.primaryColor
    }

    TextField {
        id: textField3
        width: parent.width
        anchors.top: label3.bottom
        color: Theme.primaryColor
        visible: false
        EnterKey.enabled: false
        text: ""
    }

    Button {
        id: enterButton
        anchors.top: textField3.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: qsTr("Login")
        onClicked: {
            yamussdk.auth.tryToGetAccessToken(textField1.text, textField2.text, textField3.text)
            //textField1.text = ""
            //textField2.text = ""
            //enterButton.enabled = false
        }
    }

    Connections {
        target: yamussdk.auth
        onAuthorized: {

            settings.setAccessToken(accessToken)
            settings.setUserId(userId)
            yamussdk.setAccessTocken(accessToken)
            yamussdk.setUserId(userId)
            pageContainer.replace(Qt.resolvedUrl("MainPage.qml"))
            loginNotification.previewBody = qsTr("Logged into Yandex Music")
            loginNotification.publish()
        }


    }
}

