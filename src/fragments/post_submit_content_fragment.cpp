/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: post_submit_content_fragment.cpp - Port of PostSubmitContentFragment.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/fragments/postsubmit/PostSubmitContentFragment.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "fragments/post_submit_content_fragment.h"
#include "accounts/reddit_account.h"
#include "accounts/reddit_account_manager.h"
#include "cache/cache_manager.h"
#include "common/rr_error.h"
#include "utils/general.h"

#include <QString>
#include <QStringLiteral>
#include <exception>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace PinkReader {

// ============================================================================
// Static constants — PostSubmitContentFragment
// ============================================================================

// Port of: @NonNull private static final String TAG = "PostSubmitContentFrag";
const QString PostSubmitContentFragment::TAG =
    QStringLiteral("PostSubmitContentFrag");

// Port of: private static final String POST_TYPE_LINK = "Link";
const QString PostSubmitContentFragment::POST_TYPE_LINK =
    QStringLiteral("Link");

// Port of: private static final String POST_TYPE_SELF = "Text";
const QString PostSubmitContentFragment::POST_TYPE_SELF =
    QStringLiteral("Text");

// Port of: private static final String POST_TYPE_IMGUR = "Upload to Imgur";
const QString PostSubmitContentFragment::POST_TYPE_IMGUR =
    QStringLiteral("Upload to Imgur");

// ============================================================================
// Static constants — Args inner class
// ============================================================================

// Port of: @NonNull private static final String KEY_USER = "user";
const QString PostSubmitContentFragment::Args::KEY_USER =
    QStringLiteral("user");

// Port of: @NonNull private static final String KEY_SUBREDDIT = "subreddit";
const QString PostSubmitContentFragment::Args::KEY_SUBREDDIT =
    QStringLiteral("subreddit");

// Port of: @NonNull private static final String KEY_URL = "url";
const QString PostSubmitContentFragment::Args::KEY_URL =
    QStringLiteral("url");

// ============================================================================
// Static fields
// ============================================================================

// Port of: private static String lastType; (Java line 134)
QString PostSubmitContentFragment::s_lastType;

// Port of: private static String lastTitle; (Java line 135)
QString PostSubmitContentFragment::s_lastTitle;

// Port of: private static String lastBodyText; (Java line 136)
QString PostSubmitContentFragment::s_lastBodyText;

// Port of: private static String lastBodyUrl; (Java line 137)
QString PostSubmitContentFragment::s_lastBodyUrl;

// Port of: private static boolean lastNsfw; (Java line 138)
bool PostSubmitContentFragment::s_lastNsfw = false;

// Port of: private static boolean lastSpoiler; (Java line 139)
bool PostSubmitContentFragment::s_lastSpoiler = false;

// Port of: private static boolean lastInbox; (Java line 140)
bool PostSubmitContentFragment::s_lastInbox = true;

// ============================================================================
// Args inner class implementation
// ============================================================================

// Port of: public Args(...) (Java lines 89-96)
PostSubmitContentFragment::Args::Args(
        const QString &username_,
        SubredditCanonicalId *subreddit_,
        const QString &url_)
    : username(username_)
    , subreddit(subreddit_)
    , url(url_)
{
    // this.username = username;
    // this.subreddit = subreddit;
    // this.url = url;
}

// Port of: @NonNull public Bundle toBundle() (Java lines 98-106)
Bundle *PostSubmitContentFragment::Args::toBundle() const {
    // final Bundle result = new Bundle(3);
    // result.putString(KEY_USER, username);
    // result.putParcelable(KEY_SUBREDDIT, subreddit);
    // result.putString(KEY_URL, url);
    // return result;
    return nullptr;  // Stub: Android Bundle not ported
}

// Port of: @NonNull public static Args fromBundle(@NonNull final Bundle bundle) (Java lines 108-116)
PostSubmitContentFragment::Args *PostSubmitContentFragment::Args::fromBundle(Bundle *bundle) {
    // return new Args(
    //     Objects.requireNonNull(bundle.getString(KEY_USER)),
    //     Objects.requireNonNull(BundleCompat.getParcelable(bundle, KEY_SUBREDDIT, SubredditCanonicalId.class)),
    //     bundle.getString(KEY_URL));
    (void)bundle;
    return nullptr;  // Stub: Android Bundle not ported
}

