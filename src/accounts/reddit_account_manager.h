/*
 * PinkReader - GPLv3
 * File: reddit_account_manager.h - Port of RedditAccountManager.java
 * Exact port: every field, method, and logic branch from the original.
 *
 * Original extends SQLiteOpenHelper. We replicate the DB schema exactly:
 *   TABLE accounts_oauth2 (username TEXT, refresh_token TEXT, priority INTEGER,
 *                          uses_new_client_id BOOLEAN NOT NULL, client_id TEXT)
 *   DB Version: 4
 */

#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QMutex>
#include <QSqlDatabase>
#include <functional>
#include <vector>
#include <memory>

#include "reddit_account.h"

namespace PinkReader {

// Port of RedditAccountChangeListener.java
class RedditAccountChangeListener {
public:
    virtual ~RedditAccountChangeListener() = default;
    virtual void onRedditAccountChanged() = 0;
};

// Port of UpdateNotifier<E> (templated listener manager)
// Original uses WeakReference<>, we use raw pointers (caller manages lifetime).
template<typename E>
class UpdateNotifier {
public:
    void addListener(E *listener) {
        QMutexLocker locker(&m_mutex);
        m_listeners.push_back(listener);
    }

    void removeListener(E *listener) {
        QMutexLocker locker(&m_mutex);
        auto it = m_listeners.begin();
        while (it != m_listeners.end()) {
            if (*it == nullptr || *it == listener) {
                it = m_listeners.erase(it);
            } else {
                ++it;
            }
        }
    }

    void updateAllListeners() {
        QMutexLocker locker(&m_mutex);
        auto it = m_listeners.begin();
        while (it != m_listeners.end()) {
            if (*it == nullptr) {
                it = m_listeners.erase(it);
            } else {
                notifyListener(*it);
                ++it;
            }
        }
    }

protected:
    virtual void notifyListener(E *listener) = 0;

private:
    QMutex m_mutex;
    std::vector<E *> m_listeners;
};

// Port of RedditAccountManager (extends SQLiteOpenHelper)
// Singleton pattern matching the original.

class RedditAccountManager : public QObject {
    Q_OBJECT

public:
    // Singleton (port of: public static synchronized RedditAccountManager getInstance)
    static RedditAccountManager &getInstance();

    // Port of: public static RedditAccount getAnon()
    static RedditAccount getAnon();

    // Port of: public synchronized void addAccount(final RedditAccount account)
    void addAccount(const RedditAccount &account);

    // Port of: public synchronized ArrayList<RedditAccount> getAccounts()
    QVector<RedditAccount> getAccounts();

    // Port of: @Nullable public RedditAccount getAccount(@NonNull final String username)
    RedditAccount getAccount(const QString &username); // returns anon if not found

    // Port of: public synchronized RedditAccount getDefaultAccount()
    RedditAccount getDefaultAccount();

    // Port of: public synchronized void setDefaultAccount(final RedditAccount newDefault)
    void setDefaultAccount(const RedditAccount &newDefault);

    // Port of: public void addUpdateListener(final RedditAccountChangeListener listener)
    void addUpdateListener(RedditAccountChangeListener *listener);

    // Port of: public void removeUpdateListener(final RedditAccountChangeListener listener)
    void removeUpdateListener(RedditAccountChangeListener *listener);

    // Port of: public void deleteAccount(final RedditAccount account)
    void deleteAccount(const RedditAccount &account);

signals:
    void accountsChanged();

private:
    // Private constructor (port of: private RedditAccountManager(final Context context))
    explicit RedditAccountManager(QObject *parent = nullptr);
    ~RedditAccountManager() override;
    Q_DISABLE_COPY(RedditAccountManager)

    // Database helpers
    bool initDatabase();
    QSqlDatabase getWritableDb();
    QSqlDatabase getReadableDb();
    void closeDb(QSqlDatabase &db);

    // Port of: private synchronized void addAccount(account, inDb)
    void addAccountInternal(const RedditAccount &account, QSqlDatabase *inDb);

    // Port of: private synchronized void reloadAccounts(final SQLiteDatabase db)
    void reloadAccounts(QSqlDatabase &db);

    // DB constants (exact port of Java constants)
    static const QString ACCOUNTS_DB_FILENAME;
    static const QString TABLE;
    static const QString FIELD_USERNAME;
    static const QString FIELD_REFRESH_TOKEN;
    static const QString FIELD_PRIORITY;
    static const QString FIELD_CLIENT_ID;
    static const QString FIELD_USES_NEW_CLIENT_ID;
    static const int ACCOUNTS_DB_VERSION;

    // Anon account (port of: private static final RedditAccount ANON)
    static RedditAccount ANON;

    // Port of: private List<RedditAccount> accountsCache = null;
    QVector<RedditAccount> m_accountsCache;
    bool m_accountsCacheValid = false;

    // Port of: private RedditAccount defaultAccountCache = null;
    RedditAccount m_defaultAccountCache;
    bool m_defaultAccountCacheValid = false;

    // Port of: private final UpdateNotifier<RedditAccountChangeListener> updateNotifier
    class AccountUpdateNotifier : public UpdateNotifier<RedditAccountChangeListener> {
    protected:
        void notifyListener(RedditAccountChangeListener *listener) override {
            listener->onRedditAccountChanged();
        }
    };
    AccountUpdateNotifier m_updateNotifier;

    // Singleton (port of: @SuppressLint("StaticFieldLeak") private static RedditAccountManager singleton)
    static RedditAccountManager *s_singleton;
    static QMutex s_singletonMutex;

    QMutex m_dbMutex;
    QString m_dbPath;
};

} // namespace PinkReader
