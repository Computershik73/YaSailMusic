#ifndef YAMUSSDK_H
#define YAMUSSDK_H
#include <QtConcurrent/QtConcurrent>
#include <QObject>

#include "authorization.h"
#include "audio.h"

class yamussdk : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Authorization* auth READ auth CONSTANT)

public:
    explicit yamussdk(QObject *parent = 0);
    ~yamussdk();

    Q_INVOKABLE void loadAuthdata();

    Authorization *auth() const;

signals:

public slots:

private:
    int _userId;
    Authorization *_auth;

    void parseAudiosList(QJsonArray array);
};

#endif // YAMUSSDK_H
