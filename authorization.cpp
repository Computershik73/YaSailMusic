
#include "authorization.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

Authorization::Authorization(QObject *parent) : QObject(parent)
{
    codeisrequired=false;
    _manager = new QNetworkAccessManager(this);
    // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

Authorization::~Authorization()
{}

/**
 * @brief The function builds URL for authorization.
 * @return The URL for authorization.
 */
QString Authorization::authUrl() {
    QUrl url("https://oauth.vk.com/authorize");
    QUrlQuery query;
    query.addQueryItem("client_id", "3140623");
    //"4803503");
    query.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");
    query.addQueryItem("display", "mobile");
    query.addQueryItem("scope", "friends,photos,audio,video,docs,notes,pages,status,wall,groups,messages,notifications,offline,stats");
    query.addQueryItem("response_type", "token");
    query.addQueryItem("v", "5.93");
    query.addQueryItem("revoke", "1");
    url.setQuery(query);
    return url.toString();
}



/**
 * @brief The function checks is a current URL is final. If true, get access token and user id
 *        or error code and error message.
 * @param url - URL for checking.
 */
void Authorization::tryToGetAccessToken(QString name, QString password, QString code) {
    //QStringList w=namepass.split(" ", QString::SkipEmptyParts);
    QUrl urll("https://mobileproxy.passport.yandex.net/2/bundle/mobile/start/?app_id=ru.yandex.mobile.music&uuid=70db875aae45466dbe932244c10a62c1&app_version_name=5.19&ifv=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA&am_version_name=5.151&deviceidhash=11387416356007369345&manufacturer=Apple&deviceid=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA&device_name=iPhone%20User&device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA&app_platform=iPhone&model=iPhone8%2C4");
    QUrlQuery query;
    query.addQueryItem("client_id", "c0ebe342af7d48fbbbfcf2d2eedb8f9e");
    query.addQueryItem("client_secret", "ad0a908f0aa341a182a37ecd75bc319e");
    query.addQueryItem("x_token_client_id", "c0ebe342af7d48fbbbfcf2d2eedb8f9e");
    query.addQueryItem("x_token_client_secret", "ad0a908f0aa341a182a37ecd75bc319e");
    query.addQueryItem("сщщлшу", "VeWdmVclDCtn6ihuP1nt");
    query.addQueryItem("login", name);
    query.addQueryItem("payment_auth_retpath", "yandexmusic://am/payment_auth");
    query.addQueryItem("display_language", "ru");
    qDebug() << "URL: " << urll.toString();
    //urll.setQuery(query);
    QNetworkRequest request(urll);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", "com.yandex.mobile.auth.sdk/5.151.69098 (Apple iPhone8,4; iOS 12.2)");
    request.setRawHeader("Cookie", "_yasc=jIPYMjq4BwfgL8w1zfZDfFTNIxqinePslHoDbyatIhhu7A==");

    QNetworkReply *reply = _manager->post(request, query.toString(QUrl::FullyEncoded).toUtf8());


    // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    QEventLoop looppp;
    QObject::connect(reply, SIGNAL(finished()) , &looppp, SLOT(quit()));
    looppp.exec();
    QByteArray dataaa = reply->readAll();
    QString DataAsString     = QString::fromUtf8(dataaa);
    qDebug() << " 1st step: " << DataAsString;
    //if (DataAsString.contains("error")) {
    QJsonDocument jDoc = QJsonDocument::fromJson(dataaa);
    QJsonObject jObj = jDoc.object();
    QString track_id = jObj.value("track_id").toString();


    QUrl url2("https://mobileproxy.passport.yandex.net/1/bundle/mobile/auth/password/");
    QUrlQuery query2;

    query2.addQueryItem("password", password);
    query2.addQueryItem("track_id", track_id);


    //url2.setQuery(query2);
    QNetworkRequest request2(url2);
    request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request2.setRawHeader("User-Agent", "com.yandex.mobile.auth.sdk/5.151.69098 (Apple iPhone8,4; iOS 12.2)");
    request.setRawHeader("Cookie", "_yasc=jIPYMjq4BwfgL8w1zfZDfFTNIxqinePslHoDbyatIhhu7A==");
    QNetworkReply *reply2 = _manager->post(request2, query2.toString(QUrl::FullyEncoded).toUtf8());


    // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    QEventLoop looppp2;
    QObject::connect(reply2, SIGNAL(finished()) , &looppp2, SLOT(quit()));
    looppp2.exec();
    QByteArray dataaa2 = reply2->readAll();
    QString DataAsString2     = QString::fromUtf8(dataaa2);
    qDebug() << " 2nd step: " << DataAsString2;
    //if (DataAsString.contains("error")) {
    QJsonDocument jDoc2 = QJsonDocument::fromJson(dataaa2);
    QJsonObject jObj2 = jDoc2.object();
    QString x_token = jObj2.value("x_token").toString();


    QUrl url3("https://mobileproxy.passport.yandex.net/1/token?app_id=ru.yandex.mobile.music&uuid=70db875aae45466dbe932244c10a62c1&app_version_name=5.19&ifv=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA&am_version_name=5.151&deviceidhash=11387416356007369345&manufacturer=Apple&deviceid=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA&device_name=iPhone%20User&device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA&app_platform=iPhone&model=iPhone8%2C4");
    QUrlQuery query3;

    query3.addQueryItem("access_token", x_token);
    query3.addQueryItem("client_id", "23cabbbdc6cd418abb4b39c32c41195d");
    query3.addQueryItem("client_secret", "53bc75238f0c4d08a118e51fe9203300");
    query3.addQueryItem("grant_type", "x-token");
    query3.addQueryItem("payment_auth_retpath", "yandexmusic://am/payment_auth");

    //url3.setQuery(query3);
    QNetworkRequest request3(url3);
    request3.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request3.setRawHeader("User-Agent", "com.yandex.mobile.auth.sdk/5.151.69098 (Apple iPhone8,4; iOS 12.2)");
    request3.setRawHeader("Cookie", "_yasc=jIPYMjq4BwfgL8w1zfZDfFTNIxqinePslHoDbyatIhhu7A==");

    QNetworkReply *reply3 = _manager->post(request3, query3.toString(QUrl::FullyEncoded).toUtf8());


    // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    QEventLoop looppp3;
    QObject::connect(reply3, SIGNAL(finished()) , &looppp3, SLOT(quit()));
    looppp3.exec();
    QByteArray dataaa3 = reply3->readAll();
    QString DataAsString3     = QString::fromUtf8(dataaa3);
    qDebug() << " 3rd step: " << DataAsString3;
    //if (DataAsString.contains("error")) {
    QJsonDocument jDoc3 = QJsonDocument::fromJson(dataaa3);
    QJsonObject jObj3 = jDoc3.object();
    if (jObj3.contains("access_token")) {

        QString access_token = jObj3.value("access_token").toString();
        emit authorized(access_token, jObj3.value("uid").toInt());
    }
    // char TASK_TYPE_K[] = "taskType";
    //reply->setProperty(TASK_TYPE_K, 0);



}

/*void Authorization::finished(QNetworkReply *reply) {

        QJsonDocument jDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jObj = jDoc.object();
        QString strFromObj = QJsonDocument(jObj).toJson(QJsonDocument::Compact).toStdString().c_str();
        qDebug() << strFromObj;
         if (jObj.contains("access_token")) {


            emit authorized(jObj.value("access_token").toString(), jObj.value("user_id").toInt());
        } else {
             if (strFromObj.contains("2fa")) {

                     QString validation_sid = jObj.value("validation_sid").toString();
                     QUrl urll("https://api.vk.com/method/auth.validatePhone");
                     QUrlQuery query;

                     query.addQueryItem("sid", validation_sid);
                     query.addQueryItem("v", "5.131");
                     urll.setQuery(query);
                     QNetworkRequest request(urll);
                     request.setRawHeader("User-Agent", "com.vk.vkclient/12 (unknown, iPhone OS 9.3.5, iPhone, Scale/2.000000)");

                     QNetworkReply *reply = _manager->get(request);
                     char TASK_TYPE_K[] = "taskType";
                     reply->setProperty(TASK_TYPE_K, 0);



                emit coderequired();


        } else {
                 if (strFromObj.contains("invalid")) {
                 qDebug() << "VK said: " << strFromObj;
                emit error(QString("Капча или неверный пароль"),QString("Капча или неверный пароль"));
                 }
             }
    }
    reply->deleteLater();
}*/


