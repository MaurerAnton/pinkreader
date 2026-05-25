// Origin: org.quantumbadger.redreader.views.RedditPostHeaderView.java
// Ported from RedReader (GPLv3)
#pragma once

#include <functional>
#include <memory>
#include <string>

namespace PinkReader {

// Forward declarations
class RedditPreparedPost;
class RedditChangeDataManager;

class RedditPostHeaderView {
public:
    // --- Constructor ---
    RedditPostHeaderView(
            void* activity,                // BaseActivity*
            RedditPreparedPost* post);

    // --- Lifecycle ---
    void onAttachedToWindow();
    void onDetachedFromWindow();

private:
    // --- Fields ---
    void* m_subtitle = nullptr;                          // TextView*
    std::function<void()> m_changeListenerAddTask;
    std::function<void()> m_changeListenerRemoveTask;

    // Note: In the original, this class extends LinearLayout and inflates
    // post_header_toolbar layout, sets up vote/reply/share/more buttons with
    // RedditPostActions.Action handlers, monitors RedditChangeDataManager for
    // upvote/downvote state changes via a listener callback.
    //
    // The changeListener lambda:
    // - Updates subtitle text and content description
    // - Checks isUpvoted/isDownvoted from changeDataManager
    // - Shows/hides add_vote and remove_vote buttons accordingly
    //
    // mChangeListenerAddTask: adds listener, fires initial callback
    // mChangeListenerRemoveTask: removes listener
    //
    // Called in onAttachedToWindow/onDetachedFromWindow lifecycle
};

} // namespace PinkReader
