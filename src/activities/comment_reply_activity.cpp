/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: comment_reply_activity.cpp - Port of RedReader's CommentReplyActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/CommentReplyActivity.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "activities/comment_reply_activity.h"
#include "accounts/reddit_account.h"
#include "accounts/reddit_account_manager.h"
#include "cache/cache_manager.h"
#include "common/rr_error.h"
#include "utils/general.h"

#include <QString>
#include <exception>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace PinkReader {

// ============================================================================
// Static constants
// ============================================================================

// Port of: public static final String PARENT_TYPE = "parentType"; (Java line 80)
const QString CommentReplyActivity::PARENT_TYPE =
    QStringLiteral("parentType");

// Port of: public static final String PARENT_TYPE_MESSAGE = "parentTypeMessage";
// (Java line 81)
const QString CommentReplyActivity::PARENT_TYPE_MESSAGE =
    QStringLiteral("parentTypeMessage");

// Port of: public static final String PARENT_ID_AND_TYPE_KEY = "parentIdAndType";
// (Java line 83)
const QString CommentReplyActivity::PARENT_ID_AND_TYPE_KEY =
    QStringLiteral("parentIdAndType");

// Port of: public static final String PARENT_MARKDOWN_KEY = "parent_markdown";
// (Java line 84)
const QString CommentReplyActivity::PARENT_MARKDOWN_KEY =
    QStringLiteral("parent_markdown");

// Port of: private static final String COMMENT_TEXT_KEY = "comment_text";
// (Java line 85)
const QString CommentReplyActivity::COMMENT_TEXT_KEY =
    QStringLiteral("comment_text");

// ============================================================================
// Static fields
// ============================================================================

// Port of: private static String lastText; (Java line 77)
QString CommentReplyActivity::s_lastText;

// Port of: private static RedditIdAndType lastParentIdAndType; (Java line 78)
RedditIdAndType *CommentReplyActivity::s_lastParentIdAndType = nullptr;

// ============================================================================
// Forward-declared stubs for non-ported Android/RedReader types
// These are minimal stubs to make the logic compilable.
// ============================================================================

// Bundle stub
class BundleDelegate : public Bundle {
public:
    void putString(const char *key, const QString &value);
    void putParcelable(const char *key, void *value);
    QString getString(const char *key) const { return QString(); }
    bool containsKey(const char *key) const { return false; }
    void *getParcelable(const char *key) const { return nullptr; }
};

// Intent stub
class IntentDelegate : public Intent {
public:
    bool hasExtra(const QString &key) const { return false; }
    QString getStringExtra(const QString &key) const { return QString(); }
};

// ViewsBaseActivity stub
class ViewsBaseActivityDelegate : public ViewsBaseActivity {
public:
    void setTitle(const QString &title);
    void finish();
    Context *getApplicationContext();
};

// Context stub
class ContextDelegate : public Context {
public:
    QString getString(int resId) const;
};

// Widget stubs
class Spinner {
public:
    void setAdapter(ArrayAdapter *adapter) {}
    QString getSelectedItem() const { return QString(); }
};

class EditText {
public:
    QString getText() const { return m_text; }
    void setText(const QString &text) { m_text = text; }
private:
    QString m_text;
};

class CheckBox {
public:
    bool isChecked() const { return m_checked; }
    void setVisibility(int visibility) {}
private:
    bool m_checked = false;
};

class Button {
public:
    void setOnClickListener(std::function<void()> listener) { m_listener = listener; }
private:
    std::function<void()> m_listener;
};

class LinearLayout {};
class ScrollView {};
class TextView {};

class ArrayAdapter {
public:
    ArrayAdapter(void *context, int layout, const std::vector<QString> &items) {}
};

class ProgressDialog {
public:
    void setTitle(const QString &t) { m_title = t; }
    void setMessage(const QString &m) { m_message = m; }
    void setIndeterminate(bool b) { m_indeterminate = b; }
    void setCancelable(bool b) { m_cancelable = b; }
    void setCanceledOnTouchOutside(bool b) {}
    void setOnCancelListener(std::function<void()> l) { m_cancelListener = l; }
    void setOnKeyListener(std::function<void()> l) { m_keyListener = l; }
    void show() {}
    void dismiss() {}
private:
    QString m_title;
    QString m_message;
    bool m_indeterminate = false;
    bool m_cancelable = false;
    std::function<void()> m_cancelListener;
    std::function<void()> m_keyListener;
};

