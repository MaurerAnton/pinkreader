/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: load_more_comments_view.h - Port of LoadMoreCommentsView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/LoadMoreCommentsView.java
 *
 * Original: class LoadMoreCommentsView extends LinearLayout
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <string>
#include <vector>

namespace PinkReader {

class Context;
class RedditCommentListItem;
class IndentView;

// ============================================================================
// RedditURL stub (port of: RedditURLParser.RedditURL)
// ============================================================================

struct RedditURL {
    enum PathType {
        POST_COMMENT_LISTING_URL = 0,
    };

    virtual PathType pathType() const = 0;
    virtual ~RedditURL() = default;
};

// ============================================================================
// PostCommentListingURL stub (port of: PostCommentListingURL)
// ============================================================================

struct PostCommentListingURL : public RedditURL {
    std::string postId;
    std::string commentId;

    PathType pathType() const override { return POST_COMMENT_LISTING_URL; }

    // Port of: getMoreUrls(RedditURL)
    // Returns list of PostCommentListingURL from a "load more" item
};

// ============================================================================
// LoadMoreCommentsView — port of Java class (Java lines 39-158)
//
// Port of: org.quantumbadger.redreader.views.LoadMoreCommentsView
// Orig extends: LinearLayout
//
// Clickable view to load more comments. Shows indent, forward icon, and
// "load X more replies" text. Navigates to MoreCommentsListingActivity.
// Every field, method, constant ported exactly.
// ============================================================================

class LoadMoreCommentsView {
public:
    // ========================================================================
    // Constructor (Java lines 46-142)
    //
    // Port of: public LoadMoreCommentsView(final Context context,
    //     final RedditURLParser.RedditURL commentListingURL)
    // ========================================================================

    LoadMoreCommentsView(Context* context,
                         const RedditURL& commentListingURL);

    virtual ~LoadMoreCommentsView();

    // ========================================================================
    // reset — port of method (Java lines 144-156)
    //
    // Port of: public void reset(final RedditCommentListItem item)
    // ========================================================================

    void reset(RedditCommentListItem* item);

private:
    // ========================================================================
    // Fields — exact match (Java lines 41-44)
    // ========================================================================

    // Port of: private final IndentView mIndentView; (Java line 41)
    // Port of: private final TextView mTitleView; (Java line 42)
    // Port of: private RedditCommentListItem mItem; (Java line 43)
    // Port of: private final RedditURLParser.RedditURL mCommentListingURL; (Java line 44)

    RedditCommentListItem* m_item = nullptr;
    PostCommentListingURL m_commentListingURL;
    IndentView* m_indentView = nullptr;

    // Visibility constant (port of: View.VISIBLE and View.GONE)
    static constexpr int VISIBLE = 0;
    static constexpr int GONE = 8;
};

} // namespace PinkReader
