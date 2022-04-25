#include "yamussdk.h"
#include "authorization.h"
#include "audio.h"
yamussdk::yamussdk(QObject *parent) : QObject(parent) {
    // basic:
    _api = new ApiRequest(this);
    _auth = new Authorization();
    qRegisterMetaType<Authorization*>("Authorization*");

    connect(_api, &ApiRequest::gotResponse, this, &yamussdk::gotResponse);
    // connect(_api, &ApiRequest::gotResponse, this, &yamussdk::gotResponse);
    //    connect(_api, &ApiRequest::gotResponse,
    //            [this] (QJsonValue value, ApiRequest::TaskType type) { QtConcurrent::run(this, &yamussdk::gotResponse, value, type); });
    //            [this] (QJsonValue value, ApiRequest::TaskType type) { gotResponse(value, type); });


    _audios = new Audios(this);
    _audios->setApi(_api);
    // requests:
    /*  _account = new Account(this);
    _audios = new Audios(this);
    _board = new Board(this);
    _friends = new Friends(this);
    _groups = new Groups(this);
    _likes = new Likes(this);
    _messages = new Messages(this);
    _newsfeed = new Newsfeed(this);
    _photos = new Photos(this);
    _stats = new Stats(this);
    _users = new Users(this);
    _videos = new Videos(this);
    _wall = new Wall(this);
//    _longPoll->setApi(_api);
    _account->setApi(_api);
    _audios->setApi(_api);
    _board->setApi(_api);
    _friends->setApi(_api);
    _groups->setApi(_api);
    _likes->setApi(_api);
    _messages->setApi(_api);
    _newsfeed->setApi(_api);
    _photos->setApi(_api);
    _stats->setApi(_api);
    _users->setApi(_api);
    _videos->setApi(_api);
    _wall->setApi(_api);
    qRegisterMetaType<LongPoll*>("LongPoll*");
    qRegisterMetaType<Account*>("Account*");
    qRegisterMetaType<Audios*>("Audios*");
    qRegisterMetaType<Board*>("Board*");
    qRegisterMetaType<Friends*>("Friends*");
    qRegisterMetaType<Groups*>("Groups*");
    qRegisterMetaType<Likes*>("Likes*");
    qRegisterMetaType<Messages*>("Messages*");
    qRegisterMetaType<Newsfeed*>("Newsfeed*");
    qRegisterMetaType<Photos*>("Photos*");
    qRegisterMetaType<Stats*>("Stats*");
    qRegisterMetaType<Users*>("Users*");
    qRegisterMetaType<Videos*>("Videos*");
    qRegisterMetaType<Wall*>("Wall*");*/

    // objects:
    /* qRegisterMetaType<Group*>("Group*");
    qRegisterMetaType<News*>("News*");
    qRegisterMetaType<User*>("User*");*/

    //models:
    /* _commentsModel = new CommentsModel(this);
    _dialogsListModel = new DialogsListModel(this);
    _friendsListModel = new FriendsListModel(this);
    _groupsListModel = new GroupsListModel(this);
    _messagesModel = new MessagesModel(this);
    _newsfeedModel = new NewsfeedModel(this);
    _wallModel = new NewsfeedModel(this);
    _photosModel = new PhotosModel(this);
    qRegisterMetaType<CommentsModel*>("CommentsModel*");
    qRegisterMetaType<DialogsListModel*>("DialogsListModel*");
    qRegisterMetaType<FriendsListModel*>("FriendsListModel*");
    qRegisterMetaType<GroupsListModel*>("GroupsListModel*");
    qRegisterMetaType<MessagesModel*>("MessagesModel*");
    qRegisterMetaType<NewsfeedModel*>("NewsfeedModel*");
    qRegisterMetaType<PhotosModel*>("PhotosModel*");*/


    //    qRegisterMetaType<Audio*>("Audio*");
    //    qRegisterMetaType<Document*>("Document*");
    //    qRegisterMetaType<Photo*>("Photo*");
    //    qRegisterMetaType<Friend*>("Friend*");
    //    qRegisterMetaType<Video*>("Video*");
}

