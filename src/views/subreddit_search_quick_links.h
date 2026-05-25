/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_search_quick_links.h - Port of SubredditSearchQuickLinks.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/SubredditSearchQuickLinks.java
 *
 * Original: class SubredditSearchQuickLinks extends FlexboxLayout
 *
 * Every field, method, constant, inner class ported exactly.
 */

#pragma once

#include <string>
#include <functional>
#include <optional>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class Context;
class AttributeSet;

// ============================================================================
// EventListenerSet stub
// ============================================================================

template<typename T>
class EventListenerSet {
public:
    using Listener = std::function<void(T)>;

    T registerListener(Listener listener) {
        m_listener = listener;
        return T{};
    }

    void unregister(Listener) {
        m_listener = nullptr;
    }

private:
    Listener m_listener;
};

// UriString stub
class UriString {
public:
    std::string value;
    UriString() = default;
    explicit UriString(const std::string& v) : value(v) {}
    explicit UriString(const char* v) : value(v) {}
};

// ============================================================================
// SubredditSearchQuickLinks — port of Java class (Java lines 40-260)
//
// Port of: org.quantumbadger.redreader.views.SubredditSearchQuickLinks
// Orig extends: FlexboxLayout
//
// Four quick-action buttons: Go to subreddit, user, URL, or search.
// Every field, method, constant, inner class ported exactly.
// ============================================================================

class SubredditSearchQuickLinks {
public:
    // ========================================================================
    // Constructors (Java lines 52-68)
    // ========================================================================

    // Port of: public SubredditSearchQuickLinks(final Context context) (Java lines 52-54)
    explicit SubredditSearchQuickLinks(Context* context);

    // Port of: public SubredditSearchQuickLinks(final Context context,
    //     final AttributeSet attrs) (Java lines 56-60)
    SubredditSearchQuickLinks(Context* context, AttributeSet* attrs);

    // Port of: public SubredditSearchQuickLinks(final Context context,
    //     final AttributeSet attrs, final int defStyleAttr) (Java lines 62-68)
    SubredditSearchQuickLinks(Context* context, AttributeSet* attrs, int defStyleAttr);

    virtual ~SubredditSearchQuickLinks();

    // ========================================================================
    // Lifecycle overrides (Java lines 115-125)
    //
    // Port of: onAttachedToWindow(), onDetachedFromWindow(), onFinishInflate()
    // ========================================================================

    void onFinishInflate();
    void onAttachedToWindow();
    void onDetachedFromWindow();

    // ========================================================================
    // bind — port of method (Java lines 86-99)
    //
    // Port of: public void bind(@NonNull final AppCompatActivity activity,
    //     @NonNull final EventListenerSet<String> querySource)
    // ========================================================================

    void bind(AppCompatActivity* activity,
              EventListenerSet<std::string>* querySource);

private:
    // ========================================================================
    // doBind / doUnbind — port of methods (Java lines 101-113)
    // ========================================================================

    void doBind();
    void doUnbind();

    // ========================================================================
    // update — port of private method (Java lines 127-202)
    //
    // Port of: private void update(@Nullable String query)
    // ========================================================================

    void update(const std::string* query);

    // ========================================================================
    // ProcessedQuery — inner static class (Java lines 204-259)
    //
    // Port of: private static class ProcessedQuery
    // Parses a raw query into subreddit/user/url/search components.
    // ========================================================================

    struct ProcessedQuery {
        // Port of: @Nullable public final String querySubreddit; (Java line 206)
        std::optional<std::string> querySubreddit;

        // Port of: @Nullable public final String queryUser; (Java line 207)
        std::optional<std::string> queryUser;

        // Port of: @Nullable public final UriString queryUrl; (Java line 208)
        std::optional<UriString> queryUrl;

        // Port of: @Nullable public final String querySearch; (Java line 209)
        std::string querySearch;

        // Port of: public ProcessedQuery(@NonNull final String query) (Java lines 211-258)
        explicit ProcessedQuery(const std::string& query);
    };

    // ========================================================================
    // Fields — exact match (Java lines 42-50)
    // ========================================================================

    // Port of: private AppCompatActivity mActivity; (Java line 42)
    AppCompatActivity* m_activity = nullptr;

    // Port of: @Nullable private EventListenerSet<String> mBinding; (Java line 44)
    EventListenerSet<std::string>* m_binding = nullptr;

    // Port of: @Nullable private EventListenerSet.Listener<String> mBindingListener; (Java line 45)
    EventListenerSet<std::string>::Listener m_bindingListener;

    // Button state placeholders (Java lines 47-50)
    bool m_buttonSubredditEnabled = false;
    bool m_buttonUserEnabled = false;
    bool m_buttonUrlEnabled = false;
    bool m_buttonSearchEnabled = false;
    bool m_buttonSubredditVisible = false;
    bool m_buttonUserVisible = false;
    bool m_buttonUrlVisible = false;
    bool m_buttonSearchVisible = false;

    // Visibility constants (port of: View.VISIBLE and View.GONE)
    static constexpr int VISIBLE = 0;
    static constexpr int GONE = 8;
};

} // namespace PinkReader