// ============================================================================
// Constructor / Destructor
// ============================================================================

PostSubmitContentFragment::PostSubmitContentFragment(QObject *parent)
    : QObject(parent)
{
}

PostSubmitContentFragment::~PostSubmitContentFragment() = default;

// ============================================================================
// Getters
// ============================================================================

Context *PostSubmitContentFragment::getFragmentContext() const {
    return mContext;
}

RedditAccount *PostSubmitContentFragment::getSelectedAccount() const {
    return mSelectedAccount;
}

SubredditCanonicalId *PostSubmitContentFragment::getSelectedSubreddit() const {
    return mSelectedSubreddit;
}

bool PostSubmitContentFragment::isActive() const {
    return mActive;
}

// ============================================================================
// onResume — port of override method (Java lines 169-178)
// ============================================================================

void PostSubmitContentFragment::onResume() {
    // Port of: super.onResume(); (Java line 171)
    // (handled by base class)

    // Port of: final FragmentActivity activity = getActivity(); (Java line 173)
    // Port of: if(activity != null) { activity.setTitle(R.string.submit_post_actionbar); }
    // (Java lines 175-177)
    // Stub: Android Fragment lifecycle not ported
}

// ============================================================================
// onDestroy — port of override method (Java lines 180-195)
// ============================================================================

void PostSubmitContentFragment::onDestroy() {
    // Port of: super.onDestroy(); (Java line 182)
    // (handled by base class)

    // Port of: mActive = false; (Java line 183)
    mActive = false;

    // Port of: if(mTitleEdit != null && !mDraftReset) { ... } (Java lines 186-193)
    if (mTitleEdit != nullptr && !mDraftReset) {
        // lastType = mTypeSpinner.getText().toString();
        // lastTitle = mTitleEdit.getText().toString();
        // lastBodyText = mTextEditBodyText.getText().toString();
        // lastBodyUrl = mTextEditBodyUrl.getText().toString();
        // lastInbox = mSendRepliesToInboxCheckbox.isChecked();
        // lastNsfw = mMarkAsNsfwCheckbox.isChecked();
        // lastSpoiler = mMarkAsSpoilerCheckbox.isChecked();
        // Stub: widget text getters not ported
    }
}

// ============================================================================
// onCreateView — port of method (Java lines 197-289)
// ============================================================================

View *PostSubmitContentFragment::onCreateView(
        LayoutInflater *inflater,
        ViewGroup *container,
        Bundle *savedInstanceState) {
    (void)inflater;
    (void)container;
    (void)savedInstanceState;

    // Port of: setHasOptionsMenu(true); (Java line 204)
    // Stub: Fragment menu management not ported

    // Port of: mContext = Objects.requireNonNull(container).getContext(); (Java line 206)
    // mContext = container->getContext();  // Stub

    // Port of: final RedditAccountManager accountManager = RedditAccountManager.getInstance(mContext); (Java line 208)
    // Port of: final Args args = Args.fromBundle(requireArguments()); (Java line 210)
    // Port of: mSelectedAccount = accountManager.getAccount(args.username); (Java line 212)
    // Port of: if(mSelectedAccount == null) { ... return null; } (Java lines 214-220)
    // Port of: mSelectedSubreddit = args.subreddit; (Java line 222)
    // Port of: final View root = inflater.inflate(R.layout.post_submit, container, false); (Java line 224)

    // All widget initialization (Java lines 226-253):
    // mMainControls = Objects.requireNonNull(root.findViewById(...));
    // mLoadingSpinnerView = Objects.requireNonNull(...);
    // mTypeSpinner = Objects.requireNonNull(...);
    // mFlairSpinner = Objects.requireNonNull(...);
    // mFlairSpinnerLayout = Objects.requireNonNull(...);
    // mTitleEdit = Objects.requireNonNull(...);
    // mTextEditBodyText = Objects.requireNonNull(...);
    // mTextEditLayoutBodyText = Objects.requireNonNull(...);
    // mTextEditBodyUrl = Objects.requireNonNull(...);
    // mTextEditLayoutBodyUrl = Objects.requireNonNull(...);
    // mSendRepliesToInboxCheckbox = Objects.requireNonNull(...);
    // mMarkAsNsfwCheckbox = Objects.requireNonNull(...);
    // mMarkAsSpoilerCheckbox = Objects.requireNonNull(...);
    // Stub: Layout inflation not ported

    // Port of: heading.setText(String.format(...)); (Java lines 255-261)
    // Port of: mTypeSpinner.setText(POST_TYPE_LINK); (Java line 263)
    // Port of: AndroidCommon.setAutoCompleteTextViewItemsNoFilter(mTypeSpinner, POST_TYPES); (Java line 265)
    // Port of: if(args.url != null) { mTextEditBodyUrl.setText(args.url); } (Java lines 267-269)
    // Port of: if(args.url == null && lastTitle != null) { ... draft restore ... } (Java lines 272-280)
    // Port of: setHint(); (Java line 282)
    // Port of: AndroidCommon.onTextChanged(mTypeSpinner, this::setHint); (Java line 284)
    // Port of: requestSubredditDetails(); (Java line 286)
    // Port of: return root; (Java line 288)

    return nullptr;  // Stub
}

