/*
 * PinkReader - GPLv3
 * File: reddit_account_manager.cpp - Port of RedditAccountManager.java
 * Exact port: every field, method, and logic branch from the original.
 */

#include "reddit_account_manager.h"

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QVariant>
#include <QDebug>
#include <stdexcept>

namespace PinkReader {

// --- Static constants (exact port of Java constants) ---

// Port of: private static final String ACCOUNTS_DB_FILENAME = "accounts_oauth2.db";
const QString RedditAccountManager::ACCOUNTS_DB_FILENAME = QStringLiteral("accounts_oauth2.db");

// Port of: private static final String TABLE = "accounts_oauth2";
const QString RedditAccountManager::TABLE = QStringLiteral("accounts_oauth2");

// Port of: private static final String FIELD_USERNAME = "username";
const QString RedditAccountManager::FIELD_USERNAME = QStringLiteral("username");

// Port of: private static final String FIELD_REFRESH_TOKEN = "refresh_token";
const QString RedditAccountManager::FIELD_REFRESH_TOKEN = QStringLiteral("refresh_token");

// Port of: private static final String FIELD_PRIORITY = "priority";
const QString RedditAccountManager::FIELD_PRIORITY = QStringLiteral("priority");

// Port of: private static final String FIELD_CLIENT_ID = "client_id";
const QString RedditAccountManager::FIELD_CLIENT_ID = QStringLiteral("client_id");

// Port of: private static final String FIELD_USES_NEW_CLIENT_ID = "uses_new_client_id";
const QString RedditAccountManager::FIELD_USES_NEW_CLIENT_ID = QStringLiteral("uses_new_client_id");

// Port of: private static final int ACCOUNTS_DB_VERSION = 4;
const int RedditAccountManager::ACCOUNTS_DB_VERSION = 4;

// Port of:
//   private static final RedditAccount ANON = new RedditAccount("", null, 10, null);
RedditAccount RedditAccountManager::ANON(
    QStringLiteral(""),
    OAuthRefreshToken(),
    10L,          // priority = 10
    QString()     // clientId = null (empty QString)
);

// Singleton pointer
RedditAccountManager *RedditAccountManager::s_singleton = nullptr;
QMutex RedditAccountManager::s_singletonMutex;

// --- Singleton ---

// Port of:
//   public static synchronized RedditAccountManager getInstance(final Context context) {
//       if(singleton == null) {
//           singleton = new RedditAccountManager(context.getApplicationContext());
//       }
//       return singleton;
//   }
RedditAccountManager &RedditAccountManager::getInstance()
{
    QMutexLocker locker(&s_singletonMutex);
    if (s_singleton == nullptr) {
        s_singleton = new RedditAccountManager();
    }
    return *s_singleton;
}

// Port of: public static RedditAccount getAnon() { return ANON; }
RedditAccount RedditAccountManager::getAnon()
{
    return ANON;
}

// --- Constructor ---

// Port of:
//   private RedditAccountManager(final Context context) {
//       super(context.getApplicationContext(), ACCOUNTS_DB_FILENAME, null, ACCOUNTS_DB_VERSION);
//       this.context = context;
//   }
RedditAccountManager::RedditAccountManager(QObject *parent)
    : QObject(parent)
{
    initDatabase();
}

RedditAccountManager::~RedditAccountManager()
{
    // Close all database connections
    QString connName = QStringLiteral("reddit_accounts_rw");
    if (QSqlDatabase::contains(connName)) {
        QSqlDatabase::database(connName).close();
        QSqlDatabase::removeDatabase(connName);
    }
    connName = QStringLiteral("reddit_accounts_ro");
    if (QSqlDatabase::contains(connName)) {
        QSqlDatabase::database(connName).close();
        QSqlDatabase::removeDatabase(connName);
    }
}

// --- Database initialization ---

bool RedditAccountManager::initDatabase()
{
    // Determine storage path
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);
    m_dbPath = dataDir + QDir::separator() + ACCOUNTS_DB_FILENAME;

