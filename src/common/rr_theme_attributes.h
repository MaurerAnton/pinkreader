// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/RRThemeAttributes.java
#pragma once

#include <cstdint>
#include <set>
#include <string>

namespace PinkReader {

/**
 * Appearance comment header item enum.
 * Equivalent to: PrefsUtility.AppearanceCommentHeaderItem (enum used by RRThemeAttributes)
 */
enum class AppearanceCommentHeaderItem {
	AUTHOR,
	FLAIR,
	SCORE,
	AGE,
	GOLD
};

/**
 * Theme attributes extracted from the current application theme.
 * Equivalent to: org.quantumbadger.redreader.common.RRThemeAttributes
 */
class RRThemeAttributes {
public:
	// Color attributes (ARGB int values, matching Android color ints)
	int32_t rrCommentHeaderBoldCol     = 0xFF000000;
	int32_t rrCommentHeaderAuthorCol   = 0xFF000000;
	int32_t rrPostSubtitleUpvoteCol    = 0xFF000000;
	int32_t rrPostSubtitleDownvoteCol  = 0xFF000000;
	int32_t rrFlairBackCol             = 0x00000000;
	int32_t rrFlairTextCol             = 0xFF000000;
	int32_t rrGoldBackCol              = 0x00000000;
	int32_t rrGoldTextCol              = 0xFF000000;
	int32_t rrCommentHeaderCol         = 0xFF000000;
	int32_t rrCommentBodyCol           = 0xFF000000;
	int32_t rrMainTextCol              = 0xFF000000;
	int32_t colorAccent                = 0xFF000000;
	int32_t rrCrosspostBackCol         = 0xFF000000;
	int32_t rrCrosspostTextCol         = 0xFF000000;

	// Font scale factors
	float rrCommentFontScale       = 1.0f;
	float rrCommentHeaderFontScale = 1.0f;

private:
	std::set<AppearanceCommentHeaderItem> mCommentHeaderItems;

public:
	/**
	 * Constructs theme attributes from a context-like object.
	 * In the original, this reads Android TypedArray attributes.
	 * In the portable C++ port, values are set directly.
	 */
	RRThemeAttributes() = default;

	/**
	 * Sets which comment header items should be shown.
	 */
	void setCommentHeaderItems(const std::set<AppearanceCommentHeaderItem>& items) {
		mCommentHeaderItems = items;
	}

	/**
	 * Returns whether the given comment header item type should be shown.
	 * Equivalent to: RRThemeAttributes.shouldShow(AppearanceCommentHeaderItem)
	 */
	bool shouldShow(AppearanceCommentHeaderItem type) const {
		return mCommentHeaderItems.count(type) > 0;
	}
};

} // namespace PinkReader