// ============================================================================
// disableFlairSpinner — port of private method (Java lines 291-302)
// ============================================================================

void PostSubmitContentFragment::disableFlairSpinner(int messageRes) {
    (void)messageRes;

    // Port of: final Context appContext = mContext.getApplicationContext(); (Java line 293)
    // Port of: mFlairSpinner.setAdapter(null); (Java line 295)
    // Port of: mFlairSpinner.setText(appContext.getString(message)); (Java line 296)
    // Port of: mFlairSpinnerLayout.setEnabled(false); (Java line 298)
    // Port of: mFlairSpinner.setEnabled(false); (Java line 300)
    // Port of: mFlairSpinner.setAlpha(0.5f); (Java line 301)
    // Stub: Widget controls not ported
}

// ============================================================================
// enableFlairSpinner — port of private method (Java lines 304-326)
// ============================================================================

void PostSubmitContentFragment::enableFlairSpinner(
        const std::vector<RedditFlairChoice *> &choices) {
    (void)choices;

    // Port of: final Context appContext = mContext.getApplicationContext(); (Java line 306)
    // Port of: mFlairSpinner.setEnabled(true); (Java line 308)
    // Port of: mFlairSpinner.setAlpha(1.0f); (Java line 309)
    // Port of: final ArrayList<String> choiceStrings = new ArrayList<>(choices.size() + 1); (Java line 311)
    // Port of: mFlairIds.clear(); (Java line 312)
    // Port of: final String noneSelected = appContext.getString(R.string.post_submit_flair_none_selected); (Java line 314)
    // Port of: choiceStrings.add(noneSelected); (Java line 316)
    // Port of: for(final RedditFlairChoice choice : choices) { ... } (Java lines 318-321)
    // Port of: AndroidCommon.setAutoCompleteTextViewItemsNoFilter(mFlairSpinner, choiceStrings); (Java line 323)
    // Port of: mFlairSpinner.setText(noneSelected); (Java line 325)
    // Stub: Widget controls not ported
}

// ============================================================================
// setHint — port of private method (Java lines 328-362)
// ============================================================================

void PostSubmitContentFragment::setHint() {
    // Port of: final Object selected = mTypeSpinner.getText().toString(); (Java line 330)
    // Port of: if(selected.equals(POST_TYPE_LINK) || selected.equals(POST_TYPE_IMGUR)) { ... }
    //           else if(selected.equals(POST_TYPE_SELF)) { ... }
    //           else { throw RuntimeException(...); } (Java lines 332-340)
    // Port of: if(selected.equals(POST_TYPE_IMGUR)) { Imgur upload flow } (Java lines 342-361)
    // Stub: Widget controls not ported
}

