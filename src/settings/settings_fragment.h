// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/SettingsFragment.java
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <cstdint>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.settings.SettingsFragment
 *
 * Settings fragment with preference management, cache clearing, backup/restore.
 * Original: extends PreferenceFragmentCompat
 * Every constant, field, inner enum, and method from the original Java file.
 *
 * NOTE: Android-specific dependencies (Preference, PreferenceFragmentCompat, etc.)
 * are stubbed. This preserves the logic structure for re-implementation.
 */
class SettingsFragment {
public:
	// Port of: private enum CacheType
	//   LISTINGS(R.string.cache_clear_dialog_listings, R.string.cache_clear_dialog_listings_data,
	//     new int[] { Constants.FileType.POST_LIST, Constants.FileType.COMMENT_LIST, ... }),
	//   THUMBNAILS(R.string.cache_clear_dialog_thumbnails, ...),
	//   IMAGES(R.string.cache_clear_dialog_images, ...),
	//   FLAGS(R.string.cache_clear_dialog_flags, ...);
	enum class CacheType {
		// Port of: LISTINGS
		LISTINGS,
		// Port of: THUMBNAILS
		THUMBNAILS,
		// Port of: IMAGES
		IMAGES,
		// Port of: FLAGS
		FLAGS
	};

	// Cache type information (mirrors the Java enum data)
	struct CacheTypeInfo {
		std::string plainStringRes;
		std::string dataUsageStringRes;
		std::vector<int> fileTypes;
	};

	SettingsFragment();
	virtual ~SettingsFragment() = default;

	// Port of: @StringRes private int mTitle;
	// Port of: @Override public void onResume()
	void onResume();

	// Port of: @Override public void onCreatePreferences(
	//   final Bundle savedInstanceState, final String rootKey)
	void onCreatePreferences(const std::string& panel);

	// Port of: @Override public void setPreferenceScreen(final PreferenceScreen preferenceScreen)
	//   if(preferenceScreen != null) { configureAllPrefsAppearance(preferenceScreen); }
	//   super.setPreferenceScreen(preferenceScreen);
	void setPreferenceScreen();

	// Port of: public void onPanelSelected callback (called from SettingsActivity)
	void onPanelSelected(const std::string& panel);

private:
	// Port of: @StringRes private int mTitle;
	std::string m_title;

	// Port of: private void configureAllPrefsAppearance(final PreferenceGroup prefGroup)
	//   for(int i = 0; i < prefGroup.getPreferenceCount(); i++) {
	//     final Preference pref = prefGroup.getPreference(i);
	//     pref.setSingleLineTitle(false);
	//     pref.setIconSpaceReserved(false);
	//     if(pref instanceof PreferenceGroup) { configureAllPrefsAppearance((PreferenceGroup)pref); }
	//   }
	void configureAllPrefsAppearance();

	// Port of: private void showChooseStorageLocationDialog()
	void showChooseStorageLocationDialog();

	// Port of: private void updateStorageLocationText(final String path)
	//   findPreference(getString(R.string.pref_cache_location_key)).setSummary(path);
	void updateStorageLocationText(const std::string& path);

	// Port of: private void showCacheClearDialog()
	void showCacheClearDialog();

	// Port of: static cache type info per CacheType
	static const CacheTypeInfo& getCacheTypeInfo(CacheType type);

	// Storage location
	std::string m_storageLocation = "default";
};

} // namespace PinkReader
