/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: post_submit_content_fragment.h - Port of PostSubmitContentFragment.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/fragments/postsubmit/PostSubmitContentFragment.java
 *
 * Original: public class PostSubmitContentFragment extends Fragment
 *
 * Every field, method, constant, and inner class ported exactly.
 * Android-specific classes (Fragment, View, Bundle, etc.) are used
 * as forward declarations and stubs where possible.
 */

#pragma once

#include <QObject>
#include <QString>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

class Bundle;
class Intent;
class View;
class ViewGroup;
class LayoutInflater;
class Menu;
class MenuInflater;
class MenuItem;
class FragmentActivity;
class Fragment;
class AppCompatActivity;
class Context;
class TextView;
class MaterialAutoCompleteTextView;
class TextInputEditText;
class TextInputLayout;
class CheckBox;

// RedReader types
class RedditAccount;
class RedditAccountManager;
class SubredditCanonicalId;
class UriString;
class RRError;
class CacheManager;
class RedditFlairChoice;
class MarkdownPreviewDialog;
class ProgressDialog;
class General;

// ============================================================================
// PostSubmitContentFragment — port of Java class (Java lines 75-634)
//
// Port of: org.quantumbadger.redreader.fragments.postsubmit.PostSubmitContentFragment
//
// Extends Fragment. Every field, method, and inner class ported exactly.
// ============================================================================

class PostSubmitContentFragment : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Inner class: Args (Java lines 79-117)
    //
    // Port of: public static class Args
    //
    // Holds arguments for the fragment: username, subreddit, optional url.
    // ========================================================================
    class Args {
    public:
        // Port of: @NonNull private static final String KEY_USER = "user";
        static const QString KEY_USER;

        // Port of: @NonNull private static final String KEY_SUBREDDIT = "subreddit";
        static const QString KEY_SUBREDDIT;

        // Port of: @NonNull private static final String KEY_URL = "url";
        static const QString KEY_URL;

        // Port of: @NonNull public final String username;
        QString username;

        // Port of: @NonNull public final SubredditCanonicalId subreddit;
        SubredditCanonicalId *subreddit = nullptr;

        // Port of: @Nullable public final String url;
        QString url;  // empty = null

        // Port of: public Args(...) (Java lines 89-96)
        Args(const QString &username_,
             SubredditCanonicalId *subreddit_,
             const QString &url_);

        // Port of: @NonNull public Bundle toBundle() (Java lines 98-106)
        Bundle *toBundle() const;

        // Port of: @NonNull public static Args fromBundle(@NonNull final Bundle bundle) (Java lines 108-116)
        static Args *fromBundle(Bundle *bundle);
    };

    // ========================================================================
    // Inner interface: Listener (Java lines 119-124)
    //
    // Port of: public interface Listener
    // ========================================================================
    class Listener {
    public:
        virtual ~Listener() = default;

        // Port of: void onContentFragmentSubmissionSuccess(@Nullable UriString redirectUrl);
        virtual void onContentFragmentSubmissionSuccess(UriString *redirectUrl) = 0;

        // Port of: void onContentFragmentSubredditDoesNotExist();
        virtual void onContentFragmentSubredditDoesNotExist() = 0;

        // Port of: void onContentFragmentSubredditPermissionDenied();
        virtual void onContentFragmentSubredditPermissionDenied() = 0;

        // Port of: void onContentFragmentFlairRequestError(@NonNull RRError error);
        virtual void onContentFragmentFlairRequestError(const RRError &error) = 0;
    };

    // ========================================================================
    // Static constants (Java lines 77, 126-130)
    // ========================================================================

    // Port of: @NonNull private static final String TAG = "PostSubmitContentFrag";
    static const QString TAG;

    // Port of: private static final String POST_TYPE_LINK = "Link";
    static const QString POST_TYPE_LINK;

    // Port of: private static final String POST_TYPE_SELF = "Text";
    static const QString POST_TYPE_SELF;

    // Port of: private static final String POST_TYPE_IMGUR = "Upload to Imgur";
    static const QString POST_TYPE_IMGUR;

    // ========================================================================
    // Static fields (Java lines 134-140)
    // ========================================================================

    // Port of: private static String lastType; (Java line 134)
    static QString s_lastType;

    // Port of: private static String lastTitle; (Java line 135)
    static QString s_lastTitle;

    // Port of: private static String lastBodyText; (Java line 136)
    static QString s_lastBodyText;

    // Port of: private static String lastBodyUrl; (Java line 137)
    static QString s_lastBodyUrl;

    // Port of: private static boolean lastNsfw; (Java line 138)
    static bool s_lastNsfw;

    // Port of: private static boolean lastSpoiler; (Java line 139)
    static bool s_lastSpoiler;

    // Port of: private static boolean lastInbox; (Java line 140)
    static bool s_lastInbox;

    // ========================================================================
    // Constructor / Destructor
    // ========================================================================
    explicit PostSubmitContentFragment(QObject *parent = nullptr);
    virtual ~PostSubmitContentFragment();

    // ========================================================================
    // Public lifecycle methods (Java lines 169-289)
    // ========================================================================

    // Port of: @Override public void onResume() (Java lines 169-178)
    void onResume();

    // Port of: @Override public void onDestroy() (Java lines 180-195)
    void onDestroy();

    // Port of: @Nullable @Override public View onCreateView(...) (Java lines 197-289)
    View *onCreateView(LayoutInflater *inflater,
                       ViewGroup *container,
                       Bundle *savedInstanceState);

    // Port of: @Override public void onCreateOptionsMenu(...) (Java lines 436-446)
    void onCreateOptionsMenu(Menu *menu, MenuInflater *inflater);

    // Port of: @Override public boolean onOptionsItemSelected(final MenuItem item) (Java lines 448-613)
    bool onOptionsItemSelected(MenuItem *item);

    // ========================================================================
    // Getters
    // ========================================================================

    // Port of: mContext getter (for internal use)
    Context *getFragmentContext() const;

    // Port of: mSelectedAccount getter
    RedditAccount *getSelectedAccount() const;

    // Port of: mSelectedSubreddit getter
    SubredditCanonicalId *getSelectedSubreddit() const;

    // Port of: isActive
    bool isActive() const;

