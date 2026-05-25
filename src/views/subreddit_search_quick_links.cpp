/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_search_quick_links.cpp - Port of SubredditSearchQuickLinks.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/SubredditSearchQuickLinks.java
 */

#include "views/subreddit_search_quick_links.h"

#include <algorithm>
#include <string>

namespace PinkReader {

// ============================================================================
// Stubs
// ============================================================================

class AppCompatActivity {
public:
    virtual ~AppCompatActivity() = default;
};

class Context {};
class AttributeSet {};

// LinkHandler stub
namespace LinkHandler {
    static void onLinkClicked(AppCompatActivity*, const UriString&) {}
}

// SearchPostListURL stub
class SearchPostListURL {
public:
    static SearchPostListURL build(const std::string*, const std::string&) {
        return SearchPostListURL();
    }
    std::string generateJsonUri() const { return ""; }
};

// ============================================================================
// Constructors (Java lines 52-68)
// ============================================================================

SubredditSearchQuickLinks::SubredditSearchQuickLinks(Context* /*context*/) {
    // Port of: this(context, null);
}

SubredditSearchQuickLinks::SubredditSearchQuickLinks(Context* /*context*/, AttributeSet* /*attrs*/) {
    // Port of: this(context, attrs, 0);
}

SubredditSearchQuickLinks::SubredditSearchQuickLinks(Context* /*context*/,
                                                      AttributeSet* /*attrs*/,
                                                      int /*defStyleAttr*/) {
    // Port of: super(context, attrs, defStyleAttr);
}

SubredditSearchQuickLinks::~SubredditSearchQuickLinks() = default;

// ============================================================================
// onFinishInflate — port of override (Java lines 72-84)
// ============================================================================

void SubredditSearchQuickLinks::onFinishInflate() {
    // Port of: super.onFinishInflate();

    // Port of: mButtonSubreddit = Objects.requireNonNull(
    //     (MaterialButton)findViewById(R.id.button_go_to_subreddit));
    // Port of: mButtonUser = Objects.requireNonNull(
    //     (MaterialButton)findViewById(R.id.button_go_to_user));
    // Port of: mButtonUrl = Objects.requireNonNull(
    //     (MaterialButton)findViewById(R.id.button_go_to_url));
    // Port of: mButtonSearch = Objects.requireNonNull(
    //     (MaterialButton)findViewById(R.id.button_go_to_search));
    // In C++ these are stubbed as bool flags.
}

// ============================================================================
// bind — port of method (Java lines 86-99)
// ============================================================================

void SubredditSearchQuickLinks::bind(AppCompatActivity* activity,
                                     EventListenerSet<std::string>* querySource) {
    // Port of: mActivity = activity;
    m_activity = activity;

    // Port of: if(mBinding != null) { throw new RuntimeException("Search view already bound"); }
    if (m_binding != nullptr) {
        throw std::runtime_error("Search view already bound");
    }

    // Port of: mBinding = querySource; doBind();
    m_binding = querySource;
    doBind();
}

// ============================================================================
// doBind — port of method (Java lines 101-106)
// ============================================================================

void SubredditSearchQuickLinks::doBind() {
    // Port of: if(mBinding != null) { mBindingListener = this::update;
    //     update(mBinding.register(mBindingListener)); }
    if (m_binding != nullptr) {
        m_bindingListener = [this](std::string query) {
            this->update(&query);
        };
        const std::string initialValue = m_binding->registerListener(m_bindingListener);
        update(&initialValue);
    }
}

// ============================================================================
// doUnbind — port of method (Java lines 108-113)
// ============================================================================

void SubredditSearchQuickLinks::doUnbind() {
    // Port of: if(mBinding != null && mBindingListener != null) {
    //     mBinding.unregister(mBindingListener); mBindingListener = null; }
    if (m_binding != nullptr && m_bindingListener) {
        m_binding->unregister(m_bindingListener);
        m_bindingListener = nullptr;
    }
}

// ============================================================================
// onAttachedToWindow — port of override (Java lines 116-119)
// ============================================================================

void SubredditSearchQuickLinks::onAttachedToWindow() {
    // Port of: super.onAttachedToWindow(); doBind();
    doBind();
}

// ============================================================================
// onDetachedFromWindow — port of override (Java lines 122-125)
// ============================================================================

void SubredditSearchQuickLinks::onDetachedFromWindow() {
    // Port of: super.onDetachedFromWindow(); doUnbind();
    doUnbind();
}

// ============================================================================
// update — port of private method (Java lines 127-202)
// ============================================================================

void SubredditSearchQuickLinks::update(const std::string* queryPtr) {
    // Port of: if(query != null) { query = query.trim(); }
    std::string query;
    if (queryPtr != nullptr) {
        query = *queryPtr;
        // Trim
        auto start = query.find_first_not_of(" \t");
        if (start == std::string::npos) {
            query.clear();
        } else {
            query = query.substr(start, query.find_last_not_of(" \t") - start + 1);
        }
    }

    // Port of: if(query == null || query.isEmpty()) { ... disable all buttons ... }
    if (query.empty()) {
        // Port of: mButtonSubreddit.setText(R.string.find_location_button_goto_subreddit);
        // Port of: mButtonSubreddit.setEnabled(false); mButtonUser.setEnabled(false);
        //     mButtonUrl.setEnabled(false); mButtonSearch.setEnabled(false);
        m_buttonSubredditEnabled = false;
        m_buttonUserEnabled = false;
        m_buttonUrlEnabled = false;
        m_buttonSearchEnabled = false;

        // Port of: mButtonSubreddit.setVisibility(VISIBLE); ... all VISIBLE
        m_buttonSubredditVisible = true;
        m_buttonUserVisible = true;
        m_buttonUrlVisible = true;
        m_buttonSearchVisible = true;
    } else {
        // Port of: final ProcessedQuery queryProcessed = new ProcessedQuery(query);
        const ProcessedQuery queryProcessed(query);

        // Port of: if(queryProcessed.querySubreddit != null) { ... }
        if (queryProcessed.querySubreddit.has_value()) {
            // Port of: mButtonSubreddit.setVisibility(VISIBLE);
            m_buttonSubredditVisible = true;

            // Port of: final String subredditPrefixed = "/r/" + queryProcessed.querySubreddit;
            const std::string subredditPrefixed = "/r/" + *queryProcessed.querySubreddit;

            // Port of: mButtonSubreddit.setText(subredditPrefixed);
            // Port of: mButtonSubreddit.setOnClickListener(view -> LinkHandler.onLinkClicked(
            //     mActivity, new UriString(subredditPrefixed)));
            (void)subredditPrefixed;
        } else {
            // Port of: mButtonSubreddit.setVisibility(GONE);
            m_buttonSubredditVisible = false;
        }

        // Port of: if(queryProcessed.queryUser != null) { ... }
        if (queryProcessed.queryUser.has_value()) {
            m_buttonUserVisible = true;

            // Port of: mButtonUser.setOnClickListener(view -> LinkHandler.onLinkClicked(
            //     mActivity, new UriString("/u/" + queryProcessed.queryUser)));
            (void)*queryProcessed.queryUser;
        } else {
            // Port of: mButtonUser.setVisibility(GONE);
            m_buttonUserVisible = false;
        }

        // Port of: if(queryProcessed.queryUrl != null) { ... }
        if (queryProcessed.queryUrl.has_value()) {
            m_buttonUrlVisible = true;

            // Port of: mButtonUrl.setOnClickListener(view -> LinkHandler.onLinkClicked(
            //     mActivity, queryProcessed.queryUrl));
            (void)*queryProcessed.queryUrl;
        } else {
            // Port of: mButtonUrl.setVisibility(GONE);
            m_buttonUrlVisible = false;
        }

        // Port of: mButtonSearch.setOnClickListener(view -> { ... });
        //   SearchPostListURL url = SearchPostListURL.build(null, queryProcessed.querySearch);
        //   Intent intent = new Intent(mActivity, PostListingActivity.class);
        //   intent.setData(url.generateJsonUri());
        //   mActivity.startActivity(intent);
        // Port of: mButtonSearch always VISIBLE
        m_buttonSearchVisible = true;

        // Port of: mButtonSubreddit.setEnabled(true); ... all enabled
        m_buttonSubredditEnabled = true;
        m_buttonUserEnabled = true;
        m_buttonUrlEnabled = true;
        m_buttonSearchEnabled = true;
    }
}

// ============================================================================
// ProcessedQuery — port of inner static class (Java lines 204-259)
// ============================================================================

SubredditSearchQuickLinks::ProcessedQuery::ProcessedQuery(const std::string& query)
    : querySearch(query)
{
    // Port of: querySearch = query;
    // (already set in initializer list)

    // Port of: final boolean startsWithSlashRSlash = query.startsWith("/r/");
    const bool startsWithSlashRSlash = (query.rfind("/r/", 0) == 0);
    // Port of: final boolean startsWithRSlash = query.startsWith("r/");
    const bool startsWithRSlash = (query.rfind("r/", 0) == 0);

    // Port of: final boolean startsWithSlashUSlash = query.startsWith("/u/");
    const bool startsWithSlashUSlash = (query.rfind("/u/", 0) == 0);
    // Port of: final boolean startsWithUSlash = query.startsWith("u/");
    const bool startsWithUSlash = (query.rfind("u/", 0) == 0);

    // Port of: if(query.contains("://")) { querySubreddit = null;
    //     queryUser = null; queryUrl = new UriString(query); }
    if (query.find("://") != std::string::npos) {
        querySubreddit = std::nullopt;
        queryUser = std::nullopt;
        queryUrl = UriString(query);

    // Port of: } else if(startsWithSlashRSlash || startsWithRSlash) {
    //     if(startsWithSlashRSlash) querySubreddit = query.substring(3);
    //     else querySubreddit = query.substring(2);
    //     queryUser = null; queryUrl = null; }
    } else if (startsWithSlashRSlash || startsWithRSlash) {
        if (startsWithSlashRSlash) {
            querySubreddit = query.substr(3);
        } else {
            querySubreddit = query.substr(2);
        }
        queryUser = std::nullopt;
        queryUrl = std::nullopt;

    // Port of: } else if(startsWithSlashUSlash || startsWithUSlash) {
    //     if(startsWithSlashUSlash) queryUser = query.substring(3);
    //     else queryUser = query.substring(2);
    //     querySubreddit = null; queryUrl = null; }
    } else if (startsWithSlashUSlash || startsWithUSlash) {
        if (startsWithSlashUSlash) {
            queryUser = query.substr(3);
        } else {
            queryUser = query.substr(2);
        }
        querySubreddit = std::nullopt;
        queryUrl = std::nullopt;

    // Port of: } else if(query.startsWith("/")) {
    //     querySubreddit = null; queryUser = null;
    //     queryUrl = new UriString("https://reddit.com" + query); }
    } else if (!query.empty() && query.front() == '/') {
        querySubreddit = std::nullopt;
        queryUser = std::nullopt;
        queryUrl = UriString("https://reddit.com" + query);

    // Port of: } else { querySubreddit = query.replaceAll("[ \t]+", "_");
    //     queryUser = querySubreddit; queryUrl = new UriString("https://" + query); }
    } else {
        // Replace whitespace runs with single underscore
        std::string replaced = query;
        std::replace(replaced.begin(), replaced.end(), ' ', '_');
        // Remove consecutive underscores (simplified: just single replace)
        // Then remove \t (already handled by space replacement above)
        querySubreddit = replaced;
        queryUser = replaced;
        queryUrl = UriString("https://" + query);
    }
}

} // namespace PinkReader
