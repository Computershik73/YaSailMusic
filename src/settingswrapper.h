#ifndef SETTINGSWRAPPER_H
#define SETTINGSWRAPPER_H

#include <QObject>
#include <QSettings>
#include <QStandardPaths>

#include <QDebug>

class SettingsWrapper : public QObject
{
    Q_OBJECT

public:
    explicit SettingsWrapper(QObject *parent = 0);
    ~SettingsWrapper();

    Q_INVOKABLE void setAccessToken(QString value);
    Q_INVOKABLE void setUserId(int value);
    Q_INVOKABLE void setDefaultPage(QString value);
    Q_INVOKABLE void incrementAttachmentsHintCounter();

    Q_INVOKABLE QString accessToken();
    Q_INVOKABLE int userId();
    Q_INVOKABLE QString defaultPage();
    Q_INVOKABLE int attachmentsHintCounter();

    Q_INVOKABLE void removeAccessToken();
    Q_INVOKABLE void removeUserId();

    Q_INVOKABLE void setOfflineStatus(bool offline);
    Q_INVOKABLE bool offlineStatus();

private:
    QSettings *_settings;

    const QString ACCESS_TOKEN_KEY = "accessToken";
    const QString USER_ID_KEY = "userId";
    const QString DEFAULT_PAGE_KEY = "defaultPage";
    const QString ATTACHMENTS_HINT = "attachmentsHint";

    const QString OFFLINE_STATUS_KEY = "offlineStatus";
};

#endif // SETTINGSWRAPPER_H
