import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    function init() {
        //busyIndicator.running = true
        //for (var index in menuItems) menuList.model.append(menuItems[index])
        /*vksdk.stats.trackVisitor()
        vksdk.users.getSelfProfile()
        vksdk.messages.getDialogs()
        yamussdk.audios.get()
        vksdk.longPoll.getLongPollServer()*/
        yamussdk.audios.get()
    }

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Моя волна")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("SecondPage.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Row {
            height: medbut.height
            id: column
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenter: parent.verticalCenter
            //width: page.width
            spacing: Theme.paddingLarge
            MediaButton {
                 //anchors.verticalCenter: parent
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
                  //anchors.left: medbut.right
                  //anchors.verticalCenter: column.verticalCenter
                //x: Theme.horizontalPageMargin
                text: qsTr("Моя волна")
                //color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
        }
    }

    Component.onCompleted: {
       // if (!settings.offlineStatus()) {
           // vksdk.account.setOnline()
            //onlineTimer.start()
        //}
        init()
    }

    Connections {
        target: player
        onMediaChanged: {
            player.model.setPlayingIndex(player.currentIndex);
            //playlist.positionViewAtIndex(player.currentIndex, ListView.Center)
        }

    }

    Connections {
        target: yamussdk
        onGotUserAudios: {
//            console.log(audios.length)
            //busyIndicator.running = false
            player.setPlaylist(audios, -1)
        }

    }
}
