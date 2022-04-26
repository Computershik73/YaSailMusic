import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: mainPage

    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: mainView
        anchors.fill: parent
        anchors{
            top: header.bottom
            left: parent.left
            right: parent.right
        }

        BusyIndicator {
            id: busyIndicator
            running: visible
            visible: true
            anchors.centerIn: parent
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("View playlist")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("PlaylistPage.qml"))
            }
        }

        PageHeader {
            id: header
            title: qsTr("My wave")
        }

        contentHeight: mainPage.height

        Item {
            id: column
            height: childrenRect.height
            width: childrenRect.width

            anchors.centerIn: parent
            visible: !busyIndicator.visible

            MediaButton {
                id: medbut
                source: player.isPlaying ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
                mouseArea.onClicked: {

                    if (player.isPlaying) {
                        player.pause()
                    } else {
                        player.play()
                    }
                }
            }

            Label {
                id: mediaButtLabel
                text: player.isPlaying ? qsTr("Pause") : qsTr("Play")
                font.pixelSize: Theme.fontSizeMedium

                anchors{
                    top: medbut.bottom
                    topMargin: Theme.paddingSmall
                    verticalCenter: medbut.verticalCenter
                }
            }
        }
    }

    Component.onCompleted: {
        yamussdk.audios.get()
    }

    Connections {
        target: player
        onMediaChanged: {
            player.model.setPlayingIndex(player.currentIndex);
        }
    }

    Connections{
        target: yamussdk.audios
        onGetFinished: busyIndicator.visible = false
    }

    Connections {
        target: yamussdk
        onGotUserAudios: {
            player.setPlaylist(audios, -1)
        }
    }
}
