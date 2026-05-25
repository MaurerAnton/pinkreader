// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/RefreshableActivity.java

#include "refreshable_activity.h"

namespace PinkReader {

RefreshableActivity::RefreshableActivity(QObject* parent)
	: ViewsBaseActivity(parent)
	, m_paused(false)
{
}

// Port of: @Override protected void onPause()
void RefreshableActivity::onPause() {
	// Port of: super.onPause();
	ViewsBaseActivity::onPause();
	// Port of: paused = true;
	m_paused = true;
}

// Port of: @Override protected void onSharedPreferenceChangedInner(
//   final SharedPrefsWrapper prefs, final String key)
void RefreshableActivity::onSharedPreferenceChangedInner(SharedPrefsWrapper* prefs, const QString& key) {
	// Port of: if(PrefsUtility.isRestartRequired(this, key)) {
	//   requestRefresh(RefreshableFragment.RESTART, false); return;
	// }
	// if(this instanceof MainActivity && PrefsUtility.isReLayoutRequired(this, key)) {
	//   requestRefresh(RefreshableFragment.MAIN_RELAYOUT, false); return;
	// }
	// if(PrefsUtility.isRefreshRequired(this, key)) {
	//   requestRefresh(RefreshableFragment.ALL, false); return;
	// }
	// if(this instanceof MainActivity) {
	//   if(key.equals(getString(R.string.pref_pinned_subreddits_key)) ||
	//     key.equals(getString(R.string.pref_blocked_subreddits_key))) {
	//     requestRefresh(RefreshableFragment.MAIN, false);
	//   }
	// }
}

// Port of: @Override protected void onResume()
void RefreshableActivity::onResume() {
	// Port of: super.onResume();
	ViewsBaseActivity::onResume();
	// Port of: paused = false;
	m_paused = false;

	// Port of: if(!refreshOnResume.isEmpty()) {
	//   for(final RefreshableFragment f : refreshOnResume) {
	//     doRefreshNow(f, false);
	//   }
	//   refreshOnResume.clear();
	// }
	if (!m_refreshOnResume.isEmpty()) {
		for (const auto& f : m_refreshOnResume) {
			doRefreshNow(f, false);
		}
		m_refreshOnResume.clear();
	}
}

// Port of: @Override public void onConfigurationChanged(@NonNull final Configuration newConfig)
void RefreshableActivity::onConfigurationChanged() {
	// Port of: invalidateOptionsMenu();
	// Port of: super.onConfigurationChanged(newConfig);
}

// Port of: protected void doRefreshNow(final RefreshableFragment which, final boolean force)
void RefreshableActivity::doRefreshNow(RefreshableFragment which, bool force) {
	// Port of: if(which == RefreshableFragment.RESTART) {
	//   General.recreateActivityNoAnimation(this);
	// } else {
	//   doRefresh(which, force, null);
	// }
	if (which == RefreshableFragment::RESTART) {
		// General.recreateActivityNoAnimation(this);
	} else {
		doRefresh(which, force, nullptr);
	}
}

// Port of: public final void requestRefresh(final RefreshableFragment which, final boolean force)
void RefreshableActivity::requestRefresh(RefreshableFragment which, bool force) {
	// Port of: runOnUiThread(() -> {
	//   if(!paused) { doRefreshNow(which, force); }
	//   else {
	//     // TODO this doesn't remember "force" // (but it doesn't really matter...)
	//     refreshOnResume.add(which);
	//   }
	// });
	if (!m_paused) {
		doRefreshNow(which, force);
	} else {
		// Port of: TODO this doesn't remember "force"
		m_refreshOnResume.insert(which);
	}
}

} // namespace PinkReader
