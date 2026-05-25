/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: draft_manager.h - Post/comment draft manager
 */


#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QJsonObject>
#include <QMutex>
#include <QTimer>

namespace PinkReader {

struct Draft {
    QString id;
    QString type;         // "post", "comment", "message"
    QString subreddit;
    QString parentId;     // For comment replies
    QString title;
    QString body;
    QDateTime createdAt;
    QDateTime updatedAt;
    bool isSending = false;

    QJsonObject toJson() const;
    static Draft fromJson(const QJsonObject &json);
};

class DraftManager : public QObject
{
    Q_OBJECT

public:
    explicit DraftManager(QObject *parent = nullptr);
    ~DraftManager() override;

    void initialize();

    QString createDraft(const QString &type, const QString &subreddit,
                        const QString &parentId = QString());
    void updateDraft(const QString &draftId, const QString &title,
                     const QString &body);
    void deleteDraft(const QString &draftId);
    void deleteAllDrafts();

    Draft draft(const QString &draftId) const;
    QVector<Draft> allDrafts() const;
    QVector<Draft> draftsForSubreddit(const QString &subreddit) const;

    int draftCount() const;
    bool hasDrafts() const;

    void setAutoSaveEnabled(bool enabled);
    bool isAutoSaveEnabled() const;

    void setAutoSaveInterval(int seconds);

signals:
    void draftCreated(const QString &draftId);
    void draftUpdated(const QString &draftId);
    void draftDeleted(const QString &draftId);
    void autoSaveTriggered();

private:
    void saveDrafts();
    void loadDrafts();
    void startAutoSave();
    void stopAutoSave();
    void autoSave();

    QVector<Draft> m_drafts;
    mutable QMutex m_mutex;
    QTimer *m_autoSaveTimer = nullptr;
    bool m_autoSaveEnabled = true;
    int m_autoSaveInterval = 30; // seconds
    int m_nextDraftNumber = 0;

    QString draftsFilePath() const;
};

} // namespace PinkReader
