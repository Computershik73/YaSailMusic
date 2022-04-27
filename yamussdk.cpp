#include "yamussdk.h"
#include "authorization.h"
#include "audio.h"
yamussdk::yamussdk(QObject *parent) : QObject(parent) {
    // basic:
    _api = new ApiRequest(this);
    _auth = new Authorization();
    qRegisterMetaType<Authorization*>("Authorization*");

    connect(_api, &ApiRequest::gotResponse, this, &yamussdk::gotResponse);

    _audios = new Audios(this);
    _audios->setApi(_api);
}

yamussdk::~yamussdk() {
    delete _api;
    delete _auth;
}

void yamussdk::loadAuthdata() {
    QSettings settings;
    _userId = settings.value("userId").toInt();
}

Authorization *yamussdk::auth() const {
    return _auth;
}

Audios *yamussdk::audios() const {
    return _audios;
}

void yamussdk::gotResponse(const QJsonValue &value, ApiRequest::TaskType type) {
    switch (type) {
    case ApiRequest::AUDIO_GET:
    case ApiRequest::AUDIO_SEARCH:
        parseAudiosList(value.toObject().value("sequence").toArray());
        break;
    default:
        break;
    }
}

void yamussdk::parseAudiosList(QJsonArray array) {
    if (array.size() > 0) {
        QVariantList audios;
        for (int index = 0; index < array.size(); ++index) {
            QJsonObject seqi = array.at(index).toObject();
            QJsonObject track = seqi.value("track").toObject();
            audios.append(QVariant::fromValue(Audio::fromJsonObject(track)));
        }
        emit gotUserAudios(audios);
    }
}
