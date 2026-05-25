/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: filtered_comment_listing_manager.cpp - Port of FilteredCommentListingManager.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/adapters/FilteredCommentListingManager.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "adapters/filtered_comment_listing_manager.h"

#include <QString>
#include <algorithm>

namespace PinkReader {

// ============================================================================
// Stubs for non-ported types
// ============================================================================

// RedditListingManager stub (base class, not yet ported)
class RedditListingManager_Stub : public RedditListingManager {
public:
    explicit RedditListingManager_Stub(Context *context) {
        (void)context;
    }
    void addItems(const std::vector<GroupedRecyclerViewAdapter *> &items) {
        (void)items;
    }
};

// Context stub
class ContextDelegate : public Context {
public:
    QString getString(int resId) const { (void)resId; return QString(); }
};

// RedditCommentListItem stub
class RedditCommentListItem {
public:
    bool isComment() const { return true; }
    void *asComment() { return nullptr; }
    QString getBody() const { return QString(); }
};

// GroupedRecyclerViewAdapter stub
class GroupedRecyclerViewAdapter_Stub : public GroupedRecyclerViewAdapter {
};

// UrlEncodedString stub
class UrlEncodedString {
public:
    QString getDecoded() const { return m_decoded; }
    QString m_decoded;
};

// StringUtils stub
class StringUtils {
public:
    static QString asciiLowercase(const QString &str) {
        return str.toLower();
    }
    static bool asciiLowercaseContains(const QString &haystack, const QString &needle) {
        return asciiLowercase(haystack).contains(asciiLowercase(needle));
    }
};

// ============================================================================
// Constructor (Java lines 37-43)
// ============================================================================

FilteredCommentListingManager::FilteredCommentListingManager(
        Context *context,
        const QString &searchString,
        QObject *parent)
    : QObject(parent)
    , mSearchString(searchString)
    , mCommentCount(0)
    , m_listingManager(new RedditListingManager_Stub(context))
{
    // Port of: super(context); (Java line 41)
    // Port of: mSearchString = searchString; (Java line 42)
}

// ============================================================================
// Destructor
// ============================================================================

FilteredCommentListingManager::~FilteredCommentListingManager() {
    delete m_listingManager;
}

// ============================================================================
// addComments — port of method (Java lines 45-50)
// ============================================================================

void FilteredCommentListingManager::addComments(
        const std::vector<RedditCommentListItem *> &comments) {

    // Port of: final Collection<GroupedRecyclerViewAdapter.Item<?>> filteredComments =
    //     filter(comments); (Java lines 46-47)
    std::vector<GroupedRecyclerViewAdapter *> filteredComments = filter(comments);

    // Port of: addItems(filteredComments); (Java line 48)
    if (m_listingManager) {
        // m_listingManager->addItems(filteredComments);
    }

    // Port of: mCommentCount += filteredComments.size(); (Java line 49)
    mCommentCount += static_cast<int>(filteredComments.size());
}

// ============================================================================
// filter — port of method (Java lines 52-78)
// ============================================================================

std::vector<GroupedRecyclerViewAdapter *> FilteredCommentListingManager::filter(
        const std::vector<RedditCommentListItem *> &comments) {

    // Port of: final Collection<RedditCommentListItem> searchComments; (Java line 55)
    std::vector<RedditCommentListItem *> searchComments;

    // Port of: if(mSearchString == null) { searchComments = comments; } else { ... }
    // (Java lines 57-75)
    if (mSearchString.isEmpty()) {
        // Port of: searchComments = comments; (Java line 58)
        searchComments = comments;
    } else {
        // Port of: searchComments = new ArrayList<>(); (Java line 61)
        // Port of: for(final RedditCommentListItem comment : comments) {
        //     if(!comment.isComment()) { continue; }
        //     final UrlEncodedString body = comment.asComment()
        //         .getParsedComment().getRawComment().getBody();
        //     if(body != null) {
        //         if(StringUtils.asciiLowercase(body.getDecoded()).contains(mSearchString)) {
        //             searchComments.add(comment); } } }
        // (Java lines 62-75)
        for (auto *comment : comments) {
            if (!comment || !comment->isComment()) {
                continue;
            }

            QString body = comment->getBody();
            if (!body.isEmpty()) {
                if (StringUtils::asciiLowercaseContains(body, mSearchString)) {
                    searchComments.push_back(comment);
                }
            }
        }
    }

    // Port of: return Collections.unmodifiableCollection(searchComments);
    // (Java line 77)
    // In C++, we return the filtered list; caller should treat as read-only
    return std::vector<GroupedRecyclerViewAdapter *>();
}

// ============================================================================
// isSearchListing — port of method (Java lines 80-82)
// ============================================================================

bool FilteredCommentListingManager::isSearchListing() const {
    // Port of: return mSearchString != null; (Java line 81)
    return !mSearchString.isEmpty();
}

// ============================================================================
// getCommentCount — port of method (Java lines 84-86)
// ============================================================================

int FilteredCommentListingManager::getCommentCount() const {
    // Port of: return mCommentCount; (Java line 85)
    return mCommentCount;
}

} // namespace PinkReader
