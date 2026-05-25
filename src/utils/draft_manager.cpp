/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: draft_manager.cpp - Draft manager implementation
 */


#include "utils/draft_manager.h"
#include "utils/logging.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QUuid>

namespace PinkReader {

QJsonObject Draft::toJson() const
{
    QJsonObject obj;
    obj["id"] = id;
    obj["type"] = type;
    obj["subreddit"] = subreddit;
    obj["parent_id"] = parentId;
    obj["title"] = title;
    obj["body"] = body;
    obj["created_at"] = createdAt.toString(Qt::ISODate);
    obj["updated_at"] = updatedAt.toString(Qt::ISODate);
    obj["is_sending"] = isSending;
    return obj;
}

Draft Draft::fromJson(const QJsonObject &json)
{
    Draft draft;
    draft.id = json["id"].toString();
    draft.type = json["type"].toString();
    draft.subreddit = json["subreddit"].toString();
    draft.parentId = json["parent_id"].toString();
    draft.title = json["title"].toString();
    draft.body = json["body"].toString();
    draft.createdAt = QDateTime::fromString(json["created_at"].toString(), Qt::ISODate);
    draft.updatedAt = QDateTime::fromString(json["updated_at"].toString(), Qt::ISODate);
    draft.isSending = json["is_sending"].toBool();
    return draft;
}

DraftManager::DraftManager(QObject *parent)
    : QObject(parent)
    , m_autoSaveTimer(new QTimer(this))
{
    connect(m_autoSaveTimer, &QTimer::timeout, this, &DraftManager::autoSave);
}

DraftManager::~DraftManager()
{
    saveDrafts();
    stopAutoSave();
}

void DraftManager::initialize()
{
    Logging::info("DraftManager", "Initializing draft manager...");
    loadDrafts();
    if (m_autoSaveEnabled) startAutoSave();
    Logging::info("DraftManager", QString("Loaded %1 drafts").arg(m_drafts.size()));
}

QString DraftManager::createDraft(const QString &type, const QString &subreddit,
                                   const QString &parentId)
{
    QMutexLocker locker(&m_mutex);

    Draft draft;
    draft.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    draft.type = type;
    draft.subreddit = subreddit;
    draft.parentId = parentId;
    draft.createdAt = QDateTime::currentDateTimeUtc();
    draft.updatedAt = draft.createdAt;

    m_drafts.prepend(draft);
    m_nextDraftNumber++;

    emit draftCreated(draft.id);
    return draft.id;
}

void DraftManager::updateDraft(const QString &draftId, const QString &title,
                                const QString &body)
{
    QMutexLocker locker(&m_mutex);

    for (auto &draft : m_drafts) {
        if (draft.id == draftId) {
            draft.title = title;
            draft.body = body;
            draft.updatedAt = QDateTime::currentDateTimeUtc();
            emit draftUpdated(draftId);
            return;
        }
    }
}

void DraftManager::deleteDraft(const QString &draftId)
{
    QMutexLocker locker(&m_mutex);
    m_drafts.erase(std::remove_if(m_drafts.begin(), m_drafts.end(),
        [&](const Draft &d) { return d.id == draftId; }),
        m_drafts.end());
    emit draftDeleted(draftId);
}

void DraftManager::deleteAllDrafts()
{
    QMutexLocker locker(&m_mutex);
    m_drafts.clear();
}

Draft DraftManager::draft(const QString &draftId) const
{
    QMutexLocker locker(&m_mutex);
    for (const auto &d : m_drafts) {
        if (d.id == draftId) return d;
    }
    return Draft();
}

QVector<Draft> DraftManager::allDrafts() const
{
    QMutexLocker locker(&m_mutex);
    return m_drafts;
}

QVector<Draft> DraftManager::draftsForSubreddit(const QString &subreddit) const
{
    QMutexLocker locker(&m_mutex);
    QVector<Draft> result;
    for (const auto &d : m_drafts) {
        if (d.subreddit == subreddit)
            result.append(d);
    }
    return result;
}

int DraftManager::draftCount() const
{
    QMutexLocker locker(&m_mutex);
    return m_drafts.size();
}

bool DraftManager::hasDrafts() const { return draftCount() > 0; }

void DraftManager::setAutoSaveEnabled(bool enabled)
{
    m_autoSaveEnabled = enabled;
    if (enabled) startAutoSave(); else stopAutoSave();
}

bool DraftManager::isAutoSaveEnabled() const { return m_autoSaveEnabled; }

void DraftManager::setAutoSaveInterval(int seconds)
{
    m_autoSaveInterval = qMax(5, qMin(seconds, 300));
    if (m_autoSaveTimer->isActive()) {
        stopAutoSave();
        startAutoSave();
    }
}

void DraftManager::saveDrafts()
{
    QJsonArray arr;
    for (const auto &d : m_drafts)
        arr.append(d.toJson());

    QJsonDocument doc(arr);
    QDir().mkpath(QFileInfo(draftsFilePath()).absolutePath());
    QFile file(draftsFilePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void DraftManager::loadDrafts()
{
    QFile file(draftsFilePath());
    if (!file.exists()) return;
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error == QJsonParseError::NoError && doc.isArray()) {
        m_drafts.clear();
        for (const auto &val : doc.array())
            m_drafts.append(Draft::fromJson(val.toObject()));
    }
}

void DraftManager::startAutoSave()
{
    if (!m_autoSaveTimer->isActive())
        m_autoSaveTimer->start(m_autoSaveInterval * 1000);
}

void DraftManager::stopAutoSave()
{
    if (m_autoSaveTimer->isActive())
        m_autoSaveTimer->stop();
}

void DraftManager::autoSave()
{
    saveDrafts();
    emit autoSaveTriggered();
}

QString DraftManager::draftsFilePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/drafts.json";
}

} // namespace PinkReader