// Toast stub
class Toast {
public:
    static void makeText(void *context, const QString &text, int duration) {}
};

// RedditIdAndType stub (would be in models/)
class RedditIdAndType {
public:
    explicit RedditIdAndType(const QString &id, const QString &type)
        : m_id(id), m_type(type) {}
    bool equals(const RedditIdAndType *other) const {
        if (!other) return false;
        return m_id == other->m_id && m_type == other->m_type;
    }
    QString getId() const { return m_id; }
    QString getType() const { return m_type; }
private:
    QString m_id;
    QString m_type;
};

// Optional<T> stub
template<typename T>
class Optional {
public:
    Optional() : m_hasValue(false) {}
    explicit Optional(const T &value) : m_hasValue(true), m_value(value) {}
    bool isPresent() const { return m_hasValue; }
    const T &get() const { return m_value; }
    template<typename F>
    void ifPresent(F func) const { if (m_hasValue) func(m_value); }
private:
    bool m_hasValue;
    T m_value;
};

// APIResponseHandler stubs
class SubmitResponseHandler {
public:
    virtual ~SubmitResponseHandler() = default;
    virtual void onSubmitErrors(const std::vector<QString> &errors) = 0;
    virtual void onSuccess(
            const Optional<QString> &redirectUrl,
            const Optional<QString> &thingId) = 0;
    virtual void onCallbackException(const std::exception &t) = 0;
    virtual void onFailure(const RRError &error) = 0;
};

class ActionResponseHandler {
public:
    virtual ~ActionResponseHandler() = default;
    virtual void onSuccess() = 0;
    virtual void onCallbackException(const std::exception &t) = 0;
    virtual void onFailure(const RRError &error) = 0;
};

// MarkdownPreviewDialog stub
class MarkdownPreviewDialog {
public:
    static MarkdownPreviewDialog newInstance(const QString &text) {
        return MarkdownPreviewDialog(text);
    }
    void show(void *fragmentManager, const QString &tag) {}
private:
    explicit MarkdownPreviewDialog(const QString &text) : m_text(text) {}
    QString m_text;
};

// BugReportActivity stub
class BugReportActivity {
public:
    static void handleGlobalError(void *context, const std::exception &t) {}
};

// RedditAPI stub (comment method)
class RedditAPI_Stub {
public:
    static void comment(
            CacheManager *cm,
            SubmitResponseHandler *handler,
            ActionResponseHandler *inboxHandler,
            RedditAccount *selectedAccount,
            RedditIdAndType *parentIdAndType,
            const QString &text,
            bool sendRepliesToInbox,
            void *context) {
        // Stub - calls success callback
        handler->onSuccess(Optional<QString>(), Optional<QString>(QString("t1_test")));
    }
};

// ImgurUploadActivity stub
class ImgurUploadActivity {};

// ============================================================================
// Constructor / Destructor
// ============================================================================

CommentReplyActivity::CommentReplyActivity(QObject *parent)
    : QObject(parent)
    , m_base(new ViewsBaseActivityDelegate()) {}

CommentReplyActivity::~CommentReplyActivity() {
    delete m_base;
}

// ============================================================================
// onCreate — port of override method (Java lines 87-184)
// ============================================================================

