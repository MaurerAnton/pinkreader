// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/RedditTermsActivity.kt

#include "reddit_terms_activity.h"

namespace PinkReader {

// Port of: @JvmStatic fun launch(activity: AppCompatActivity, launchMainOnClose: Boolean)
void RedditTermsActivity::launch(QObject* activity, bool launchMainOnClose) {
	// Port of: val intent = Intent(activity, RedditTermsActivity::class.java)
	// intent.putExtra(extraLaunchMain, launchMainOnClose)
	// activity.startActivity(intent)
}

RedditTermsActivity::RedditTermsActivity(QObject* parent)
	: ViewsBaseActivity(parent)
{
}

// Port of: override fun baseActivityIsActionBarBackEnabled() = false
bool RedditTermsActivity::baseActivityIsActionBarBackEnabled() {
	return false;
}

// Port of: override fun onCreate(savedInstanceState: Bundle?)
void RedditTermsActivity::onCreate(Bundle* savedInstanceState) {
	// Port of: PrefsUtility.applySettingsTheme(this)
	ViewsBaseActivity::onCreate(savedInstanceState);

	// Port of: window.navigationBarColor = Color.rgb(0x55, 0x55, 0x55)
	// Port of: setBaseActivityListing(R.layout.reddit_terms_activity)

	// Port of: val launchMainOnClose = intent.getBooleanExtra(extraLaunchMain, false)
	bool launchMainOnClose = false; // Retrieved from intent extras

	// Port of: onClick(R.id.terms_button_view) {
	//   LinkHandler.onLinkClicked(this,
	//     UriString("https://www.redditinc.com/policies/user-agreement-april-18-2023"))
	// }
	// Port of: onClick(R.id.terms_button_decline) {
	//   PrefsUtility.declineRedditUserAgreement()
	//   onDone()
	// }
	// Port of: onClick(R.id.terms_button_accept) {
	//   PrefsUtility.acceptRedditUserAgreement()
	//   onDone()
	// }
	// UI binding handled by framework in C++ port
}

} // namespace PinkReader