yamussdk::~yamussdk() {
    delete _api;
    delete _auth;
    /* delete _longPoll;

    delete _account;
    delete _audios;
    delete _board;
    delete _friends;
    delete _groups;
    delete _likes;
    delete _messages;
    delete _newsfeed;
    delete _photos;
    delete _stats;
    delete _users;
    delete _videos;
    delete _wall;

    delete _commentsModel;
    delete _dialogsListModel;
    delete _friendsListModel;
    delete _groupsListModel;
    delete _messagesModel;
    delete _newsfeedModel;
    delete _wallModel;
    delete _photosModel;*/

    //    delete _selfProfile;
}

void yamussdk::setAccessTocken(QString value) {
    _api->setAccessToken(value);
    //_longPoll->setAccessToken(value);
}

bool yamussdk::checkToken(QString token) {
    Q_UNUSED(token);
    /* QUrl urll("https://api.vk.com/method/audio.get");
    QUrlQuery query;

    query.addQueryItem("count", "1");
    query.addQueryItem("access_token", token);
    query.addQueryItem("v", "5.153");
    urll.setQuery(query);
    QNetworkRequest request(urll);
    request.setRawHeader("User-Agent", "com.vk.vkclient/1654 (iPhone, iOS 12.2, iPhone8,4, Scale/2.0)");

    request.setRawHeader("Authorization", "Bearer "+token.toUtf8());
    qDebug () << request.rawHeader("Authorization");
    QNetworkAccessManager* _manager = new QNetworkAccessManager();
   // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    QNetworkReply *reply = _manager->get(request);
    QEventLoop looppp;
    QObject::connect(reply, SIGNAL(finished()) , &looppp, SLOT(quit()));
    looppp.exec();
     QByteArray dataaa = reply->readAll();
     QString DataAsString     = QString::fromUtf8(dataaa);
     qDebug() << DataAsString;
     if (DataAsString.contains("error")) {
         return false;
     } else if (DataAsString.contains("authorization failed")) {
         return false;
     } else {
         return true;
     }*/
    return true;
}

void yamussdk::setUserId(int value) {
    _userId = value;
}

Authorization *yamussdk::auth() const {
    return _auth;
}

Audios *yamussdk::audios() const {
    return _audios;
}

/*LongPoll *yamussdk::longPoll() const {
    return _longPoll;
}

Account *yamussdk::account() const {
    return _account;
}



Board *yamussdk::board() const {
    return _board;
}

Friends *yamussdk::friends() const {
    return _friends;
}

Groups *yamussdk::groups() const {
    return _groups;
}

Likes *yamussdk::likes() const {
    return _likes;
}

Messages *yamussdk::messages() const {
    return _messages;
}

Photos *yamussdk::photos() const {
    return _photos;
}

Stats *yamussdk::stats() const {
    return _stats;
}

Newsfeed *yamussdk::newsfeed() const {
    return _newsfeed;
}

Wall *yamussdk::wall() const {
    return _wall;
}

CommentsModel *yamussdk::commentsModel() const {
    return _commentsModel;
}

Users *yamussdk::users() const {
    return _users;
}

Videos *yamussdk::videos() const {
    return _videos;
}

DialogsListModel *yamussdk::dialogsListModel() const {
    return _dialogsListModel;
}

FriendsListModel *yamussdk::friendsListModel() const {
    return _friendsListModel;
}

GroupsListModel *yamussdk::groupsListModel() const {
    return _groupsListModel;
}

MessagesModel *yamussdk::messagesModel() const {
    return _messagesModel;
}

NewsfeedModel* yamussdk::newsfeedModel() const {
    return _newsfeedModel;
}

NewsfeedModel *yamussdk::wallModel() const {
    return _wallModel;
}

PhotosModel *yamussdk::photosModel() const {
    return _photosModel;
}

void yamussdk::attachPhotoToMessage(QString path) {
    _pathToPhoto = path;
    _photos->getMessagesUploadServer();
}*/