private:
    // ========================================================================
    // Private fields (Java lines 132-167)
    // ========================================================================

    // Port of: private boolean mDraftReset = false; (Java line 132)
    bool mDraftReset = false;

    // Port of: private boolean mActive = true; (Java line 142)
    bool mActive = true;

    // Port of: private Context mContext; (Java line 144)
    Context *mContext = nullptr;

    // Port of: private RedditAccount mSelectedAccount; (Java line 146)
    RedditAccount *mSelectedAccount = nullptr;

    // Port of: private SubredditCanonicalId mSelectedSubreddit; (Java line 147)
    SubredditCanonicalId *mSelectedSubreddit = nullptr;

    // Port of: private View mLoadingSpinnerView; (Java line 149)
    View *mLoadingSpinnerView = nullptr;

    // Port of: private View mMainControls; (Java line 150)
    View *mMainControls = nullptr;

    // Port of: private MaterialAutoCompleteTextView mTypeSpinner; (Java line 152)
    MaterialAutoCompleteTextView *mTypeSpinner = nullptr;

    // Port of: private MaterialAutoCompleteTextView mFlairSpinner; (Java line 153)
    MaterialAutoCompleteTextView *mFlairSpinner = nullptr;

    // Port of: private TextInputLayout mFlairSpinnerLayout; (Java line 154)
    TextInputLayout *mFlairSpinnerLayout = nullptr;

    // Port of: private TextInputEditText mTitleEdit; (Java line 155)
    TextInputEditText *mTitleEdit = nullptr;

    // Port of: private TextInputEditText mTextEditBodyText; (Java line 157)
    TextInputEditText *mTextEditBodyText = nullptr;

    // Port of: private TextInputLayout mTextEditLayoutBodyText; (Java line 158)
    TextInputLayout *mTextEditLayoutBodyText = nullptr;

    // Port of: private TextInputEditText mTextEditBodyUrl; (Java line 160)
    TextInputEditText *mTextEditBodyUrl = nullptr;

    // Port of: private TextInputLayout mTextEditLayoutBodyUrl; (Java line 161)
    TextInputLayout *mTextEditLayoutBodyUrl = nullptr;

    // Port of: private CheckBox mSendRepliesToInboxCheckbox; (Java line 163)
    CheckBox *mSendRepliesToInboxCheckbox = nullptr;

    // Port of: private CheckBox mMarkAsNsfwCheckbox; (Java line 164)
    CheckBox *mMarkAsNsfwCheckbox = nullptr;

    // Port of: private CheckBox mMarkAsSpoilerCheckbox; (Java line 165)
    CheckBox *mMarkAsSpoilerCheckbox = nullptr;

    // Port of: private final HashMap<String, String> mFlairIds = new HashMap<>(); (Java line 167)
    std::unordered_map<QString, QString> mFlairIds;

    // ========================================================================
    // Private methods (Java lines 291-633)
    // ========================================================================

    // Port of: private void disableFlairSpinner(@StringRes final int message) (Java lines 291-302)
    void disableFlairSpinner(int messageRes);

    // Port of: private void enableFlairSpinner(@NonNull final Collection<RedditFlairChoice> choices) (Java lines 304-326)
    void enableFlairSpinner(const std::vector<RedditFlairChoice *> &choices);

    // Port of: private void setHint() (Java lines 328-362)
    void setHint();

    // Port of: private void requestSubredditDetails() (Java lines 364-434)
    void requestSubredditDetails();

    // Port of: private void resetDraft() (Java lines 615-624)
    void resetDraft();

    // Port of: private void ifActivityNotNull(@NonNull final Consumer<Listener> action) (Java lines 626-633)
    void ifActivityNotNull(std::function<void(Listener *)> action);
};

} // namespace PinkReader
