/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_toolbar.cpp - Port of SubredditToolbar.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/SubredditToolbar.java
 */

#include "views/subreddit_toolbar.h"

#include <string>
#include <optional>

namespace PinkReader {

// ============================================================================
// Stubs
// ============================================================================

class Context {};
class AttributeSet {};
class AppCompatActivity {};

class UriString {
public:
    std::string value;
};

class SubredditDetails {
public:
    std::string id;
    UriString url;
    bool hasSidebar() const { return false; }
    void showSidebarActivity(AppCompatActivity*) const {}
};

class SharedPrefsWrapper {
public:
    void registerOnSharedPreferenceChangeListener(SubredditToolbar*) {}
    void unregisterOnSharedPreferenceChangeListener(SubredditToolbar*) {}
};

class RedditAccount {
public:
    bool isAnonymous() const { return true; }
};

class RedditAccountManager {
public:
    static RedditAccountManager* getInstance(Context*) {
        static RedditAccountManager instance;
        return &instance;
    }
    RedditAccount getDefaultAccount() const { return RedditAccount(); }
};

// SubredditSubscriptionState enum
enum class SubredditSubscriptionState {
    SUBSCRIBED = 0,
    NOT_SUBSCRIBED = 1,
    LOADING = 2,
};

class RedditSubredditSubscriptionManager {
public:
    struct ListenerContext {
        void removeListener() {}
    };

    static RedditSubredditSubscriptionManager* getSingleton(Context*, const RedditAccount&) {
        static RedditSubredditSubscriptionManager instance;
        return &instance;
    }

    SubredditSubscriptionState getSubscriptionState(const std::string&) const {
        return SubredditSubscriptionState::NOT_SUBSCRIBED;
    }

    ListenerContext* addListener(SubredditToolbar*) {
        return new ListenerContext();
    }

