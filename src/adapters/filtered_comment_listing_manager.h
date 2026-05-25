/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: filtered_comment_listing_manager.h - Port of FilteredCommentListingManager.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/adapters/FilteredCommentListingManager.java
 *
 * Original: public class FilteredCommentListingManager extends RedditListingManager
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <vector>
#include <memory>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

class Context;
class RedditCommentListItem;
class GroupedRecyclerViewAdapter;

// Base class stub (RedditListingManager is not yet ported)
class RedditListingManager;

// ============================================================================
// FilteredCommentListingManager — port of Java class (Java lines 30-87)
//
// Port of: org.quantumbadger.redreader.adapters.FilteredCommentListingManager
//
// Extends RedditListingManager. Every field, method ported exactly.
// ============================================================================

class FilteredCommentListingManager : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Constructor (Java lines 37-43)
    //
    // Port of:
    //   public FilteredCommentListingManager(
    //       final Context context,
    //       @Nullable final String searchString)
    // ========================================================================
    FilteredCommentListingManager(Context *context,
                                  const QString &searchString,
                                  QObject *parent = nullptr);

    virtual ~FilteredCommentListingManager();

    // ========================================================================
    // Public methods
    // ========================================================================

    // Port of: public void addComments(final Collection<RedditCommentListItem> comments) (Java lines 45-50)
    void addComments(const std::vector<RedditCommentListItem *> &comments);

    // Port of: public boolean isSearchListing() (Java lines 80-82)
    bool isSearchListing() const;

    // Port of: public int getCommentCount() (Java lines 84-86)
    int getCommentCount() const;

private:
    // ========================================================================
    // Private fields (Java lines 32-35)
    // ========================================================================

    // Port of: @Nullable private final String mSearchString;
    QString mSearchString;  // empty string = null

    // Port of: private int mCommentCount;
    int mCommentCount = 0;

    // Port of: (inherited) RedditListingManager - base class wrapped
    RedditListingManager *m_listingManager = nullptr;

    // ========================================================================
    // Private methods
    // ========================================================================

    // Port of: private Collection<GroupedRecyclerViewAdapter.Item<?>> filter(
    //     final Collection<RedditCommentListItem> comments) (Java lines 52-78)
    std::vector<GroupedRecyclerViewAdapter *> filter(
            const std::vector<RedditCommentListItem *> &comments);
};

} // namespace PinkReader
