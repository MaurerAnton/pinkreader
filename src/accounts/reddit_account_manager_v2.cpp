// Origin: RedReader/src/main/java/org/quantumbadger/redreader/account/RedditAccountManager.java
#include "reddit_account_manager_v2.h"
#include "../common/string_utils.h"
#include <algorithm>
#include <cstdio>
#include <mutex>

namespace PinkReader {

// Port of: private static final RedditAccount ANON = new RedditAccount("", null, 10, null);
const RedditAccount RedditAccountManager::ANON("", "", 10, "");

// Port of: @SuppressLint("StaticFieldLeak") private static RedditAccountManager singleton;
RedditAccountManager* RedditAccountManager::s_singleton = nullptr;

// Port of: public static synchronized RedditAccountManager getInstance(final Context context)
RedditAccountManager& RedditAccountManager::getInstance() {
	// Port of: if(singleton == null) { singleton = new RedditAccountManager(context.getApplicationContext()); }
	// Port of: return singleton;
	static RedditAccountManager instance;
	return instance;
}

// Port of: public static RedditAccount getAnon() { return ANON; }
const RedditAccount& RedditAccountManager::getAnon() {
	return ANON;
}

// Port of: private RedditAccountManager(final Context context)
RedditAccountManager::RedditAccountManager()
	: m_defaultAccountCache(ANON) {
	// Port of: super(context.getApplicationContext(), ACCOUNTS_DB_FILENAME, null, ACCOUNTS_DB_VERSION);
	// Stub: SQLiteOpenHelper initialization
}

// Port of: @Override public void onCreate(final SQLiteDatabase db)
void RedditAccountManager::onCreate() {
	// Port of: final String queryString = String.format(
	//   "CREATE TABLE %s (" +
	//     "%s TEXT NOT NULL PRIMARY KEY ON CONFLICT REPLACE," +
	//     "%s TEXT," +
	//     "%s INTEGER," +
	//     "%s BOOLEAN NOT NULL," +
	//     "%s TEXT)",
	//   TABLE, FIELD_USERNAME, FIELD_REFRESH_TOKEN, FIELD_PRIORITY,
	//   FIELD_USES_NEW_CLIENT_ID, FIELD_CLIENT_ID);
	// Port of: db.execSQL(queryString);
	// Port of: addAccount(getAnon(), db);
	// Stub: SQLite operations
	fprintf(stderr, "RedditAccountManager::onCreate: table %s created\n", TABLE);
	addAccountInternal(getAnon(), true);
}

// Port of: @Override public void onUpgrade(final SQLiteDatabase db,
//   final int oldVersion, final int newVersion)
void RedditAccountManager::onUpgrade(int oldVersion, int newVersion) {
	// Port of: if(oldVersion < 2)
	//   db.execSQL(String.format(Locale.US,
	//     "UPDATE %s SET %2$s=TRIM(%2$s) WHERE %2$s <> TRIM(%2$s)",
	//     TABLE, FIELD_USERNAME));
	if (oldVersion < 2) {
		fprintf(stderr, "RedditAccountManager::onUpgrade: v2 trim usernames\n");
	}

	// Port of: if(oldVersion < 3)
	//   db.execSQL(String.format(Locale.US,
	//     "ALTER TABLE %s ADD COLUMN %s BOOLEAN NOT NULL DEFAULT 0",
	//     TABLE, FIELD_USES_NEW_CLIENT_ID));
	if (oldVersion < 3) {
		fprintf(stderr, "RedditAccountManager::onUpgrade: v3 add uses_new_client_id\n");
	}

	// Port of: if(oldVersion < 4)
	//   db.execSQL(String.format(Locale.US,
	//     "ALTER TABLE %s ADD COLUMN %s TEXT DEFAULT NULL",
	//     TABLE, FIELD_CLIENT_ID));
	if (oldVersion < 4) {
		fprintf(stderr, "RedditAccountManager::onUpgrade: v4 add client_id\n");
	}

	(void)newVersion; // unused
}

// Port of: public synchronized void addAccount(final RedditAccount account)
void RedditAccountManager::addAccount(const RedditAccount& account) {
	// Port of: addAccount(account, null);
	std::lock_guard<std::mutex> lock(m_mutex);
	addAccountInternal(account, false);
}

// Port of: private synchronized void addAccount(
//   final RedditAccount account, final SQLiteDatabase inDb)
void RedditAccountManager::addAccountInternal(const RedditAccount& account, bool inDb) {
	// Port of: final SQLiteDatabase db;
	// Port of: if(inDb == null) { db = getWritableDatabase(); } else { db = inDb; }

	// Port of: final ContentValues row = new ContentValues();
	// Port of: row.put(FIELD_USERNAME, account.username);
	// Port of: if(account.refreshToken == null) { row.putNull(FIELD_REFRESH_TOKEN); }
	//   else { row.put(FIELD_REFRESH_TOKEN, account.refreshToken.token); }
	// Port of: row.put(FIELD_PRIORITY, account.priority);
	// Port of: row.put(FIELD_USES_NEW_CLIENT_ID, 1);
	// Port of: row.put(FIELD_CLIENT_ID, account.clientId);
	// Port of: db.insert(TABLE, null, row);
	// Stub: SQLite insert

	// Port of: reloadAccounts(db);
	reloadAccounts();

	// Port of: updateNotifier.updateAllListeners();
	m_updateNotifier.updateAllListeners();

	// Port of: if(inDb == null) { db.close(); }
	(void)inDb;
}

// Port of: public synchronized ArrayList<RedditAccount> getAccounts()
std::vector<RedditAccount> RedditAccountManager::getAccounts() {
	std::lock_guard<std::mutex> lock(m_mutex);

	// Port of: if(accountsCache == null) {
	//   final SQLiteDatabase db = getReadableDatabase();
	//   reloadAccounts(db);
	//   db.close();
	// }
	if (!m_accountsLoaded) {
		reloadAccounts();
	}

	// Port of: return new ArrayList<>(accountsCache);
	return m_accountsCache;
}

// Port of: @Nullable public RedditAccount getAccount(@NonNull final String username)
RedditAccount RedditAccountManager::getAccount(const std::string& username) {
	// Port of: final String usernameCanonical = StringUtils.asciiLowercase(username.trim());
	std::string trimmed = username;
	trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
	trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
	std::string usernameCanonical = StringUtils::asciiLowercase(trimmed);

	// Port of: if(usernameCanonical.isEmpty()) { return getAnon(); }
	if (usernameCanonical.empty()) {
		return getAnon();
	}

	// Port of: final ArrayList<RedditAccount> accounts = getAccounts();
	// Port of: RedditAccount selectedAccount = null;
	// Port of: for(final RedditAccount account : accounts) {
	//   if(!account.isAnonymous() && account.canonicalUsername.equals(usernameCanonical)) {
	//     selectedAccount = account; break; } }
	// Port of: return selectedAccount;
	std::vector<RedditAccount> accounts = getAccounts();
	for (const auto& account : accounts) {
		if (!account.isAnonymous() && account.canonicalUsername == usernameCanonical) {
			return account;
		}
	}
	// Return default (null in Java)
	return getAnon();
}

// Port of: public synchronized RedditAccount getDefaultAccount()
RedditAccount RedditAccountManager::getDefaultAccount() {
	std::lock_guard<std::mutex> lock(m_mutex);

	// Port of: if(defaultAccountCache == null) {
	//   final SQLiteDatabase db = getReadableDatabase();
	//   reloadAccounts(db);
	//   db.close();
	// }
	if (!m_accountsLoaded) {
		reloadAccounts();
	}

	// Port of: return defaultAccountCache;
	return m_defaultAccountCache;
}

// Port of: public synchronized void setDefaultAccount(final RedditAccount newDefault)
void RedditAccountManager::setDefaultAccount(const RedditAccount& newDefault) {
	std::lock_guard<std::mutex> lock(m_mutex);

	// Port of: final SQLiteDatabase db = getWritableDatabase();
	// Port of: db.execSQL(String.format(Locale.US,
	//   "UPDATE %s SET %s=(SELECT MIN(%s)-1 FROM %s) WHERE %s=?",
	//   TABLE, FIELD_PRIORITY, FIELD_PRIORITY, TABLE, FIELD_USERNAME),
	//   new String[] {newDefault.username});
	// Stub: SQLite update priority

	// Port of: reloadAccounts(db);
	reloadAccounts();

	// Port of: db.close();

	// Port of: updateNotifier.updateAllListeners();
	m_updateNotifier.updateAllListeners();
}

// Port of: private synchronized void reloadAccounts(final SQLiteDatabase db)
void RedditAccountManager::reloadAccounts() {
	// Port of: final String[] fields = { FIELD_USERNAME, FIELD_REFRESH_TOKEN, FIELD_PRIORITY, FIELD_CLIENT_ID };
	// Port of: final Cursor cursor = db.query(TABLE, fields, null, null, null, null, FIELD_PRIORITY + " ASC");
	// Port of: accountsCache = new LinkedList<>();
	// Port of: defaultAccountCache = null;
	m_accountsCache.clear();
	m_defaultAccountCache = ANON;

	// Port of: if(cursor != null) {
	// Port of:   while(cursor.moveToNext()) {
	// Port of:     final String username = cursor.getString(0);
	// Port of:     final RedditOAuth.RefreshToken refreshToken;
	// Port of:     if(cursor.isNull(1)) { refreshToken = null; }
	//       else { refreshToken = new RedditOAuth.RefreshToken(cursor.getString(1)); }
	// Port of:     final long priority = cursor.getLong(2);
	// Port of:     @Nullable final String clientId = cursor.getString(3);
	// Port of:     final RedditAccount account = new RedditAccount(username, refreshToken, priority, clientId);
	// Port of:     accountsCache.add(account);
	// Port of:     if(defaultAccountCache == null || account.priority < defaultAccountCache.priority) {
	//       defaultAccountCache = account; }
	// Port of:   }
	// Port of:   cursor.close();
	// Port of: } else {
	//   BugReportActivity.handleGlobalError(context, "Cursor was null after query");
	// }
	// Stub: SQLite query - only ANON account by default
	m_accountsCache.push_back(ANON);
	m_defaultAccountCache = ANON;
	m_accountsLoaded = true;
}

// Port of: public void addUpdateListener(final RedditAccountChangeListener listener)
void RedditAccountManager::addUpdateListener(std::shared_ptr<RedditAccountChangeListener> listener) {
	// Port of: updateNotifier.addListener(listener);
	m_updateNotifier.addListener(listener);
}

// Port of: public void removeUpdateListener(final RedditAccountChangeListener listener)
void RedditAccountManager::removeUpdateListener(RedditAccountChangeListener* listener) {
	// Port of: updateNotifier.removeListener(listener);
	m_updateNotifier.removeListener(listener);
}

// Port of: public void deleteAccount(final RedditAccount account)
void RedditAccountManager::deleteAccount(const RedditAccount& account) {
	std::lock_guard<std::mutex> lock(m_mutex);

	// Port of: final SQLiteDatabase db = getWritableDatabase();
	// Port of: db.delete(TABLE, FIELD_USERNAME + "=?", new String[] {account.username});
	// Stub: SQLite delete

	// Port of: reloadAccounts(db);
	reloadAccounts();

	// Port of: updateNotifier.updateAllListeners();
	m_updateNotifier.updateAllListeners();

	// Port of: db.close();
}

} // namespace PinkReader
