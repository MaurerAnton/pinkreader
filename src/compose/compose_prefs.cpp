// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/prefs/ComposePrefs.kt
#include "compose_prefs.h"
#include <algorithm>
#include <cstdlib>

namespace PinkReader {

// --- ComposePrefsSingleton ---

ComposePrefs* ComposePrefsSingleton::singleton_ = nullptr;

void ComposePrefsSingleton::init(Context& context) {
	// Ported from: init(context: Context) { General.checkThisIsUIThread(); SINGLETON = ComposePrefsImpl(context) }
	// Original checks UI thread, then creates the impl singleton
	singleton_ = new ComposePrefsImpl(context);
}

ComposePrefs& ComposePrefsSingleton::instance() {
	// Ported from: val instance: ComposePrefs get() = SINGLETON
	return *singleton_;
}

ComposePrefs& LocalComposePrefs() {
	// Ported from: val LocalComposePrefs = staticCompositionLocalOf { ComposePrefsSingleton.instance }
	return ComposePrefsSingleton::instance();
}

// --- ComposePrefsImpl ---

ComposePrefsImpl::ComposePrefsImpl(Context& context)
	: context_(context)
{
	// Ported from init block:
	// General.checkThisIsUIThread()
	// sharedPrefs.registerOnSharedPreferenceChangeListener { _, key -> changeObservers[key]?.invoke() }

	// Initialize all preference members (using string keys matching original)
	// appearance_fontscale_global_ = FloatPref("pref_appearance_fontscale_global_key", 1f)
	appearance_fontscale_global_ = std::make_unique<FloatPref>(*this, "pref_appearance_fontscale_global_key", 1.0f);
	appearance_fontscale_bodytext_ = std::make_unique<FloatPref>(*this, "pref_appearance_fontscale_bodytext_key", -1.0f);
	appearance_fontscale_posts_ = std::make_unique<FloatPref>(*this, "pref_appearance_fontscale_posts_key", -1.0f);
	appearance_fontscale_post_subtitles_ = std::make_unique<FloatPref>(*this, "pref_appearance_fontscale_post_subtitles_key", -1.0f);

	appearanceTheme_ = std::make_unique<EnumPref>(*this, "pref_appearance_theme_key", "RED");
	albumViewMode_ = std::make_unique<EnumPref>(*this, "album_view_mode", "Cards");

	albumCardShowButtons_ = std::make_unique<BoolPref>(*this, "album_card_show_buttons", true);
	albumListShowThumbnails_ = std::make_unique<BoolPref>(*this, "album_list_show_thumbnails", true);
	albumListThumbnailSize_ = std::make_unique<IntPref>(*this, "album_list_thumbnail_size", 64);
	albumListShowButtons_ = std::make_unique<BoolPref>(*this, "album_list_show_buttons", true);
	albumGridCropToSquare_ = std::make_unique<BoolPref>(*this, "album_grid_crop_to_square", true);
	albumGridColumns_ = std::make_unique<IntPref>(*this, "album_grid_columns", 3);
	albumGridRoundedCorners_ = std::make_unique<BoolPref>(*this, "album_grid_rounded_corners", true);
	albumGridHorizontalPadding_ = std::make_unique<BoolPref>(*this, "album_grid_horizontal_padding", true);
	albumCompactTitle_ = std::make_unique<BoolPref>(*this, "album_compact_title", false);
}

// --- FloatPref (inner class of ComposePrefsImpl) ---

class ComposePrefsImpl::FloatPref : public Preference<float> {
public:
	FloatPref(ComposePrefsImpl& parent, const std::string& key, float def)
		: parent_(parent), key_(key), default_(def), value_(parent_.getFloat(key_, def))
	{
		// Ported from: init { changeObservers[key] = { mutableState.floatValue = loadPref() } }
		parent_.changeObservers_[key_] = [this]() {
			value_ = parent_.getFloat(key_, default_);
		};
	}

	float getValue() const override { return value_; }
	void setValue(float value) override {
		// Ported from: set(value) { sharedPrefs.edit().putFloat(key, value).apply(); mutableState.floatValue = value }
		parent_.putFloat(key_, value);
		value_ = value;
	}

private:
	ComposePrefsImpl& parent_;
	std::string key_;
	float default_;
	float value_;
};

// --- IntPref ---

class ComposePrefsImpl::IntPref : public Preference<int> {
public:
	IntPref(ComposePrefsImpl& parent, const std::string& key, int def)
		: parent_(parent), key_(key), default_(def), value_(parent_.getInt(key_, def))
	{
		parent_.changeObservers_[key_] = [this]() {
			value_ = parent_.getInt(key_, default_);
		};
	}

