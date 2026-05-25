/*
 * PinkReader - GPLv3
 * File: comment_listing_controller.h - Port of CommentListingController.java
 *
 * Exact port of:
 *   redreader/src/main/java/org/quantumbadger/redreader/listingcontrollers/CommentListingController.java
 *
 * Every field, method, and logic branch ported exactly.
 *
 * Original: public class CommentListingController
 * 140 lines of Java ported.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class Bundle;
class CommentListingFragment;
class CommentListingURL;
class OptionsMenuUtility;
class PrefsUtility;
class PostCommentSort;
class UserCommentSort;
class RedditURLParser;

/**
 * @brief Port of org.quantumbadger.redreader.listingcontrollers.CommentListingController
 *
 * Controls comment listing navigation and sort options.
 * Every field, method, and logic branch ported exactly.
 */
class CommentListingController : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Constructor — Java lines 49-67
    // ========================================================================

    // Java lines 49-67:
    // public CommentListingController(RedditURLParser.RedditURL url)
    CommentListingController(RedditURLParser *url, QObject *parent = nullptr);

    ~CommentListingController() override = default;

    // ========================================================================
    // Public methods — Java lines 41-139
    // ========================================================================

    // Java lines 41-43: public UUID getSession()
    QUuid getSession() const;

    // Java lines 45-47: public void setSession(final UUID session)
    void setSession(const QUuid &session);

    // Java lines 77-87: public void setSort(final PostCommentSort s) / setSort(final UserCommentSort s)
    void setSort(const PostCommentSort *s);
    void setSort(const UserCommentSort *s);

    // Java lines 89-98: public OptionsMenuUtility.Sort getSort()
    OptionsMenuUtility *getSort() const;

    // Java lines 100-106: public void setSearchString(...) / getSearchString()
    void setSearchString(const QString &searchString);
    QString getSearchString() const;

    // Java lines 108-110: public Uri getUri()
    QString getUri() const;  // Returns Uri.toString() equivalent

    // Java lines 112-114: public CommentListingURL getCommentListingUrl()
    CommentListingURL *getCommentListingUrl() const;

    // Java lines 116-130: public CommentListingFragment get(...)
    CommentListingFragment *get(
            AppCompatActivity *parent,
            bool force,
            Bundle *savedInstanceState);

    // Java lines 132-135: public boolean isSortable()
    bool isSortable() const;

    // Java lines 137-139: public boolean isUserCommentListing()
    bool isUserCommentListing() const;

private:
    // ========================================================================
    // Private methods — Java lines 69-75
    // ========================================================================

    // Java lines 69-71: private PostCommentSort defaultOrder()
    PostCommentSort *defaultOrder() const;

    // Java lines 73-75: private UserCommentSort defaultUserOrder() const;
    UserCommentSort *defaultUserOrder() const;

    // ========================================================================
    // Fields — Java lines 37-39
    // ========================================================================

    // Java line 37: private CommentListingURL mUrl;
    CommentListingURL *mUrl = nullptr;

    // Java line 38: private UUID mSession = null;
    QUuid mSession;

    // Java line 39: private String mSearchString = null;
    QString mSearchString;
};

} // namespace PinkReader
