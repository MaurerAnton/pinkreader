#pragma once

#include "models.hpp"

#include <QObject>
#include <QSqlDatabase>
#include <QVector>

namespace PinkReader {

class CacheManager : public QObject {
    Q_OBJECT
public:
    enum class CacheMode { Offline, OnlineOnly, CacheFirst, CacheThenNetwork };

    explicit CacheManager(const QString& dbPath, QObject* parent = nullptr);
    ~CacheManager();

    bool initialize();
    void setCacheMode(CacheMode mode) { m_mode = mode; }
    CacheMode cacheMode() const { return m_mode; }

    void cachePosts(const QString& key, const QVector<Post>& posts);
    void cacheComments(const QString& postId, const QVector<Comment>& comments);
    void cacheSubreddit(const QString& name, const Subreddit& subreddit);

    std::optional<QVector<Post>> getCachedPosts(const QString& key);
    std::optional<QVector<Comment>> getCachedComments(const QString& postId);
    std::optional<Subreddit> getCachedSubreddit(const QString& name);

    void cacheThumbnail(const QString& url, const QByteArray& data);
    std::optional<QByteArray> getCachedThumbnail(const QString& url);

    QStringList getCachedSubreddits();
    void clearOldCache(int maxAgeDays = 30);
    qint64 cacheSize();
    void clearAll();

signals:
    void cacheCleared();
    void cacheError(const QString& error);

private:
    void createTables();
    QSqlDatabase m_db;
    QString m_dbPath;
    CacheMode m_mode = CacheMode::CacheThenNetwork;
};

}  // namespace PinkReader
