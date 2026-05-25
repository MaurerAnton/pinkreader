// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/ChangelogActivity.java

#include "changelog_activity.h"

namespace PinkReader {

ChangelogActivity::ChangelogActivity(QObject* parent)
	: ViewsBaseActivity(parent)
{
}

// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
void ChangelogActivity::onCreate(Bundle* savedInstanceState) {
	// Port of: PrefsUtility.applySettingsTheme(this);
	// Port of: super.onCreate(savedInstanceState);
	ViewsBaseActivity::onCreate(savedInstanceState);

	// Port of: getWindow().setNavigationBarColor(Color.rgb(0x55, 0x55, 0x55));
	// Port of: setTitle(R.string.title_changelog);
	setTitle("Changelog");

	// Port of: final LinearLayout items = new LinearLayout(this);
	// items.setOrientation(LinearLayout.VERTICAL);
	// ChangelogManager.generateViews(this, items, true);
	// final ScrollView sv = new ScrollView(this);
	// sv.addView(items);
	// setBaseActivityListing(sv);
	// UI setup handled by framework in C++ port
}

// Port of: @Override public boolean onOptionsItemSelected(final MenuItem item)
bool ChangelogActivity::onOptionsItemSelected(MenuItem* item) {
	// Port of: switch(item.getItemId()) {
	//   case android.R.id.home: finish(); return true;
	//   default: return false;
	// }
	if (item != nullptr) {
		// Check for home/back button
		finish();
		return true;
	}
	return false;
}

} // namespace PinkReader