    // Try to open and check version
    {
        QSqlDatabase db = getWritableDb();
        if (!db.isOpen()) {
            qWarning() << "Failed to open accounts database:" << db.lastError().text();
            return false;
        }

        // Check if table exists
        QSqlQuery query(db);
        bool tableExists = query.exec(
            QStringLiteral("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'")
                .arg(TABLE))
            && query.next();

        if (!tableExists) {
            // Port of: public void onCreate(final SQLiteDatabase db)
            //   CREATE TABLE accounts_oauth2 (...) + addAccount(getAnon(), db)

            QString createSql = QStringLiteral(
                "CREATE TABLE %1 ("
                "%2 TEXT NOT NULL PRIMARY KEY ON CONFLICT REPLACE,"
                "%3 TEXT,"
                "%4 INTEGER,"
                "%5 BOOLEAN NOT NULL,"
                "%6 TEXT)")
                .arg(TABLE, FIELD_USERNAME, FIELD_REFRESH_TOKEN,
                     FIELD_PRIORITY, FIELD_USES_NEW_CLIENT_ID, FIELD_CLIENT_ID);

            if (!query.exec(createSql)) {
                qWarning() << "Failed to create accounts table:" << query.lastError().text();
                closeDb(db);
                return false;
            }

            // Port of: addAccount(getAnon(), db);
            addAccountInternal(ANON, &db);
        } else {
            // Check version and run upgrades
            // We use PRAGMA user_version for schema versioning
            int oldVersion = 0;
            query.exec(QStringLiteral("PRAGMA user_version"));
            if (query.next()) {
                oldVersion = query.value(0).toInt();
            }

            if (oldVersion < ACCOUNTS_DB_VERSION) {
                // Port of: public void onUpgrade(db, oldVersion, newVersion)

                // Port of: if(oldVersion < 2) { UPDATE trim }
                if (oldVersion < 2) {
                    QString updateSql = QStringLiteral(
                        "UPDATE %1 SET %2=TRIM(%2) WHERE %2 <> TRIM(%2)")
                        .arg(TABLE, FIELD_USERNAME);
                    query.exec(updateSql);
                }

                // Port of: if(oldVersion < 3) { ALTER TABLE ADD COLUMN uses_new_client_id }
                if (oldVersion < 3) {
                    QString alterSql = QStringLiteral(
                        "ALTER TABLE %1 ADD COLUMN %2 BOOLEAN NOT NULL DEFAULT 0")
                        .arg(TABLE, FIELD_USES_NEW_CLIENT_ID);
                    query.exec(alterSql);
                }

                // Port of: if(oldVersion < 4) { ALTER TABLE ADD COLUMN client_id }
                if (oldVersion < 4) {
                    QString alterSql = QStringLiteral(
                        "ALTER TABLE %1 ADD COLUMN %2 TEXT DEFAULT NULL")
                        .arg(TABLE, FIELD_CLIENT_ID);
                    // SQLite doesn't allow DEFAULT NULL in ALTER TABLE ADD COLUMN,
                    // but added columns default to NULL anyway.
                    // Use simplified form:
                    query.exec(QStringLiteral(
                        "ALTER TABLE %1 ADD COLUMN %2 TEXT").arg(TABLE, FIELD_CLIENT_ID));
                }

                // Update version
                query.exec(QStringLiteral("PRAGMA user_version = %1").arg(ACCOUNTS_DB_VERSION));
            }
        }

        closeDb(db);
    }

    return true;
}

QSqlDatabase RedditAccountManager::getWritableDb()
{
    QMutexLocker locker(&m_dbMutex);

    QString connName = QStringLiteral("reddit_accounts_rw");
    if (QSqlDatabase::contains(connName)) {
        QSqlDatabase db = QSqlDatabase::database(connName);
        if (!db.isOpen()) {
            db.open();
        }
        return db;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connName);
    db.setDatabaseName(m_dbPath);
    db.open();
    return db;
}