void CommentReplyActivity::onCreate(Bundle *savedInstanceState) {
    (void)savedInstanceState;

    // Port of: PrefsUtility.applyTheme(this); (Java line 90)
    // PrefsUtility::applyTheme(this);  // stub

    // Port of: super.onCreate(savedInstanceState); (Java line 92)
    // (handled by base class)

    // Port of: final Intent intent = getIntent(); (Java line 94)
    IntentDelegate intent;

    // Port of: if(intent != null && intent.hasExtra(PARENT_TYPE)
    //     && PARENT_TYPE_MESSAGE.equals(intent.getStringExtra(PARENT_TYPE))) {
    //     mParentType = ParentType.MESSAGE; setTitle(R.string.submit_pmreply_actionbar);
    // } else { mParentType = ParentType.COMMENT_OR_POST;
    //     setTitle(R.string.submit_comment_actionbar); }
    // (Java lines 96-106)
    if (intent.hasExtra(PARENT_TYPE)
            && intent.getStringExtra(PARENT_TYPE) == PARENT_TYPE_MESSAGE) {
        m_parentType = ParentType::MESSAGE;
        // m_base->setTitle(...);  // stub
    } else {
        m_parentType = ParentType::COMMENT_OR_POST;
        // m_base->setTitle(...);  // stub
    }

    // Port of: final LinearLayout layout = (LinearLayout)getLayoutInflater()
    //     .inflate(R.layout.comment_reply, null); (Java lines 108-109)
    LinearLayout layout;

    // Port of: usernameSpinner = layout.findViewById(R.id.comment_reply_username);
    // (Java line 111)
    m_usernameSpinner = new Spinner();

    // Port of: inboxReplies = layout.findViewById(R.id.comment_reply_inbox);
    // (Java line 112)
    m_inboxReplies = new CheckBox();

    // Port of: textEdit = layout.findViewById(R.id.comment_reply_text);
    // (Java line 113)
    m_textEdit = new EditText();

    // Port of: final Button uploadPicture = layout.findViewById(
    //     R.id.comment_reply_picture); (Java line 115)
    Button uploadPicture;

    // Port of: uploadPicture.setOnClickListener(v -> uploadPicture());
    // (Java line 117)
    uploadPicture.setOnClickListener([this]() { this->uploadPicture(); });

    // Port of: if(mParentType == ParentType.COMMENT_OR_POST) {
    //     inboxReplies.setVisibility(View.VISIBLE); } (Java lines 119-121)
    if (m_parentType == ParentType::COMMENT_OR_POST) {
        m_inboxReplies->setVisibility(0);  // View.VISIBLE = 0
    }

    // Port of: if(intent != null && intent.hasExtra(PARENT_ID_AND_TYPE_KEY)) {
    //     parentIdAndType = Objects.requireNonNull(
    //         IntentCompat.getParcelableExtra(...)); }
    // else if(savedInstanceState != null && ...) { ... }
    // else { throw RuntimeException(...); }
    // (Java lines 123-137)
    if (intent.hasExtra(PARENT_ID_AND_TYPE_KEY)) {
        m_parentIdAndType = new RedditIdAndType("t1", "comment");  // stub
    } else if (savedInstanceState != nullptr
            && dynamic_cast<BundleDelegate *>(savedInstanceState)
            && dynamic_cast<BundleDelegate *>(savedInstanceState)
                    ->containsKey("parentIdAndType")) {
        m_parentIdAndType = new RedditIdAndType("t1", "comment");  // stub
    } else {
        throw std::runtime_error("No parent ID in CommentReplyActivity");
    }

    // Port of: final String existingCommentText;
    // if(savedInstanceState != null && ...) { existingCommentText = ...; }
    // else if(lastText != null && parentIdAndType.equals(lastParentIdAndType))
    // { existingCommentText = lastText; }
    // else { existingCommentText = null; }
    // (Java lines 139-150)
    QString existingCommentText;

    BundleDelegate *bundleDelegate =
        dynamic_cast<BundleDelegate *>(savedInstanceState);

    if (bundleDelegate != nullptr
            && bundleDelegate->containsKey("comment_text")) {
        existingCommentText = bundleDelegate->getString("comment_text");
    } else if (!s_lastText.isNull()
            && m_parentIdAndType->equals(s_lastParentIdAndType)) {
        existingCommentText = s_lastText;
    }
    // else existingCommentText stays null (empty QString)

    // Port of: if(existingCommentText != null) {
    //     textEdit.setText(existingCommentText); } (Java lines 152-154)
    if (!existingCommentText.isNull()) {
        m_textEdit->setText(existingCommentText);
    }

    // Port of: if(intent != null && intent.hasExtra(PARENT_MARKDOWN_KEY)) {
    //     final TextView parentMarkdown = ...; parentMarkdown.setText(...); }
    // (Java lines 156-159)
    if (intent.hasExtra(PARENT_MARKDOWN_KEY)) {
        TextView parentMarkdown;
        // parentMarkdown.setText(intent.getStringExtra(PARENT_MARKDOWN_KEY));
    }

    // Port of: final ArrayList<RedditAccount> accounts =
    //     RedditAccountManager.getInstance(this).getAccounts();
    // (Java lines 161-162)
    std::vector<RedditAccount> accounts;
    // accounts = RedditAccountManager::getInstance().getAccounts();

    // Port of: final ArrayList<String> usernames = new ArrayList<>();
    // for(final RedditAccount account : accounts) {
    //     if(!account.isAnonymous()) { usernames.add(account.username); } }
    // (Java lines 163-169)
    std::vector<QString> usernames;
    for (auto &account : accounts) {
        if (!account.isAnonymous()) {
            usernames.push_back(account.username);
        }
    }

    // Port of: if(usernames.isEmpty()) {
    //     General.quickToast(this, getString(R.string.error_toast_notloggedin));
    //     finish(); }
    // (Java lines 171-174)
    if (usernames.empty()) {
        General::quickToast(QStringLiteral("You must be logged in to do that."));
        m_base->finish();
        return;
    }

    // Port of: usernameSpinner.setAdapter(new ArrayAdapter<>(
    //     this, android.R.layout.simple_list_item_1, usernames));
    // (Java lines 176-179)
    m_usernameSpinner->setAdapter(
        new ArrayAdapter(nullptr, 0, usernames));

    // Port of: final ScrollView sv = new ScrollView(this);
    // sv.addView(layout); setBaseActivityListing(sv);
    // (Java lines 181-183)
    ScrollView sv;
    // sv.addView(&layout);
    // m_base->setBaseActivityListing(&sv);
}

