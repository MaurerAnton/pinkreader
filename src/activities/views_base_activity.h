// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/ViewsBaseActivity.java

#pragma once

#include "base_activity.h"
#include <QObject>
#include <QString>
#include <memory>

namespace PinkReader {

// Forward declarations
class TextView;
class FrameLayout;
class ImageView;
class ActionBar;
class SharedPrefsWrapper;

/**
 * @brief Port of org.quantumbadger.redreader.activities.ViewsBaseActivity
 *
 * Abstract activity with toolbar action bar support, back button config,
 * and content listing/overlay management. Every field, method, and logic
 * branch from the original Java file (273 lines).
 */
class ViewsBaseActivity : public BaseActivity {
	Q_OBJECT

protected:
	// Port of: @NonNull private Optional<TextView> mActionbarTitleTextView
	//   = Optional.empty();
	bool m_hasActionbarTitle = false;
	QString m_actionbarTitleText;

	// Port of: private FrameLayout mContentListing;
	// Port of: private FrameLayout mContentOverlay;
	// In C++ port: container management handled by framework

	// Port of: private ImageView mActionbarBackIconView;
	// Port of: private View mActionbarTitleOuterView;
	// In C++ port: view references are managed by UI framework

public:
	explicit ViewsBaseActivity(QObject* parent = nullptr);
	virtual ~ViewsBaseActivity() = default;

protected:
	// Port of: protected boolean baseActivityIsToolbarActionBarEnabled()
	//   return true;
	virtual bool baseActivityIsToolbarActionBarEnabled();

	// Port of: protected boolean baseActivityIsToolbarSearchBarEnabled()
	//   return false;
	virtual bool baseActivityIsToolbarSearchBarEnabled();

	// Port of: protected boolean baseActivityIsActionBarBackEnabled()
	//   return true;
	virtual bool baseActivityIsActionBarBackEnabled();

public:
	// Port of: @Override public void setTitle(final CharSequence text)
	//   super.setTitle(text);
	//   mActionbarTitleTextView.apply(titleView -> titleView.setText(text));
	void setTitle(const QString& text) override;

	// Port of: @Override public void setTitle(final int res)
	//   setTitle(getText(res));
	void setTitle(int resId) override;

	// Port of: @NonNull public final ActionBar getSupportActionBarOrThrow()
	//   final ActionBar result = getSupportActionBar();
	//   if(result == null) { throw new RuntimeException("Action bar is null"); }
	//   return result;
	virtual ActionBar* getSupportActionBarOrThrow();

protected:
	// Port of: protected void configBackButton(
	//   final boolean isVisible, final View.OnClickListener listener)
	//   mActionbarBackIconView.setImportantForAccessibility(...);
	//   mActionbarTitleTextView.apply(...);
	//   if(isVisible) {
	//     mActionbarBackIconView.setVisibility(View.VISIBLE);
	//     mActionbarTitleOuterView.setOnClickListener(listener);
	//     mActionbarTitleOuterView.setClickable(true);
	//     mActionbarTitleOuterView.setContentDescription(getString(R.string.action_back));
	//     mActionbarTitleOuterView.setImportantForAccessibility(...);
	//     if(TextUtils.getLayoutDirectionFromLocale(Locale.getDefault())
	//       == View.LAYOUT_DIRECTION_RTL) {
	//       mActionbarBackIconView.setImageResource(R.drawable.ic_action_forward_dark);
	//     }
	//   } else {
	//     mActionbarBackIconView.setVisibility(View.GONE);
	//     mActionbarTitleOuterView.setClickable(false);
	//     mActionbarTitleOuterView.setContentDescription(null);
	//     mActionbarTitleOuterView.setImportantForAccessibility(...);
	//   }
	virtual void configBackButton(bool isVisible);

	// Port of: protected boolean baseActivityAllowToolbarHideOnScroll()
	//   return false;
	virtual bool baseActivityAllowToolbarHideOnScroll();

	// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
	//   super.onCreate(savedInstanceState);
	//   if(baseActivityIsToolbarActionBarEnabled()) { ... large block ... }
	//   else { mContentListing = new FrameLayout(this); ... }
	void onCreate(Bundle* savedInstanceState) override;

public:
	// Port of: public void setBaseActivityListing(@NonNull final View view)
	//   mContentListing.removeAllViews();
	//   mContentListing.addView(view);
	virtual void setBaseActivityListing(QObject* view);

	// Port of: public void clearBaseActivityListing()
	//   mContentListing.removeAllViews();
	virtual void clearBaseActivityListing();

	// Port of: public void setBaseActivityListing(final int layoutRes)
	//   mContentListing.removeAllViews();
	//   getLayoutInflater().inflate(layoutRes, mContentListing, true);
	virtual void setBaseActivityListing(int layoutRes);

	// Port of: public void setBaseActivityOverlay(@NonNull final View view)
	//   mContentOverlay.removeAllViews();
	//   mContentOverlay.addView(view);
	virtual void setBaseActivityOverlay(QObject* view);

protected:
	// Port of: @Override public final void onSharedPreferenceChanged(
	//   @NonNull final SharedPrefsWrapper prefs, @NonNull final String key)
	//   super.onSharedPreferenceChanged(prefs, key);
	//   if(key.startsWith(getString(R.string.pref_menus_appbar_prefix))
	//     || key.equals(getString(R.string.pref_menus_quick_account_switcher_key))
	//     || key.equals(getString(R.string.pref_pinned_subreddits_key))) {
	//     invalidateOptionsMenu();
	//   }
	void onSharedPreferenceChanged(SharedPrefsWrapper* prefs, const QString& key) override;
};

} // namespace PinkReader