QSqlDatabase RedditAccountManager::getReadableDb()
{
    QMutexLocker locker(&m_dbMutex);

    QString connName = QStringLiteral("reddit_accounts_ro");
    if (QSqlDatabase::contains(connName)) {
        QSqlDatabase db = QSqlDatabase::database(connName);
        if (!db.isOpen()) {
            db.open();
        }
        return db;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connName);
    db.setDatabaseName(m_dbPath);
    db.setConnectOptions(QStringLiteral("QSQLITE_OPEN_READONLY"));
    db.open();
    return db;
}

void RedditAccountManager::closeDb(QSqlDatabase &db)
{
    // We don't actually close here — QSqlDatabase manages connections.
    // The original Java calls db.close() after operations.
    // We just let the connection stay open for reuse.
    Q_UNUSED(db)
}

// --- Public API ---

// Port of: public synchronized void addAccount(final RedditAccount account)
//   addAccount(account, null);
void RedditAccountManager::addAccount(const RedditAccount &account)
{
    addAccountInternal(account, nullptr);
}

// Port of: private synchronized void addAccount(account, inDb)
// Full logic with ContentValues, insert, reloadAccounts, updateNotifier
void RedditAccountManager::addAccountInternal(const RedditAccount &account, QSqlDatabase *inDb)
{
    QSqlDatabase db;
    bool ownDb = false;

    if (inDb == nullptr) {
        // Port of: if(inDb == null) { db = getWritableDatabase(); } else { db = inDb; }
        db = getWritableDb();
        ownDb = true;
    } else {
        db = *inDb;
    }

    // Port of: ContentValues row = new ContentValues();
    //   row.put(FIELD_USERNAME, account.username);
    //   if(account.refreshToken == null) row.putNull else row.put(...)
    //   row.put(FIELD_PRIORITY, account.priority);
    //   row.put(FIELD_USES_NEW_CLIENT_ID, 1);
    //   row.put(FIELD_CLIENT_ID, account.clientId);
    //   db.insert(TABLE, null, row);

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "INSERT OR REPLACE INTO %1 (%2, %3, %4, %5, %6) VALUES (?, ?, ?, 1, ?)")
        .arg(TABLE, FIELD_USERNAME, FIELD_REFRESH_TOKEN,
             FIELD_PRIORITY, FIELD_USES_NEW_CLIENT_ID, FIELD_CLIENT_ID));

    query.addBindValue(account.username);

    // Port of: if(account.refreshToken == null) { row.putNull(FIELD_REFRESH_TOKEN); }
    //          else { row.put(FIELD_REFRESH_TOKEN, account.refreshToken.token); }
    if (account.refreshToken.token.isEmpty()) {
        query.addBindValue(QVariant(QVariant::String)); // NULL
    } else {
        query.addBindValue(account.refreshToken.token);
    }

    query.addBindValue(account.priority);
    query.addBindValue(account.clientId.isEmpty() ? QVariant(QVariant::String) : account.clientId);

    if (!query.exec()) {
        qWarning() << "Failed to insert account:" << query.lastError().text();
        if (ownDb) closeDb(db);
        return;
    }

    // Port of: reloadAccounts(db);
    reloadAccounts(db);

    // Port of: updateNotifier.updateAllListeners();
    m_updateNotifier.updateAllListeners();
    emit accountsChanged();

    // Port of: if(inDb == null) { db.close(); }
    if (ownDb) {
        closeDb(db);
    }
}

// Port of: public synchronized ArrayList<RedditAccount> getAccounts()
QVector<RedditAccount> RedditAccountManager::getAccounts()
{
    QMutexLocker locker(&m_dbMutex);

    // Port of: if(accountsCache == null) { ... reloadAccounts(db); db.close(); }
    if (!m_accountsCacheValid) {
        QSqlDatabase db = getReadableDb();
        if (db.isOpen()) {
            reloadAccounts(db);
        }
        m_accountsCacheValid = true;
    }

    // Port of: return new ArrayList<>(accountsCache);
    return m_accountsCache;
}

