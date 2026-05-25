// Origin: RedReader/src/main/java/org/quantumbadger/redreader/account/RedditAccountManager.java
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include "../common/update_notifier.h"
#include "reddit_account.h"
#include "reddit_account_change_listener.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.account.RedditAccountManager
 *
 * Manages Reddit accounts using SQLite as backing store.
 * Original: extends SQLiteOpenHelper
 * Every constant, field, and method from the original Java file.
 *
 * NOTE: SQLite database operations are stubbed. This provides the
 * interface and logic structure for re-implementation on a target platform.
 */
class RedditAccountManager {
public:
	// Port of: private static final RedditAccount ANON = new RedditAccount("", null, 10, null);
	static const RedditAccount ANON;

	// Port of: private static final String ACCOUNTS_DB_FILENAME = "accounts_oauth2.db";
	static constexpr const char* ACCOUNTS_DB_FILENAME = "accounts_oauth2.db";
	// Port of: private static final String TABLE = "accounts_oauth2";
	static constexpr const char* TABLE = "accounts_oauth2";
	// Port of: private static final String FIELD_USERNAME = "username";
	static constexpr const char* FIELD_USERNAME = "username";
	// Port of: private static final String FIELD_REFRESH_TOKEN = "refresh_token";
	static constexpr const char* FIELD_REFRESH_TOKEN = "refresh_token";
	// Port of: private static final String FIELD_PRIORITY = "priority";
	static constexpr const char* FIELD_PRIORITY = "priority";
	// Port of: private static final String FIELD_CLIENT_ID = "client_id";
	static constexpr const char* FIELD_CLIENT_ID = "client_id";
	// Port of: private static final String FIELD_USES_NEW_CLIENT_ID = "uses_new_client_id";
	static constexpr const char* FIELD_USES_NEW_CLIENT_ID = "uses_new_client_id";
	// Port of: private static final int ACCOUNTS_DB_VERSION = 4;
	static constexpr int ACCOUNTS_DB_VERSION = 4;

	// Port of: @SuppressLint("StaticFieldLeak") private static RedditAccountManager singleton;
	// Port of: public static synchronized RedditAccountManager getInstance(final Context context)
	// Port of: public static RedditAccount getAnon() { return ANON; }
	static RedditAccountManager& getInstance();
	static const RedditAccount& getAnon();

	// Port of: private RedditAccountManager(final Context context)
	//   super(context.getApplicationContext(), ACCOUNTS_DB_FILENAME, null, ACCOUNTS_DB_VERSION);
	RedditAccountManager();
	~RedditAccountManager() = default;

	// Port of: @Override public void onCreate(final SQLiteDatabase db)
	//   final String queryString = String.format("CREATE TABLE %s ("
	//     + "%s TEXT NOT NULL PRIMARY KEY ON CONFLICT REPLACE,"
	//     + "%s TEXT, %s INTEGER, %s BOOLEAN NOT NULL, %s TEXT)", ...);
	//   db.execSQL(queryString);
	//   addAccount(getAnon(), db);
	void onCreate();

	// Port of: @Override public void onUpgrade(final SQLiteDatabase db,
	//   final int oldVersion, final int newVersion)
	void onUpgrade(int oldVersion, int newVersion);

	// Port of: public synchronized void addAccount(final RedditAccount account)
	//   addAccount(account, null);
	void addAccount(const RedditAccount& account);

	// Port of: public synchronized ArrayList<RedditAccount> getAccounts()
	std::vector<RedditAccount> getAccounts();

	// Port of: @Nullable public RedditAccount getAccount(@NonNull final String username)
	RedditAccount getAccount(const std::string& username);

	// Port of: public synchronized RedditAccount getDefaultAccount()
	RedditAccount getDefaultAccount();

	// Port of: public synchronized void setDefaultAccount(final RedditAccount newDefault)
	void setDefaultAccount(const RedditAccount& newDefault);

	// Port of: public void addUpdateListener(final RedditAccountChangeListener listener)
	void addUpdateListener(std::shared_ptr<RedditAccountChangeListener> listener);

	// Port of: public void removeUpdateListener(final RedditAccountChangeListener listener)
	void removeUpdateListener(RedditAccountChangeListener* listener);

	// Port of: public void deleteAccount(final RedditAccount account)
	void deleteAccount(const RedditAccount& account);

private:
	// Port of: private synchronized void addAccount(final RedditAccount account, final SQLiteDatabase inDb)
	void addAccountInternal(const RedditAccount& account, bool inDb);

	// Port of: private synchronized void reloadAccounts(final SQLiteDatabase db)
	void reloadAccounts();

	// Port of: private List<RedditAccount> accountsCache = null;
	std::vector<RedditAccount> m_accountsCache;
	// Port of: private RedditAccount defaultAccountCache = null;
	RedditAccount m_defaultAccountCache;
	bool m_accountsLoaded = false;

	// Port of: private final UpdateNotifier<RedditAccountChangeListener> updateNotifier
	//   = new UpdateNotifier<RedditAccountChangeListener>() {
	//     @Override protected void notifyListener(final RedditAccountChangeListener listener) {
	//       listener.onRedditAccountChanged(); } };
	class AccountUpdateNotifier : public UpdateNotifier<RedditAccountChangeListener> {
	protected:
		void notifyListener(RedditAccountChangeListener* listener) override {
			listener->onRedditAccountChanged();
		}
	};
	AccountUpdateNotifier m_updateNotifier;

	// Port of: synchronized methods
	std::mutex m_mutex;

	// Port of: @SuppressLint("StaticFieldLeak") private static RedditAccountManager singleton;
	static RedditAccountManager* s_singleton;
};

} // namespace PinkReader
