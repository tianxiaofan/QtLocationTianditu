/**************************************************************************
 *   文件名	：tigooglemapprovider.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-8-30
 *   邮   箱	：499131808@qq.com
 *   Q Q		：499131808
 *   公   司      ：
 *   功能描述      ：
 *   
 *   ======================================================================
 *   修改者	：
 *   修改日期	：
 *   修改内容	：
 *   ======================================================================
 *
 ***************************************************************************/
#include "tigooglemapprovider.h"

#include <QNetworkProxy>
#include <QNetworkReply>
#include <QRegExp>

TiGoogleMapProvider::TiGoogleMapProvider(const QString& format, const quint32 averageSize,
                                         const QGeoMapType::MapStyle mapType, QObject* parent) :
    TiMapProvider(format, averageSize, mapType, parent)
{
    // Google version strings
    _versionGoogleMap       = QStringLiteral("m@354000000");
    _versionGoogleSatellite = QStringLiteral("692");
    _versionGoogleLabels    = QStringLiteral("h@336");
    _versionGoogleTerrain   = QStringLiteral("t@354,r@354000000");
    _versionGoogleHybrid    = QStringLiteral("y");
    _secGoogleWord          = QStringLiteral("Galileo");
}

void TiGoogleMapProvider::_networkReplyError(QNetworkReply::NetworkError error)
{
    if (_googleReply) {
        _googleReply->deleteLater();
        _googleReply = nullptr;
    }
}

void TiGoogleMapProvider::_googleVersionCompleted()
{
    if (!_googleReply || (_googleReply->error() != QNetworkReply::NoError)) {
        qDebug() << "Error collecting Google maps version info";
        return;
    }
    const QString html = QString(_googleReply->readAll());

#if defined(DEBUG_GOOGLE_MAPS)
    QString filename = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    filename += QStringLiteral("/google.output");
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << html << endl;
    }
#endif

    QRegExp reg(QStringLiteral("\"*https?://mt\\D?\\d..*/vt\\?lyrs=m@(\\d*)"), Qt::CaseInsensitive);
    if (reg.indexIn(html) != -1) {
        _versionGoogleMap
                = QString(QStringLiteral("m@%1")).arg(reg.capturedTexts().value(1, QString()));
    }
    reg = QRegExp(QStringLiteral("\"*https?://khm\\D?\\d.googleapis.com/kh\\?v=(\\d*)"),
                  Qt::CaseInsensitive);
    if (reg.indexIn(html) != -1) {
        _versionGoogleSatellite = reg.capturedTexts().value(1);
    }
    reg = QRegExp(QStringLiteral("\"*https?://mt\\D?\\d..*/vt\\?lyrs=t@(\\d*),r@(\\d*)"),
                  Qt::CaseInsensitive);
    if (reg.indexIn(html) != -1) {
        const QStringList gc  = reg.capturedTexts();
        _versionGoogleTerrain = QString(QStringLiteral("t@%1,r@%2")).arg(gc.value(1), gc.value(2));
    }
    _googleReply->deleteLater();
    _googleReply = nullptr;
}

void TiGoogleMapProvider::_replyDestroyed()
{
    _googleReply = nullptr;
}

void TiGoogleMapProvider::getSecGoogleWords(const int x, const int y, QString& sec1, QString& sec2) const
{
    sec1       = QStringLiteral(""); // after &x=...
    sec2       = QStringLiteral(""); // after &zoom=...
    int seclen = ((x * 3) + y) % 8;
    sec2       = _secGoogleWord.left(seclen);
    if (y >= 10000 && y < 100000) {
        sec1 = QStringLiteral("&s=");
    }
}

void TiGoogleMapProvider::tryCorrectGoogleVersions(QNetworkAccessManager* networkManager)
{
    QMutexLocker locker(&_googleVersionMutex);
    if (_googleVersionRetrieved) {
        return;
    }
    _googleVersionRetrieved = true;
    if (networkManager) {
        QNetworkRequest qheader;
        QNetworkProxy   proxy = networkManager->proxy();
        QNetworkProxy   tProxy;
        tProxy.setType(QNetworkProxy::DefaultProxy);
        networkManager->setProxy(tProxy);
        QSslConfiguration conf = qheader.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        qheader.setSslConfiguration(conf);
        const QString url = QStringLiteral("http://maps.google.com/maps/api/js?v=3.2&sensor=false");
        qheader.setUrl(QUrl(url));
        QByteArray ua;
        ua.append("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:31.0) Gecko/20130401 Firefox/31.0");
        qheader.setRawHeader("User-Agent", ua);
        _googleReply = networkManager->get(qheader);
        connect(_googleReply, &QNetworkReply::finished, this,
                &TiGoogleMapProvider::_googleVersionCompleted);
        connect(_googleReply, &QNetworkReply::destroyed, this, &TiGoogleMapProvider::_replyDestroyed);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        connect(_googleReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
                this, &GoogleMapProvider::_networkReplyError);
#else
        connect(_googleReply, &QNetworkReply::errorOccurred, this,
                &TiGoogleMapProvider::_networkReplyError);
#endif
        networkManager->setProxy(proxy);
    }
}


