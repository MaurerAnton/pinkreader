#include "cache_manager.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

namespace PinkReader {

CacheManager::CacheManager(const QString& dbPath, QObject* parent)
    : QObject(parent)
    , m_dbPath(dbPath)
{
}

CacheManager::~CacheManager() {
    if (m_db.isOpen()) m_db.close();
}

bool CacheManager::initialize() {
    m_db = QSqlDatabase::addDatabase("QSQLITE", "pinkreader_cache");
    m_db.setDatabaseName(m_dbPath);
    
    if (!m_db.open()) {
        emit cacheError("Failed to open cache database: " + m_db.lastError().text());
        return false;
    }
    
    createTables();
    return true;
}

void CacheManager::createTables() {
    QSqlQuery q(m_db);
    
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS posts (
            id TEXT PRIMARY KEY,
            fullname TEXT,
            feed_key TEXT,
            json_data TEXT,
            cached_at INTEGER,
            UNIQUE(id, feed_key)
        )
    )");
    
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS comments (
            post_id TEXT,
            json_data TEXT,
            cached_at INTEGER,
            PRIMARY KEY(post_id)
        )
    )");
    
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS subreddits (
            name TEXT PRIMARY KEY,
            json_data TEXT,
            cached_at INTEGER
        )
    )");
    
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS thumbnails (
            url_hash TEXT PRIMARY KEY,
            data BLOB,
            cached_at INTEGER
        )
    )");
    
    q.exec(R"(
        CREATE INDEX IF NOT EXISTS idx_posts_feed ON posts(feed_key, cached_at)
    )");
    
    q.exec(R"(
        CREATE INDEX IF NOT EXISTS idx_thumbnails ON thumbnails(cached_at)
    )");
}

