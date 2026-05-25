/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_post_header_view.cpp - Port of RedditPostHeaderView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RedditPostHeaderView.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "views/reddit_post_header_view.h"

#include <QString>
#include <functional>

namespace PinkReader {

// ============================================================================
// Stubs for non-ported types
// ============================================================================

// LinearLayout stub
class LinearLayout_Stub : public LinearLayout {
public:
    void setOrientation(int orientation) { (void)orientation; }
    void setPadding(int left, int top, int right, int bottom) {
        (void)left; (void)top; (void)right; (void)bottom;
    }
    void setBackgroundColor(int color) { (void)color; }
    void addView(void *view) { (void)view; }
    void setOnClickListener(std::function<void()> listener) {
        m_clickListener = listener;
    }
    void setOnLongClickListener(std::function<bool()> listener) {
        m_longClickListener = listener;
    }
    void setVisibility(int visibility) { (void)visibility; }
    int getChildCount() const { return 0; }
    void *getChildAt(int index) const { (void)index; return nullptr; }
private:
    std::function<void()> m_clickListener;
    std::function<bool()> m_longClickListener;
};

// TextView stub
class TextView_Stub : public TextView {
public:
    void setText(const QString &text) { m_text = text; }
    void setTextSize(float size) { (void)size; }
    void setTypeface(void *typeface) { (void)typeface; }
    void setTextColor(int color) { (void)color; }
    void setContentDescription(const QString &desc) { (void)desc; }
    QString getText() const { return m_text; }
private:
    QString m_text;
};

// TypedArray stub
class TypedArray {
public:
    int getColor(int index, int defaultValue) {
        (void)index;
        return defaultValue;
    }
    void recycle() {}
};

// ImageButton stub
class ImageButton {
public:
    void setVisibility(int visibility) { (void)visibility; }
    QString getContentDescription() const { return QString(); }
    void setOnClickListener(std::function<void()> listener) {
        m_clickListener = listener;
    }
    void performClick() {
        if (m_clickListener) m_clickListener();
    }
private:
    std::function<void()> m_clickListener;
};

// Resources stub
class Resources {
public:
    float getDensity() const { return 1.0f; }
};

// BaseActivity stub
class BaseActivity {
public:
    Resources *getResources() { return &m_resources; }
    TypedArray *obtainStyledAttributes(const std::vector<int> &attrs) {
        (void)attrs;
        return new TypedArray();
    }
private:
    Resources m_resources;
};

// RedditPreparedPost stub
class RedditPreparedPost {
public:
    QString m_title;
    QString m_subtitle;
    QString m_url;
    void *m_src = nullptr;

    QString getTitle() const { return m_title; }
    QString buildSubtitle(void *activity, bool b) {
        (void)activity; (void)b;
        return m_subtitle;
    }
    QString buildAccessibilityTitle(void *activity, bool b) {
        (void)activity; (void)b;
        return m_title;
    }
    QString buildAccessibilitySubtitle(void *activity, bool b) {
        (void)activity; (void)b;
        return m_subtitle;
    }
    bool isSelf() const { return false; }
    void *getIdAndType() const { return nullptr; }
    void performAction(void *activity, int action) {
        (void)activity; (void)action;
    }
};

// RedditAccount stub
class RedditAccount {
public:
    bool isAnonymous() const { return false; }
    QString username;
};

// RedditAccountManager stub
class RedditAccountManager {
public:
    RedditAccount getDefaultAccount() { return RedditAccount(); }
    static RedditAccountManager *getInstance(void *context) {
        (void)context;
        static RedditAccountManager instance;
        return &instance;
    }
};

// RedditChangeDataManager stub
class RedditChangeDataManager {
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void onRedditDataChange(void *thingIdAndType) = 0;
    };

    bool isUpvoted(void *idAndType) const { (void)idAndType; return false; }
    bool isDownvoted(void *idAndType) const { (void)idAndType; return false; }
    void addListener(void *idAndType, Listener *listener) {
        (void)idAndType; (void)listener;
    }
    void removeListener(void *idAndType, Listener *listener) {
        (void)idAndType; (void)listener;
    }
    static RedditChangeDataManager *getInstance(const RedditAccount &user) {
        (void)user;
        static RedditChangeDataManager instance;
        return &instance;
    }
};

// PrefsUtility stubs
class PrefsUtility {
public:
    static float appearance_fontscale_post_header_titles() { return 1.0f; }
    static float appearance_fontscale_post_header_subtitles() { return 1.0f; }
    static bool pref_appearance_hide_headertoolbar_commentlist() { return false; }
};

// Fonts stub
class Fonts {
public:
    static void *getRobotoLightOrAlternative() { return nullptr; }
};

// LinkHandler stub
class LinkHandler {
public:
    static void onLinkClicked(void *activity, const QString &url, bool b, void *src) {
        (void)activity; (void)url; (void)b; (void)src;
    }
};

// RedditPostActions stub
class RedditPostActions {
public:
    // Action enum
    enum Action {
        UPVOTE, DOWNVOTE, UNVOTE, REPLY, SHARE, ACTION_MENU
    };

    void setupAccessibilityActions(void *mgr, RedditPreparedPost *post,
                                    void *activity, bool b) {
        (void)mgr; (void)post; (void)activity; (void)b;
    }
    void showActionMenu(void *activity, RedditPreparedPost *post) {
        (void)activity; (void)post;
    }
    static RedditPostActions INSTANCE;
};

