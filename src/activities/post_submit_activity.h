/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: post_submit_activity.h - Port of RedReader's PostSubmitActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/PostSubmitActivity.java
 *
 * Original: public class PostSubmitActivity extends ViewsBaseActivity implements
 *     PostSubmitSubredditSelectionFragment.Listener,
 *     PostSubmitContentFragment.Listener
 *
 * Every field, method, constant, and inner class ported exactly.
 * Android-specific classes (Activity, Fragment, Intent, Bundle, etc.) are used
 * as forward declarations and stubs where possible.
 */

#pragma once

#include <QObject>
#include <QString>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

class Bundle;                         // Android os.Bundle stub
class Intent;                         // Android content.Intent stub
class Context;                        // Android content.Context stub
class ViewsBaseActivity;              // Base activity class
class UriString;                      // Port of common.UriString
class SubredditCanonicalId;          // Port of reddit.SubredditCanonicalId
class RRError;                        // Port of common.RRError

// Fragment listener interfaces (forward declared)
namespace PostSubmitSubredditSelectionFragment {
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void onSubredditSelected(
                const QString &username,
                const SubredditCanonicalId &subreddit) = 0;
        virtual void onNotLoggedIn() = 0;
    };
}

namespace PostSubmitContentFragment {
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void onContentFragmentSubmissionSuccess(
                const UriString *redirectUrl) = 0;
        virtual void onContentFragmentSubredditDoesNotExist() = 0;
        virtual void onContentFragmentSubredditPermissionDenied() = 0;
        virtual void onContentFragmentFlairRequestError(
                const RRError &error) = 0;
    };

    // Port of: PostSubmitContentFragment.Args (used via toBundle())
    class Args {
    public:
        Args(const QString &username,
             const SubredditCanonicalId &subreddit,
             const QString &intentUrl);  // nullable intentUrl
        Bundle *toBundle() const;

    private:
        QString m_username;
        const SubredditCanonicalId &m_subreddit;
        QString m_intentUrl;  // empty string = null
    };
}

namespace PostSubmitSubredditSelectionFragment {
    // Port of: PostSubmitSubredditSelectionFragment.Args (used via toBundle())
    class Args {
    public:
        explicit Args(const SubredditCanonicalId *intentSubreddit);  // nullable
        Bundle *toBundle() const;

    private:
        const SubredditCanonicalId *m_intentSubreddit;
    };
}

// ============================================================================
// PostSubmitActivity — port of Java class (Java lines 40-165)
//
// Port of: org.quantumbadger.redreader.activities.PostSubmitActivity
//
// Extends ViewsBaseActivity.
// Implements PostSubmitSubredditSelectionFragment.Listener,
//             PostSubmitContentFragment.Listener.
//
// Handles post submission flow:
//   1. Select subreddit (PostSubmitSubredditSelectionFragment)
//   2. Fill in post content (PostSubmitContentFragment)
//   3. Submit
// ============================================================================

class PostSubmitActivity : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // TAG — port of static field (Java line 44)
    // Port of: @NonNull private static final String TAG = "PostSubmitActivity";
    // ========================================================================
    static const QString TAG;

    // ========================================================================
    // Constructor / Lifecycle
    // ========================================================================
    explicit PostSubmitActivity(QObject *parent = nullptr);
    virtual ~PostSubmitActivity();

    // ========================================================================
    // onCreate — port of override method (Java lines 48-87)
    //
    // Port of:
    //   @Override protected void onCreate(final Bundle savedInstanceState)
    //
    // 1. Applies theme via PrefsUtility.applyTheme
    // 2. Reads Intent for subreddit extra and ACTION_SEND
    // 3. Sets up PostSubmitSubredditSelectionFragment
    // ========================================================================
    void onCreate(Bundle *savedInstanceState);

    // ========================================================================
    // onBackPressed — port of override method (Java lines 89-94)
    //
    // Port of:
    //   @Override public void onBackPressed()
    //
    // Uses General.onBackPressed() for double-back-to-exit logic.
    // ========================================================================
    void onBackPressed();

    // ========================================================================
    // onSubredditSelected — port of Listener method (Java lines 96-112)
    //
    // Port of: PostSubmitSubredditSelectionFragment.Listener
    //
    // Replaces subreddit selection fragment with content fragment.
    // ========================================================================
    void onSubredditSelected(
            const QString &username,
            const SubredditCanonicalId &subreddit);

    // ========================================================================
    // onNotLoggedIn — port of Listener method (Java lines 114-118)
    //
    // Port of: PostSubmitSubredditSelectionFragment.Listener
    //
    // Shows toast and finishes activity.
    // ========================================================================
    void onNotLoggedIn();

    // ========================================================================
    // onContentFragmentSubmissionSuccess — port of Listener method
    // (Java lines 120-128)
    //
    // Port of: PostSubmitContentFragment.Listener
    //
    // Handles successful submission: follows redirect if any, then finish.
    // ========================================================================
    void onContentFragmentSubmissionSuccess(const UriString *redirectUrl);

    // ========================================================================
    // onContentFragmentSubredditDoesNotExist — port of Listener method
    // (Java lines 130-142)
    //
    // Port of: PostSubmitContentFragment.Listener
    //
    // Goes back and shows error dialog.
    // ========================================================================
    void onContentFragmentSubredditDoesNotExist();

    // ========================================================================
    // onContentFragmentSubredditPermissionDenied — port of Listener method
    // (Java lines 144-158)
    //
    // Port of: PostSubmitContentFragment.Listener
    //
    // Goes back and shows permission-denied dialog.
    // ========================================================================
    void onContentFragmentSubredditPermissionDenied();

    // ========================================================================
    // onContentFragmentFlairRequestError — port of Listener method
    // (Java lines 160-164)
    //
    // Port of: PostSubmitContentFragment.Listener
    //
    // Goes back and shows flair error dialog.
    // ========================================================================
    void onContentFragmentFlairRequestError(const RRError &error);

private:
    // ========================================================================
    // mIntentUrl — port of field (Java line 46)
    // Port of: @Nullable private String mIntentUrl;
    //
    // URL received via ACTION_SEND intent, pre-filled in post content.
    // ========================================================================
    QString mIntentUrl;  // empty string = null

    // ========================================================================
    // Reference to ViewsBaseActivity base class
    // ========================================================================
    ViewsBaseActivity *m_base;
};

} // namespace PinkReader
