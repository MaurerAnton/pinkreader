#include "offline_queue.hpp"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

namespace PinkReader {

QJsonObject PendingAction::toJson() const {
    QJsonObject obj;
    obj["type"] = static_cast<int>(type);
    obj["fullname"] = fullname;
    obj["direction"] = direction;
    obj["text"] = text;
    obj["timestamp"] = timestamp;
    return obj;
}

PendingAction PendingAction::fromJson(const QJsonObject& obj) {
    PendingAction a;
    a.type = static_cast<Type>(obj["type"].toInt());
    a.fullname = obj["fullname"].toString();
    a.direction = obj["direction"].toInt();
    a.text = obj["text"].toString();
    a.timestamp = obj["timestamp"].toInteger();
    return a;
}

OfflineQueue::OfflineQueue(QObject* parent) : QObject(parent) {
    loadQueue();
}

void OfflineQueue::enqueueVote(const QString& fullname, int direction) {
    PendingAction a;
    a.type = PendingAction::Vote;
    a.fullname = fullname;
    a.direction = direction;
    a.timestamp = QDateTime::currentSecsSinceEpoch();
    m_queue.append(a);
    saveQueue();
    emit queueChanged();
}

void OfflineQueue::enqueueSave(const QString& fullname, bool saveFlag) {
    PendingAction a;
    a.type = PendingAction::Save;
    a.fullname = fullname;
    a.direction = saveFlag ? 1 : 0;
    a.timestamp = QDateTime::currentSecsSinceEpoch();
    m_queue.append(a);
    saveQueue();
    emit queueChanged();
}

void OfflineQueue::enqueueHide(const QString& fullname) {
    PendingAction a;
    a.type = PendingAction::Hide;
    a.fullname = fullname;
    a.timestamp = QDateTime::currentSecsSinceEpoch();
    m_queue.append(a);
    saveQueue();
    emit queueChanged();
}

void OfflineQueue::flush() {
    if (m_queue.isEmpty() || m_flushing || !m_handler)
        return;
    m_flushing = true;
    processNext();
}

void OfflineQueue::processNext() {
    if (m_queue.isEmpty()) {
        m_flushing = false;
        return;
    }

    auto action = m_queue.takeFirst();
    saveQueue();
    emit queueChanged();

    m_handler(action, [this](bool success) {
        if (success) {
            emit actionFlushed(m_queue.isEmpty() ? PendingAction{} : m_queue.first());
        } else {
            emit flushError(m_queue.isEmpty() ? PendingAction{} : m_queue.first(), "Action failed");
        }

        if (m_queue.isEmpty()) {
            m_flushing = false;
            emit flushComplete(0, 0);
        } else {
            processNext();
        }
    });
}

void OfflineQueue::clear() {
    m_queue.clear();
    saveQueue();
    emit queueChanged();
}

void OfflineQueue::saveQueue() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/offline_queue.json";
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    QJsonArray arr;
    for (const auto& a : m_queue) {
        arr.append(a.toJson());
    }

    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(arr).toJson());
        file.close();
    }
}

void OfflineQueue::loadQueue() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/offline_queue.json";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    for (const auto& val : doc.array()) {
        m_queue.append(PendingAction::fromJson(val.toObject()));
    }
}

}  // namespace PinkReader
