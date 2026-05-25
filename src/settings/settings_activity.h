// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/SettingsActivity.java
#pragma once

#include <string>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.settings.SettingsActivity
 *
 * Android Activity for settings UI. Extends ViewsBaseActivity.
 * Every method from the original Java file.
 *
 * NOTE: Android-specific dependencies (Activity, Fragment, Bundle, etc.)
 * are stubbed. This serves as a logic reference for re-implementation
 * on a target platform.
 */
class SettingsActivity {
public:
	// Port of: public class SettingsActivity extends ViewsBaseActivity
	SettingsActivity() = default;
	virtual ~SettingsActivity() = default;

	// Port of: private void launchFragment(@NonNull final String panel)
	//   final Bundle bundle = new Bundle(); bundle.putString("panel", panel);
	//   getSupportFragmentManager().beginTransaction()
	//     .setReorderingAllowed(false)
	//     .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN)
	//     .replace(R.id.single_fragment_container, SettingsFragment.class, bundle)
	//     .addToBackStack("Settings: " + panel)
	//     .commit();
	void launchFragment(const std::string& panel);

	// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
	//   PrefsUtility.applySettingsTheme(this);
	//   super.onCreate(savedInstanceState);
	//   getWindow().setNavigationBarColor(Color.rgb(0x55, 0x55, 0x55));
	//   setBaseActivityListing(R.layout.single_fragment_layout);
	//   final Bundle bundle = new Bundle(); bundle.putString("panel", "root");
	//   getSupportFragmentManager().beginTransaction()
	//     .setReorderingAllowed(false)
	//     .replace(R.id.single_fragment_container, SettingsFragment.class, bundle)
	//     .commit();
	void onCreate();

	// Port of: public void onPanelSelected(@NonNull final String panel)
	//   launchFragment(panel);
	void onPanelSelected(const std::string& panel);
};

} // namespace PinkReader
