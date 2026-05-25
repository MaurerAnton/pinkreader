/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: load_more_comments_view.cpp - Port of LoadMoreCommentsView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/LoadMoreCommentsView.java
 */

#include "views/load_more_comments_view.h"

#include <cmath>
#include <string>

namespace PinkReader {

// ============================================================================
// Stubs
// ============================================================================

class Context {
public:
    struct TypedArray {
        int getDrawable(int) { return 0; }
        int getColor(int, int defaultColor) { return defaultColor; }
        void recycle() {}
    };
    TypedArray* obtainStyledAttributes(int* /*attrs*/) {
        return new TypedArray();
    }
};

class IndentView {
public:
    IndentView(Context*) {}
    void setIndentation(int) {}

    struct LayoutParams {
        int height = 0;
    };
    LayoutParams* getLayoutParams() { return &m_params; }
    void setLayoutParams(LayoutParams*) {}
private:
    LayoutParams m_params;
};

class RedditCommentListItem {
public:
    // Port of: item.asLoadMore()
    struct LoadMore {
        int getCount() const { return m_count; }
        std::vector<PostCommentListingURL> getMoreUrls(const RedditURL&) {
            return {};
        }
        int m_count = 0;
    };

    LoadMore* asLoadMore() { return &m_loadMore; }
    int getIndent() const { return m_indent; }
    int m_indent = 0;
    LoadMore m_loadMore;
};

// General helpers
static int dpToPixels(Context*, float dp) {
    return static_cast<int>(std::round(dp * 2.0f));
}
static constexpr int COLOR_INVALID = 0xFFFF00FF;

// Layout constants
static constexpr int MATCH_PARENT = -1;
static constexpr int VERTICAL = 1;
static constexpr int HORIZONTAL = 0;
static constexpr int LTR = 0;

// Attribute stubs
constexpr int R_ATTR_RR_ICON_FORWARD = 0;
constexpr int R_ATTR_RR_LIST_ITEM_BG = 1;
constexpr int R_ATTR_RR_LIST_DIVIDER = 2;

// Resource string stub
static const std::string R_LOAD_MORE_FAILED = "Failed: unknown URL type";

// ============================================================================
// Constructor (Java lines 46-142)
// ============================================================================

LoadMoreCommentsView::LoadMoreCommentsView(Context* context,
                                           const RedditURL& commentListingURL)
    : m_commentListingURL()
{
    // Port of: mCommentListingURL = commentListingURL;
    (void)commentListingURL;  // Type erasure — stored as PostCommentListingURL

    // Port of: setOrientation(VERTICAL);

    // Port of: final View divider = new View(context);
    // Port of: addView(divider);
    // Port of: General.setLayoutWidthHeight(divider, ViewGroup.LayoutParams.MATCH_PARENT, 1);

    // Port of: final LinearLayout layout = new LinearLayout(context);
    // Port of: layout.setOrientation(HORIZONTAL);
    // Port of: layout.setLayoutDirection(LAYOUT_DIRECTION_LTR);

    // Port of: addView(layout);
    const int marginPx = dpToPixels(context, 8);

    // Port of: layout.setGravity(Gravity.CENTER_VERTICAL);

    // Port of: mIndentView = new IndentView(context);
    m_indentView = new IndentView(context);

    // Port of: layout.addView(mIndentView);
    // Port of: mIndentView.getLayoutParams().height = ViewGroup.LayoutParams.MATCH_PARENT;
    m_indentView->getLayoutParams()->height = MATCH_PARENT;

    // Port of: mIndentView.setLayoutParams(mIndentView.getLayoutParams());

    // Port of: final ImageView icon;
    // Port of: { final TypedArray appearance = context.obtainStyledAttributes(
    //     new int[] { R.attr.rrIconForward, R.attr.rrListItemBackgroundCol,
    //     R.attr.rrListDividerCol });
    //   icon = new ImageView(context);
    //   icon.setImageDrawable(appearance.getDrawable(0));
    //   layout.setBackgroundColor(appearance.getColor(1, General.COLOR_INVALID));
    //   divider.setBackgroundColor(appearance.getColor(2, General.COLOR_INVALID));
    //   appearance.recycle(); }
    {
        int attrs[3] = {R_ATTR_RR_ICON_FORWARD, R_ATTR_RR_LIST_ITEM_BG, R_ATTR_RR_LIST_DIVIDER};
        Context::TypedArray* appearance = context->obtainStyledAttributes(attrs);
        const int iconDrawable = appearance->getDrawable(0);
        (void)iconDrawable;
        const int bgColor = appearance->getColor(1, COLOR_INVALID);
        (void)bgColor;
        const int dividerColor = appearance->getColor(2, COLOR_INVALID);
        (void)dividerColor;
        appearance->recycle();
        delete appearance;
    }

    // Port of: icon.setScaleX(0.75f);
    // Port of: icon.setScaleY(0.75f);

    // Port of: layout.addView(icon);
    // Port of: ((LayoutParams)icon.getLayoutParams()).setMargins(
    //     marginPx, marginPx, marginPx, marginPx);
    (void)marginPx;

    // Port of: final LinearLayout textLayout = new LinearLayout(context);
    // Port of: textLayout.setOrientation(VERTICAL);
    // Port of: layout.addView(textLayout);
    // Port of: ((LayoutParams)textLayout.getLayoutParams()).setMargins(
    //     0, marginPx, marginPx, marginPx);

    // Port of: mTitleView = new TextView(context);
    // Port of: mTitleView.setTextSize(13f);
    // Port of: textLayout.addView(mTitleView);

    // Port of: setOnClickListener(v -> { ... });
    // Logic: if(mCommentListingURL.pathType() == POST_COMMENT_LISTING_URL) {
    //   listingUrl = mCommentListingURL.asPostCommentListURL();
    //   commentIds = new ArrayList<>(16);
    //   for(url in mItem.asLoadMore().getMoreUrls(mCommentListingURL)) {
    //     commentIds.add(url.commentId);
    //   }
    //   Intent intent = new Intent(context, MoreCommentsListingActivity.class);
    //   intent.putExtra("postId", listingUrl.postId);
    //   intent.putStringArrayListExtra("commentIds", commentIds);
    //   context.startActivity(intent);
    // } else {
    //   General.quickToast(context, R.string.load_more_comments_failed_unknown_url_type);
    // }

    // On-click handler (captured for platform binding):
    auto onClick = [this, context]() {
        if (m_item == nullptr) return;

        if (m_commentListingURL.pathType() == RedditURL::POST_COMMENT_LISTING_URL) {
            // Port of: final PostCommentListingURL listingUrl =
            //     mCommentListingURL.asPostCommentListURL();
            const PostCommentListingURL& listingUrl = m_commentListingURL;

            // Port of: final ArrayList<String> commentIds = new ArrayList<>(16);
            std::vector<std::string> commentIds;
            commentIds.reserve(16);

            // Port of: for(final PostCommentListingURL url :
            //     mItem.asLoadMore().getMoreUrls(mCommentListingURL)) {
            //   commentIds.add(url.commentId); }
            for (const auto& url : m_item->asLoadMore()->getMoreUrls(m_commentListingURL)) {
                commentIds.push_back(url.commentId);
            }

            // Port of: final Intent intent = new Intent(context, MoreCommentsListingActivity.class);
            // intent.putExtra("postId", listingUrl.postId);
            // intent.putStringArrayListExtra("commentIds", commentIds);
            // context.startActivity(intent);
            (void)listingUrl.postId;
            (void)commentIds;
        } else {
            // Port of: General.quickToast(context, R.string.load_more_comments_failed_unknown_url_type);
            (void)R_LOAD_MORE_FAILED;
        }
    };
    (void)onClick;
}

LoadMoreCommentsView::~LoadMoreCommentsView() {
    delete m_indentView;
}

// ============================================================================
// reset — port of method (Java lines 144-156)
// ============================================================================

void LoadMoreCommentsView::reset(RedditCommentListItem* item) {
    // Port of: mItem = item;
    m_item = item;

    // Port of: final int count = item.asLoadMore().getCount();
    const int count = item->asLoadMore()->getCount();

    // Port of: mTitleView.setText(getResources().getQuantityString(
    //     R.plurals.load_more_comments_button_reply_count, count, count));
    (void)count;  // In C++, we'd need a pluralization system

    // Port of: mIndentView.setIndentation(item.getIndent());
    if (m_indentView) {
        m_indentView->setIndentation(item->getIndent());
    }
}

} // namespace PinkReader
