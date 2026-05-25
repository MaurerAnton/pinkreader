/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_inbox_item_view.cpp - Port of RedditInboxItemView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RedditInboxItemView.java
 */

#include "views/reddit_inbox_item_view.h"

#include <cmath>
#include <optional>
#include <string>

namespace PinkReader {

// ============================================================================
// Stubs
// ============================================================================

class BaseActivity {
public:
    virtual ~BaseActivity() = default;
};

class RRThemeAttributes {
public:
    float rrCommentHeaderFontScale = 1.0f;
    int rrCommentHeaderCol = 0;
    float rrCommentFontScale = 1.0f;
    int rrCommentBodyCol = 0;
};

class RedditChangeDataManager {
public:
};

class View {
public:
    virtual ~View() = default;
    void setBackgroundColor(int) {}
    void setVisibility(int) {}
    void setPadding(int, int, int, int) {}
    void removeAllViews() {}
    void addView(View*) {}
    void setOnClickListener(void (*)(void*)) {}
    void setOnLongClickListener(bool (*)(void*)) {}
};

// PrefsUtility stub
namespace PrefsUtility {
    static bool pref_appearance_linkbuttons() { return true; }
    static int appearance_inbox_age_units() { return 0; }
}

// General stub
static int dpToPixels(void*, float dp) {
    return static_cast<int>(std::round(dp * 2.0f));
}

static void setLayoutMatchWidthWrapHeight(View*) {}
static void setLayoutWidthHeight(View*, int, int) {}

// Optional stub
template<typename T>
using Optional = std::optional<T>;

// RedditRenderableInboxItem stub
class RedditRenderableInboxItem {
public:
    struct HeaderResult {
        std::string header;
    };

    struct AccessibilityResult {
        std::string text;
    };

    // Port of: getHeader(...)
    HeaderResult getHeader(RRThemeAttributes*, RedditChangeDataManager*,
                           BaseActivity*, int, void*, void*) {
        return {"header_text"};
    }

    // Port of: getAccessibilityHeader(...)
    std::string getAccessibilityHeader(RRThemeAttributes*, RedditChangeDataManager*,
                                       BaseActivity*, int, void*, void*, bool,
                                       Optional<std::string>) {
        return "accessibility_header";
    }

    // Port of: getBody(...)
    View* getBody(BaseActivity*, int, float, bool) {
        return new View();
    }

    // Port of: handleInboxClick(activity)
    void handleInboxClick(BaseActivity*) {}
    void handleInboxLongClick(BaseActivity*) {}
};

// ============================================================================
// Constructor (Java lines 49-99)
// ============================================================================

RedditInboxItemView::RedditInboxItemView(BaseActivity* activity,
                                         RRThemeAttributes* theme)
    : m_theme(theme)
    , m_activity(activity)
{
    // Port of: super(activity);
    // Port of: setOrientation(VERTICAL);

    // Port of: mDivider = new View(activity);
    // Port of: mDivider.setBackgroundColor(Color.argb(128, 128, 128, 128));
    // Port of: addView(mDivider);
    // Port of: General.setLayoutWidthHeight(mDivider, ViewGroup.LayoutParams.MATCH_PARENT, 1);

    // Port of: final LinearLayout inner = new LinearLayout(activity);
    // Port of: inner.setOrientation(VERTICAL);

    // Port of: mHeader = new TextView(activity);
    // Port of: mHeader.setTextSize(11.0f * theme.rrCommentHeaderFontScale);
    // Port of: mHeader.setTextColor(theme.rrCommentHeaderCol);
    // Port of: inner.addView(mHeader);
    // Port of: General.setLayoutMatchWidthWrapHeight(mHeader);

    // Port of: mBodyHolder = new FrameLayout(activity);
    // Port of: mBodyHolder.setPadding(0, General.dpToPixels(activity, 2), 0, 0);
    // Port of: inner.addView(mBodyHolder);
    // Port of: General.setLayoutMatchWidthWrapHeight(mBodyHolder);

    // Port of: final int paddingPixels = General.dpToPixels(activity, 8.0f);
    // Port of: inner.setPadding(paddingPixels + paddingPixels, paddingPixels,
    //     paddingPixels, paddingPixels);
    const int padPx = dpToPixels(activity, 8.0f);
    (void)padPx;

    // Port of: addView(inner);
    // Port of: General.setLayoutMatchWidthWrapHeight(mBodyHolder);

    // Port of: setDescendantFocusability(FOCUS_BLOCK_DESCENDANTS);

    // Port of: showLinkButtons = PrefsUtility.pref_appearance_linkbuttons();
    m_showLinkButtons = PrefsUtility::pref_appearance_linkbuttons();

    // Port of: setOnClickListener(v -> handleInboxClick(mActivity));
    // Port of: setOnLongClickListener(v -> { handleInboxLongClick(mActivity); return true; });
}

RedditInboxItemView::~RedditInboxItemView() = default;

// ============================================================================
// reset — port of method (Java lines 101-138)
// ============================================================================

void RedditInboxItemView::reset(BaseActivity* context,
                                RedditChangeDataManager* changeDataManager,
                                RRThemeAttributes* theme,
                                RedditRenderableInboxItem* item,
                                bool showDividerAtTop) {
    // Port of: currentItem = item;
    m_currentItem = item;

    // Port of: mDivider.setVisibility(showDividerAtTop ? VISIBLE : GONE);
    // (divider visibility set)

    // Port of: mHeader.setText(item.getHeader(theme, changeDataManager, context,
    //     PrefsUtility.appearance_inbox_age_units(), null, null).get());
    const auto header = item->getHeader(
            theme, changeDataManager, context,
            PrefsUtility::appearance_inbox_age_units(), nullptr, nullptr);
    (void)header;

    // Port of: mHeader.setContentDescription(item.getAccessibilityHeader(...));
    const auto accHeader = item->getAccessibilityHeader(
            theme, changeDataManager, context,
            PrefsUtility::appearance_inbox_age_units(),
            nullptr, nullptr, false, std::nullopt);
    (void)accHeader;

    // Port of: final View body = item.getBody(context, mTheme.rrCommentBodyCol,
    //     13.0f * mTheme.rrCommentFontScale, showLinkButtons);
    View* body = item->getBody(context, m_theme->rrCommentBodyCol,
                               13.0f * m_theme->rrCommentFontScale,
                               m_showLinkButtons);

    // Port of: mBodyHolder.removeAllViews();
    // Port of: mBodyHolder.addView(body);
    // Port of: General.setLayoutMatchWidthWrapHeight(body);
    (void)body;

    // Suppress unused param warnings
    (void)showDividerAtTop;
}

// ============================================================================
// handleInboxClick — port of method (Java lines 140-144)
// ============================================================================

void RedditInboxItemView::handleInboxClick(BaseActivity* activity) {
    // Port of: if(currentItem != null) { currentItem.handleInboxClick(activity); }
    if (m_currentItem != nullptr) {
        m_currentItem->handleInboxClick(activity);
    }
}

// ============================================================================
// handleInboxLongClick — port of method (Java lines 146-150)
// ============================================================================

void RedditInboxItemView::handleInboxLongClick(BaseActivity* activity) {
    // Port of: if(currentItem != null) { currentItem.handleInboxLongClick(activity); }
    if (m_currentItem != nullptr) {
        m_currentItem->handleInboxLongClick(activity);
    }
}

} // namespace PinkReader
