// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/SettingsActivity.java
#include "settings_activity.h"
#include <cstdio>

namespace PinkReader {

// Port of: private void launchFragment(@NonNull final String panel)
void SettingsActivity::launchFragment(const std::string& panel) {
	// Port of: final Bundle bundle = new Bundle();
	// Port of: bundle.putString("panel", panel);
	// Port of: getSupportFragmentManager().beginTransaction()
	//   .setReorderingAllowed(false)
	//   .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN)
	//   .replace(R.id.single_fragment_container, SettingsFragment.class, bundle)
	//   .addToBackStack("Settings: " + panel)
	//   .commit();
	// Stub: Android-specific fragment transaction
	fprintf(stderr, "SettingsActivity::launchFragment: panel=%s\n", panel.c_str());
}

// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
void SettingsActivity::onCreate() {
	// Port of: PrefsUtility.applySettingsTheme(this);
	// Port of: super.onCreate(savedInstanceState);
	// Port of: getWindow().setNavigationBarColor(Color.rgb(0x55, 0x55, 0x55));
	// Port of: setBaseActivityListing(R.layout.single_fragment_layout);
	// Port of: final Bundle bundle = new Bundle();
	// Port of: bundle.putString("panel", "root");
	// Port of: getSupportFragmentManager().beginTransaction()
	//   .setReorderingAllowed(false)
	//   .replace(R.id.single_fragment_container, SettingsFragment.class, bundle)
	//   .commit();
	// Stub: Android-specific activity lifecycle
	fprintf(stderr, "SettingsActivity::onCreate\n");
}

// Port of: public void onPanelSelected(@NonNull final String panel)
void SettingsActivity::onPanelSelected(const std::string& panel) {
	// Port of: launchFragment(panel);
	launchFragment(panel);
}

} // namespace PinkReader
