/*
 * PinkReader - GPLv3
 * File: link_handler.h - Port of LinkHandler.kt (object LinkHandler)
 * Exact port: every method, regex pattern, and logic branch from the original.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QSet>
#include <QVector>
#include <functional>
#include <string>

namespace PinkReader {

// Forward declarations
class RedditPost;
struct ImageInfo;
struct AlbumInfo;
class GetImageInfoListener;
class GetAlbumInfoListener;
class Priority;

/**
 * @brief Port of org.quantumbadger.redreader.common.LinkHandler (Kotlin object)
 *
 * Every public @JvmStatic method is ported.
 * Regex patterns are ported exactly.
 */
class LinkHandler {
public:
    LinkHandler() = delete; // Cannot instantiate (Kotlin object)

    // --- Port of: enum class LinkAction ---
    enum class LinkAction {
        SHARE,
        COPY_URL,
        SHARE_IMAGE,
        SAVE_IMAGE,
        EXTERNAL
    };

    // --- Port of: onLinkClicked ---
    // The main link routing method. Port of:
    //   @JvmStatic @JvmOverloads fun onLinkClicked(
    //       activity: AppCompatActivity, url: UriString?, forceNoImage: Boolean = false,
    //       post: RedditPost? = null, albumInfo: AlbumInfo? = null,
    //       albumImageIndex: Int? = null, fromExternalIntent: Boolean = false)
    //
    // In C++, we replace the activity/navigation side effects with callback functions.
    struct LinkContext {
        std::function<void(const QString &url, const RedditPost *post)> startImageView;
        std::function<void(const QString &url, const RedditPost *post)> startAlbumListing;
        std::function<void(const QString &url, const RedditPost *post)> startInternalBrowser;
        std::function<void(const QString &url, bool fromExternal)> startWebBrowser;
        std::function<void(const QString &url, const RedditPost *post)> startCustomTab;
        std::function<void(const QString &url)> startPostListing;
        std::function<void(const QString &url)> startCommentListing;
        std::function<void(const QString &url, const QString &recipient,
                           const QString &subject, const QString &message)> startPMSend;
        std::function<void(const QString &username)> showUserProfile;
        std::function<void(const QString &title, const QString &message)> showDialog;
        std::function<QVector<std::pair<QString, QString>>(const QString &baseUri)>
            queryIntentActivities;
        std::function<bool()> useInternalBrowser;
        std::function<bool()> useCustomTabs;
        std::function<int()> albumViewMode; // 0=INTERNAL_LIST, 1=INTERNAL_BROWSER, 2=EXTERNAL
        std::function<QString(const QString &uri)> resolveRedirectUri;
        std::function<std::optional<QString>(const QString &url)> parseRedditUrl;
        std::function<std::optional<QString>(const QString &url)> generateJsonUri;
        std::function<std::optional<QString>(const QString &url)> asComposeMessageURL;
        std::function<std::optional<QString>(const QString &url)> asUserProfileURL;
    };

    static void onLinkClicked(LinkContext &ctx,
                              const QString &url,
                              bool forceNoImage = false,
                              const RedditPost *post = nullptr,
                              const AlbumInfo *albumInfo = nullptr,
                              int albumImageIndex = -1,
                              bool fromExternalIntent = false);

    // --- Port of: onLinkLongClicked ---
    // Port of:
    //   @JvmOverloads @JvmStatic fun onLinkLongClicked(
    //       activity: BaseActivity, uri: UriString?, forceNoImage: Boolean = false)
    struct LongClickContext {
        std::function<QSet<LinkAction>()> getContextMenuItems;
        std::function<QString(int resId)> getString;
        std::function<void(const QString &uri, LinkAction action)> onActionSelected;
        std::function<QVector<QString>(const QVector<QString> &items)> showMenu;
    };
    static void onLinkLongClicked(LongClickContext &ctx,
                                   const QString &uri,
                                   bool forceNoImage = false);

    // --- Port of: onActionMenuItemSelected ---
    // Port of:
    //   fun onActionMenuItemSelected(uri: UriString, activity: BaseActivity, action: LinkAction)
    struct ActionContext {
        std::function<void(const QString &subject, const QString &text)> shareText;
        std::function<void(const QString &text)> copyToClipboard;
        std::function<void(const QString &text)> quickToast;
        std::function<void(const QString &uri)> startExternalView;
        std::function<void(const QString &uri)> shareImage;
        std::function<void(const QString &uri)> saveImage;
        std::function<QString(const QString &uri)> getPreferredRedditUriString;
    };
    static void onActionMenuItemSelected(ActionContext &ctx,
                                          const QString &uri,
                                          LinkAction action);

    // --- Port of: openWebBrowser ---
    // Port of:
    //   @JvmStatic fun openWebBrowser(activity, uri: Uri, fromExternalIntent: Boolean): Boolean
    static bool openWebBrowser(LinkContext &ctx,
                                const QString &uri,
                                bool fromExternalIntent);