void yamussdk::gotResponse(const QJsonValue &value, ApiRequest::TaskType type) {
    switch (type) {
    /* case ApiRequest::ACCOUNT_BAN_USER:
        emit banSettingChanged(true);
        break;
    case ApiRequest::ACCOUNT_UNBAN_USER:
        emit banSettingChanged(false);
        break;*/
    case ApiRequest::AUDIO_GET:
    case ApiRequest::AUDIO_SEARCH:
        parseAudiosList(value.toObject().value("sequence").toArray());
        break;
        /* case ApiRequest::BOARD_GET_TOPICS:
        parseTopicsList(value.toObject().value("items").toArray());
        break;
    case ApiRequest::FRIENDS_GET:
        parseEntireFriendsList(value.toObject().value("items").toArray());
        break;
    case ApiRequest::FRIENDS_GET_MUTUAL:
    case ApiRequest::FRIENDS_GET_ONLINE:
        parseLimitedFriendsList(value.toArray());
        break;
    case ApiRequest::GROUPS_GET:
        parseGroupsList(value.toObject().value("items").toArray());
        break;
    case ApiRequest::GROUPS_GET_BY_ID:
        emit gotGroup(Group::fromJsonObject(value.toArray().at(0).toObject()));
        break;
    case ApiRequest::MESSAGES_GET_BY_ID:
        parseNewMessage(value.toObject().value("items").toArray().at(0).toObject());
        break;
    case ApiRequest::MESSAGES_GET_CHAT:
        parseChatsInfo(value.toArray());
        break;
    case ApiRequest::MESSAGES_GET_DIALOGS:
        parseDialogsInfo(value.toObject());
        break;
    case ApiRequest::MESSAGES_GET_HISTORY:
        parseMessages(value.toObject().value("items").toArray());
        break;
    case ApiRequest::PHOTOS_GET_ALBUMS:
        parsePhotoAlbums(value.toObject().value("items").toArray());
        break;
    case ApiRequest::PHOTOS_GET:
    case ApiRequest::PHOTOS_GET_ALL:
        parsePhotosList(value.toObject());
        break;
    case ApiRequest::PHOTOS_GET_MESSAGES_UPLOAD_SERVER:
        parseUploadServerData(value.toObject());
        break;
    case ApiRequest::PHOTOS_SAVE_MESSAGES_PHOTO:
        parseSavedPhotoData(value.toArray());
        break;
    case ApiRequest::PHOTOS_UPLOAD_TO_SERVER:
        parseUploadedPhotoData(value.toObject());
        break;
    case ApiRequest::STATS_GET:
        parseStatistics(value.toArray());
        break;
    case ApiRequest::NEWSFEED_GET:
        parseNewsfeed(value.toObject(), false);
        break;
    case ApiRequest::USERS_GET:
        if (_messagePreview.isEmpty()) emit gotProfile(parseUserProfile(value.toArray()));
        else {
            emit gotNewMessage(QString("%1 %2").arg(parseUserProfile(value.toArray())->getFirstName())
                                               .arg(parseUserProfile(value.toArray())->getLastName()),
                               _messagePreview);
            _messagePreview.clear();
        }
        break;
    case ApiRequest::USERS_GET_FRIENDS:
        parseFriendsInfo(value.toArray());
        break;
    case ApiRequest::VIDEO_GET:
        emit gotVideo(parseVideoInfo(value.toObject().value("items").toArray()));
        break;
    case ApiRequest::BOARD_CREATE_COMMENT:
    case ApiRequest::WALL_CREATE_COMMENT:
        emit commentCreated();
        break;
    case ApiRequest::WALL_GET:
        parseNewsfeed(value.toObject(), true);
        break;
    case ApiRequest::WALL_GET_BY_ID:
        emit gotWallpost(parseWallpost(value.toObject().value("items").toArray()));
        break;
    case ApiRequest::BOARD_GET_COMMENTS:
    case ApiRequest::WALL_GET_COMMENTS:
        parseComments(value.toObject());
        break;*/
    default:
        break;
    }
}

/*void yamussdk::_gotNewMessage(int id) {
    _messages->getById(id);
}

void yamussdk::_readMessages(qint64 peerId, qint64 localId, bool out) {
    _dialogsListModel->readMessages(peerId, localId, out);
    _messagesModel->readMessages(peerId, localId, out);
}

void yamussdk::_userTyping(qint64 userId, qint64 chatId) {
    emit userTyping(userId, chatId);
}*/

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

