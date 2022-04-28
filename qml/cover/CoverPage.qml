import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

CoverBackground {

    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingMedium
        Label {
            visible: rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState
            id: artist
            width: contentWidth>cover.width ? cover.width : contentWidth
            anchors.horizontalCenter: parent.horizontalCenter
            text: (rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState) ? playListModel.get(currentIndex).artistName : "Idle"
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            truncationMode: TruncationMode.Fade
        }

        Label {
            visible: rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState
            id: song
            width: contentWidth>cover.width ? cover.width : contentWidth
            anchors.horizontalCenter: parent.horizontalCenter
            text: (rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState) ? playListModel.get(currentIndex).trackName : "Idle"
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            truncationMode: TruncationMode.Fade
        }
    }

    CoverActionList {
        id: activecover
        enabled: rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState
        CoverAction {
            iconSource: "image://theme/icon-cover-previous-song"
            onTriggered: {
                --playListModel.currentIndex
                //label.text = playListModel.get(currentIndex).trackName
            }
        }

        CoverAction {
            iconSource: (rootAudio.playbackState === MediaPlayer.PlayingState) ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
            onTriggered: {
                if (rootAudio.playbackState === MediaPlayer.PlayingState) {
                    rootAudio.pause()
                } else {
                    rootAudio.play()
                }
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next-song"
            onTriggered: {
                ++playListModel.currentIndex
                //label.text = playListModel.get(currentIndex).trackName
            }
        }


    }

    Connections{
        target: playListModel
        onCurrentIndexChanged: {
            song.text = playListModel.get(currentIndex).trackName
            artist.text = playListModel.get(currentIndex).artistName
        }
    }
}