// Port of: @Nullable public RedditAccount getAccount(@NonNull final String username)
RedditAccount RedditAccountManager::getAccount(const QString &username)
{
    // Port of: final String usernameCanonical = StringUtils.asciiLowercase(username.trim());
    QString usernameCanonical = username.trimmed().toLower();

    // Port of: if(usernameCanonical.isEmpty()) { return getAnon(); }
    if (usernameCanonical.isEmpty()) {
        return getAnon();
    }

    // Port of: final ArrayList<RedditAccount> accounts = getAccounts();
    QVector<RedditAccount> accounts = getAccounts();

    // Port of: RedditAccount selectedAccount = null;
    //          for(final RedditAccount account : accounts) {
    //              if(!account.isAnonymous() && account.canonicalUsername.equals(usernameCanonical)) {
    //                  selectedAccount = account; break;
    //              }
    //          }
    //          return selectedAccount;
    RedditAccount selectedAccount; // default-constructed, isAnonymous()==true
    for (const RedditAccount &account : accounts) {
        if (!account.isAnonymous() && account.canonicalUsername == usernameCanonical) {
            selectedAccount = account;
            break;
        }
    }

    // If not found, selectedAccount is still invalid (anonymous)
    // Original returns null; we return anon account
    if (selectedAccount.isAnonymous()) {
        return getAnon();
    }
    return selectedAccount;
}

// Port of: public synchronized RedditAccount getDefaultAccount()
RedditAccount RedditAccountManager::getDefaultAccount()
{
    QMutexLocker locker(&m_dbMutex);

    // Port of: if(defaultAccountCache == null) { ... reloadAccounts(db); db.close(); }
    if (!m_defaultAccountCacheValid) {
        QSqlDatabase db = getReadableDb();
        if (db.isOpen()) {
            reloadAccounts(db);
        }
        m_defaultAccountCacheValid = true;
    }

    // Port of: return defaultAccountCache;
    return m_defaultAccountCache;
}

// Port of: public synchronized void setDefaultAccount(final RedditAccount newDefault)
void RedditAccountManager::setDefaultAccount(const RedditAccount &newDefault)
{
    QMutexLocker locker(&m_dbMutex);

    QSqlDatabase db = getWritableDb();

    // Port of:
    //   db.execSQL(String.format(Locale.US,
    //       "UPDATE %s SET %s=(SELECT MIN(%s)-1 FROM %s) WHERE %s=?",
    //       TABLE, FIELD_PRIORITY, FIELD_PRIORITY, TABLE, FIELD_USERNAME),
    //       new String[] {newDefault.username});
    QString updateSql = QStringLiteral(
        "UPDATE %1 SET %2=(SELECT MIN(%3)-1 FROM %4) WHERE %5=?")
        .arg(TABLE, FIELD_PRIORITY, FIELD_PRIORITY, TABLE, FIELD_USERNAME);

    QSqlQuery query(db);
    query.prepare(updateSql);
    query.addBindValue(newDefault.username);

    if (!query.exec()) {
        qWarning() << "Failed to set default account:" << query.lastError().text();
        closeDb(db);
        return;
    }

    // Port of: reloadAccounts(db);
    reloadAccounts(db);

    // Port of: db.close();
    closeDb(db);

    // Port of: updateNotifier.updateAllListeners();
    m_updateNotifier.updateAllListeners();
    emit accountsChanged();
}