    // --- Port of: openCustomTab ---
    // Port of:
    //   @JvmStatic fun openCustomTab(activity, uri: Uri, post: RedditPost?, showShare: Boolean)
    static void openCustomTab(LinkContext &ctx,
                               const QString &uri,
                               const RedditPost *post,
                               bool showShare);

    // --- Port of: isRedGifsImage ---
    // Port of:
    //   @JvmStatic fun isRedGifsImage(url: UriString): Boolean
    static bool isRedGifsImage(const QString &url);

    // --- Port of: isProbablyAnImage ---
    // Port of:
    //   @JvmStatic fun isProbablyAnImage(url: UriString?): Boolean
    static bool isProbablyAnImage(const QString &url);

    // --- Port of: getImgurImageInfo ---
    // Port of:
    //   @JvmStatic fun getImgurImageInfo(context, imgId, priority, returnUrlOnFailure, listener)
    static void getImgurImageInfo(const QString &imgId,
                                   const Priority &priority,
                                   bool returnUrlOnFailure,
                                   GetImageInfoListener &listener);

    // --- Port of: getAlbumInfo ---
    // Port of:
    //   @JvmStatic fun getAlbumInfo(context, url: UriString, priority, listener)
    static void getAlbumInfo(const QString &url,
                              const Priority &priority,
                              GetAlbumInfoListener &listener);

    // --- Port of: getImageInfo ---
    // Port of:
    //   @JvmStatic fun getImageInfo(context, url: UriString?, priority, listener)
    static void getImageInfo(const QString &url,
                              const Priority &priority,
                              GetImageInfoListener &listener);

    // --- Port of: computeAllLinks ---
    // Port of:
    //   @JvmStatic fun computeAllLinks(text: String): LinkedHashSet<UriString>
    static QSet<QString> computeAllLinks(const QString &text);

    // --- Port of: shareText ---
    // Port of:
    //   @JvmStatic fun shareText(activity, subject: String?, text: String?)
    static void shareText(ActionContext &ctx,
                           const QString &subject,
                           const QString &text);

    // --- Port of: convertAndNormalizeUri ---
    // Port of:
    //   fun convertAndNormalizeUri(uri: UriString): Uri
    static QString convertAndNormalizeUri(const QString &uri);

    // --- Port of: getPreferredRedditUriString ---
    // Port of:
    //   @JvmStatic fun getPreferredRedditUriString(uri: UriString): UriString
    static QString getPreferredRedditUriString(const QString &uri);

    // --- Exposed regex patterns (originally @JvmField) ---
    // Port of: @JvmField val imgurAlbumPattern: Pattern
    static const QRegularExpression imgurAlbumPattern;

private:
    // Port of: private val youtubeDotComPattern = ...
    static const QRegularExpression youtubeDotComPattern;
    // Port of: private val youtuDotBePattern = ...
    static const QRegularExpression youtuDotBePattern;
    // Port of: private val vimeoPattern = ...
    static const QRegularExpression vimeoPattern;
    // Port of: private val googlePlayPattern = ...
    static const QRegularExpression googlePlayPattern;
    // Port of: private val imgurPattern = ...
    static const QRegularExpression imgurPattern;
    // Port of: private val redditGalleryPattern = ...
    static const QRegularExpression redditGalleryPattern;
    // Port of: private val qkmePattern1 = ...
    static const QRegularExpression qkmePattern1;
    // Port of: private val qkmePattern2 = ...
    static const QRegularExpression qkmePattern2;
    // Port of: private val lvmePattern = ...
    static const QRegularExpression lvmePattern;
    // Port of: private val gfycatPattern = ...
    static const QRegularExpression gfycatPattern;
    // Port of: private val redgifsPattern = ...
    static const QRegularExpression redgifsPattern;
    // Port of: private val streamablePattern = ...
    static const QRegularExpression streamablePattern;
    // Port of: private val reddituploadsPattern = ...
    static const QRegularExpression reddituploadsPattern;
    // Port of: private val redditVideosPattern = ...
    static const QRegularExpression redditVideosPattern;
    // Port of: private val imgflipPattern = ...
    static const QRegularExpression imgflipPattern;
    // Port of: private val makeamemePattern = ...
    static const QRegularExpression makeamemePattern;
    // Port of: private val deviantartPattern = ...
    static const QRegularExpression deviantartPattern;
    // Port of: private val giphyPattern = ...
    static const QRegularExpression giphyPattern;

    // --- Private helpers ---

    // Port of: private fun openInternalBrowser(activity, url, post)
    static void openInternalBrowser(LinkContext &ctx,
                                     const QString &url,
                                     const RedditPost *post);

    // Port of: private fun getImgurAlbumInfo(context, albumUrl, albumId, priority, listener)
    static void getImgurAlbumInfo(const QString &albumUrl,
                                   const QString &albumId,
                                   const Priority &priority,
                                   GetAlbumInfoListener &listener);

    // Port of: private fun getImageUrlPatternMatch(url: UriString): ImageInfo?
    // Returns true if matched and fills info
    static bool getImageUrlPatternMatch(const QString &url);
};

} // namespace PinkReader