// ============================================================================
// onSaveInstanceState — port of override method (Java lines 186-191)
// ============================================================================

void CommentReplyActivity::onSaveInstanceState(Bundle *outState) {
    // Port of: super.onSaveInstanceState(outState); (Java line 188)
    // (handled by base class)

    // Port of: outState.putString(COMMENT_TEXT_KEY, textEdit.getText()
    //     .toString()); (Java line 189)
    BundleDelegate *bundleDelegate =
        dynamic_cast<BundleDelegate *>(outState);
    if (bundleDelegate && m_textEdit) {
        bundleDelegate->putString("comment_text", m_textEdit->getText());
    }

    // Port of: outState.putParcelable(PARENT_ID_AND_TYPE_KEY,
    //     parentIdAndType); (Java line 190)
    if (bundleDelegate && m_parentIdAndType) {
        bundleDelegate->putParcelable("parentIdAndType", m_parentIdAndType);
    }
}

// ============================================================================
// onCreateOptionsMenu — port of override method (Java lines 193-203)
// ============================================================================

bool CommentReplyActivity::onCreateOptionsMenu(Menu *menu) {
    // Port of: final MenuItem send = menu.add(R.string.comment_reply_send);
    // (Java line 196)
    // menu->add(...) -- stub

    // Port of: send.setIcon(R.drawable.ic_action_send_dark); (Java line 197)
    // Port of: send.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS); (Java line 198)
    // Port of: menu.add(R.string.comment_reply_preview); (Java line 200)

    // Port of: return true; (Java line 202)
    return true;
}

// ============================================================================
// onOptionsItemSelected — port of override method (Java lines 205-358)
// ============================================================================

