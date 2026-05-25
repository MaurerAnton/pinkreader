// Origin: org.quantumbadger.redreader.views.RedditPostHeaderView.java
// Ported from RedReader (GPLv3)

#include "views/reddit_post_header_view_v2.h"

namespace PinkReader {

RedditPostHeaderView::RedditPostHeaderView(
        void* activity,
        RedditPreparedPost* post) {
    // Original constructor (253 lines) does:
    //
    // 1. super(activity); setOrientation(VERTICAL);
    // 2. Create greyHeader LinearLayout, set orientation, padding
    // 3. Setup accessibility actions via RedditPostActions.INSTANCE.setupAccessibilityActions()
    // 4. Create title TextView:
    //    - Font scale from PrefsUtility.appearance_fontscale_post_header_titles()
    //    - RobotoLight typeface, text from post.src.getTitle()
    //    - White color, set content description
    // 5. Create subtitle TextView:
    //    - Font scale from PrefsUtility.appearance_fontscale_post_header_subtitles()
    //    - Text from post.buildSubtitle()
    //    - Light gray color (200, 200, 200)
    // 6. Set header background from styled attribute R.attr.rrPostListHeaderBackgroundCol
    // 7. greyHeader onClick: LinkHandler.onLinkClicked if not self post
    // 8. greyHeader onLongClick: RedditPostActions.INSTANCE.showActionMenu()
    // 9. Add greyHeader view
    //
    // 10. If user is not anonymous:
    //     - If toolbar not hidden: inflate post_header_toolbar, find vote/reply/share/more buttons
    //     - Set click listeners for each button calling post.performAction()
    //     - Create changeListener: updates subtitle + vote button visibility based on upvote/downvote state
    //     - Store add/remove tasks for lifecycle
    //     - Otherwise: simple changeListener that just updates subtitle
    //
    (void)activity;
    (void)post;
}

void RedditPostHeaderView::onAttachedToWindow() {
    // super.onAttachedToWindow();
    if (m_changeListenerAddTask) {
        m_changeListenerAddTask();
    }
}

void RedditPostHeaderView::onDetachedFromWindow() {
    // super.onDetachedFromWindow();
    if (m_changeListenerRemoveTask) {
        m_changeListenerRemoveTask();
    }
}

} // namespace PinkReader
