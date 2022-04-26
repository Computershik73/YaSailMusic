#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class Authorization : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString authUrl READ authUrl CONSTANT)

public:
    explicit Authorization(QObject *parent = 0);
    ~Authorization();

    QString authUrl();

    Q_INVOKABLE void tryToGetAccessToken(QString name, QString password, QString code);

    Q_INVOKABLE bool codeisrequired;
QNetworkAccessManager* _manager;
public slots:
   // void finished(QNetworkReply *reply);
signals:
    void authorized(QString accessToken, int userId);
    void error(QString errorMessage);

   void coderequired();
};

#endif // AUTHORIZATION_H
