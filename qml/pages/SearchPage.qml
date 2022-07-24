import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All



    SilicaListView {
        id: listView
        model: searchModel
        anchors.fill: parent
        header: SearchField {
            id: topsearchField
            width: parent.width
            placeholderText: qsTr("Search audio...")
            visible: true
            EnterKey.enabled: true
            onTextChanged: {
                if (text.length !== 0) {
                    //busyIndicator.running = true
                    //search
                    //searchModel.clear()
                    //if ( !searchModel.m_loading) {
                     busyIndicator.visible = true
                    searchModel.searchTracks(text)
                    //}
                }
                else {
                    //busyIndicator.running = true
                    //visible = false
                    focus = false

                }
            }
        }
        delegate: BackgroundItem {
            id: delegate

            Image {
                id: cover
                source: "https://"+albumCover.replace("%%","200x200")
                width: height
                height: parent.height
            }

            Label {
                x: Theme.horizontalPageMargin
                text: artistName + " - " + trackName
                anchors{
                    left: cover.right
                    leftMargin: 20
                    top: parent.top
                }

                color:  searchModel.currentIndex === index ? Theme.highlightColor : Theme.primaryColor
                //delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            onClicked: {
                searchModel.setCurrentIndex(index)
                //playListModel.sendFeedback("trackStarted")
                //playListModel.playTrack()
                //playListModel.sendFeedback("trackFinished")
            }
        }
        BusyIndicator {
                   id: busyIndicator
                   anchors.centerIn: parent
                   size: BusyIndicatorSize.Large
                   running: visible
                   visible: false
                }

        VerticalScrollDecorator {}
    }

    Connections{
        target: searchModel
        onLoadFirstDataFinished: busyIndicator.visible = false
    }
}
