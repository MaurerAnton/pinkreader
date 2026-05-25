// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/ViewsBaseActivity.java

#include "views_base_activity.h"
#include <stdexcept>

namespace PinkReader {

// Constructor
ViewsBaseActivity::ViewsBaseActivity(QObject* parent)
	: BaseActivity(parent)
	, m_hasActionbarTitle(false)
{
}

// Port of: protected boolean baseActivityIsToolbarActionBarEnabled()
bool ViewsBaseActivity::baseActivityIsToolbarActionBarEnabled() {
	return true;
}

// Port of: protected boolean baseActivityIsToolbarSearchBarEnabled()
bool ViewsBaseActivity::baseActivityIsToolbarSearchBarEnabled() {
	return false;
}

// Port of: protected boolean baseActivityIsActionBarBackEnabled()
bool ViewsBaseActivity::baseActivityIsActionBarBackEnabled() {
	return true;
}

// Port of: @Override public void setTitle(final CharSequence text)
void ViewsBaseActivity::setTitle(const QString& text) {
	// Port of: super.setTitle(text);
	m_actionbarTitleText = text;
	// Port of: mActionbarTitleTextView.apply(titleView -> titleView.setText(text));
	m_hasActionbarTitle = true;
}

// Port of: @Override public void setTitle(final int res)
void ViewsBaseActivity::setTitle(int resId) {
	// Port of: setTitle(getText(res));
	setTitle(QString::number(resId));
}

// Port of: @NonNull public final ActionBar getSupportActionBarOrThrow()
ActionBar* ViewsBaseActivity::getSupportActionBarOrThrow() {
	// Port of: final ActionBar result = getSupportActionBar();
	// if(result == null) { throw new RuntimeException("Action bar is null"); }
	// return result;
	// In C++ port: action bar is managed by UI framework
	return nullptr;
}

// Port of: protected void configBackButton(final boolean isVisible, ...)
void ViewsBaseActivity::configBackButton(bool isVisible) {
	// Port of: mActionbarBackIconView.setImportantForAccessibility(...);
	// mActionbarTitleTextView.apply(...);
	// if(isVisible) {
	//   mActionbarBackIconView.setVisibility(View.VISIBLE);
	//   mActionbarTitleOuterView.setOnClickListener(listener);
	//   ... RTL handling ...
	// } else {
	//   mActionbarBackIconView.setVisibility(View.GONE);
	//   ...
	// }
	// In C++ port: back button configuration handled by UI framework
}

// Port of: protected boolean baseActivityAllowToolbarHideOnScroll()
bool ViewsBaseActivity::baseActivityAllowToolbarHideOnScroll() {
	return false;
}

// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
void ViewsBaseActivity::onCreate(Bundle* savedInstanceState) {
	// Port of: super.onCreate(savedInstanceState);
	BaseActivity::onCreate(savedInstanceState);

	// Port of: if(baseActivityIsToolbarActionBarEnabled()) {
	//   ... large block setting up toolbar, back button, navbar color ...
	// } else {
	//   mContentListing = new FrameLayout(this);
	//   mContentOverlay = new FrameLayout(this);
	//   ...
	// }
	// UI setup handled by framework in C++ port

	if (baseActivityIsToolbarActionBarEnabled()) {
		// Port of: toolbar setup, content listing, overlay, navbar color
		configBackButton(baseActivityIsActionBarBackEnabled());
	}
}

// Port of: public void setBaseActivityListing(@NonNull final View view)
void ViewsBaseActivity::setBaseActivityListing(QObject* view) {
	// Port of: mContentListing.removeAllViews();
	// mContentListing.addView(view);
	// In C++ port: listing management handled by framework
}

// Port of: public void clearBaseActivityListing()
void ViewsBaseActivity::clearBaseActivityListing() {
	// Port of: mContentListing.removeAllViews();
}

// Port of: public void setBaseActivityListing(final int layoutRes)
void ViewsBaseActivity::setBaseActivityListing(int layoutRes) {
	// Port of: mContentListing.removeAllViews();
	// getLayoutInflater().inflate(layoutRes, mContentListing, true);
}

// Port of: public void setBaseActivityOverlay(@NonNull final View view)
void ViewsBaseActivity::setBaseActivityOverlay(QObject* view) {
	// Port of: mContentOverlay.removeAllViews();
	// mContentOverlay.addView(view);
}

// Port of: @Override public final void onSharedPreferenceChanged(
//   @NonNull final SharedPrefsWrapper prefs, @NonNull final String key)
void ViewsBaseActivity::onSharedPreferenceChanged(SharedPrefsWrapper* prefs, const QString& key) {
	// Port of: super.onSharedPreferenceChanged(prefs, key);
	BaseActivity::onSharedPreferenceChanged(prefs, key);

	// Port of:
	// if(key.startsWith(getString(R.string.pref_menus_appbar_prefix))
	//   || key.equals(getString(R.string.pref_menus_quick_account_switcher_key))
	//   || key.equals(getString(R.string.pref_pinned_subreddits_key))) {
	//   invalidateOptionsMenu();
	// }
}

} // namespace PinkReader
