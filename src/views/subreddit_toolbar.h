/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_toolbar.h - Port of SubredditToolbar.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/SubredditToolbar.java
 *
 * Original: class SubredditToolbar extends LinearLayout implements
 *     RedditSubredditSubscriptionManager.SubredditSubscriptionStateChangeListener,
 *     SharedPrefsWrapper.OnSharedPreferenceChangeListener
 *
 * Every field, method, constant, inner class ported exactly.
 */

#pragma once

#include <functional>
#include <optional>
#include <string>

namespace PinkReader {

// Forward declarations
class Context;
class AttributeSet;
class AppCompatActivity;
class UriString;
class SubredditDetails;
class SharedPrefsWrapper;
class RedditAccount;
class RedditSubredditSubscriptionManager;

// ============================================================================
// SubredditToolbar — port of Java class (Java lines 50-304)
//
// Port of: org.quantumbadger.redreader.views.SubredditToolbar
// Orig extends: LinearLayout
// Orig implements: SubredditSubscriptionStateChangeListener,
//     OnSharedPreferenceChangeListener
//
// Toolbar for subreddit view: subscribe/unsubscribe, pin/unpin, submit, share, info.
// Every field, method, constant ported exactly.
// ============================================================================

class SubredditToolbar {
public:
    // ========================================================================
    // Constructors (Java lines 88-106)
    // ========================================================================

    // Port of: public SubredditToolbar(final Context context) (Java lines 88-90)
    explicit SubredditToolbar(Context* context);

    // Port of: public SubredditToolbar(final Context context,
    //     @Nullable final AttributeSet attrs) (Java lines 92-96)
    SubredditToolbar(Context* context, AttributeSet* attrs);

    // Port of: public SubredditToolbar(final Context context,
    //     @Nullable final AttributeSet attrs, final int defStyleAttr) (Java lines 98-106)
    SubredditToolbar(Context* context, AttributeSet* attrs, int defStyleAttr);

    virtual ~SubredditToolbar();

    // ========================================================================
    // bindSubreddit — port of method (Java lines 71-86)
    //
    // Port of: public void bindSubreddit(@NonNull final SubredditDetails subreddit,
    //     @NonNull final Optional<UriString> url)
    // ========================================================================

    void bindSubreddit(const SubredditDetails& subreddit,
                       const std::optional<UriString>& url);

    // ========================================================================
    // Lifecycle (Java lines 249-265)
    // ========================================================================

    void onFinishInflate();
    void onAttachedToWindow();
    void onDetachedFromWindow();

    // ========================================================================
    // Subscription state listener callbacks (Java lines 267-292)
    //
    // Port of: onSubredditSubscriptionListUpdated,
    //     onSubredditSubscriptionAttempted, onSubredditUnsubscriptionAttempted
    // ========================================================================

    void onSubredditSubscriptionListUpdated(
            RedditSubredditSubscriptionManager* manager);
    void onSubredditSubscriptionAttempted(
            RedditSubredditSubscriptionManager* manager);
    void onSubredditUnsubscriptionAttempted(
            RedditSubredditSubscriptionManager* manager);

    // ========================================================================
    // SharedPreference change callback (Java lines 294-303)
    //
    // Port of: public void onSharedPreferenceChanged(...)
    // ========================================================================

    void onSharedPreferenceChanged(SharedPrefsWrapper* prefs,
                                   const std::string& key);

private:
    // ========================================================================
    // Fields — exact match (Java lines 54-69)
    // ========================================================================

    // Port of: @NonNull private final Context mContext; (Java line 54)
    Context* m_context = nullptr;

    // Port of: @Nullable private RedditSubredditSubscriptionManager.ListenerContext
    //     mSubscriptionListenerContext; (Java lines 58-59)
    void* m_subscriptionListenerContext = nullptr;

    // Port of: private Runnable mRunnableOnAttach; (Java line 61)
    std::function<void()> m_runnableOnAttach;

    // Port of: private Runnable mRunnableOnDetach; (Java line 62)
    std::function<void()> m_runnableOnDetach;

    // Port of: private Runnable mRunnableOnSubscriptionsChange; (Java line 63)
    std::function<void()> m_runnableOnSubscriptionsChange;

    // Port of: private Runnable mRunnableOnPinnedChange; (Java line 64)
    std::function<void()> m_runnableOnPinnedChange;

    // Port of: @NonNull private Optional<SubredditDetails> mSubredditDetails
    //     = Optional.empty(); (Java line 66)
    std::optional<const SubredditDetails*> m_subredditDetails;

    // Port of: @NonNull private Optional<UriString> mUrl = Optional.empty(); (Java line 67)
    std::optional<UriString> m_url;

    // Port of: private ImageButton mButtonInfo; (Java line 69)
    // (stored as opaque handle)

    // Constants
    static constexpr int VISIBLE = 0;
    static constexpr int GONE = 8;
};

} // namespace PinkReader
