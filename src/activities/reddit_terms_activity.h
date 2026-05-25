// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/RedditTermsActivity.kt

#pragma once

#include "views_base_activity.h"
#include <QObject>
#include <QString>

namespace PinkReader {

// Forward declarations
class Bundle;

/**
 * @brief Port of org.quantumbadger.redreader.activities.RedditTermsActivity
 *
 * Displays Reddit terms of service agreement with accept/decline actions.
 * Accepting/declining sets preferences and finishes the activity.
 * Every field, method, companion object, and logic branch from the
 * original Kotlin file (87 lines).
 */
class RedditTermsActivity : public ViewsBaseActivity {
	Q_OBJECT

public:
	// Port of: companion object extraLaunchMain
	static constexpr const char* EXTRA_LAUNCH_MAIN = "launch_main";

	// Port of: @JvmStatic fun launch(activity: AppCompatActivity, launchMainOnClose: Boolean)
	//   val intent = Intent(activity, RedditTermsActivity::class.java)
	//   intent.putExtra(extraLaunchMain, launchMainOnClose)
	//   activity.startActivity(intent)
	static void launch(QObject* activity, bool launchMainOnClose);

	explicit RedditTermsActivity(QObject* parent = nullptr);
	virtual ~RedditTermsActivity() = default;

protected:
	// Port of: override fun baseActivityIsActionBarBackEnabled() = false
	bool baseActivityIsActionBarBackEnabled() override;

	// Port of: override fun onCreate(savedInstanceState: Bundle?)
	//   PrefsUtility.applySettingsTheme(this)
	//   super.onCreate(savedInstanceState)
	//   window.navigationBarColor = Color.rgb(0x55, 0x55, 0x55)
	//   setBaseActivityListing(R.layout.reddit_terms_activity)
	//   val launchMainOnClose = intent.getBooleanExtra(extraLaunchMain, false)
	//   fun onClick(@IdRes id: Int, action: () -> Unit) { ... }
	//   fun onDone() {
	//     if(launchMainOnClose) { startActivity(Intent(this, MainActivity::class.java)) }
	//     finish()
	//   }
	//   onClick(R.id.terms_button_view) {
	//     LinkHandler.onLinkClicked(this, UriString("https://www.redditinc.com/..."))
	//   }
	//   onClick(R.id.terms_button_decline) {
	//     PrefsUtility.declineRedditUserAgreement()
	//     onDone()
	//   }
	//   onClick(R.id.terms_button_accept) {
	//     PrefsUtility.acceptRedditUserAgreement()
	//     onDone()
	//   }
	void onCreate(Bundle* savedInstanceState) override;

private:
	// Port of: private const val extraLaunchMain = "launch_main"
	// Port of: fun onClick(@IdRes id: Int, action: () -> Unit)
	// Port of: fun onDone()
	// These are handled inline in onCreate in the C++ port
};

} // namespace PinkReader
