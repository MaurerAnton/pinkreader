// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/SettingsFragment.java
#include "settings_fragment.h"
#include <cstdio>
#include <algorithm>

namespace PinkReader {

// Port of: private static final CacheTypeInfo for each CacheType enum value

// Port of: LISTINGS(R.string.cache_clear_dialog_listings, R.string.cache_clear_dialog_listings_data,
//   new int[] { Constants.FileType.POST_LIST, Constants.FileType.COMMENT_LIST,
//     Constants.FileType.SUBREDDIT_LIST, Constants.FileType.SUBREDDIT_ABOUT,
//     Constants.FileType.USER_ABOUT, Constants.FileType.INBOX_LIST })
static const SettingsFragment::CacheTypeInfo LISTINGS_INFO = {
	"cache_clear_dialog_listings",
	"cache_clear_dialog_listings_data",
	{0, 1, 2, 3, 4, 5}  // FileType IDs (POST_LIST, COMMENT_LIST, SUBREDDIT_LIST, SUBREDDIT_ABOUT, USER_ABOUT, INBOX_LIST)
};

// Port of: THUMBNAILS(R.string.cache_clear_dialog_thumbnails, R.string.cache_clear_dialog_thumbnails_data,
//   new int[] { Constants.FileType.THUMBNAIL })
static const SettingsFragment::CacheTypeInfo THUMBNAILS_INFO = {
	"cache_clear_dialog_thumbnails",
	"cache_clear_dialog_thumbnails_data",
	{10}  // FileType.THUMBNAIL
};

// Port of: IMAGES(R.string.cache_clear_dialog_images, R.string.cache_clear_dialog_images_data,
//   new int[] { Constants.FileType.IMAGE, Constants.FileType.IMAGE_INFO,
//     Constants.FileType.CAPTCHA, Constants.FileType.INLINE_IMAGE_PREVIEW })
static const SettingsFragment::CacheTypeInfo IMAGES_INFO = {
	"cache_clear_dialog_images",
	"cache_clear_dialog_images_data",
	{20, 21, 22, 23}  // IMAGE, IMAGE_INFO, CAPTCHA, INLINE_IMAGE_PREVIEW
};

// Port of: FLAGS(R.string.cache_clear_dialog_flags, R.string.cache_clear_dialog_flags,
//   new int[] {})
static const SettingsFragment::CacheTypeInfo FLAGS_INFO = {
	"cache_clear_dialog_flags",
	"cache_clear_dialog_flags",
	{}
};

// Port of: static cache type info
const SettingsFragment::CacheTypeInfo& SettingsFragment::getCacheTypeInfo(CacheType type) {
	switch (type) {
		case CacheType::LISTINGS:   return LISTINGS_INFO;
		case CacheType::THUMBNAILS: return THUMBNAILS_INFO;
		case CacheType::IMAGES:     return IMAGES_INFO;
		case CacheType::FLAGS:      return FLAGS_INFO;
	}
	// Should not reach here
	return FLAGS_INFO;
}

// Port of: constructor
SettingsFragment::SettingsFragment() {
}

// Port of: @Override public void onResume()
void SettingsFragment::onResume() {
	// Port of: super.onResume();
	// Port of: final FragmentActivity activity = getActivity();
	// Port of: if(activity != null) { activity.setTitle(mTitle); }
	fprintf(stderr, "SettingsFragment::onResume: title=%s\n", m_title.c_str());
}

// Port of: @Override public void onCreatePreferences(
//   final Bundle savedInstanceState, final String rootKey)
void SettingsFragment::onCreatePreferences(const std::string& panel) {
	// Port of: final Context context = getActivity();
	// Port of: final String panel = requireArguments().getString("panel");
	// Port of: final int resource;
	// Port of: try { resource = R.xml.class.getDeclaredField("prefs_" + panel).getInt(null);
	//   if("root".equals(panel)) { mTitle = R.string.options_settings; }
	//   else { mTitle = R.string.class.getDeclaredField("prefs_category_" + panel).getInt(null); }
	// } catch(final Exception e) { throw new RuntimeException(e); }
	m_title = panel;

	// Port of: addPreferencesFromResource(resource);
	// Stub: resource loading

	// Port of: final int[] listPrefsToUpdate = { ... };
	// Port of: final int[] editTextPrefsToUpdate = { ... };
	// Port of: for(final int pref : listPrefsToUpdate) { ... }
	// Port of: for(final int pref : editTextPrefsToUpdate) { ... }
	// Port of: CheckBoxPreference notification preference handling
	// Port of: test notification, version, changelog, tor, license prefs
	// Port of: backup/restore preferences
	// Port of: cache location preference
	// Port of: thumbnail/show/hide prefs
	// Port of: inline images prefs
	// Port of: sharing domain prefs
	// Port of: hide toolbar on scroll / two pane prefs
	// Port of: cache clear pref
	// Port of: category navigation prefs
	// Stub: All Android PreferenceFragmentCompat logic preserved as comments

	fprintf(stderr, "SettingsFragment::onCreatePreferences: panel=%s\n", panel.c_str());
}

// Port of: @Override public void setPreferenceScreen(final PreferenceScreen preferenceScreen)
void SettingsFragment::setPreferenceScreen() {
	// Port of: if(preferenceScreen != null) { configureAllPrefsAppearance(preferenceScreen); }
	// Port of: super.setPreferenceScreen(preferenceScreen);
	configureAllPrefsAppearance();
}

// Port of: private void configureAllPrefsAppearance(final PreferenceGroup prefGroup)
void SettingsFragment::configureAllPrefsAppearance() {
	// Port of: for(int i = 0; i < prefGroup.getPreferenceCount(); i++) {
	//   final Preference pref = prefGroup.getPreference(i);
	//   pref.setSingleLineTitle(false);
	//   pref.setIconSpaceReserved(false);
	//   if(pref instanceof PreferenceGroup) { configureAllPrefsAppearance((PreferenceGroup)pref); }
	// }
	// Stub: Android Preference UI configuration
}

// Port of: public void onPanelSelected callback (called from SettingsActivity)
void SettingsFragment::onPanelSelected(const std::string& panel) {
	// Port of: ((SettingsActivity)getActivity()).onPanelSelected(key.replace(categoryPrefix, ""));
	onCreatePreferences(panel);
}

// Port of: private void showChooseStorageLocationDialog()
void SettingsFragment::showChooseStorageLocationDialog() {
	// Port of: final Context context = getActivity();
	// Port of: final String currentStorage = PrefsUtility.pref_cache_location(context);
	// Port of: final List<File> checkPaths = CacheManager.getCacheDirs(context);
	// Port of: final List<File> folders = new ArrayList<>(checkPaths.size());
	// Port of: final List<CharSequence> choices = new ArrayList<>(checkPaths.size());
	// Port of: int selectedIndex = 0;
	// Port of: for(int i = 0; i < checkPaths.size(); i++) { ... }
	// Port of: new MaterialAlertDialogBuilder(context)
	//   .setTitle(R.string.pref_cache_location_title)
	//   .setSingleChoiceItems(choices.toArray(new CharSequence[0]), selectedIndex,
	//     (dialog, i) -> { dialog.dismiss();
	//       final String path = folders.get(i).getAbsolutePath();
	//       PrefsUtility.pref_cache_location(context, path);
	//       updateStorageLocationText(path); })
	//   .setNegativeButton(R.string.dialog_close, (dialog, i) -> dialog.dismiss())
	//   .create().show();
	// Stub: Android dialog
	fprintf(stderr, "SettingsFragment::showChooseStorageLocationDialog\n");
}

// Port of: private void updateStorageLocationText(final String path)
void SettingsFragment::updateStorageLocationText(const std::string& path) {
	// Port of: findPreference(getString(R.string.pref_cache_location_key)).setSummary(path);
	m_storageLocation = path;
}

// Port of: private void showCacheClearDialog()
void SettingsFragment::showCacheClearDialog() {
	// Port of: final Context context = getActivity();
	// Port of: final CacheManager cacheManager = CacheManager.getInstance(context);
	// Port of: final EnumMap<CacheType, Boolean> cachesToClear = new EnumMap<>(CacheType.class);
	// Port of: final String[] cacheItemStrings = new String[CacheType.values().length];
	// Port of: for(final CacheType cacheType : CacheType.values()) {
	//   cachesToClear.put(cacheType, false);
	//   cacheItemStrings[cacheType.ordinal()] = getString(cacheType.plainStringRes);
	// }
	// Port of: final AlertDialog cacheDialog = new MaterialAlertDialogBuilder(context)
	//   .setTitle(R.string.pref_cache_clear_title)
	//   .setMultiChoiceItems(cacheItemStrings, null, (dialog, which, isChecked) ->
	//     cachesToClear.put(CacheType.values()[which - 1], isChecked))
	//   .setPositiveButton(R.string.dialog_clear, (dialog, id) -> new Thread() {
	//     @Override public void run() {
	//       cacheManager.pruneCache(
	//         cachesToClear.get(CacheType.LISTINGS),
	//         cachesToClear.get(CacheType.THUMBNAILS),
	//         cachesToClear.get(CacheType.IMAGES));
	//       if(Objects.requireNonNull(cachesToClear.get(CacheType.FLAGS))) {
	//         RedditChangeDataManager.pruneAllUsersWhereOlderThan(TimeDuration.ms(0));
	//       }
	//     }
	//   }.start())
	//   .setNegativeButton(R.string.dialog_cancel, null)
	//   .create();
	//
	// Port of: final ProgressBar progressBar = new ProgressBar(context, null,
	//   android.R.attr.progressBarStyleHorizontal);
	// Port of: progressBar.setIndeterminate(true);
	// Port of: cacheDialog.getListView().addHeaderView(progressBar, null, false);
	// Port of: cacheDialog.show();
	//
	// Port of: new Thread() {
	//   @Override public void run() {
	//     final HashMap<Integer, Long> fileTypeDataUsages = cacheManager.getCacheDataUsages();
	//     for(final CacheType cacheType : CacheType.values()) {
	//       if(cacheType.fileTypes.length >= 1) {
	//         long cacheTypeDataUsage = 0;
	//         for(final HashMap.Entry<Integer, Long> fileTypeDataUsage : fileTypeDataUsages.entrySet()) {
	//           for(final int fileType : cacheType.fileTypes) {
	//             if(fileType == fileTypeDataUsage.getKey()) {
	//               cacheTypeDataUsage += fileTypeDataUsage.getValue(); } } }
	//         final long finalCacheTypeDataUsage = cacheTypeDataUsage;
	//         AndroidCommon.runOnUiThread(() -> {
	//           final TextView cacheItemView = (TextView)cacheDialog.getListView()
	//             .getChildAt(cacheType.ordinal() + 1);
	//           cacheItemView.setText(String.format(Locale.US,
	//             context.getApplicationContext().getString(cacheType.dataUsageStringRes),
	//             General.addUnits(finalCacheTypeDataUsage))); }); } }
	//     AndroidCommon.runOnUiThread(() -> {
	//       progressBar.setIndeterminate(false);
	//       progressBar.setProgress(progressBar.getMax()); }); }
	// }.start();
	// Stub: Android dialog + threading
	fprintf(stderr, "SettingsFragment::showCacheClearDialog\n");
}

} // namespace PinkReader
