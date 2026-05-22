#pragma once

#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <functional>

namespace PinkReader {

struct PendingAction {
    enum Type { Vote, Save, Hide, Comment };
    Type type;
    QString fullname;
    int direction = 0;        // 1=up, -1=down for vote; 1=save, 0=unsave
    QString text;             // comment text
    qint64 timestamp = 0;

    QJsonObject toJson() const;
    static PendingAction fromJson(const QJsonObject& obj);
};

class OfflineQueue : public QObject {
    Q_OBJECT
    Q_PROPERTY(int pendingCount READ pendingCount NOTIFY queueChanged)

public:
    explicit OfflineQueue(QObject* parent = nullptr);

    int pendingCount() const { return m_queue.size(); }

    Q_INVOKABLE void enqueueVote(const QString& fullname, int direction);
    Q_INVOKABLE void enqueueSave(const QString& fullname, bool saveFlag);
    Q_INVOKABLE void enqueueHide(const QString& fullname);

    using ActionHandler = std::function<void(const PendingAction&, std::function<void(bool)>)>;
    void setActionHandler(ActionHandler handler) { m_handler = std::move(handler); }

    Q_INVOKABLE void flush();
    Q_INVOKABLE void clear();

signals:
    void queueChanged();
    void actionFlushed(const PendingAction& action);
    void flushError(const PendingAction& action, const QString& error);
    void flushComplete(int successCount, int failCount);

private:
    void processNext();
    void saveQueue();
    void loadQueue();

    QVector<PendingAction> m_queue;
    ActionHandler m_handler;
    bool m_flushing = false;
};

} // namespace PinkReader
