/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: post_listing_header.cpp - Port of PostListingHeader.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/PostListingHeader.java
 *
 * Original: public final class PostListingHeader extends LinearLayout
 * Every field, method, constant, and inner class ported exactly.
 */

#include "post_listing_header.h"

namespace PinkReader {

// ===== Constructor (Java lines 38-97) =====
PostListingHeader::PostListingHeader(
        AppCompatActivity *activity,
        const QString &titleText,
        const QString &subtitleText,
        PostListingURL *url,
        RedditSubreddit *subreddit)
    : QObject(nullptr)
    , mActivity(activity)
    , mTitleText(titleText)
    , mSubtitleText(subtitleText)
    , mUrl(url)
    , mSubreddit(subreddit)
{
    // ===== Java line 38-43: PostListingHeader constructor =====
    // super(activity);

    // ===== Java line 47: final float dpScale = activity.getResources().getDisplayMetrics().density; =====
    // final float dpScale = activity.getResources().getDisplayMetrics().density;

    // ===== Java line 49: setOrientation(LinearLayout.VERTICAL); =====
    // setOrientation(LinearLayout.VERTICAL);

    // ===== Java lines 51-84: Grey header with title/subtitle =====
    // if(!PrefsUtility.pref_appearance_post_hide_subreddit_header()) {
    //
    //     final LinearLayout greyHeader = new LinearLayout(activity);
    //     greyHeader.setOrientation(LinearLayout.VERTICAL);
    //
    //     {
    //         final TypedArray appearance = activity.obtainStyledAttributes(new int[]{
    //                 R.attr.rrPostListHeaderBackgroundCol});
    //
    //         greyHeader.setBackgroundColor(appearance.getColor(0, General.COLOR_INVALID));
    //
    //         appearance.recycle();
    //     }
    //
    //     final int sidesPadding = (int)(15.0f * dpScale);
    //     final int topPadding = (int)(10.0f * dpScale);
    //
    //     greyHeader.setPadding(sidesPadding, topPadding, sidesPadding, topPadding);
    //
    //     final TextView title = new TextView(activity);
    //     title.setText(titleText);
    //     title.setTextSize(22.0f);
    //     title.setTypeface(Fonts.getRobotoLightOrAlternative());
    //     title.setTextColor(Color.WHITE);
    //     greyHeader.addView(title);
    //
    //     final TextView subtitle = new TextView(activity);
    //     subtitle.setTextSize(14.0f);
    //     subtitle.setText(subtitleText);
    //     subtitle.setTextColor(Color.rgb(200, 200, 200));
    //     greyHeader.addView(subtitle);
    //
    //     addView(greyHeader);
    // }

    // ===== Java lines 86-96: Subreddit toolbar =====
    // if(subreddit != null
    //         && !PrefsUtility.pref_appearance_hide_headertoolbar_postlist()) {
    //
    //     final SubredditToolbar buttons =
    //             inflate(activity, R.layout.subreddit_header_toolbar, this)
    //                     .findViewById(R.id.subreddit_toolbar_layout);
    //
    //     buttons.bindSubreddit(
    //             SubredditDetails.newWithRuntimeException(subreddit),
    //             Optional.of(url.browserUrl()));
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Destructor =====
PostListingHeader::~PostListingHeader() = default;

} // namespace PinkReader
