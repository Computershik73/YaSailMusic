import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

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
        Column {
            id: column
            anchors.centerIn: page
            //width: page.width
            spacing: Theme.paddingLarge
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
                  anchors.left: medbut.right
                //x: Theme.horizontalPageMargin
                text: qsTr("Моя волна")
                //color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
        }
    }
}