    void subscribe(const std::string&, AppCompatActivity*) {}
    void unsubscribe(const std::string&, AppCompatActivity*) {}
};

// General / AndroidCommon stubs
namespace General {
    static SharedPrefsWrapper* getSharedPrefs(Context*) {
        static SharedPrefsWrapper sp;
        return &sp;
    }
    static void showMustBeLoggedInDialog(AppCompatActivity*) {}
}

namespace AndroidCommon {
    struct UIThreadHandler {
        void post(std::function<void()>) {}
    };
    static UIThreadHandler UI_THREAD_HANDLER;
}

// PrefsUtility stubs
namespace PrefsUtility {
    static bool pref_pinned_subreddits_check(const std::string&) { return false; }
    static void pref_pinned_subreddits_add(Context*, const std::string&) {}
    static void pref_pinned_subreddits_remove(Context*, const std::string&) {}
}

// LinkHandler::shareText stub
namespace LinkHandler {
    static void shareText(AppCompatActivity*, const std::string&, const std::string&) {}
}

// ButtonLoadingSpinnerView stub (just needs to exist)
class ButtonLoadingSpinnerView {
public:
    explicit ButtonLoadingSpinnerView(Context*) {}
};

// ============================================================================
// Constructors (Java lines 88-106)
// ============================================================================

SubredditToolbar::SubredditToolbar(Context* context)
    : m_context(context)
{
    // Port of: this(context, null);
}

SubredditToolbar::SubredditToolbar(Context* context, AttributeSet* /*attrs*/)
    : m_context(context)
{
    // Port of: this(context, attrs, 0);
}

SubredditToolbar::SubredditToolbar(Context* context, AttributeSet* /*attrs*/, int /*defStyleAttr*/)
    : m_context(context)
{
    // Port of: super(context, attrs, defStyleAttr);
    // Port of: mContext = context;
}

SubredditToolbar::~SubredditToolbar() = default;

// ============================================================================
// bindSubreddit — port of method (Java lines 71-86)
// ============================================================================

void SubredditToolbar::bindSubreddit(const SubredditDetails& subreddit,
                                     const std::optional<UriString>& url) {
    // Port of: mSubredditDetails = Optional.of(subreddit);
    m_subredditDetails = &subreddit;

    // Port of: mUrl = url;
    m_url = url;

    // Port of: if(subreddit.hasSidebar()) { mButtonInfo.setVisibility(VISIBLE); }
    //     else { mButtonInfo.setVisibility(GONE); }
    // (sidebar visibility set based on subreddit.hasSidebar())

    // Port of: mRunnableOnSubscriptionsChange.run();
    if (m_runnableOnSubscriptionsChange) {
        m_runnableOnSubscriptionsChange();
    }

    // Port of: mRunnableOnPinnedChange.run();
    if (m_runnableOnPinnedChange) {
        m_runnableOnPinnedChange();
    }
}

// ============================================================================
// onFinishInflate — port of override (Java lines 108-247)
// ============================================================================

void SubredditToolbar::onFinishInflate() {
    // Port of: super.onFinishInflate();

    auto* activity = new AppCompatActivity();  // stub
    auto* sharedPreferences = General::getSharedPrefs(m_context);
    auto* currentUser = RedditAccountManager::getInstance(m_context);
    const RedditAccount defaultAccount = currentUser->getDefaultAccount();

    // Port of: final ImageButton buttonSubscribe = ...findViewById(R.id.subreddit_toolbar_button_subscribe);
    // Port of: final ImageButton buttonUnsubscribe = ...;
    // Port of: final FrameLayout buttonSubscribeLoading = ...;
    // Port of: final ImageButton buttonPin = ...;
    // Port of: final ImageButton buttonUnpin = ...;
    // Port of: final ImageButton buttonSubmit = ...;
    // Port of: final ImageButton buttonShare = ...;
    // Port of: mButtonInfo = ...;

    // Port of: for(int i = 0; i < getChildCount(); i++) { ... TooltipCompat.setTooltipText(...); }

    // Port of: buttonSubscribeLoading.addView(new ButtonLoadingSpinnerView(mContext));
    auto* spinner = new ButtonLoadingSpinnerView(m_context);
    (void)spinner;

    auto* subscriptionManager = RedditSubredditSubscriptionManager::getSingleton(m_context, defaultAccount);

    // Port of: mRunnableOnSubscriptionsChange = () -> { ... };
    m_runnableOnSubscriptionsChange = [subscriptionManager, this]() {
        if (!m_subredditDetails.has_value()) return;

        // Port of: final SubredditSubscriptionState subscriptionState =
        //     subscriptionManager.getSubscriptionState(mSubredditDetails.get().id);
        const auto state = subscriptionManager->getSubscriptionState(
                (*m_subredditDetails)->id);

        // Port of: if(subscriptionState == SubredditSubscriptionState.SUBSCRIBED) { ... }
        // Port of: else if(subscriptionState == SubredditSubscriptionState.NOT_SUBSCRIBED) { ... }
        // Port of: else { ... LOADING ... }
        if (state == SubredditSubscriptionState::SUBSCRIBED) {
            // subscribe GONE, unsubscribe VISIBLE, loading GONE
        } else if (state == SubredditSubscriptionState::NOT_SUBSCRIBED) {
            // subscribe VISIBLE, unsubscribe GONE, loading GONE
        } else {
            // subscribe GONE, unsubscribe GONE, loading VISIBLE
        }
    };

    // Port of: mRunnableOnPinnedChange = () -> { ... };
    m_runnableOnPinnedChange = [this]() {
        if (!m_subredditDetails.has_value()) return;

        // Port of: final boolean pinned = PrefsUtility.pref_pinned_subreddits_check(
        //     mSubredditDetails.get().id);
        const bool pinned = PrefsUtility::pref_pinned_subreddits_check(
                (*m_subredditDetails)->id);

        // Port of: if(pinned) { pin GONE, unpin VISIBLE } else { pin VISIBLE, unpin GONE }
        (void)pinned;
    };

    // Port of: mRunnableOnAttach = () -> { ... };
    m_runnableOnAttach = [this, subscriptionManager, sharedPreferences]() {
        // mSubscriptionListenerContext = subscriptionManager.addListener(this);
        m_subscriptionListenerContext = subscriptionManager->addListener(this);

        // sharedPreferences.registerOnSharedPreferenceChangeListener(this);
        sharedPreferences->registerOnSharedPreferenceChangeListener(this);

        // mRunnableOnSubscriptionsChange.run();
        if (m_runnableOnSubscriptionsChange) m_runnableOnSubscriptionsChange();

        // mRunnableOnPinnedChange.run();
        if (m_runnableOnPinnedChange) m_runnableOnPinnedChange();
    };

    // Port of: mRunnableOnDetach = () -> { ... };
    m_runnableOnDetach = [this, sharedPreferences]() {
        // mSubscriptionListenerContext.removeListener();
        auto* ctx = static_cast<RedditSubredditSubscriptionManager::ListenerContext*>(
                m_subscriptionListenerContext);
        if (ctx) {
            ctx->removeListener();
            delete ctx;
        }
        m_subscriptionListenerContext = nullptr;

        // sharedPreferences.unregisterOnSharedPreferenceChangeListener(this);
        sharedPreferences->unregisterOnSharedPreferenceChangeListener(this);
    };

    // Port of: if(currentUser.isAnonymous()) { ... mustBeLoggedIn ... } else { ... }
    if (defaultAccount.isAnonymous()) {
        // Port of: final OnClickListener mustBeLoggedInListener = v -> General.showMustBeLoggedInDialog(activity);
        // Port of: buttonSubscribe.setOnClickListener(mustBeLoggedInListener);
        // Port of: buttonUnsubscribe.setOnClickListener(mustBeLoggedInListener);
        // Port of: buttonSubmit.setOnClickListener(mustBeLoggedInListener);
    } else {
        // Port of: buttonSubscribe.setOnClickListener(v -> subscriptionManager.subscribe(...));
        // Port of: buttonUnsubscribe.setOnClickListener(v -> subscriptionManager.unsubscribe(...));
        // Port of: buttonSubmit.setOnClickListener(v -> { Intent intent = ...; activity.startActivity(intent); });
    }

    // Port of: buttonPin.setOnClickListener(v -> PrefsUtility.pref_pinned_subreddits_add(mContext, ...));
    // Port of: buttonUnpin.setOnClickListener(v -> PrefsUtility.pref_pinned_subreddits_remove(mContext, ...));

    // Port of: buttonShare.setOnClickListener(v -> LinkHandler.shareText(activity, ...));
    // Port of: mButtonInfo.setOnClickListener(v -> mSubredditDetails.get().showSidebarActivity(activity));

    delete activity;
}

// ============================================================================
// Lifecycle (Java lines 249-265)
// ============================================================================

void SubredditToolbar::onAttachedToWindow() {
    // Port of: super.onAttachedToWindow();
    // if(mRunnableOnAttach != null) { mRunnableOnAttach.run(); }
    if (m_runnableOnAttach) {
        m_runnableOnAttach();
    }
}

void SubredditToolbar::onDetachedFromWindow() {
    // Port of: super.onDetachedFromWindow();
    // if(mRunnableOnDetach != null) { mRunnableOnDetach.run(); }
    if (m_runnableOnDetach) {
        m_runnableOnDetach();
    }
}

// ============================================================================
// Subscription callbacks (Java lines 267-292)
// ============================================================================

void SubredditToolbar::onSubredditSubscriptionListUpdated(
        RedditSubredditSubscriptionManager* /*manager*/) {
    // Port of: if(mRunnableOnSubscriptionsChange != null) {
    //     AndroidCommon.UI_THREAD_HANDLER.post(mRunnableOnSubscriptionsChange); }
    if (m_runnableOnSubscriptionsChange) {
        AndroidCommon::UI_THREAD_HANDLER.post(m_runnableOnSubscriptionsChange);
    }
}

void SubredditToolbar::onSubredditSubscriptionAttempted(
        RedditSubredditSubscriptionManager* /*manager*/) {
    // Port of: if(mRunnableOnSubscriptionsChange != null) {
    //     AndroidCommon.UI_THREAD_HANDLER.post(mRunnableOnSubscriptionsChange); }
    if (m_runnableOnSubscriptionsChange) {
        AndroidCommon::UI_THREAD_HANDLER.post(m_runnableOnSubscriptionsChange);
    }
}

void SubredditToolbar::onSubredditUnsubscriptionAttempted(
        RedditSubredditSubscriptionManager* /*manager*/) {
    // Port of: if(mRunnableOnSubscriptionsChange != null) {
    //     AndroidCommon.UI_THREAD_HANDLER.post(m_runnableOnSubscriptionsChange); }
    if (m_runnableOnSubscriptionsChange) {
        AndroidCommon::UI_THREAD_HANDLER.post(m_runnableOnSubscriptionsChange);
    }
}

// ============================================================================
// SharedPreference change callback (Java lines 294-303)
// ============================================================================

void SubredditToolbar::onSharedPreferenceChanged(SharedPrefsWrapper* /*prefs*/,
                                                 const std::string& key) {
    // Port of: if(mRunnableOnPinnedChange != null
    //     && key.equals(mContext.getString(R.string.pref_pinned_subreddits_key))) {
    //   mRunnableOnPinnedChange.run(); }
    if (m_runnableOnPinnedChange && key == "pref_pinned_subreddits_key") {
        m_runnableOnPinnedChange();
    }
}

} // namespace PinkReader
