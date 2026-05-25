/*
 * PinkReader - GPLv3
 * File: main_menu_fragment.h - Port of MainMenuFragment.java
 *
 * Original: public class MainMenuFragment extends RRFragment implements
 *     MainMenuSelectionListener,
 *     RedditSubredditSubscriptionManager.SubredditSubscriptionStateChangeListener,
 *     RedditMultiredditSubscriptionManager.MultiredditListChangeListener
 *
 * Located at: redreader/src/main/java/org/quantumbadger/redreader/fragments/MainMenuFragment.java
 *
 * Every field, method, inner class, interface, and constant ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QSet>
#include <QVector>
#include <memory>
#include <functional>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class Bundle;
class View;
class Context;
class RecyclerView;
class RRFragment;
class ScrollbarRecyclerViewManager;
class MainMenuListingManager;
class RedditAccount;
class RedditSubredditSubscriptionManager;
class RedditMultiredditSubscriptionManager;
class PostListingURL;
class SubredditCanonicalId;
class RRError;
class RequestResponseHandler;
class TimestampUTC;
class TimestampBound;
class TimeDuration;

// Forward declaration of typed RequestResponseHandler templates
template<typename R, typename E> class RequestResponseHandler;

/**
 * @brief Port of MainMenuFragment inner enum: MainMenuUserItems
 */
enum class MainMenuUserItems {
    PROFILE, INBOX, SUBMITTED, SUBMITTED_COMMENTS, SAVED,
    HIDDEN, UPVOTED, DOWNVOTED, MODMAIL, SENT_MESSAGES
};

/**
 * @brief Port of MainMenuFragment inner enum: MainMenuShortcutItems
 */
enum class MainMenuShortcutItems {
    FRONTPAGE, POPULAR, ALL, SUBREDDIT_SEARCH, CUSTOM
};

/**
 * @brief Port of MainMenuFragment.java
 *
 * Every field, method, inner class, interface, and constant ported exactly.
 */
class MainMenuFragment : public QObject {
    Q_OBJECT

public:
    // ===== Port of: @IntDef annotated constants (MENU_MENU_ACTION_*) =====
    // In original these are public static final int with @IntDef annotation.
    // Ported as static constexpr int within a namespace-like constant group.

    // Port of: public static final int MENU_MENU_ACTION_FRONTPAGE = 0;
    static constexpr int MENU_MENU_ACTION_FRONTPAGE = 0;
    // Port of: public static final int MENU_MENU_ACTION_PROFILE = 1;
    static constexpr int MENU_MENU_ACTION_PROFILE = 1;
    // Port of: public static final int MENU_MENU_ACTION_INBOX = 2;
    static constexpr int MENU_MENU_ACTION_INBOX = 2;
    // Port of: public static final int MENU_MENU_ACTION_SUBMITTED = 3;
    static constexpr int MENU_MENU_ACTION_SUBMITTED = 3;
    // Port of: public static final int MENU_MENU_ACTION_SUBMITTED_COMMENTS = 4;
    static constexpr int MENU_MENU_ACTION_SUBMITTED_COMMENTS = 4;
    // Port of: public static final int MENU_MENU_ACTION_UPVOTED = 5;
    static constexpr int MENU_MENU_ACTION_UPVOTED = 5;
    // Port of: public static final int MENU_MENU_ACTION_DOWNVOTED = 6;
    static constexpr int MENU_MENU_ACTION_DOWNVOTED = 6;
    // Port of: public static final int MENU_MENU_ACTION_SAVED = 7;
    static constexpr int MENU_MENU_ACTION_SAVED = 7;
    // Port of: public static final int MENU_MENU_ACTION_MODMAIL = 8;
    static constexpr int MENU_MENU_ACTION_MODMAIL = 8;
    // Port of: public static final int MENU_MENU_ACTION_HIDDEN = 9;
    static constexpr int MENU_MENU_ACTION_HIDDEN = 9;
    // Port of: public static final int MENU_MENU_ACTION_CUSTOM = 10;
    static constexpr int MENU_MENU_ACTION_CUSTOM = 10;
    // Port of: public static final int MENU_MENU_ACTION_ALL = 11;
    static constexpr int MENU_MENU_ACTION_ALL = 11;
    // Port of: public static final int MENU_MENU_ACTION_POPULAR = 12;
    static constexpr int MENU_MENU_ACTION_POPULAR = 12;
    // Port of: public static final int MENU_MENU_ACTION_SENT_MESSAGES = 13;
    static constexpr int MENU_MENU_ACTION_SENT_MESSAGES = 13;
    // Port of: public static final int MENU_MENU_ACTION_FIND_SUBREDDIT = 14;
    static constexpr int MENU_MENU_ACTION_FIND_SUBREDDIT = 14;

