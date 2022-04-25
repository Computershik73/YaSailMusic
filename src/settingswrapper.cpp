

#include "settingswrapper.h"

SettingsWrapper::SettingsWrapper(QObject *parent) : QObject(parent) {
    _settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
                              "/org.ilyavysotsky/YaSailMusic/YaSailMusic.conf", QSettings::NativeFormat);
}

SettingsWrapper::~SettingsWrapper() {
    delete _settings;
}

void SettingsWrapper::setAccessToken(QString value) {
    _settings->setValue(ACCESS_TOKEN_KEY, value);
}

void SettingsWrapper::setUserId(int value) {
    _settings->setValue(USER_ID_KEY, value);
}

void SettingsWrapper::setDefaultPage(QString value) {
    _settings->setValue(DEFAULT_PAGE_KEY, value);
}

void SettingsWrapper::incrementAttachmentsHintCounter() {
    _settings->setValue(ATTACHMENTS_HINT, attachmentsHintCounter() + 1);
}

QString SettingsWrapper::accessToken() {
    return _settings->value(ACCESS_TOKEN_KEY).toString();
}

int SettingsWrapper::userId() {
    return _settings->value(USER_ID_KEY).toInt();
}

QString SettingsWrapper::defaultPage() {
    return _settings->value(DEFAULT_PAGE_KEY).toString();
}

int SettingsWrapper::attachmentsHintCounter() {
    return _settings->value(ATTACHMENTS_HINT, 0).toInt();
}

void SettingsWrapper::removeAccessToken() {
    _settings->remove(ACCESS_TOKEN_KEY);
}

void SettingsWrapper::removeUserId() {
    _settings->remove(USER_ID_KEY);
}

void SettingsWrapper::setOfflineStatus(bool offline) {
    _settings->setValue(OFFLINE_STATUS_KEY, offline);
}

bool SettingsWrapper::offlineStatus() {
    return _settings->value(OFFLINE_STATUS_KEY).toBool();
}
