#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>

#include "authorization.h"
#include "settings.h"
#include <QEventLoop>
#include "YaSailMusic.h"
#include "json.h"
using QtJson::JsonObject;
using QtJson::JsonArray;

Authorization::Authorization(QObject *parent) : QObject(parent)
{
   baseValues_->m_manager = new QNetworkAccessManager(this);
        QString csrfUrl = "https://passport.yandex.ru/am?app_platform=ios&app_id=ru.yandex.mobile.music&app_version=120366&am_version_name=6.10.1&device_id="+baseValues_->uid+"&scheme=yandexmusic&theme=dark&lang=ru&locale=ru&source=&mode=welcome&reg_type=neophonish&device_name=iPhone&auth_type=lite,social,yandex&siwa=true&uuid="+baseValues_->uid+"&noreturn=1";
        QUrl csfrUri(csrfUrl);
        QNetworkRequest requestttnews(csfrUri);
        // requestttnews.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        requestttnews.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
        requestttnews.setRawHeader("Connection", "keep-alive");
        requestttnews.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
        requestttnews.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 16_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 PassportSDK/6.10.1.120366");

        QNetworkReply *replyyynews;
        QByteArray dataaanews;

       replyyynews =  baseValues_->m_manager->get(requestttnews);
        replyyynews->ignoreSslErrors();
        QEventLoop loopppnews;
        QObject::connect(replyyynews, SIGNAL(finished()) , &loopppnews, SLOT(quit()));
        loopppnews.exec();
         dataaanews = replyyynews->readAll();


        QString DataAsStringggnews     = QString::fromUtf8(dataaanews);
        baseValues_->csrf = DataAsStringggnews.mid(DataAsStringggnews.indexOf("csrf\"")+7, DataAsStringggnews.indexOf("\",\"currentUrl")-DataAsStringggnews.indexOf("csrf\"")-7);
        process_uuid = DataAsStringggnews.mid(DataAsStringggnews.indexOf("process_uuid\":")+15, DataAsStringggnews.indexOf("\",\"loginForLogoId")-DataAsStringggnews.indexOf("process_uuid\":")-15);
        QString cookies = QString::fromUtf8(replyyynews->rawHeader("Set-Cookie"));
        QStringList stlist = cookies.split("\n");
        QString st1 = stlist.at(1);
        qDebug() << st1;
        yandexuid = st1.left(st1.indexOf(";"));
        QString st2 = stlist.at(2);
        qDebug() << st2;
        uniqueuid = st2.left(st2.indexOf(";"));

}

Authorization::~Authorization()
{
}

void Authorization::setupRequest(QNetworkRequest *r)
{
    Settings settings;
    QString accessToken = settings.value("accessToken").toString();

    r->setRawHeader("Accept", "*/*");
    r->setRawHeader("X-Requested-With", "XMLHttpRequest");
    r->setHeader(QNetworkRequest::UserAgentHeader, "Yandex-Music-API");
    r->setRawHeader("X-Yandex-Music-Client", "YandexMusicAndroid/23020251");

    if(accessToken.size() > 0) {
        r->setRawHeader("Authorization", QString("OAuth %1").arg(accessToken).toLatin1());
    }
}

