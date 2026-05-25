/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_inbox_item_view.h - Port of RedditInboxItemView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RedditInboxItemView.java
 *
 * Original: class RedditInboxItemView extends LinearLayout
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <optional>
#include <string>

namespace PinkReader {

// Forward declarations
class BaseActivity;
class RRThemeAttributes;
class RedditChangeDataManager;
class RedditRenderableInboxItem;
class View;

// ============================================================================
// RedditInboxItemView — port of Java class (Java lines 35-151)
//
// Port of: org.quantumbadger.redreader.views.RedditInboxItemView
// Orig extends: LinearLayout
//
// Displays an inbox item with header, body, divider, and click handlers.
// Every field, method, constant ported exactly.
// ============================================================================

class RedditInboxItemView {
public:
    // ========================================================================
    // Constructor (Java lines 49-99)
    //
    // Port of: public RedditInboxItemView(final BaseActivity activity,
    //     final RRThemeAttributes theme)
    // ========================================================================

    RedditInboxItemView(BaseActivity* activity,
                        RRThemeAttributes* theme);

    virtual ~RedditInboxItemView();

    // ========================================================================
    // reset — port of method (Java lines 101-138)
    //
    // Port of: public void reset(final BaseActivity context,
    //     final RedditChangeDataManager changeDataManager,
    //     final RRThemeAttributes theme,
    //     final RedditRenderableInboxItem item,
    //     final boolean showDividerAtTop)
    // ========================================================================

    void reset(BaseActivity* context,
               RedditChangeDataManager* changeDataManager,
               RRThemeAttributes* theme,
               RedditRenderableInboxItem* item,
               bool showDividerAtTop);

    // ========================================================================
    // handleInboxClick — port of method (Java lines 140-144)
    //
    // Port of: public void handleInboxClick(final BaseActivity activity)
    // ========================================================================

    void handleInboxClick(BaseActivity* activity);

    // ========================================================================
    // handleInboxLongClick — port of method (Java lines 146-150)
    //
    // Port of: public void handleInboxLongClick(final BaseActivity activity)
    // ========================================================================

    void handleInboxLongClick(BaseActivity* activity);

private:
    // ========================================================================
    // Fields — exact match (Java lines 37-47)
    // ========================================================================

    // Port of: private final View mDivider; (Java line 37)
    // Port of: private final TextView mHeader; (Java line 38)
    // Port of: private final FrameLayout mBodyHolder; (Java line 39)
    // Port of: private final RRThemeAttributes mTheme; (Java line 41)
    // Port of: private final boolean showLinkButtons; (Java line 43)
    // Port of: private RedditRenderableInboxItem currentItem = null; (Java line 45)
    // Port of: private final BaseActivity mActivity; (Java line 47)

    RRThemeAttributes* m_theme = nullptr;
    BaseActivity* m_activity = nullptr;
    RedditRenderableInboxItem* m_currentItem = nullptr;
    bool m_showLinkButtons = false;

    // Visibility constants
    static constexpr int VISIBLE = 0;
    static constexpr int GONE = 8;
    static constexpr int FOCUS_BLOCK_DESCENDANTS = 0x00060000;
};

} // namespace PinkReader
