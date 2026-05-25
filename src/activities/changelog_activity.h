// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/ChangelogActivity.java

#pragma once

#include "views_base_activity.h"
#include <QObject>

namespace PinkReader {

// Forward declarations
class Bundle;
class MenuItem;

/**
 * @brief Port of org.quantumbadger.redreader.activities.ChangelogActivity
 *
 * Displays the changelog using ChangelogManager. Every field, method, and
 * logic branch from the original Java file (63 lines).
 */
class ChangelogActivity : public ViewsBaseActivity {
	Q_OBJECT

public:
	explicit ChangelogActivity(QObject* parent = nullptr);
	virtual ~ChangelogActivity() = default;

protected:
	// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
	//   PrefsUtility.applySettingsTheme(this);
	//   super.onCreate(savedInstanceState);
	//   getWindow().setNavigationBarColor(Color.rgb(0x55, 0x55, 0x55));
	//   setTitle(R.string.title_changelog);
	//   final LinearLayout items = new LinearLayout(this);
	//   items.setOrientation(LinearLayout.VERTICAL);
	//   ChangelogManager.generateViews(this, items, true);
	//   final ScrollView sv = new ScrollView(this);
	//   sv.addView(items);
	//   setBaseActivityListing(sv);
	void onCreate(Bundle* savedInstanceState) override;

	// Port of: @Override public boolean onOptionsItemSelected(final MenuItem item)
	//   switch(item.getItemId()) {
	//     case android.R.id.home: finish(); return true;
	//     default: return false;
	//   }
	bool onOptionsItemSelected(MenuItem* item);
};

} // namespace PinkReader