void Authorization::doAuth(QString username, QString password, QString hint, QString smscode)
{
    if (smscode.length()>0) {
           if (hint.contains("СМС")) {
           QString pccUrl = "https://passport.yandex.ru/registration-validations/phone-confirm-code";
           QUrl pccurll(pccUrl);
           QUrlQuery pccq;


           pccq.addQueryItem("csrf_token", baseValues_->csrf);
           pccq.addQueryItem("code", smscode);

           pccq.addQueryItem("track_id", track_id);






           QNetworkRequest pccr(pccurll);


           pccr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
           pccr.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
           pccr.setRawHeader("X-Requested-With", "XMLHttpRequest");
           pccr.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 16_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 PassportSDK/6.10.1.120366");
           pccr.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid).toUtf8());
           //setupRequest(&r);

           // QNetworkAccessManager* m = new QNetworkAccessManager(this);

           QNetworkReply *pccreply = baseValues_->m_manager->post(pccr, pccq.toString(QUrl::FullyEncoded).toUtf8());
           pccreply->ignoreSslErrors();
           //replyyy->ignoreSslErrors();
           //QString urll = "http://vikamobile.ru/yy.txt";
           qDebug() << pccreply->errorString();

           QEventLoop pcclooppp;
           QObject::connect(pccreply, SIGNAL(finished()) , &pcclooppp, SLOT(quit()));
           pcclooppp.exec();
           QByteArray pccdataaa = pccreply->readAll();
           QString pccDataAsStringgg     = QString::fromUtf8(pccdataaa);
           } else {

           }

           QString accUrl = "https://passport.yandex.ru/registration-validations/auth/challenge/commit";

           QUrl accurll(accUrl);
           QUrlQuery accq;


           accq.addQueryItem("csrf_token", baseValues_->csrf);
           if (hint.contains("СМС")) {
           accq.addQueryItem("challenge", "phone_confirmation");
           } else {
             accq.addQueryItem("challenge", "question");
             accq.addQueryItem("answer", smscode);
           }
           accq.addQueryItem("track_id", track_id);
           accq.addQueryItem("appId", "ru.yandex.mobile.music");






           QNetworkRequest accr(accurll);


           accr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
           accr.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
           accr.setRawHeader("X-Requested-With", "XMLHttpRequest");
           accr.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 16_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 PassportSDK/6.10.1.120366");
           accr.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid).toUtf8());
           //setupRequest(&r);

           // QNetworkAccessManager* m = new QNetworkAccessManager(this);

           QNetworkReply *accreply = baseValues_->m_manager->post(accr, accq.toString(QUrl::FullyEncoded).toUtf8());
           accreply->ignoreSslErrors();
           //replyyy->ignoreSslErrors();
           //QString urll = "http://vikamobile.ru/yy.txt";
           qDebug() << accreply->errorString();

           QEventLoop acclooppp;
           QObject::connect(accreply, SIGNAL(finished()) , &acclooppp, SLOT(quit()));
           acclooppp.exec();
           QByteArray accdataaa = accreply->readAll();
           QString accDataAsStringgg     = QString::fromUtf8(accdataaa);

           QString cookies = QString::fromUtf8(accreply->rawHeader("Set-Cookie"));


           QStringList stlist = cookies.split("\n");
           QString st1 = stlist.at(0);
           qDebug() << st1;
           Session_id = st1.left(st1.indexOf(";"));

           QString st2 = stlist.at(1);
           qDebug() << st2;
           sessionid2 = st2.left(st2.indexOf(";"));

           QString st3 = stlist.at(2);
           qDebug() << st3;
           sessguard= st3.left(st3.indexOf(";"));


           QString st4 = stlist.at(3);
           qDebug() << st4;
           yp = st4.left(st4.indexOf(";"));


           QString st5 = stlist.at(4);
           qDebug() << st5;
           ys = st5.left(st5.indexOf(";"));


           QString st6 = stlist.at(5);
           qDebug() << st6;
           eL = st6.left(st6.indexOf(";"));


           QString st7 = stlist.at(6);
           qDebug() << st7;
           yandex_login = st7.left(st7.indexOf(";"));

           QString st8 = stlist.at(7);
           qDebug() << st8;
           lah = st8.left(st8.indexOf(";"));

           QString st9 = stlist.at(8);
           qDebug() << st9;
           mda2_beacon = st9.left(st9.indexOf(";"));

           QString tbsiUrl = "https://mobileproxy.passport.yandex.net/1/bundle/oauth/token_by_sessionid?app_id=ru.yandex.mobile.music&uuid="+baseValues_->uid+"&app_version_name=6.06&ifv=537792CB-95BA-4B9D-A97C-46A24AEC0265&am_version_name=6.10.1&yandex_mobile_metrica_get_ad_host=https%3A%2F%2Fmobile.yandexadexchange.net&deviceidhash=18425540953542086750&deviceid="+baseValues_->uid+"&manufacturer=Apple&device_name=iPhone&app_platform=iPhone&device_id="+baseValues_->uid+"&model=iPhone8%2C1";

           QUrl tbsiurll(tbsiUrl);
           QUrlQuery tbsiq;
           QByteArray tbsidata;

           tbsiq.addQueryItem("client_id", "c0ebe342af7d48fbbbfcf2d2eedb8f9e");
           tbsiq.addQueryItem("client_secret", "ad0a908f0aa341a182a37ecd75bc319e");

           tbsiq.addQueryItem("cookies", yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";");
           tbsiq.addQueryItem("appId", "ru.yandex.mobile.music");






           QNetworkRequest tbsir(tbsiurll);

          // QSslConfiguration sSlConfigprepare;



           tbsir.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
           tbsir.setRawHeader("Accept", "*/*");
           tbsir.setRawHeader("Ya-Client-Host", "yandex.ru");
           tbsir.setRawHeader("Ya-Client-Cookie", (yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";").toUtf8());


           tbsir.setRawHeader("User-Agent", "com.yandex.mobile.auth.sdk/6.10.1.120366 (Apple iPhone8,1; iOS 14.8)");
           tbsir.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";").toUtf8());
           //setupRequest(&r);

           // QNetworkAccessManager* m = new QNetworkAccessManager(this);

           QNetworkReply *tbsireply = baseValues_->m_manager->post(tbsir, tbsiq.toString(QUrl::FullyEncoded).toUtf8());
           tbsireply->ignoreSslErrors();
           //replyyy->ignoreSslErrors();
           //QString urll = "http://vikamobile.ru/yy.txt";
           qDebug() << tbsireply->errorString();

           QEventLoop tbsilooppp;
           QObject::connect(tbsireply, SIGNAL(finished()) , &tbsilooppp, SLOT(quit()));
           tbsilooppp.exec();
           QByteArray tbsidataaa = tbsireply->readAll();
           QString tbsiDataAsStringgg     = QString::fromUtf8(tbsidataaa);
           bool otk;
           QtJson::JsonObject jto = QtJson::parse(tbsiDataAsStringgg, otk).toMap();
           QString mm_token = "";
           if(jto.contains("access_token")) {
               mm_token = jto.value("access_token").toString();
               m_userId = "";

           }

           QString ltUrl = "https://mobileproxy.passport.yandex.net/1/token?app_id=ru.yandex.mobile.music&uuid="+baseValues_->uid+"&app_version_name=6.06&ifv=537792CB-95BA-4B9D-A97C-46A24AEC0265&am_version_name=6.10.1&yandex_mobile_metrica_get_ad_host=https%3A%2F%2Fmobile.yandexadexchange.net&deviceidhash=18425540953542086750&deviceid="+baseValues_->uid+"&manufacturer=Apple&device_name=iPhone&app_platform=iPhone&device_id="+baseValues_->uid+"&model=iPhone8%2C1";


           QUrl lturll(ltUrl);
           QUrlQuery ltq;
           QByteArray ltdata;

           ltq.addQueryItem("access_token", mm_token);
           ltq.addQueryItem("client_id", "23cabbbdc6cd418abb4b39c32c41195d");
           ltq.addQueryItem("client_secret", "53bc75238f0c4d08a118e51fe9203300");

           //ltq.addQueryItem("cookies", yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";");
           ltq.addQueryItem("grant_type", "x-token");
           ltq.addQueryItem("payment_auth_retpath", "yandexmusic://am/payment_auth");




           ltdata = ltq.toString(QUrl::FullyEncoded).toUtf8();

           QNetworkRequest ltr(lturll);



           ltr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
           ltr.setRawHeader("Accept", "*/*");
          // ltr.setRawHeader("Ya-Client-Host", "yandex.ru");
          // ltr.setRawHeader("Ya-Client-Cookie", (yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";").toUtf8());


           ltr.setRawHeader("User-Agent", "com.yandex.mobile.auth.sdk/6.10.1.120366 (Apple iPhone8,1; iOS 14.8)");
          // ltr.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";").toUtf8());
           //setupRequest(&r);

           // QNetworkAccessManager* m = new QNetworkAccessManager(this);

           QNetworkReply *ltreply = baseValues_->m_manager->post(ltr,ltdata);
           ltreply->ignoreSslErrors();
           //replyyy->ignoreSslErrors();
           //QString urll = "http://vikamobile.ru/yy.txt";
           qDebug() << ltreply->errorString();

           QEventLoop ltlooppp;
           QObject::connect(ltreply, SIGNAL(finished()) , &ltlooppp, SLOT(quit()));
           ltlooppp.exec();
           QByteArray ltdataaa = ltreply->readAll();
           QString ltDataAsStringgg     = QString::fromUtf8(ltdataaa);
           bool ottk;
           QtJson::JsonObject ltjto = QtJson::parse(ltDataAsStringgg, ottk).toMap();

           if(ltjto.contains("access_token")) {
               m_token = ltjto.value("access_token").toString();
               m_userId = baseValues_->uid;
               Settings settings;

               settings.setValue("accessToken", m_token);
               settings.setValue("userId", m_userId);

               settings.sync();
               qDebug() << settings.fileName();

           }




           emit authorized(m_token, m_userId);

           return;


       } else {
           QString mm_oauthURL =  "https://passport.yandex.ru/registration-validations/auth/multi_step/start";
           //   m_clientID = "23cabbbdc6cd418abb4b39c32c41195d";
           // m_clientSecret = "53bc75238f0c4d08a118e51fe9203300";
           QUrl url(mm_oauthURL);
           QUrlQuery q;
           QByteArray data;
           //QUrl proxyurl = QUrl("https://oauth.yandex.ru/token");
           // QUrl proxyurl = QUrl("http://vikamobile.ru/pproxy.php");
           //  url.addQueryItem("u", proxyurl.toString());
           //url.addQueryItem("post", "1");
           q.addQueryItem("csrf_token", baseValues_->csrf);
           q.addQueryItem("login", username);
           q.addQueryItem("process_uuid", process_uuid);
           /* if (x_captcha_key.length()>0) {
       q.addQueryItem("x_captcha_key", x_captcha_key);
       q.addQueryItem("x_captcha_answer", x_captcha_answer);
       }*/
           q.addQueryItem("retpath", "https://passport.yandex.ru/am/finish?status=ok");
           q.addQueryItem("isAm", "true");
           q.addQueryItem("appId", "ru.yandex.mobile.music");
           q.addQueryItem("am_version_name", "6.10.1");
           q.addQueryItem("app_id", "ru.yandex.mobile.music");
           q.addQueryItem("app_platform", "ios");
           q.addQueryItem("device_id", baseValues_->uid);
           q.addQueryItem("device_name", "iPhone");
           q.addQueryItem("display_language", "ru");
           q.addQueryItem("uuid", baseValues_->uid);

           data = q.toString(QUrl::FullyEncoded).toUtf8();

           QNetworkRequest r(url);



           r.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid).toUtf8());
           r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
           r.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
           r.setRawHeader("X-Requested-With", "XMLHttpRequest");
           r.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 16_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 PassportSDK/6.10.1.120366");

           //setupRequest(&r);

           // QNetworkAccessManager* m = new QNetworkAccessManager(this);

           QNetworkReply *reply = baseValues_->m_manager->post(r,data);
           reply->ignoreSslErrors();
           //replyyy->ignoreSslErrors();
           //QString urll = "http://vikamobile.ru/yy.txt";
           qDebug() << reply->errorString();

           QEventLoop looppp;
           QObject::connect(reply, SIGNAL(finished()) , &looppp, SLOT(quit()));
           looppp.exec();
           QByteArray dataaa = reply->readAll();
           QString DataAsStringgg     = QString::fromUtf8(dataaa);
           bool ok;
           QtJson::JsonObject jo = QtJson::parse(DataAsStringgg, ok).toMap();

           if(jo.contains("track_id")) {
               track_id = jo.value("track_id").toString();
           }


           QString commitUrl = "https://passport.yandex.ru/registration-validations/auth/multi_step/commit_password";

           QUrl commiturll(commitUrl);
           QUrlQuery commitq;
           QByteArray commitdata;
           //QUrl proxyurl = QUrl("https://oauth.yandex.ru/token");
           // QUrl proxyurl = QUrl("http://vikamobile.ru/pproxy.php");
           //  url.addQueryItem("u", proxyurl.toString());
           //url.addQueryItem("post", "1");
           commitq.addQueryItem("csrf_token", baseValues_->csrf);
           commitq.addQueryItem("track_id", track_id);
           commitq.addQueryItem("password", password);
           /* if (x_captcha_key.length()>0) {
       q.addQueryItem("retpath", "");
       q.addQueryItem("x_captcha_answer", x_captcha_answer);
       }*/
           commitq.addQueryItem("retpath", "https://passport.yandex.ru/am/finish?status=ok&from=Login");
           commitq.addQueryItem("lang", "ru");
           commitq.addQueryItem("appId", "ru.yandex.mobile.music");


           commitdata = commitq.toString(QUrl::FullyEncoded).toUtf8();

           QNetworkRequest commitr(commiturll);



           commitr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
           commitr.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
           commitr.setRawHeader("X-Requested-With", "XMLHttpRequest");
           commitr.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 16_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 PassportSDK/6.10.1.120366");
           commitr.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid).toUtf8());
           //setupRequest(&r);

           // QNetworkAccessManager* m = new QNetworkAccessManager(this);

           QNetworkReply *commitreply = baseValues_->m_manager->post(commitr,commitdata);
           commitreply->ignoreSslErrors();
           //replyyy->ignoreSslErrors();
           //QString urll = "http://vikamobile.ru/yy.txt";
           qDebug() << commitreply->errorString();

           QEventLoop commitlooppp;
           QObject::connect(commitreply, SIGNAL(finished()) , &commitlooppp, SLOT(quit()));
           commitlooppp.exec();
           QByteArray commitdataaa = commitreply->readAll();
           QString commitDataAsStringgg     = QString::fromUtf8(commitdataaa);
           if (commitDataAsStringgg.contains("match")) {
                   emit error(QString("Неверный логин или пароль"));
                   return;
           }
           if (commitDataAsStringgg.contains("auth_challenge")) {
               QString submitUrl  = "https://passport.yandex.ru/registration-validations/auth/challenge/submit";

               QUrl submiturll(submitUrl);
               QUrlQuery submitq;
               QByteArray submitdata;

               submitq.addQueryItem("csrf_token", baseValues_->csrf);
               submitq.addQueryItem("track_id", track_id);





               submitdata = submitq.toString(QUrl::FullyEncoded).toUtf8();

               QNetworkRequest submitr(submiturll);




               submitr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
               submitr.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
               submitr.setRawHeader("X-Requested-With", "XMLHttpRequest");
               submitr.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 16_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 PassportSDK/6.10.1.120366");
               submitr.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid).toUtf8());
               //setupRequest(&r);

               // QNetworkAccessManager* m = new QNetworkAccessManager(this);

               QNetworkReply *submitreply = baseValues_->m_manager->post(submitr,submitdata);
               submitreply->ignoreSslErrors();
               //replyyy->ignoreSslErrors();
               //QString urll = "http://vikamobile.ru/yy.txt";
               qDebug() << submitreply->errorString();

               QEventLoop submitlooppp;
               QObject::connect(submitreply, SIGNAL(finished()) , &submitlooppp, SLOT(quit()));
               submitlooppp.exec();
               QByteArray submitdataaa = submitreply->readAll();
               QString submitDataAsStringgg     = QString::fromUtf8(submitdataaa);
               bool okkk;
               QtJson::JsonObject joo = QtJson::parse(submitDataAsStringgg, okkk).toMap();

               //if(jo.contains("challenge")) {
               //    track_id = jo.value("track_id").toString();
               //}
               QtJson::JsonObject challenge = joo["challenge"].toMap();
               QString hint = challenge["hint"].toString();
               QString challengeType = challenge["challengeType"].toString();
               if (challengeType.contains("question")) {
                   emit captcha(hint);
                   return;
               } else {


               QString phoneId = QString::number(challenge["phoneId"].toInt());
               baseValues_->m_manager = new QNetworkAccessManager(this);
               QString accountsUrl = "https://passport.yandex.ru/registration-validations/auth/accounts";

               QUrl accountsurll(accountsUrl);
               QUrlQuery accountsq;
               QByteArray accountsdata;

               accountsq.addQueryItem("csrf_token", baseValues_->csrf);






               accountsdata = accountsq.toString(QUrl::FullyEncoded).toUtf8();

               QNetworkRequest accountsr(accountsurll);

             //  QSslConfiguration sSlConfigprepare;



               accountsr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
               accountsr.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
               accountsr.setRawHeader("X-Requested-With", "XMLHttpRequest");
               accountsr.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 16_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 PassportSDK/6.10.1.120366");
               accountsr.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid).toUtf8());
               //setupRequest(&r);

               // QNetworkAccessManager* m = new QNetworkAccessManager(this);

               QNetworkReply *accountsreply = baseValues_->m_manager->post(accountsr,accountsdata);
               accountsreply->ignoreSslErrors();
               //replyyy->ignoreSslErrors();
               //QString urll = "http://vikamobile.ru/yy.txt";
               qDebug() << accountsreply->errorString();

               QEventLoop accountslooppp;
               QObject::connect(accountsreply, SIGNAL(finished()) , &accountslooppp, SLOT(quit()));
               accountslooppp.exec();
               QByteArray accountsdataaa = accountsreply->readAll();
               QString accountsDataAsStringgg     = QString::fromUtf8(accountsdataaa);
               bool okkkaccounts;
               QtJson::JsonObject accountsjoo = QtJson::parse(accountsDataAsStringgg, okkkaccounts).toMap();
               baseValues_->csrf = accountsjoo.value("csrf").toString();
               //return;

               QString pccsUrl = "https://passport.yandex.ru/registration-validations/phone-confirm-code-submit";
               QUrl pccsurll(pccsUrl);
               QUrlQuery pccsq;
               QByteArray pccsdata;

               pccsq.addQueryItem("csrf_token", baseValues_->csrf);
               pccsq.addQueryItem("phone_id", phoneId);
               pccsq.addQueryItem("confirm_method", "by_sms");
               pccsq.addQueryItem("track_id", track_id);
               pccsq.addQueryItem("isCodeWithFormat", "true");



               pccsdata = pccsq.toString(QUrl::FullyEncoded).toUtf8();

               QNetworkRequest pccsr(pccsurll);



               pccsr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
               pccsr.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
               pccsr.setRawHeader("X-Requested-With", "XMLHttpRequest");
               pccsr.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 16_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 PassportSDK/6.10.1.120366");
               pccsr.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid).toUtf8());
               //setupRequest(&r);

               // QNetworkAccessManager* m = new QNetworkAccessManager(this);

               QNetworkReply *pccsreply = baseValues_->m_manager->post(pccsr,pccsdata);
               pccsreply->ignoreSslErrors();
               //replyyy->ignoreSslErrors();
               //QString urll = "http://vikamobile.ru/yy.txt";
               qDebug() << pccsreply->errorString();

               QEventLoop pccslooppp;
               QObject::connect(pccsreply, SIGNAL(finished()) , &pccslooppp, SLOT(quit()));
               pccslooppp.exec();
               QByteArray pccsdataaa = pccsreply->readAll();
               QString pccsDataAsStringgg     = QString::fromUtf8(pccsdataaa);
               bool okkkpccs;
               QtJson::JsonObject pccsjoo = QtJson::parse(pccsDataAsStringgg, okkkpccs).toMap();

               emit captcha(QString("Код отправлен в СМС на ")+hint);
               return;
                   }
           } else {
               /*  QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie>>(commitreply->header
         (QNetworkRequest::SetCookieHeader));
       QString cookiesstring;
       //foreach(QNetworkCookie &cookie, cookies) {

       for (int ii = 0; ii < cookies.count(); ++ii) {
           cookiesstring.append(cookies[ii].name()+"="+cookies[ii].value()+";");

       }
   */
               QString cookiess = QString::fromUtf8(commitreply->rawHeader("Set-Cookie"));


               QStringList stlist = cookiess.split("\n");
               QString st1 = stlist.at(0);
               qDebug() << st1;
               Session_id = st1.left(st1.indexOf(";"));

               QString st2 = stlist.at(1);
               qDebug() << st2;
               sessionid2 = st2.left(st2.indexOf(";"));

               QString st3 = stlist.at(2);
               qDebug() << st3;
               sessguard= st3.left(st3.indexOf(";"));


               QString st4 = stlist.at(3);
               qDebug() << st4;
               yp = st4.left(st4.indexOf(";"));


               QString st5 = stlist.at(4);
               qDebug() << st5;
               ys = st5.left(st5.indexOf(";"));


               QString st6 = stlist.at(5);
               qDebug() << st6;
               eL = st6.left(st6.indexOf(";"));


               QString st7 = stlist.at(6);
               qDebug() << st7;
               yandex_login = st7.left(st7.indexOf(";"));

               QString st8 = stlist.at(7);
               qDebug() << st8;
               lah = st8.left(st8.indexOf(";"));

               QString st9 = stlist.at(8);
               qDebug() << st9;
               mda2_beacon = st9.left(st9.indexOf(";"));


               QString tbsiUrl = "https://mobileproxy.passport.yandex.net/1/bundle/oauth/token_by_sessionid?app_id=ru.yandex.mobile.music&uuid="+baseValues_->uid+"&app_version_name=6.06&ifv=537792CB-95BA-4B9D-A97C-46A24AEC0265&am_version_name=6.10.1&yandex_mobile_metrica_get_ad_host=https%3A%2F%2Fmobile.yandexadexchange.net&deviceidhash=18425540953542086750&deviceid="+baseValues_->uid+"&manufacturer=Apple&device_name=iPhone&app_platform=iPhone&device_id="+baseValues_->uid+"&model=iPhone8%2C1";

               QUrl tbsiurll(tbsiUrl);
               QUrlQuery tbsiq;
               QByteArray tbsidata;

               tbsiq.addQueryItem("client_id", "c0ebe342af7d48fbbbfcf2d2eedb8f9e");
               tbsiq.addQueryItem("client_secret", "ad0a908f0aa341a182a37ecd75bc319e");

               tbsiq.addQueryItem("cookies", yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";");
               tbsiq.addQueryItem("appId", "ru.yandex.mobile.music");




               tbsidata = tbsiq.toString(QUrl::FullyEncoded).toUtf8();

               QNetworkRequest tbsir(tbsiurll);



               tbsir.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
               tbsir.setRawHeader("Accept", "*/*");
               tbsir.setRawHeader("Ya-Client-Host", "yandex.ru");
               tbsir.setRawHeader("Ya-Client-Cookie", (yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";").toUtf8());


               tbsir.setRawHeader("User-Agent", "com.yandex.mobile.auth.sdk/6.10.1.120366 (Apple iPhone8,1; iOS 14.8)");
               tbsir.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";").toUtf8());
               //setupRequest(&r);

               // QNetworkAccessManager* m = new QNetworkAccessManager(this);

               QNetworkReply *tbsireply = baseValues_->m_manager->post(tbsir,tbsidata);
               tbsireply->ignoreSslErrors();
               //replyyy->ignoreSslErrors();
               //QString urll = "http://vikamobile.ru/yy.txt";
               qDebug() << tbsireply->errorString();

               QEventLoop tbsilooppp;
               QObject::connect(tbsireply, SIGNAL(finished()) , &tbsilooppp, SLOT(quit()));
               tbsilooppp.exec();
               QByteArray tbsidataaa = tbsireply->readAll();
               QString tbsiDataAsStringgg     = QString::fromUtf8(tbsidataaa);
               bool otk;
               QtJson::JsonObject jto = QtJson::parse(tbsiDataAsStringgg, otk).toMap();
   //
               QString mm_token = "";
               if(jto.contains("access_token")) {
                   mm_token = jto.value("access_token").toString();
                   m_userId = "";

               }

               QString ltUrl = "https://mobileproxy.passport.yandex.net/1/token?app_id=ru.yandex.mobile.music&uuid="+baseValues_->uid+"&app_version_name=6.06&ifv=537792CB-95BA-4B9D-A97C-46A24AEC0265&am_version_name=6.10.1&yandex_mobile_metrica_get_ad_host=https%3A%2F%2Fmobile.yandexadexchange.net&deviceidhash=18425540953542086750&deviceid="+baseValues_->uid+"&manufacturer=Apple&device_name=iPhone&app_platform=iPhone&device_id="+baseValues_->uid+"&model=iPhone8%2C1";


               QUrl lturll(ltUrl);
               QUrlQuery ltq;
               QByteArray ltdata;

               ltq.addQueryItem("access_token", mm_token);
               ltq.addQueryItem("client_id", "23cabbbdc6cd418abb4b39c32c41195d");
               ltq.addQueryItem("client_secret", "53bc75238f0c4d08a118e51fe9203300");

               //ltq.addQueryItem("cookies", yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";");
               ltq.addQueryItem("grant_type", "x-token");
               ltq.addQueryItem("payment_auth_retpath", "yandexmusic://am/payment_auth");




               ltdata = ltq.toString(QUrl::FullyEncoded).toUtf8();

               QNetworkRequest ltr(lturll);



               ltr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
               ltr.setRawHeader("Accept", "*/*");
              // ltr.setRawHeader("Ya-Client-Host", "yandex.ru");
              // ltr.setRawHeader("Ya-Client-Cookie", (yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";").toUtf8());


               ltr.setRawHeader("User-Agent", "com.yandex.mobile.auth.sdk/6.10.1.120366 (Apple iPhone8,1; iOS 14.8)");
              // ltr.setRawHeader("Cookie", (yandexuid+"; "+uniqueuid+"; "+Session_id+"; "+sessionid2+"; "+sessguard+"; "+yp+"; "+ys+"; "+eL+"; "+yandex_login+"; "+lah+"; "+mda2_beacon+";").toUtf8());
               //setupRequest(&r);

               // QNetworkAccessManager* m = new QNetworkAccessManager(this);

               QNetworkReply *ltreply = baseValues_->m_manager->post(ltr,ltdata);
               ltreply->ignoreSslErrors();
               //replyyy->ignoreSslErrors();
               //QString urll = "http://vikamobile.ru/yy.txt";
               qDebug() << ltreply->errorString();

               QEventLoop ltlooppp;
               QObject::connect(ltreply, SIGNAL(finished()) , &ltlooppp, SLOT(quit()));
               ltlooppp.exec();
               QByteArray ltdataaa = ltreply->readAll();
               QString ltDataAsStringgg     = QString::fromUtf8(ltdataaa);
               bool ottk;
               QtJson::JsonObject ltjto = QtJson::parse(ltDataAsStringgg, ottk).toMap();

               if(ltjto.contains("access_token")) {
                   m_token = ltjto.value("access_token").toString();
                   m_userId = baseValues_->uid;
                   Settings settings;

                   settings.setValue("accessToken", m_token);
                   settings.setValue("userId", m_userId);

                   settings.sync();
                   qDebug() << settings.fileName();

               }
               emit authorized(m_token, m_userId);

               //qDebug() << "cookies: " << cookiesstring;
           }


       }
       // QString DataAsStringgg     = QString::fromUtf8(dataaa);
       // qDebug() << DataAsStringgg;

}

