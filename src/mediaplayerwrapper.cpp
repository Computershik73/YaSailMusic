#include "mediaplayerwrapper.h"
#include <QMediaPlayer>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "audio.h"
#include "audios.h"
#include "settingswrapper.h"
#include <QXmlStreamReader>

MediaPlayerWrapper::MediaPlayerWrapper(QObject *parent) : QObject(parent) {
    qsrand(time(NULL));
    _player = new QMediaPlayer(this);
    _player->stop();
    _model = new PlaylistModel(this);
    _settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
                              "/org.ilyavysotsky/YaSailMusic/YaSailMusic.conf", QSettings::NativeFormat);
    connect(_player, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(_mediaChanged(QMediaContent)));
    connect(_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(_stateChanged(QMediaPlayer::State)));
    connect(_player, SIGNAL(positionChanged(qint64)), this, SLOT(_positionChanged(qint64)));
    qRegisterMetaType<PlaylistModel*>("PlaylistModel*");
}

MediaPlayerWrapper::~MediaPlayerWrapper() {
    delete _player;
    delete _model;
}

void MediaPlayerWrapper::setPlaylist(QVariantList audios, int index) {
    _audios.clear();
    _model->clear();
    QMediaPlaylist *_playlist = new QMediaPlaylist();
    _playlist->setPlaybackMode(QMediaPlaylist::Loop);
    foreach (QVariant audio, audios) {
        Audio *_audio = (Audio*)audio.value<QObject*>();
        _playlist->addMedia(QUrl(_audio->url()));
        _audios.append(_audio);
        _model->add(_audio);
    }
    _player->setPlaylist(_playlist);
    /*if (index != -1) {
        _playlist->setCurrentIndex(index);
        _model->setPlayingIndex(index);
        _player->play();
    }*/
}




