/*
 * PinkReader - GPLv3
 * File: post_listing_controller.h - Port of PostListingController.java
 *
 * Exact port of:
 *   redreader/src/main/java/org/quantumbadger/redreader/listingcontrollers/PostListingController.java
 *
 * Every field, method, and logic branch ported exactly.
 *
 * Original: public class PostListingController
 * 220 lines of Java ported.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class Bundle;
class Context;
class InvalidSubredditNameException;
class PostListingFragment;
class PostListingURL;
class PostSort;
class SubredditCanonicalId;
class RedditURLParser;

/**
 * @brief Port of org.quantumbadger.redreader.listingcontrollers.PostListingController
 *
 * Controls post listing navigation and sort options for subreddits,
 * multireddits, user post listings, and search results.
 * Every field, method, and logic branch ported exactly.
 */
class PostListingController : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Constructor — Java lines 50-78
    // ========================================================================

    // Java lines 50-78:
    // public PostListingController(PostListingURL url, final Context context)
    PostListingController(PostListingURL *url, Context *context, QObject *parent = nullptr);

    ~PostListingController() override = default;

    // ========================================================================
    // Public methods — Java lines 42-219
    // ========================================================================

    // Java lines 42-44: public void setSession(final UUID session)
    void setSession(const QUuid &session);

    // Java lines 46-48: public UUID getSession()
    QUuid getSession() const;

    // Java lines 80-87: public boolean isSortable()
    bool isSortable() const;

    // Java lines 89-93: public boolean isFrontPage()
    bool isFrontPage() const;

    // Java lines 95-111: public final void setSort(final PostSort order)
    void setSort(PostSort *order);

    // Java lines 113-132: public final PostSort getSort()
    PostSort *getSort() const;

    // Java lines 134-136: public Uri getUri()
    QString getUri() const;

    // Java lines 138-151: public final PostListingFragment get(...)
    PostListingFragment *get(
            AppCompatActivity *parent,
            bool force,
            Bundle *savedInstanceState);

    // Java lines 153-157: public final boolean isSubreddit()
    bool isSubreddit() const;

    // Java lines 159-163: public final boolean isSubredditCombination()
    bool isSubredditCombination() const;

    // Java lines 165-167: public final boolean isMultireddit()
    bool isMultireddit() const;

    // Java lines 169-171: public final boolean isSearchResults()
    bool isSearchResults() const;

    // Java lines 173-175: public final boolean isSubredditSearchResults()
    bool isSubredditSearchResults() const;

    // Java lines 177-179: public final boolean isUserPostListing()
    bool isUserPostListing() const;

    // Java lines 181-203: public final SubredditCanonicalId subredditCanonicalName()
    SubredditCanonicalId *subredditCanonicalName() const;

    // Java lines 205-211: public final String multiredditName()
    QString multiredditName() const;

    // Java lines 213-219: public final String multiredditUsername()
    QString multiredditUsername() const;

private:
    // ========================================================================
    // Fields — Java lines 39-40
    // ========================================================================

    // Java line 39: private UUID session = null;
    QUuid session;

    // Java line 40: private PostListingURL url;
    PostListingURL *url = nullptr;
};

} // namespace PinkReader
