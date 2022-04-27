import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Label {
        id: label
        anchors.centerIn: parent
        text:  "hello world"
    }

    CoverActionList {
        id: activecover
        CoverAction {
            iconSource: "image://theme/icon-cover-previous-song"
            onTriggered: {
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next-song"
            onTriggered: {
            }
        }
    }
}
