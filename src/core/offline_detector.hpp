#pragma once

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>

namespace PinkReader {

class OfflineDetector : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY onlineChanged)
    Q_PROPERTY(bool isOffline READ isOffline NOTIFY onlineChanged)

public:
    explicit OfflineDetector(QObject* parent = nullptr);

    bool isOnline() const { return m_online; }
    bool isOffline() const { return !m_online; }

    Q_INVOKABLE void checkNow();

signals:
    void onlineChanged(bool online);
    void wentOnline();
    void wentOffline();

private:
    void doCheck();
    bool m_online = true;
    QTimer* m_checkTimer;
    QNetworkAccessManager* m_nam;
};

} // namespace PinkReader