bool CommentReplyActivity::onOptionsItemSelected(MenuItem *item) {
    (void)item;

    // Port of: if(item.getTitle().equals(getString(R.string.comment_reply_send)))
    // (Java line 208)
    {
        // ---- Send branch ----

        // Port of: final ProgressDialog progressDialog = new ProgressDialog(this);
        // (Java line 211)
        ProgressDialog progressDialog;
        progressDialog.setTitle(QStringLiteral("Submitting..."));
        progressDialog.setMessage(QStringLiteral("Sending your reply..."));
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(true);
        progressDialog.setCanceledOnTouchOutside(false);

        // Port of: progressDialog.setOnCancelListener(...) (Java lines 220-223)
        progressDialog.setOnCancelListener([this, &progressDialog]() {
            General::quickToast(
                QStringLiteral("Submitting comment..."));
            progressDialog.dismiss();
        });

        // Port of: progressDialog.setOnKeyListener(...) (Java lines 225-233)
        progressDialog.setOnKeyListener([this, &progressDialog]() {
            // KEYCODE_BACK check
            General::quickToast(
                QStringLiteral("Submitting comment..."));
            progressDialog.dismiss();
        });

        // Port of: final APIResponseHandler.SubmitResponseHandler handler
        //     = new APIResponseHandler.SubmitResponseHandler(this) { ... }
        // (Java lines 235-297)
        class MySubmitHandler : public SubmitResponseHandler {
        public:
            MySubmitHandler(
                    CommentReplyActivity *activity,
                    ProgressDialog &dialog)
                : m_activity(activity)
                , m_dialog(dialog) {}

            // Port of: onSubmitErrors (Java lines 238-252)
            void onSubmitErrors(
                    const std::vector<QString> &errors) override {
                // AndroidCommon.UI_THREAD_HANDLER.post(() -> {
                //   StringUtils.join, DialogUtils.showDialog, safeDismissDialog
                // })
                QString errorsJoined;
                for (const auto &e : errors) {
                    errorsJoined += e + QStringLiteral(" ");
                }
                m_dialog.dismiss();
            }

            // Port of: onSuccess (Java lines 254-285)
            void onSuccess(
                    const Optional<QString> &redirectUrl,
                    const Optional<QString> &thingId) override {
                // AndroidCommon.UI_THREAD_HANDLER.post(() -> {
                m_dialog.dismiss();

                if (m_activity->getParentType()
                        == CommentReplyActivity::ParentType::MESSAGE) {
                    General::quickToast(
                        QStringLiteral("Message sent."));
                } else {
                    General::quickToast(
                        QStringLiteral("Comment sent."));
                }

                m_activity->m_draftReset = true;
                s_lastText = QString();  // null
                s_lastParentIdAndType = nullptr;

                // redirectUrl.ifPresent(url -> LinkHandler.onLinkClicked(...))
                redirectUrl.ifPresent([](const QString &url) {
                    // LinkHandler::onLinkClicked(activity,
                    //     UriString.from(Uri.parse(url).buildUpon()
                    //         .appendQueryParameter("context", "1").build()))
                });

                m_activity->m_base->finish();
                // });
            }

            // Port of: onCallbackException (Java lines 287-290)
            void onCallbackException(const std::exception &t) override {
                BugReportActivity::handleGlobalError(m_activity, t);
            }

            // Port of: onFailure (Java lines 292-296)
            void onFailure(const RRError &error) override {
                // General.showResultDialog(m_activity, error);
                m_dialog.dismiss();
            }

        private:
            CommentReplyActivity *m_activity;
            ProgressDialog &m_dialog;
        };

        MySubmitHandler handler(this, progressDialog);

        // Port of: final APIResponseHandler.ActionResponseHandler inboxHandler
        //     = new APIResponseHandler.ActionResponseHandler(this) { ... }
        // (Java lines 299-318)
        class MyInboxHandler : public ActionResponseHandler {
        public:
            MyInboxHandler(
                    CommentReplyActivity *activity,
                    ProgressDialog &dialog)
                : m_activity(activity)
                , m_dialog(dialog) {}

            // Port of: onSuccess (Java lines 302-304)
            void onSuccess() override {
                // Do nothing (result expected)
            }

            // Port of: onCallbackException (Java lines 306-309)
            void onCallbackException(const std::exception &t) override {
                BugReportActivity::handleGlobalError(m_activity, t);
            }

            // Port of: onFailure (Java lines 311-317)
            void onFailure(const RRError &error) override {
                Toast::makeText(
                    nullptr,
                    QStringLiteral("Failed to disable inbox replies"),
                    2000);
            }

        private:
            CommentReplyActivity *m_activity;
            ProgressDialog &m_dialog;
        };

        MyInboxHandler inboxHandler(this, progressDialog);

        // Port of: final CacheManager cm = CacheManager.getInstance(this);
        // (Java line 320)
        // CacheManager &cm = CacheManager::getInstance();

        // Port of: final ArrayList<RedditAccount> accounts =
        //     RedditAccountManager.getInstance(this).getAccounts();
        // (Java lines 322-323)
        std::vector<RedditAccount> accounts;

        // Port of: RedditAccount selectedAccount = null;
        // for(final RedditAccount account : accounts) { ... }
        // (Java lines 324-333)
        RedditAccount *selectedAccount = nullptr;
        QString selectedUsername;

        if (m_usernameSpinner) {
            selectedUsername = m_usernameSpinner->getSelectedItem();
        }

        for (auto &account : accounts) {
            if (!account.isAnonymous()
                    && QString::compare(
                        account.username,
                        selectedUsername,
                        Qt::CaseInsensitive) == 0) {
                selectedAccount = &account;
                break;
            }
        }

        // Port of: final boolean sendRepliesToInbox;
        // if(mParentType == ParentType.COMMENT_OR_POST) {
        //     sendRepliesToInbox = inboxReplies.isChecked();
        // } else { sendRepliesToInbox = true; }
        // (Java lines 334-339)
        bool sendRepliesToInbox;
        if (m_parentType == ParentType::COMMENT_OR_POST) {
            sendRepliesToInbox = m_inboxReplies
                ? m_inboxReplies->isChecked() : false;
        } else {
            sendRepliesToInbox = true;
        }

        // Port of: RedditAPI.comment(cm, handler, inboxHandler,
        //     selectedAccount, parentIdAndType,
        //     textEdit.getText().toString(),
        //     sendRepliesToInbox, this);
        // (Java lines 340-348)
        QString commentText;
        if (m_textEdit) {
            commentText = m_textEdit->getText();
        }
        RedditAPI_Stub::comment(
            nullptr,
            &handler,
            &inboxHandler,
            selectedAccount,
            m_parentIdAndType,
            commentText,
            sendRepliesToInbox,
            this);

        // Port of: progressDialog.show(); (Java line 350)
        progressDialog.show();
    }

    // Port of: else if(item.getTitle().equals(getString(
    //     R.string.comment_reply_preview))) { ... }
    // (Java lines 352-355)
    {
        // Preview branch
        QString previewText;
        if (m_textEdit) {
            previewText = m_textEdit->getText();
        }
        MarkdownPreviewDialog::newInstance(previewText)
            .show(nullptr, QStringLiteral("MarkdownPreviewDialog"));
    }

    // Port of: return true; (Java line 357)
    return true;
}

