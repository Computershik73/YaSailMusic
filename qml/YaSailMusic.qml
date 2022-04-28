import QtQuick 2.0
import QtMultimedia 5.5

import Sailfish.Silica 1.0

import org.nemomobile.mpris 1.0
import org.ilyavysotsky.yasailmusic 1.0

import "pages"

ApplicationWindow {
    id: root
    initialPage: {
        if (auth.checkToken()) {
            return Qt.createComponent(Qt.resolvedUrl("pages/MainPage.qml"))
        } else {
            return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
        }
    }

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    PlaylistModel{
        id: playListModel
        onCurrentIndexChanged: {
            rootAudio.source = "file://" + playListModel.get(currentIndex).fileUrl;
            mprisPlayer.song = playListModel.get(currentIndex).trackName
            mprisPlayer.artist = playListModel.get(currentIndex).artistName
            rootAudio.play();
        }
    }

    MediaPlayer{
        id: rootAudio
        onStopped: {
            if (rootAudio.status == MediaPlayer.EndOfMedia) {
                playListModel.playTrack()
                ++playListModel.currentIndex
            }
        }
    }

    MprisPlayer {
        id: mprisPlayer

        property string artist: qsTr("Loading")
        property string song: qsTr("tags...")

        serviceName: "yandex-music"
        identity: "YaSailMusic"
        supportedUriSchemes: ["file"]
        supportedMimeTypes: ["audio/x-wav", "audio/x-vorbis+ogg", "audio/mpeg"]


        canSeek: true

        canControl: true

        canGoNext: true
        canGoPrevious: true
        canPause: true
        canPlay: true

        playbackStatus: (rootAudio.playbackState === MediaPlayer.PlayingState) ? Mpris.Playing : Mpris.Paused

        onArtistChanged: {
            var metadata = mprisPlayer.metadata
            metadata[Mpris.metadataToString(Mpris.Artist)] = [artist] // List of strings
            mprisPlayer.metadata = metadata
        }

        onSongChanged: {
            var metadata = mprisPlayer.metadata
            metadata[Mpris.metadataToString(Mpris.Title)] = song // String
            mprisPlayer.metadata = metadata
        }

        onPauseRequested: {
            rootAudio.pause()
        }
        onPlayRequested: {
            rootAudio.play()
        }
        onPlayPauseRequested: {
            if (rootAudio.playbackState === MediaPlayer.PlayingState) {
                rootAudio.pause()
            } else {
                rootAudio.play()
            }
        }
        onStopRequested: {
            rootAudio.stop()
        }

        onNextRequested: {
            ++playListModel.currentIndex
        }
        onPreviousRequested: {
            --playListModel.currentIndex
        }

    }


}