	int getValue() const override { return value_; }
	void setValue(int value) override {
		parent_.putInt(key_, value);
		value_ = value;
	}

private:
	ComposePrefsImpl& parent_;
	std::string key_;
	int default_;
	int value_;
};

// --- BoolPref ---

class ComposePrefsImpl::BoolPref : public Preference<bool> {
public:
	BoolPref(ComposePrefsImpl& parent, const std::string& key, bool def)
		: parent_(parent), key_(key), default_(def), value_(parent_.getBool(key_, def))
	{
		parent_.changeObservers_[key_] = [this]() {
			value_ = parent_.getBool(key_, default_);
		};
	}

	bool getValue() const override { return value_; }
	void setValue(bool value) override {
		parent_.putBool(key_, value);
		value_ = value;
	}

private:
	ComposePrefsImpl& parent_;
	std::string key_;
	bool default_;
	bool value_;
};

// --- EnumPref ---

class ComposePrefsImpl::EnumPref : public Preference<std::string> {
public:
	EnumPref(ComposePrefsImpl& parent, const std::string& key, const std::string& def)
		: parent_(parent), key_(key), default_(def), value_(parent_.getString(key_, def))
	{
		parent_.changeObservers_[key_] = [this]() {
			value_ = parent_.getString(key_, default_);
		};
	}

	std::string getValue() const override { return value_; }
	void setValue(std::string value) override {
		parent_.putString(key_, value);
		value_ = value;
	}

private:
	ComposePrefsImpl& parent_;
	std::string key_;
	std::string default_;
	std::string value_;
};

// --- ComposePrefsImpl accessor methods ---

float ComposePrefsImpl::fontScale(const Preference<float>& pref) const {
	// Ported from: private fun fontScale(pref: FloatPref) = pref.value.takeUnless { it == -1f }
	//     ?: appearance_fontscale_global.value
	float val = pref.getValue();
	if (val == -1.0f) {
		return appearance_fontscale_global_->getValue();
	}
	return val;
}

float ComposePrefsImpl::appearanceFontScaleGlobal() const { return appearance_fontscale_global_->getValue(); }
float ComposePrefsImpl::appearanceFontScaleBodyText() const { return fontScale(*appearance_fontscale_bodytext_); }
float ComposePrefsImpl::appearanceFontScalePosts() const { return fontScale(*appearance_fontscale_posts_); }
float ComposePrefsImpl::appearanceFontScalePostSubtitles() const { return fontScale(*appearance_fontscale_post_subtitles_); }

Preference<std::string>& ComposePrefsImpl::appearanceTheme() { return *appearanceTheme_; }
Preference<std::string>& ComposePrefsImpl::albumViewMode() { return *albumViewMode_; }
Preference<bool>& ComposePrefsImpl::albumCardShowButtons() { return *albumCardShowButtons_; }
Preference<bool>& ComposePrefsImpl::albumListShowThumbnails() { return *albumListShowThumbnails_; }
Preference<int>& ComposePrefsImpl::albumListThumbnailSize() { return *albumListThumbnailSize_; }
Preference<bool>& ComposePrefsImpl::albumListShowButtons() { return *albumListShowButtons_; }
Preference<bool>& ComposePrefsImpl::albumGridCropToSquare() { return *albumGridCropToSquare_; }
Preference<int>& ComposePrefsImpl::albumGridColumns() { return *albumGridColumns_; }
Preference<bool>& ComposePrefsImpl::albumGridRoundedCorners() { return *albumGridRoundedCorners_; }
Preference<bool>& ComposePrefsImpl::albumGridHorizontalPadding() { return *albumGridHorizontalPadding_; }
Preference<bool>& ComposePrefsImpl::albumCompactTitle() { return *albumCompactTitle_; }

// --- SharedPrefs helper methods ---

std::string ComposePrefsImpl::getString(const std::string& key, const std::string& def) const {
	auto it = sharedPrefs_.find(key);
	return (it != sharedPrefs_.end()) ? it->second : def;
}

int ComposePrefsImpl::getInt(const std::string& key, int def) const {
	auto it = sharedPrefs_.find(key);
	if (it != sharedPrefs_.end()) {
		try { return std::stoi(it->second); } catch (...) {}
	}
	return def;
}

bool ComposePrefsImpl::getBool(const std::string& key, bool def) const {
	auto it = sharedPrefs_.find(key);
	if (it != sharedPrefs_.end()) {
		return it->second == "true";
	}
	return def;
}

float ComposePrefsImpl::getFloat(const std::string& key, float def) const {
	auto it = sharedPrefs_.find(key);
	if (it != sharedPrefs_.end()) {
		try { return std::stof(it->second); } catch (...) {}
	}
	return def;
}

void ComposePrefsImpl::putString(const std::string& key, const std::string& value) {
	sharedPrefs_[key] = value;
}

void ComposePrefsImpl::putInt(const std::string& key, int value) {
	sharedPrefs_[key] = std::to_string(value);
}

void ComposePrefsImpl::putBool(const std::string& key, bool value) {
	sharedPrefs_[key] = value ? "true" : "false";
}

void ComposePrefsImpl::putFloat(const std::string& key, float value) {
	sharedPrefs_[key] = std::to_string(value);
}

} // namespace PinkReader
