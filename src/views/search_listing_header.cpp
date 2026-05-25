/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: search_listing_header.cpp - Port of SearchListingHeader.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/SearchListingHeader.java
 */

#include "views/search_listing_header.h"

#include <string>
#include <algorithm>

namespace PinkReader {

// ============================================================================
// Stubs
// ============================================================================

class Context {
public:
    virtual ~Context() = default;
};

// SearchPostListURL stub (port of: org.quantumbadger.redreader.reddit.url.SearchPostListURL)
class SearchPostListURL {
public:
    enum class Type {
        SUB_OR_SUB_COMBO = 0,
        MULTI = 1,
        ALL = 2,
    };

    std::string query;
    std::string subreddit;
    std::string name;
    std::string username;
    Type type = Type::ALL;

    // Port of: SearchPostListURL.build(location, query)
    static SearchPostListURL build(const std::string& location,
                                   const std::string& query);

    // Port of: url.generateJsonUri()
    std::string generateJsonUri() const { return ""; }
};

// Port of: SearchPostListURL.build(String location, String query)
SearchPostListURL SearchPostListURL::build(const std::string& location,
                                           const std::string& query) {
    SearchPostListURL url;
    url.query = query;
    // Parse location string to determine type, subreddit, name, username
    if (location.find("/m/") != std::string::npos) {
        url.type = SearchPostListURL::Type::MULTI;
        // Parse /u/username/m/name or /me/m/name
    } else if (!location.empty() && location != "all") {
        url.type = SearchPostListURL::Type::SUB_OR_SUB_COMBO;
        url.subreddit = location;
    } else {
        url.type = SearchPostListURL::Type::ALL;
    }
    return url;
}

// StringUtils.isEmpty stub
static bool stringIsEmpty(const std::string& s) {
    return s.empty();
}

// ============================================================================
// Constructor (Java lines 42-83)
// ============================================================================

SearchListingHeader::SearchListingHeader(Context* parentActivity,
                                         const SearchPostListURL& url)
    : m_url(url)
{
    // Port of: super(parentActivity);
    // Port of: mUrl = url;

    // Port of: final LayoutInflater layoutInflater = (LayoutInflater)
    //     parentActivity.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    // Port of: layoutInflater.inflate(R.layout.search_listing_header, this, true);

    // Port of: mQuery = findViewById(R.id.search_listing_header_query_editText);
    // Port of: mQuery.setText(url.query);
    m_query = url.query;

    // Port of: mQuery.setImeOptions(EditorInfo.IME_ACTION_NEXT);

    // Port of: mLocation = findViewById(R.id.search_listing_header_sub_editText);

    // Port of: if(url.type == SearchPostListURL.Type.SUB_OR_SUB_COMBO
    //     && url.subreddit != null) { mLocation.setText(url.subreddit); }
    // Port of: else if(url.type == SearchPostListURL.Type.MULTI && url.name != null) {
    //     if(url.username != null) mLocation.setText("/u/" + url.username + "/m/" + url.name);
    //     else mLocation.setText("/me/m/" + url.name); }
    // Port of: else { mLocation.setText("all"); }
    if (url.type == SearchPostListURL::Type::SUB_OR_SUB_COMBO && !url.subreddit.empty()) {
        m_location = url.subreddit;
    } else if (url.type == SearchPostListURL::Type::MULTI && !url.name.empty()) {
        if (!url.username.empty()) {
            m_location = "/u/" + url.username + "/m/" + url.name;
        } else {
            m_location = "/me/m/" + url.name;
        }
    } else {
        m_location = "all";
    }

    // Port of: final TextView.OnEditorActionListener onEnter = (v, actionId, event) -> {
    //     performSearch(parentActivity, mLocation, mQuery); return true; };
    // Port of: mLocation.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
    // Port of: mLocation.setOnEditorActionListener(onEnter);

    // Port of: mSearchButton = findViewById(R.id.search_listing_header_search);
    // Port of: mSearchButton.setOnClickListener(v -> performSearch(parentActivity, mLocation, mQuery));
}

SearchListingHeader::~SearchListingHeader() = default;

// ============================================================================
// performSearch — port of static method (Java lines 85-104)
// ============================================================================

void SearchListingHeader::performSearch(Context* parentActivity,
                                       const std::string& location,
                                       const std::string& query) {
    // Port of: String location = mLocation.getText().toString().trim();
    // We receive a trimmed copy; trim on our side:
    std::string loc = location;
    // Remove trailing whitespace
    while (!loc.empty() && (loc.back() == ' ' || loc.back() == '\t')) {
        loc.pop_back();
    }
    // Remove leading whitespace
    while (!loc.empty() && (loc.front() == ' ' || loc.front() == '\t')) {
        loc.erase(loc.begin());
    }

    // Port of: if(StringUtils.isEmpty(location)) { location = null; }
    if (stringIsEmpty(loc)) {
        loc.clear();
    }

    // Port of: final SearchPostListURL url = SearchPostListURL.build(
    //     location, mQuery.getText().toString().trim());
    std::string trimmedQuery = query;
    while (!trimmedQuery.empty() && (trimmedQuery.back() == ' ' || trimmedQuery.back() == '\t')) {
        trimmedQuery.pop_back();
    }
    while (!trimmedQuery.empty() && (trimmedQuery.front() == ' ' || trimmedQuery.front() == '\t')) {
        trimmedQuery.erase(trimmedQuery.begin());
    }

    const SearchPostListURL url = SearchPostListURL::build(loc, trimmedQuery);

    // Port of: final Intent intent = new Intent(parentActivity, PostListingActivity.class);
    // Port of: intent.setData(url.generateJsonUri());
    // Port of: parentActivity.startActivity(intent);
    // Port of: parentActivity.finish();
    (void)parentActivity;
    (void)url;
}

} // namespace PinkReader