QString TiGoogleStreetMapProvider::getURL(const int x, const int y, const int zoom,
                                          QNetworkAccessManager* networkManager)
{
    // http://mt1.google.com/vt/lyrs=m
    QString server  = QStringLiteral("mt");
    QString request = QStringLiteral("vt");
    QString sec1; // after &x=...
    QString sec2; // after &zoom=...
    getSecGoogleWords(x, y, sec1, sec2);
    tryCorrectGoogleVersions(networkManager);
    return QString(QStringLiteral("http://%1%2.google.com/%3/lyrs=%4&hl=%5&x=%6%7&y=%8&z=%9&s=%10"))
            .arg(server)
            .arg(getServerNum(x, y, 4))
            .arg(request)
            .arg(_versionGoogleMap)
            .arg(m_language)
            .arg(x)
            .arg(sec1)
            .arg(y)
            .arg(zoom)
            .arg(sec2);
}

QString TiGoogleSatelliteMapProvider::getURL(const int x, const int y, const int zoom,
                                             QNetworkAccessManager* networkManager)
{
    // http://mt1.google.com/vt/lyrs=s
    QString server  = QStringLiteral("khm");
    QString request = QStringLiteral("kh");
    QString sec1; // after &x=...
    QString sec2; // after &zoom=...
    getSecGoogleWords(x, y, sec1, sec2);
    tryCorrectGoogleVersions(networkManager);
    return QString(QStringLiteral("http://%1%2.google.com/%3/v=%4&hl=%5&x=%6%7&y=%8&z=%9&s=%10"))
            .arg(server)
            .arg(getServerNum(x, y, 4))
            .arg(request)
            .arg(_versionGoogleSatellite)
            .arg(m_language)
            .arg(x)
            .arg(sec1)
            .arg(y)
            .arg(zoom)
            .arg(sec2);
}

QString TiGoogleLabelsMapProvider::getURL(const int x, const int y, const int zoom,
                                          QNetworkAccessManager* networkManager)
{
    QString server  = "mts";
    QString request = "vt";
    QString sec1; // after &x=...
    QString sec2; // after &zoom=...
    getSecGoogleWords(x, y, sec1, sec2);
    tryCorrectGoogleVersions(networkManager);
    return QString(QStringLiteral("http://%1%2.google.com/%3/lyrs=%4&hl=%5&x=%6%7&y=%8&z=%9&s=%10"))
            .arg(server)
            .arg(getServerNum(x, y, 4))
            .arg(request)
            .arg(_versionGoogleLabels)
            .arg(m_language)
            .arg(x)
            .arg(sec1)
            .arg(y)
            .arg(zoom)
            .arg(sec2);
}

QString TiGoogleTerrainMapProvider::getURL(const int x, const int y, const int zoom,
                                           QNetworkAccessManager* networkManager)
{
    QString server  = QStringLiteral("mt");
    QString request = QStringLiteral("vt");
    QString sec1; // after &x=...
    QString sec2; // after &zoom=...
    getSecGoogleWords(x, y, sec1, sec2);
    tryCorrectGoogleVersions(networkManager);
    return QString(QStringLiteral("http://%1%2.google.com/%3/v=%4&hl=%5&x=%6%7&y=%8&z=%9&s=%10"))
            .arg(server)
            .arg(getServerNum(x, y, 4))
            .arg(request)
            .arg(_versionGoogleTerrain)
            .arg(m_language)
            .arg(x)
            .arg(sec1)
            .arg(y)
            .arg(zoom)
            .arg(sec2);
}

QString TiGoogleHybridMapProvider::getURL(const int x, const int y, const int zoom,
                                          QNetworkAccessManager* networkManager)
{
    QString server  = QStringLiteral("mt");
    QString request = QStringLiteral("vt");
    QString sec1; // after &x=...
    QString sec2; // after &zoom=...
    getSecGoogleWords(x, y, sec1, sec2);
    tryCorrectGoogleVersions(networkManager);
    return QString(QStringLiteral("http://%1%2.google.com/%3/lyrs=%4&hl=%5&x=%6%7&y=%8&z=%9&s=%10"))
            .arg(server)
            .arg(getServerNum(x, y, 4))
            .arg(request)
            .arg(_versionGoogleHybrid)
            .arg(m_language)
            .arg(x)
            .arg(sec1)
            .arg(y)
            .arg(zoom)
            .arg(sec2);
}