void CacheManager::cachePosts(const QString& key, const QVector<Post>& posts) {
    QSqlQuery q(m_db);
    q.prepare("INSERT OR REPLACE INTO posts (id, fullname, feed_key, json_data, cached_at) VALUES (?, ?, ?, ?, ?)");
    
    qint64 now = QDateTime::currentSecsSinceEpoch();
    
    for (const auto& post : posts) {
        QJsonObject obj = post.toJson();
        QJsonDocument doc(obj);
        
        q.addBindValue(post.id);
        q.addBindValue(post.fullname);
        q.addBindValue(key);
        q.addBindValue(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
        q.addBindValue(now);
        
        if (!q.exec()) {
            qWarning() << "Cache post error:" << q.lastError().text();
        }
    }
}

void CacheManager::cacheComments(const QString& postId, const QVector<Comment>& comments) {
    QSqlQuery q(m_db);
    
    QJsonArray arr;
    for (const auto& c : comments) {
        arr.append(c.toJson());
    }
    QJsonDocument doc(arr);
    
    q.prepare("INSERT OR REPLACE INTO comments (post_id, json_data, cached_at) VALUES (?, ?, ?)");
    q.addBindValue(postId);
    q.addBindValue(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    q.addBindValue(QDateTime::currentSecsSinceEpoch());
    
    if (!q.exec()) {
        qWarning() << "Cache comments error:" << q.lastError().text();
    }
}

void CacheManager::cacheSubreddit(const QString& name, const Subreddit& subreddit) {
    QSqlQuery q(m_db);
    QJsonDocument doc(subreddit.toJson());
    
    q.prepare("INSERT OR REPLACE INTO subreddits (name, json_data, cached_at) VALUES (?, ?, ?)");
    q.addBindValue(name);
    q.addBindValue(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    q.addBindValue(QDateTime::currentSecsSinceEpoch());
    q.exec();
}

std::optional<QVector<Post>> CacheManager::getCachedPosts(const QString& key) {
    QSqlQuery q(m_db);
    q.prepare("SELECT json_data FROM posts WHERE feed_key = ? ORDER BY cached_at DESC");
    q.addBindValue(key);
    
    if (!q.exec()) return std::nullopt;
    
    QVector<Post> posts;
    while (q.next()) {
        QJsonDocument doc = QJsonDocument::fromJson(q.value(0).toString().toUtf8());
        posts.append(Post::fromJson(doc.object()));
    }
    
    return posts.isEmpty() ? std::nullopt : std::make_optional(posts);
}

std::optional<QVector<Comment>> CacheManager::getCachedComments(const QString& postId) {
    QSqlQuery q(m_db);
    q.prepare("SELECT json_data FROM comments WHERE post_id = ?");
    q.addBindValue(postId);
    
    if (!q.exec() || !q.next()) return std::nullopt;
    
    QJsonDocument doc = QJsonDocument::fromJson(q.value(0).toString().toUtf8());
    QVector<Comment> comments;
    for (const auto& val : doc.array()) {
        comments.append(Comment::fromJson(val.toObject()));
    }
    
    return comments.isEmpty() ? std::nullopt : std::make_optional(comments);
}

std::optional<Subreddit> CacheManager::getCachedSubreddit(const QString& name) {
    QSqlQuery q(m_db);
    q.prepare("SELECT json_data FROM subreddits WHERE name = ?");
    q.addBindValue(name);
    
    if (!q.exec() || !q.next()) return std::nullopt;
    
    QJsonDocument doc = QJsonDocument::fromJson(q.value(0).toString().toUtf8());
    return Subreddit::fromJson(doc.object());
}

void CacheManager::cacheThumbnail(const QString& url, const QByteArray& data) {
    QSqlQuery q(m_db);
    q.prepare("INSERT OR REPLACE INTO thumbnails (url_hash, data, cached_at) VALUES (?, ?, ?)");
    q.addBindValue(QString::number(qHash(url)));
    q.addBindValue(data);
    q.addBindValue(QDateTime::currentSecsSinceEpoch());
    q.exec();
}

std::optional<QByteArray> CacheManager::getCachedThumbnail(const QString& url) {
    QSqlQuery q(m_db);
    q.prepare("SELECT data FROM thumbnails WHERE url_hash = ?");
    q.addBindValue(QString::number(qHash(url)));
    
    if (q.exec() && q.next()) {
        return q.value(0).toByteArray();
    }
    return std::nullopt;
}

QStringList CacheManager::getCachedSubreddits() {
    QStringList result;
    QSqlQuery q(m_db);
    q.exec("SELECT name FROM subreddits ORDER BY name");
    while (q.next()) {
        result.append(q.value(0).toString());
    }
    return result;
}

void CacheManager::clearOldCache(int maxAgeDays) {
    qint64 cutoff = QDateTime::currentSecsSinceEpoch() - (maxAgeDays * 86400);
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM posts WHERE cached_at < ?");
    q.addBindValue(cutoff);
    q.exec();
    
    q.prepare("DELETE FROM comments WHERE cached_at < ?");
    q.addBindValue(cutoff);
    q.exec();
    
    q.prepare("DELETE FROM subreddits WHERE cached_at < ?");
    q.addBindValue(cutoff);
    q.exec();
    
    q.prepare("DELETE FROM thumbnails WHERE cached_at < ?");
    q.addBindValue(cutoff);
    q.exec();
    
    q.exec("VACUUM");
}

qint64 CacheManager::cacheSize() {
    QSqlQuery q(m_db);
    q.exec("SELECT SUM(LENGTH(json_data)) + SUM(LENGTH(data)) FROM posts, comments, thumbnails");
    if (q.next()) return q.value(0).toLongLong();
    return 0;
}

void CacheManager::clearAll() {
    QSqlQuery q(m_db);
    q.exec("DELETE FROM posts");
    q.exec("DELETE FROM comments");
    q.exec("DELETE FROM subreddits");
    q.exec("DELETE FROM thumbnails");
    q.exec("VACUUM");
    emit cacheCleared();
}

} // namespace PinkReader