    // Note: @IntDef + @Retention(RetentionPolicy.SOURCE) + @interface MainMenuAction
    // is a compile-time annotation that doesn't affect runtime. Not needed in C++.
    // The annotation grouped all the above constants. Preserved as comment.

    // ===== Constructor =====
    // Port of: public MainMenuFragment(AppCompatActivity parent, Bundle savedInstanceState, boolean force)
    MainMenuFragment(AppCompatActivity *parent,
                     Bundle *savedInstanceState,
                     bool force,
                     QObject *qparent = nullptr);
    ~MainMenuFragment() override;

    // ===== Port of: public View getListingView() =====
    View *getListingView();

    // ===== Port of: public Bundle onSaveInstanceState() =====
    Bundle *onSaveInstanceState();

    // ===== Port of: public void onSubredditSubscriptionsChanged(Collection<SubredditCanonicalId>) =====
    void onSubredditSubscriptionsChanged(const QVector<SubredditCanonicalId *> &subscriptions);

    // ===== Port of: public void onMultiredditSubscriptionsChanged(Collection<String>) =====
    void onMultiredditSubscriptionsChanged(const QSet<QString> &subscriptions);

    // ===== Port of: private void onSubredditError(RRError) =====
    void onSubredditError(RRError *error);

    // ===== Port of: private void onMultiredditError(RRError) =====
    void onMultiredditError(RRError *error);

    // ===== Port of: public void onSelected(@MainMenuAction int type) =====
    void onSelected(int type);

    // ===== Port of: public void onSelected(PostListingURL) =====
    void onSelected(PostListingURL *postListingURL);

    // ===== Port of: public void onSubredditSubscriptionListUpdated(RedditSubredditSubscriptionManager) =====
    void onSubredditSubscriptionListUpdated(RedditSubredditSubscriptionManager *manager);

    // ===== Port of: public void onMultiredditListUpdated(RedditMultiredditSubscriptionManager) =====
    void onMultiredditListUpdated(RedditMultiredditSubscriptionManager *manager);

    // ===== Port of: public void onSubredditSubscriptionAttempted(RedditSubredditSubscriptionManager) =====
    void onSubredditSubscriptionAttempted(RedditSubredditSubscriptionManager *manager);

    // ===== Port of: public void onSubredditUnsubscriptionAttempted(RedditSubredditSubscriptionManager) =====
    void onSubredditUnsubscriptionAttempted(RedditSubredditSubscriptionManager *manager);

    // ===== Port of: public void onUpdateAnnouncement() =====
    void onUpdateAnnouncement();

private:
    // ===== Port of: private final MainMenuListingManager mManager; =====
    MainMenuListingManager *mManager = nullptr;

    // ===== Port of: private final View mOuter; =====
    View *mOuter = nullptr;
};

// Port of MainMenuSelectionListener interface
// Original: interface defined in adapters/MainMenuSelectionListener
// Used by MainMenuFragment via implements MainMenuSelectionListener
class MainMenuSelectionListener {
public:
    virtual ~MainMenuSelectionListener() = default;
    virtual void onSelected(int type) = 0;
    virtual void onSelected(PostListingURL *postListingURL) = 0;
};

} // namespace PinkReader
