#ifndef YAMUSSDK_H
#define YAMUSSDK_H
#include <QtConcurrent/QtConcurrent>
#include <QObject>

#include "authorization.h"
#include "audios.h"
/*#include "longpoll.h"
#include "objects/group.h"
#include "objects/news.h"
#include "objects/user.h"
#include "objects/video.h"
#include "models/commentsmodel.h"
#include "models/dialogslistmodel.h"
#include "models/friendslistmodel.h"
#include "models/groupslistmodel.h"
#include "models/messagesmodel.h"
#include "models/newsfeedmodel.h"
#include "models/photosmodel.h"
#include "requests/account.h"
#include "requests/apirequest.h"
#include "requests/audios.h"
#include "requests/board.h"
#include "requests/friends.h"
#include "requests/groups.h"
#include "requests/likes.h"
#include "requests/messages.h"
#include "requests/newsfeed.h"
#include "requests/photos.h"
#include "requests/stats.h"
#include "requests/users.h"
#include "requests/videos.h"
#include "requests/wall.h"*/

#include "audio.h"
//#include "objects/chat.h"
//#include "objects/dialog.h"
//#include "objects/document.h"
//#include "objects/group.h"
//#include "objects/photo.h"
//#include "objects/friend.h"

class yamussdk : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Authorization* auth READ auth CONSTANT)
    Q_PROPERTY(Audios* audios READ audios CONSTANT)

public:
    explicit yamussdk(QObject *parent = 0);
    ~yamussdk();

    Q_INVOKABLE void loadAuthdata();

    Authorization *auth() const;
    Audios* audios() const;

signals:
    void gotUserAudios(QVariantList audios);

public slots:
    void gotResponse(const QJsonValue &value, ApiRequest::TaskType type);

private:
    QString _accessToken;
    int _userId;

    ApiRequest *_api;
    Authorization *_auth;
    Audios *_audios;

    void parseAudiosList(QJsonArray array);
};

#endif // YAMUSSDK_H
