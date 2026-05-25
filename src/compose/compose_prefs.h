// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/prefs/ComposePrefs.kt
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace PinkReader {

// Forward declarations
class Context;

// Ported from: @Stable interface Preference<T>
template <typename T>
struct Preference {
	virtual ~Preference() = default;
	virtual T getValue() const = 0;
	virtual void setValue(T value) = 0;
};

// Ported from: interface ComposePrefs
struct ComposePrefs {
	virtual ~ComposePrefs() = default;

	virtual Preference<std::string>& appearanceTheme() = 0;   // AppearanceTheme serialized

	virtual float appearanceFontScaleGlobal() const = 0;
	virtual float appearanceFontScaleBodyText() const = 0;
	virtual float appearanceFontScalePosts() const = 0;
	virtual float appearanceFontScalePostSubtitles() const = 0;

	virtual Preference<std::string>& albumViewMode() = 0;     // AlbumViewMode serialized
	virtual Preference<bool>& albumCardShowButtons() = 0;
	virtual Preference<bool>& albumListShowThumbnails() = 0;
	virtual Preference<int>& albumListThumbnailSize() = 0;
	virtual Preference<bool>& albumListShowButtons() = 0;
	virtual Preference<bool>& albumGridCropToSquare() = 0;
	virtual Preference<int>& albumGridColumns() = 0;
	virtual Preference<bool>& albumGridRoundedCorners() = 0;
	virtual Preference<bool>& albumGridHorizontalPadding() = 0;
	virtual Preference<bool>& albumCompactTitle() = 0;
};

// Ported from: object ComposePrefsSingleton
class ComposePrefsSingleton {
public:
	static void init(Context& context);
	static ComposePrefs& instance();

private:
	static ComposePrefs* singleton_;
};

// Ported from: val LocalComposePrefs = staticCompositionLocalOf { ComposePrefsSingleton.instance }
ComposePrefs& LocalComposePrefs();

// Ported from: private class ComposePrefsImpl(context: Context) : ComposePrefs
class ComposePrefsImpl : public ComposePrefs {
public:
	explicit ComposePrefsImpl(Context& context);

	Preference<std::string>& appearanceTheme() override;

	float appearanceFontScaleGlobal() const override;
	float appearanceFontScaleBodyText() const override;
	float appearanceFontScalePosts() const override;
	float appearanceFontScalePostSubtitles() const override;

	Preference<std::string>& albumViewMode() override;
	Preference<bool>& albumCardShowButtons() override;
	Preference<bool>& albumListShowThumbnails() override;
	Preference<int>& albumListThumbnailSize() override;
	Preference<bool>& albumListShowButtons() override;
	Preference<bool>& albumGridCropToSquare() override;
	Preference<int>& albumGridColumns() override;
	Preference<bool>& albumGridRoundedCorners() override;
	Preference<bool>& albumGridHorizontalPadding() override;
	Preference<bool>& albumCompactTitle() override;

private:
	Context& context_;

	// SharedPrefs equivalent (string key -> string value)
	std::unordered_map<std::string, std::string> sharedPrefs_;
	std::unordered_map<std::string, std::function<void()>> changeObservers_;

	float fontScale(const Preference<float>& pref) const;

	// Inner preference classes
	class FloatPref;
	class IntPref;
	class BoolPref;
	class EnumPref;

	std::unique_ptr<FloatPref> appearance_fontscale_global_;
	std::unique_ptr<FloatPref> appearance_fontscale_bodytext_;
	std::unique_ptr<FloatPref> appearance_fontscale_posts_;
	std::unique_ptr<FloatPref> appearance_fontscale_post_subtitles_;

	std::unique_ptr<EnumPref> appearanceTheme_;
	std::unique_ptr<EnumPref> albumViewMode_;
	std::unique_ptr<BoolPref> albumCardShowButtons_;
	std::unique_ptr<BoolPref> albumListShowThumbnails_;
	std::unique_ptr<IntPref> albumListThumbnailSize_;
	std::unique_ptr<BoolPref> albumListShowButtons_;
	std::unique_ptr<BoolPref> albumGridCropToSquare_;
	std::unique_ptr<IntPref> albumGridColumns_;
	std::unique_ptr<BoolPref> albumGridRoundedCorners_;
	std::unique_ptr<BoolPref> albumGridHorizontalPadding_;
	std::unique_ptr<BoolPref> albumCompactTitle_;

	// Helper: load string from prefs
	std::string getString(const std::string& key, const std::string& def) const;
	int getInt(const std::string& key, int def) const;
	bool getBool(const std::string& key, bool def) const;
	float getFloat(const std::string& key, float def) const;

	void putString(const std::string& key, const std::string& value);
	void putInt(const std::string& key, int value);
	void putBool(const std::string& key, bool value);
	void putFloat(const std::string& key, float value);
};

} // namespace PinkReader