/*void yamussdk::parseChatsInfo(QJsonArray array) {
    //_chatsIds.clear();
    for (int index = 0; index < array.size(); ++index) {
        Chat *chat = Chat::fromJsonObject(array.at(index).toObject());
        if (chat->users().size()>0) {
        foreach (QVariant user, chat->users()) _usersIds.append(user.toString());
        }

        //foreach (QVariant user, chat->users()) _usersIds.append(user.toString());

        _dialogsListModel->addChat(chat);
    }
    if (array.size()>0) {
    _usersIds.removeDuplicates();
    _users->getUsersByIds(_usersIds);
    }
}

void yamussdk::parseComments(QJsonObject object) {
    QJsonArray comments = object.value("items").toArray();
    QJsonArray profiles = object.value("profiles").toArray();
    QJsonArray groups = object.value("groups").toArray();
    for (int index = 0; index < comments.size(); ++index) {
        Comment *comment = Comment::fromJsonObject(comments.at(index).toObject());
        _commentsModel->addComment(comment);
    }
    for (int index = 0; index < profiles.size(); ++index) {
        User *profile = User::fromJsonObject(profiles.at(index).toObject());
        _commentsModel->addUser(profile);
    }
    for (int index = 0; index < groups.size(); ++index) {
        Group *group = Group::fromJsonObject(groups.at(index).toObject());
        _commentsModel->addGroup(group);
    }
}

void yamussdk::parseDialogsInfo(QJsonObject object) {
    if (object.contains("unread_count")) emit gotUnreadCounter(object.value("unread_count").toInt());
    parseFriendsInfo(object.value("profiles").toArray());
    parseGroupsInfo(object.value("groups").toArray());
    QJsonArray dialogs = object.value("items").toArray();
    for (int index = 0; index < dialogs.size(); ++index) {
        Dialog *dialog = Dialog::fromJsonObject(dialogs.at(index).toObject());
        if (dialog->isChat()) _chatsIds.append(QString::number(dialog->lastMessage()->chatId() - 2000000000));
        else _usersIds.append(QString::number(dialog->lastMessage()->peerId()));
        _dialogsListModel->add(dialog);
    }
   if (_chatsIds.empty()) {
       //if (!(_usersIds.empty())) {
        _usersIds.removeDuplicates();

        _users->getUsersByIds(_usersIds);


         //}
    } else _messages->getChat(_chatsIds);
}

void yamussdk::parseEntireFriendsList(QJsonArray array) {
    for (int index = 0; index < array.size(); ++index) {
        Friend *profile = Friend::fromJsonObject(array.at(index).toObject(), false);
        _friendsListModel->add(profile);
    }
}

void yamussdk::parseFriendsInfo(QJsonArray array) {
    if (!_usersIds.empty()) {
        _usersIds.clear();
        for (int index = 0; index < array.size(); ++index) {
            Friend *profile = Friend::fromJsonObject(array.at(index).toObject(), false);
            _dialogsListModel->addProfile(profile);
        }
    } else if (!_chatUsersIds.empty()) {
        _chatUsersIds.clear();
        for (int index = 0; index < array.size(); ++index) {
            Friend *profile = Friend::fromJsonObject(array.at(index).toObject(), false);
            _messagesModel->addProfile(profile);
        }
    } else {
        for (int index = 0; index < array.size(); ++index) {
            Friend *profile = Friend::fromJsonObject(array.at(index).toObject(), false);
            _friendsListModel->add(profile);
        }
    }
}

void yamussdk::parseGroupsInfo(QJsonArray array) {

    if (array.size() == 0) return;
    foreach (QJsonValue value, array) {
        _dialogsListModel->addProfile(Friend::fromJsonObject(value.toObject(), true));
    }

}


void yamussdk::parseGroupsList(QJsonArray array) {
    if (array.size() == 0) return;
    foreach (QJsonValue value, array) {
        _groupsListModel->add(Group::fromJsonObject(value.toObject()));
    }
}

void yamussdk::parseLimitedFriendsList(QJsonArray array) {
    QStringList ids;
    for (int index = 0; index < array.size(); ++index) {
        ids.append(QString::number(array.at(index).toInt()));
    }
    _users->getUsersByIds(ids);
}

void yamussdk::parseMessages(QJsonArray array) {
    for (int index = 0; index < array.size(); ++index) {
        Message *message = Message::fromJsonObject(array.at(index).toObject());
        _chatUsersIds.append(QString::number(message->fromId()));
        foreach (QObject *object, message->fwdMessagesList()) {
            Message *fwd = qobject_cast<Message*>(object);
            _chatUsersIds.append(QString::number(fwd->fromId()));
        }
        _messagesModel->add(message);
    };
    _chatUsersIds.removeDuplicates();
    _users->getUsersByIds(_chatUsersIds);
}




//User *yamussdk::selfProfile() const {
//    return _selfProfile;
//}

//void yamussdk::gotFriendsList(QList<QObject *> friendsList) {
//    emit gotFriends(QVariant::fromValue(friendsList));
//}

//void yamussdk::gotMessagesList(QList<QObject *> messagesList) {
//    foreach (QObject *object, messagesList) {
//        Message *message = qobject_cast<Message*>(object);
//        _messagesModel->add(message);
//        _chatUsersIds.append(QString("%1").arg(message->fromId()));
//    }
//    _users->get(_chatUsersIds);
//}

//void yamussdk::gotMutualFriendsIds(QVariantList ids) {
//    QStringList sIds;
//    foreach (QVariant id, ids) sIds.append(id.toString());
//    _users->get(sIds);
//}

//void yamussdk::gotNewsfeed(QList<News *> items, QList<User *> profiles, QList<Group *> groups, QString nextFrom)
//{
//    foreach (News *item, items) _newsfeedModel->addNews(item);
//    foreach (User *user, profiles) _newsfeedModel->addUser(user);
//    foreach (Group *group, groups) _newsfeedModel->addGroup(group);
//    _newsfeedModel->setNextFrom(nextFrom);
//    //    emit newsfeedModelChanged();
//}

//void yamussdk::gotUnreadDialogsCounter(int value) {
//    emit gotUnreadCounter(value);
//}

//void yamussdk::gotUserProfile(User *user) {
//    if (user->id() == _userId) {
//        _selfProfile = user;
//        emit gotSelfProfile();
//    } else emit gotProfile(user);
//}

//void yamussdk::gotUsersList(QList<QObject *> usersList) {
//    if (_usersIds.size() > 0) {
//        _usersIds.clear();
//        foreach (QObject *object, usersList) {
//            Friend *user = qobject_cast<Friend*>(object);
//            _dialogsListModel->addProfile(user);
//        }
//    } else if (_chatUsersIds.size() > 0) {
//        _chatUsersIds.clear();
//        foreach (QObject *object, usersList) {
//            Friend *user = qobject_cast<Friend*>(object);
//            _messagesModel->addProfile(user);
//        }
//    } else emit gotFriends(QVariant::fromValue(usersList));
//}

//void yamussdk::gotVideoObject(Video *video)
//{
//    emit gotVideo(video);
//}

//void yamussdk::gotWallpostObject(News *wallpost)
//{
//    emit gotWallpost(wallpost);
//}

//void yamussdk::gotChatsList(QList<QObject *> chatsList) {
//    _chatsIds.clear();
//    foreach (QObject *object, chatsList) {
//        Chat *chat = qobject_cast<Chat*>(object);
//        foreach (QVariant user, chat->users()) {
//            if (!_usersIds.contains(user.toString())) _usersIds.append(user.toString());
//        }
//        _dialogsListModel->addChat(chat);
//    }
//    _users->get(_usersIds);
//}

//void yamussdk::gotDialogList(QList<Dialog *> dialogsList) {
//    foreach (Dialog *dialog, dialogsList) {
//        _dialogsListModel->add(dialog);
//        if (dialog->isChat()) _chatsIds.append(QString("%1").arg(dialog->lastMessage()->chatId()));
//        else _usersIds.append(QString("%1").arg(dialog->lastMessage()->userId()));
//    }
//    if (_chatsIds.empty()) _users->get(_usersIds);
//    else _messages->getChat(_chatsIds);
//}

//QStringList yamussdk::_getIdsFromMessages(QList<QObject *> messages) {
//    QStringList ids;
//    for (int index = 0; index < messages.size(); ++index) {
//        Message *msg = qobject_cast<Message*>(messages.at(index));
//        QString id = QString("%1").arg(msg->fromId());
//        if (!ids.contains(id) && id != "0") {
//            ids.append(id);
//        }
//        if (!msg->fwdMessagesList().isEmpty()) ids += _getIdsFromMessages(msg->fwdMessagesList());
//    }
//    return ids;
//}
*/