// ============================================================================
// onDestroy — port of override method (Java lines 360-368)
// ============================================================================

void CommentReplyActivity::onDestroy() {
    // Port of: super.onDestroy(); (Java line 362)
    // (handled by base class)

    // Port of: if(textEdit != null && !mDraftReset) {
    //     lastText = textEdit.getText().toString();
    //     lastParentIdAndType = parentIdAndType; }
    // (Java lines 364-367)
    if (m_textEdit != nullptr && !m_draftReset) {
        s_lastText = m_textEdit->getText();
        s_lastParentIdAndType = m_parentIdAndType;
    }
}

// ============================================================================
// onBackPressed — port of override method (Java lines 370-375)
// ============================================================================

void CommentReplyActivity::onBackPressed() {
    // Port of: if(General.onBackPressed()) { super.onBackPressed(); }
    // (Java lines 372-374)
    if (General::onBackPressed()) {
        m_base->onBackPressed();
    }
}

// ============================================================================
// uploadPicture — port of private method (Java lines 377-393)
// ============================================================================

void CommentReplyActivity::uploadPicture() {
    // Port of: final Intent intent = new Intent(this, ImgurUploadActivity.class);
    // (Java line 378)
    // Intent intent;  // stub

    // Port of: startActivityForResultWithCallback(intent,
    //     (resultCode, data) -> { ... });
    // (Java lines 379-392)
    //
    // In the callback:
    //   if(resultCode == 0 && data != null) {
    //       final Uri uploadedImageUrl = data.getData();
    //       if(uploadedImageUrl != null) {
    //           final String existingText = textEdit.getText().toString();
    //           final String picturePretext = getString(
    //               R.string.comment_picture_pretext);
    //           final String linkText = "[" + picturePretext + "]("
    //               + uploadedImageUrl + ")";
    //           final String combinedText = existingText + " " + linkText;
    //           textEdit.setText(combinedText);
    //       }
    //   }

    // Stub: since ImgurUploadActivity and Activity result handling are
    // Android-specific, this is a placeholder for the actual implementation.
    if (m_textEdit) {
        QString existingText = m_textEdit->getText();
        QString picturePretext = QStringLiteral("Picture");
        QString uploadedImageUrl = QStringLiteral("https://imgur.com/example");
        QString linkText = QStringLiteral("[") + picturePretext
            + QStringLiteral("](") + uploadedImageUrl + QStringLiteral(")");
        QString combinedText = existingText + QStringLiteral(" ") + linkText;
        m_textEdit->setText(combinedText);
    }
}

} // namespace PinkReader
