// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/RefreshableActivity.java

#pragma once

#include "views_base_activity.h"
#include <QObject>
#include <QSet>
#include <QString>

namespace PinkReader {

// Forward declarations
class Bundle;
class SharedPrefsWrapper;

/**
 * @brief Port of org.quantumbadger.redreader.activities.RefreshableActivity
 *
 * Abstract base activity for refreshable fragments. Manages paused state and
 * deferred refresh requests via onResume. Every field, method, enum, and
 * logic branch from the original Java file.
 */
class RefreshableActivity : public ViewsBaseActivity {
	Q_OBJECT

public:
	// Port of: public enum RefreshableFragment {
	//   MAIN, MAIN_RELAYOUT, POSTS, COMMENTS, RESTART, ALL }
	enum class RefreshableFragment {
		MAIN,
		MAIN_RELAYOUT,
		POSTS,
		COMMENTS,
		RESTART,
		ALL
	};

protected:
	// Port of: private boolean paused = false;
	bool m_paused = false;
	// Port of: private final EnumSet<RefreshableFragment> refreshOnResume
	//   = EnumSet.noneOf(RefreshableFragment.class);
	QSet<RefreshableFragment> m_refreshOnResume;

public:
	explicit RefreshableActivity(QObject* parent = nullptr);
	virtual ~RefreshableActivity() = default;

protected:
	// Port of: @Override protected void onPause()
	//   super.onPause();
	//   paused = true;
	void onPause() override;

	// Port of: @Override protected void onSharedPreferenceChangedInner(
	//   final SharedPrefsWrapper prefs, final String key)
	//   if(PrefsUtility.isRestartRequired(this, key)) {
	//     requestRefresh(RefreshableFragment.RESTART, false); return;
	//   }
	//   if(this instanceof MainActivity && PrefsUtility.isReLayoutRequired(this, key)) {
	//     requestRefresh(RefreshableFragment.MAIN_RELAYOUT, false); return;
	//   }
	//   if(PrefsUtility.isRefreshRequired(this, key)) {
	//     requestRefresh(RefreshableFragment.ALL, false); return;
	//   }
	//   if(this instanceof MainActivity) {
	//     if(key.equals(getString(R.string.pref_pinned_subreddits_key)) ||
	//       key.equals(getString(R.string.pref_blocked_subreddits_key))) {
	//       requestRefresh(RefreshableFragment.MAIN, false);
	//     }
	//   }
	void onSharedPreferenceChangedInner(SharedPrefsWrapper* prefs, const QString& key) override;

	// Port of: @Override protected void onResume()
	//   super.onResume();
	//   paused = false;
	//   if(!refreshOnResume.isEmpty()) {
	//     for(final RefreshableFragment f : refreshOnResume) {
	//       doRefreshNow(f, false);
	//     }
	//     refreshOnResume.clear();
	//   }
	void onResume() override;

	// Port of: @Override public void onConfigurationChanged(
	//   @NonNull final Configuration newConfig)
	//   invalidateOptionsMenu();
	//   super.onConfigurationChanged(newConfig);
	void onConfigurationChanged();

	// Port of: protected void doRefreshNow(
	//   final RefreshableFragment which, final boolean force)
	//   if(which == RefreshableFragment.RESTART) {
	//     General.recreateActivityNoAnimation(this);
	//   } else {
	//     doRefresh(which, force, null);
	//   }
	virtual void doRefreshNow(RefreshableFragment which, bool force);

	// Port of: protected abstract void doRefresh(
	//   RefreshableFragment which, boolean force, final Bundle savedInstanceState);
	virtual void doRefresh(RefreshableFragment which, bool force, Bundle* savedInstanceState) = 0;

public:
	// Port of: public final void requestRefresh(
	//   final RefreshableFragment which, final boolean force)
	//   runOnUiThread(() -> {
	//     if(!paused) { doRefreshNow(which, force); }
	//     else {
	//       // TODO this doesn't remember "force" // (but it doesn't really matter...)
	//       refreshOnResume.add(which);
	//     }
	//   });
	void requestRefresh(RefreshableFragment which, bool force);
};

} // namespace PinkReader