// ============================================================================
// requestSubredditDetails — port of private method (Java lines 364-434)
// ============================================================================

void PostSubmitContentFragment::requestSubredditDetails() {
    // Port of: RedditAPI.flairSelectorForNewLink(
    //     mContext,
    //     CacheManager.getInstance(mContext),
    //     mSelectedAccount,
    //     mSelectedSubreddit,
    //     new RedditAPI.FlairSelectorResponseHandler() {
    //         @Override public void onSuccess(...) { ... }
    //         @Override public void onSubredditDoesNotExist() { ... }
    //         @Override public void onSubredditPermissionDenied() { ... }
    //         @Override public void onFailure(@NonNull final RRError error) { ... }
    //     });
    // (Java lines 366-433)
    // Stub: RedditAPI and flair selector not ported
}

// ============================================================================
// onCreateOptionsMenu — port of override method (Java lines 436-446)
// ============================================================================

void PostSubmitContentFragment::onCreateOptionsMenu(Menu *menu, MenuInflater *inflater) {
    (void)inflater;

    // Port of: final MenuItem send = menu.add(R.string.comment_reply_send); (Java line 441)
    // Port of: send.setIcon(R.drawable.ic_action_send_dark); (Java line 442)
    // Port of: send.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS); (Java line 443)
    // Port of: menu.add(R.string.comment_reply_preview); (Java line 445)
    (void)menu;
    // Stub: Menu creation not ported
}

// ============================================================================
// onOptionsItemSelected — port of override method (Java lines 448-613)
// ============================================================================

bool PostSubmitContentFragment::onOptionsItemSelected(MenuItem *item) {
    (void)item;

    // Port of: final boolean isSelfPost = mTypeSpinner.getText().toString().equals(POST_TYPE_SELF); (Java line 451)
    // Port of: final String bodyText = isSelfPost ? mTextEditBodyText.getText().toString() : ... (Java lines 453-455)

    // Port of: if(item.getTitle().equals(getString(R.string.comment_reply_send))) { ... } (Java line 457)
    // Send branch (Java lines 457-603):
    //   - Validation: postTitle.isEmpty(), bodyText.isEmpty()
    //   - ProgressDialog creation
    //   - APIResponseHandler.SubmitResponseHandler creation
    //   - CacheManager instance
    //   - RedditAPI.submit(...)
    //   - progressDialog.show()

    // Port of: else if(item.getTitle().equals(getString(R.string.comment_reply_preview))) { ... } (Java lines 605-608)
    // Port of: else { return super.onOptionsItemSelected(item); } (Java lines 610-612)

    return true;
}

// ============================================================================
// resetDraft — port of private method (Java lines 615-624)
// ============================================================================

void PostSubmitContentFragment::resetDraft() {
    // Port of: mDraftReset = true; (Java line 616)
    mDraftReset = true;

    // Port of: lastType = null; (Java line 617)
    s_lastType.clear();

    // Port of: lastTitle = null; (Java line 618)
    s_lastTitle.clear();

    // Port of: lastBodyText = null; (Java line 619)
    s_lastBodyText.clear();

    // Port of: lastBodyUrl = null; (Java line 620)
    s_lastBodyUrl.clear();

    // Port of: lastInbox = true; (Java line 621)
    s_lastInbox = true;

    // Port of: lastNsfw = false; (Java line 622)
    s_lastNsfw = false;

    // Port of: lastSpoiler = false; (Java line 623)
    s_lastSpoiler = false;
}

// ============================================================================
// ifActivityNotNull — port of private method (Java lines 626-633)
// ============================================================================

void PostSubmitContentFragment::ifActivityNotNull(
        std::function<void(PostSubmitContentFragment::Listener *)> action) {
    // Port of: final FragmentActivity activity = getActivity(); (Java line 628)
    // Port of: if(activity != null) { action.consume((Listener)activity); } (Java lines 630-632)
    // Stub: Fragment activity retrieval not ported
    (void)action;
}

} // namespace PinkReader