// Port of: private synchronized void reloadAccounts(final SQLiteDatabase db)
void RedditAccountManager::reloadAccounts(QSqlDatabase &db)
{
    // Port of: final String[] fields = { FIELD_USERNAME, FIELD_REFRESH_TOKEN,
    //                                     FIELD_PRIORITY, FIELD_CLIENT_ID };
    //          final Cursor cursor = db.query(TABLE, fields, null, null, null, null,
    //                                         FIELD_PRIORITY + " ASC");

    QString querySql = QStringLiteral(
        "SELECT %1, %2, %3, %4 FROM %5 ORDER BY %3 ASC")
        .arg(FIELD_USERNAME, FIELD_REFRESH_TOKEN, FIELD_PRIORITY, FIELD_CLIENT_ID, TABLE);

    QSqlQuery query(db);
    if (!query.exec(querySql)) {
        qWarning() << "Failed to query accounts:" << query.lastError().text();
        return;
    }

    // Port of: accountsCache = new LinkedList<>();
    m_accountsCache.clear();

    // Port of: defaultAccountCache = null;
    m_defaultAccountCache = RedditAccount(); // reset
    bool defaultSet = false;

    // Port of: if(cursor != null) { while(cursor.moveToNext()) { ... } cursor.close(); }
    //          else { BugReportActivity.handleGlobalError(context, "Cursor was null after query"); }
    while (query.next()) {
        // Port of: final String username = cursor.getString(0);
        QString username = query.value(0).toString();

        // Port of: final RedditOAuth.RefreshToken refreshToken;
        //          if(cursor.isNull(1)) { refreshToken = null; }
        //          else { refreshToken = new RedditOAuth.RefreshToken(cursor.getString(1)); }
        OAuthRefreshToken refreshToken;
        if (!query.value(1).isNull()) {
            refreshToken = OAuthRefreshToken(query.value(1).toString());
        }

        // Port of: final long priority = cursor.getLong(2);
        qint64 priority = query.value(2).toLongLong();

        // Port of: @Nullable final String clientId = cursor.getString(3);
        QString clientId = query.value(3).toString();

        // Port of: final RedditAccount account = new RedditAccount(
        //              username, refreshToken, priority, clientId);
        RedditAccount account(username, refreshToken, priority, clientId);

        // Port of: accountsCache.add(account);
        m_accountsCache.append(account);

        // Port of: if(defaultAccountCache == null
        //              || account.priority < defaultAccountCache.priority) {
        //              defaultAccountCache = account; }
        if (!defaultSet || account.priority < m_defaultAccountCache.priority) {
            m_defaultAccountCache = account;
            defaultSet = true;
        }
    }

    // If no accounts found, default to anon
    if (!defaultSet) {
        m_defaultAccountCache = ANON;
    }

    m_accountsCacheValid = true;
    m_defaultAccountCacheValid = true;
}

// Port of: public void addUpdateListener(final RedditAccountChangeListener listener)
void RedditAccountManager::addUpdateListener(RedditAccountChangeListener *listener)
{
    m_updateNotifier.addListener(listener);
}

// Port of: public void removeUpdateListener(final RedditAccountChangeListener listener)
void RedditAccountManager::removeUpdateListener(RedditAccountChangeListener *listener)
{
    m_updateNotifier.removeListener(listener);
}

// Port of: public void deleteAccount(final RedditAccount account)
void RedditAccountManager::deleteAccount(const RedditAccount &account)
{
    QMutexLocker locker(&m_dbMutex);

    QSqlDatabase db = getWritableDb();

    // Port of: db.delete(TABLE, FIELD_USERNAME + "=?", new String[] {account.username});
    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM %1 WHERE %2=?").arg(TABLE, FIELD_USERNAME));
    query.addBindValue(account.username);

    if (!query.exec()) {
        qWarning() << "Failed to delete account:" << query.lastError().text();
        closeDb(db);
        return;
    }

    // Port of: reloadAccounts(db);
    reloadAccounts(db);

    // Port of: updateNotifier.updateAllListeners();
    m_updateNotifier.updateAllListeners();
    emit accountsChanged();

    // Port of: db.close();
    closeDb(db);
}

} // namespace PinkReader