bool Authorization::checkToken()
{
    Settings settings;
    QString accessToken = settings.value("accessToken").toString();
   // QString userId = settings.value("userId").toString();
    //QDateTime ttl = settings.value("ttl").toDateTime();

    if(!accessToken.isEmpty()) {
        return true;
    }

    return false;
}


void Authorization::removeAccessToken() {
    Settings settings;
    settings.remove("accessToken");
}

void Authorization::removeUserId() {
    Settings settings;
    settings.remove("userId");
}

void Authorization::doAuthFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError) {
        const QByteArray info = reply->readAll();
        qDebug() << Q_FUNC_INFO << info;
        QJsonDocument doc = QJsonDocument::fromJson(info);
        QJsonObject jo = doc.object();
        if(jo.contains("access_token")) {
            m_userId = QString::number(jo.value("uid").toInt());
            m_token = jo.value("access_token").toString();
            m_ttl = QDateTime::currentDateTime().addSecs(jo.value("expires_in").toInt());

            Settings settings;
            settings.setValue("accessToken", m_token);
            settings.setValue("userId", m_userId);
            settings.setValue("ttl", m_ttl);
            settings.sync();

            emit authorized(m_token, m_userId);
        } else {
            emit error("Strange response");
        }
    } else {
        emit error("Login failed");
    }
}