void MediaPlayerWrapper::play() {
    if (!(_audios.at(_player->playlist()->currentIndex())->url().contains("get-mp3"))) {
        QUrl urll = _audios.at(_player->playlist()->currentIndex())->url();
        QString trueurl = urll.toString().trimmed();
        urll = QUrl(trueurl);
        qDebug() << "QUrl: " << urll.toString();
            QNetworkRequest request(urll);
            request.setRawHeader("Authorization", ("OAuth "+(_settings->value("accessToken").toString())).toUtf8());
            request.setRawHeader("X-Yandex-Music-Content-Type", "adult");
            request.setRawHeader("X-Yandex-Music-Device", "os=iOS; os_version=12.2; manufacturer=Apple; model=iPhone8,4; clid=0; device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA; uuid=70db875aae45466dbe932244c10a62c1");
            request.setRawHeader("User-Agent", "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
            request.setRawHeader("X-Yandex-Music-Client", "YandexMusic/519");
             request.setRawHeader("Ya-Client-User-Agent", "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
              request.setRawHeader("Cookie", "_yasc=jIPYMjq4BwfgL8w1zfZDfFTNIxqinePslHoDbyatIhhu7A==");

            QNetworkAccessManager* _manager = new QNetworkAccessManager(this);
           // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
            QNetworkReply *reply = _manager->get(request);
            QEventLoop looppp;
            QObject::connect(reply, SIGNAL(finished()) , &looppp, SLOT(quit()));
            looppp.exec();
             QByteArray dataaa = reply->readAll();
             QString DataAsString     = QString::fromUtf8(dataaa);
             qDebug() << DataAsString;
             QJsonDocument jDoc = QJsonDocument::fromJson(dataaa);
             QJsonObject jObj = jDoc.object();
              QJsonArray res = jObj.value("result").toArray();
              QJsonObject downloadInfos = res.at(0).toObject();
              QString downloadInfoUrl = downloadInfos.value("downloadInfoUrl").toString();

              QUrl url2(downloadInfoUrl);
              QString trueurl2 = url2.toString().trimmed();
              url2 = QUrl(trueurl2);
                  qDebug() << "QUrl2: " << url2.toString();
                  QNetworkRequest request2(url2);
                  //request2.setRawHeader("Authorization", ("OAuth "+(_settings->value("accessToken").toString())).toUtf8());
                  request2.setRawHeader("X-Yandex-Music-Content-Type", "adult");
                  request2.setRawHeader("X-Yandex-Music-Device", "os=iOS; os_version=12.2; manufacturer=Apple; model=iPhone8,4; clid=0; device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA; uuid=70db875aae45466dbe932244c10a62c1");
                  request2.setRawHeader("user-agent", "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
                  request2.setRawHeader("x-yandex-music-client", "YandexMusic/519");
                   request2.setRawHeader("ya-client-user-agent", "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
                    request2.setRawHeader("cookie", "_yasc=jIPYMjq4BwfgL8w1zfZDfFTNIxqinePslHoDbyatIhhu7A==");


                 // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
                 QNetworkAccessManager* _manager2 = new QNetworkAccessManager(this);
                    QNetworkReply *reply2 = _manager2->get(request2);
                  QEventLoop looppp2;
                  QObject::connect(reply2, SIGNAL(finished()) , &looppp2, SLOT(quit()));
                  looppp2.exec();
                   QByteArray dataaa2 = reply2->readAll();
                   QString DataAsString2     = QString::fromUtf8(dataaa2);

                   qDebug() << "result2: " << DataAsString2;
                   QString host = "";
                    QString path = "";
                     QString ts = "";
                      QString region = "";
                       QString s = "";

                   QXmlStreamReader reader(DataAsString2);
                   while(!reader.atEnd() && !reader.hasError()) {
                       if(reader.readNext() == QXmlStreamReader::StartElement) {
                           if (reader.name() == "host") {
                           host = reader.readElementText();
                            }
                           if (reader.name() == "path") {
                           path =reader.readElementText();
                            }
                           if (reader.name() == "ts") {
                           ts = reader.readElementText();
                            }
                           if (reader.name() == "region") {
                           region = reader.readElementText();
                            }
                           if (reader.name() == "s") {
                           s = reader.readElementText();
                            }
                       }

                   }
        qDebug() << " " << host << " " << path << ts << region << s << " ";
        QString  sign  = QString(QCryptographicHash::hash((("XGRlBW9FXlekgbPrRHuSiA" + path.mid(1) + s).toUtf8()),QCryptographicHash::Md5).toHex());
        QString finalUrl = "https://"+host+"/get-mp3/"+sign+"/"+ts+path;
        //QString  sign = md5(("XGRlBW9FXlekgbPrRHuSiA" + path.left(1) + s).toUtf8()).hexdigest();
        _audios.at(_player->playlist()->currentIndex())->setUrl(finalUrl);
        int cur = _player->playlist()->currentIndex();
        _player->playlist()->removeMedia(cur);
        _player->playlist()->insertMedia(cur, QUrl(finalUrl));
        _player->playlist()->setCurrentIndex(cur);
        QModelIndex index = _model->index(_player->playlist()->currentIndex());
        _model->setData(index, _audios.at(_player->playlist()->currentIndex())->url(), _model->UrlRole);

    }
    _player->play();
}

void MediaPlayerWrapper::pause() {
    _player->pause();
}

void MediaPlayerWrapper::next() {
    if (_shuffle) _player->playlist()->setCurrentIndex(qrand() % _audios.size());
    else {
        if (_player->playlist()->currentIndex()<=_audios.size()-1) {
        _player->playlist()->next();
        }
    }
}

void MediaPlayerWrapper::prev() {
    if (_shuffle) _player->playlist()->setCurrentIndex(qrand() % _audios.size());
    else _player->playlist()->previous();
}

void MediaPlayerWrapper::seekTo(int value) {
    _player->setPosition(value * 1000);
}

void MediaPlayerWrapper::jumpTo(int index) {
    _player->playlist()->setCurrentIndex(index);
}

qint64 MediaPlayerWrapper::position() {
    return _player->position();
}

int MediaPlayerWrapper::currentIndex() const {
    if (_player->playlist() == NULL) return -1;
    return _player->playlist()->currentIndex();
}

int MediaPlayerWrapper::size() const {
    return _audios.size();
}

qint64 MediaPlayerWrapper::duration() {
    if (_player->playlist() == NULL) return 0;
    return _audios.at(_player->playlist()->currentIndex())->duration();
}

QString MediaPlayerWrapper::author() {
    if (_player->playlist() == NULL) return "";
    return _audios.at(_player->playlist()->currentIndex())->artist();
}

QString MediaPlayerWrapper::title() {
    if (_player->playlist() == NULL) return "";
    return _audios.at(_player->playlist()->currentIndex())->title();
}

qint64 MediaPlayerWrapper::ownerId() {
    return _audios.at(_player->playlist()->currentIndex())->ownerId();
}

qint64 MediaPlayerWrapper::audioId() {
    return _audios.at(_player->playlist()->currentIndex())->id();
}

void MediaPlayerWrapper::_mediaChanged(QMediaContent content) {
    /*if (!((_player==NULL) || (_player->playlist()==NULL) || (content==NULL))) {
    if ((repeat()) || (_player->playlist()->currentIndex()>=_audios.size()-1)) {
        _player->playlist()->setCurrentIndex(_currIndex);
        emit mediaChanged();
    }
    else {
        if (_shuffle && _shuffleNow) {
            _shuffleNow = false;
            _player->playlist()->setCurrentIndex(qrand() % _audios.size());
        } else _shuffleNow = true;
        _currIndex = _player->playlist()->currentIndex();
        emit mediaChanged();
    }
    }*/
     emit mediaChanged();
}

void MediaPlayerWrapper::_positionChanged(qint64 pos) {
    emit positionChanged(pos);
}

void MediaPlayerWrapper::_stateChanged(QMediaPlayer::State state) {
    /*if (state == QMediaPlayer::State::StoppedState)
            {
                _player->play();
            }*/
    //_player->play();
    emit stateChanged();
}

bool MediaPlayerWrapper::repeat() const
{
    return _repeat;
}

void MediaPlayerWrapper::setRepeat(bool repeat)
{
    _repeat = repeat;
}

bool MediaPlayerWrapper::shuffle() const
{
    return _shuffle;
}

void MediaPlayerWrapper::setShuffle(bool shuffle)
{
    _shuffle = shuffle;
    _shuffleNow = true;
}

PlaylistModel *MediaPlayerWrapper::model() const {
    return _model;
}

bool MediaPlayerWrapper::isPlaying() const {
    return _player->state() == QMediaPlayer::PlayingState;
}

bool MediaPlayerWrapper::isPaused() const {
    return _player->state() == QMediaPlayer::PausedState;
}

bool MediaPlayerWrapper::isStopped() const {
    return _player->state() == QMediaPlayer::StoppedState;
}

