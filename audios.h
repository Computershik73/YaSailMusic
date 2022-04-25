#ifndef AUDIOS_H
#define AUDIOS_H

#include "requestbase.h"

class Audios : public RequestBase
{
    Q_OBJECT
public:
    explicit Audios(QObject *parent = 0);

    Q_INVOKABLE void add(qint64 ownerId, qint64 audioId);
    Q_INVOKABLE void get();
    Q_INVOKABLE void getRecommendations(int count = 50);
    Q_INVOKABLE void search(QString query);
};

#endif // AUDIOS_H
