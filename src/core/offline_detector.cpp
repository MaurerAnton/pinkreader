#include "offline_detector.hpp"
#include <QNetworkReply>
#include <QNetworkRequest>

namespace PinkReader {

OfflineDetector::OfflineDetector(QObject* parent)
    : QObject(parent)
    , m_checkTimer(new QTimer(this))
    , m_nam(new QNetworkAccessManager(this))
{
    m_checkTimer->setInterval(30000);
    connect(m_checkTimer, &QTimer::timeout, this, &OfflineDetector::checkNow);
    m_checkTimer->start();
}

void OfflineDetector::checkNow() {
    doCheck();
}

void OfflineDetector::doCheck() {
    QNetworkRequest req(QUrl("https://www.reddit.com/generate_204"));
    req.setTransferTimeout(5000);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);

    auto* reply = m_nam->head(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        bool online = (reply->error() == QNetworkReply::NoError)
                    || (reply->error() == QNetworkReply::ContentAccessDenied)
                    || (reply->error() == QNetworkReply::ContentNotFoundError);

        if (online != m_online) {
            m_online = online;
            emit onlineChanged(m_online);
            if (m_online) emit wentOnline();
            else emit wentOffline();
        }
    });
}

} // namespace PinkReader