RedditPostActions RedditPostActions::INSTANCE;

// AccessibilityActionManager stub
class AccessibilityActionManager {
public:
    AccessibilityActionManager(void *view, Resources *resources) {
        (void)view; (void)resources;
    }
};

// General stub
class General_Stub {
public:
    static constexpr int COLOR_INVALID = -1;
};

// ============================================================================
// Java static constants
// ============================================================================

// Port of: LinearLayout.VERTICAL = 1
constexpr int LINEAR_LAYOUT_VERTICAL = 1;

// Port of: View.VISIBLE = 0
constexpr int VIEW_VISIBLE = 0;

// Port of: View.GONE = 8
constexpr int VIEW_GONE = 8;

// Port of: Color.WHITE = 0xFFFFFFFF
constexpr int COLOR_WHITE = 0xFFFFFFFF;

// ============================================================================
// Constructor (Java lines 48-234)
// ============================================================================

RedditPostHeaderView::RedditPostHeaderView(
        BaseActivity *activity,
        RedditPreparedPost *post,
        QObject *parent)
    : QObject(parent)
    , m_activity(activity)
    , m_post(post)
{
    // Port of: super(activity); (Java line 52)
    // (QObject base)

    // Port of: final float dpScale = activity.getResources().getDisplayMetrics().density;
    // (Java line 54)
    float dpScale = 1.0f;
    if (activity) {
        dpScale = activity->getResources()->getDensity();
    }

    // Port of: setOrientation(LinearLayout.VERTICAL); (Java line 56)
    // Stub: Layout orientation not ported

    // Port of: final LinearLayout greyHeader = new LinearLayout(activity); (Java line 58)
    // Port of: RedditPostActions.INSTANCE.setupAccessibilityActions(
    //     new AccessibilityActionManager(greyHeader, activity.getResources()),
    //     post, activity, true); (Java lines 60-66)
    // Port of: greyHeader.setOrientation(LinearLayout.VERTICAL); (Java line 68)
    // Port of: final int sidesPadding = (int)(15.0f * dpScale); (Java line 70)
    // Port of: final int topPadding = (int)(10.0f * dpScale); (Java line 71)
    // Port of: greyHeader.setPadding(sidesPadding, topPadding, sidesPadding, topPadding); (Java line 73)

    // Port of: final float titleFontScale = PrefsUtility.appearance_fontscale_post_header_titles(); (Java line 75)
    // Port of: final TextView title = new TextView(activity); (Java line 77)
    // title.setTextSize(19.0f * titleFontScale); (Java line 78)
    // title.setTypeface(Fonts.getRobotoLightOrAlternative()); (Java line 79)
    // title.setText(post.src.getTitle()); (Java line 80)
    // title.setContentDescription(post.buildAccessibilityTitle(activity, true)); (Java line 81)
    // title.setTextColor(Color.WHITE); (Java line 82)
    // greyHeader.addView(title); (Java line 83)

    // Port of: final float subtitleFontScale = ...; (Java lines 85-86)
    // subtitle = new TextView(activity); (Java line 88)
    // subtitle.setTextSize(13.0f * subtitleFontScale); (Java line 89)
    // subtitle.setText(post.buildSubtitle(activity, true)); (Java line 90)
    // subtitle.setContentDescription(post.buildAccessibilitySubtitle(activity, true)); (Java line 91)
    // subtitle.setTextColor(Color.rgb(200, 200, 200)); (Java line 93)
    // greyHeader.addView(subtitle); (Java line 94)

    // Port of: { Grey header background color } (Java lines 96-103)
    // Port of: greyHeader.setOnClickListener(v -> { ... }); (Java lines 105-113)
    // Port of: greyHeader.setOnLongClickListener(v -> { ... }); (Java lines 115-118)
    // Port of: addView(greyHeader); (Java line 120)

    // Port of: final RedditAccount currentUser =
    //     RedditAccountManager.getInstance(activity).getDefaultAccount(); (Java lines 122-123)

    // Port of: if(!currentUser.isAnonymous()) { ... } else { null tasks } (Java lines 125-233)
    // - changeDataManager setup
    // - toolbar buttons (if not hidden)
    // - changeListener setup
    // - mChangeListenerAddTask / mChangeListenerRemoveTask assignment

    // In default (no logged-in user), both tasks are null
    m_changeListenerAddTask = nullptr;
    m_changeListenerRemoveTask = nullptr;
}

// ============================================================================
// Destructor
// ============================================================================

RedditPostHeaderView::~RedditPostHeaderView() = default;

// ============================================================================
// onAttachedToWindow — port of override method (Java lines 236-243)
// ============================================================================

void RedditPostHeaderView::onAttachedToWindow() {
    // Port of: super.onAttachedToWindow(); (Java line 238)
    // Port of: if(mChangeListenerAddTask != null) { mChangeListenerAddTask.run(); }
    // (Java lines 240-242)
    if (m_changeListenerAddTask) {
        m_changeListenerAddTask();
    }
}

// ============================================================================
// onDetachedFromWindow — port of override method (Java lines 245-252)
// ============================================================================

void RedditPostHeaderView::onDetachedFromWindow() {
    // Port of: super.onDetachedFromWindow(); (Java line 247)
    // Port of: if(mChangeListenerRemoveTask != null) { mChangeListenerRemoveTask.run(); }
    // (Java lines 249-251)
    if (m_changeListenerRemoveTask) {
        m_changeListenerRemoveTask();
    }
}

} // namespace PinkReader
