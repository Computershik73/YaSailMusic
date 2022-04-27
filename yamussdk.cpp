#include "yamussdk.h"
#include "authorization.h"

yamussdk::yamussdk(QObject *parent) : QObject(parent) {
    _auth = new Authorization();
    qRegisterMetaType<Authorization*>("Authorization*");
}

yamussdk::~yamussdk() {
    delete _auth;
}

void yamussdk::loadAuthdata() {
    QSettings settings;
    _userId = settings.value("userId").toInt();
}

Authorization *yamussdk::auth() const {
    return _auth;
}

