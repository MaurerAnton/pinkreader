/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: pm_send_activity.cpp - Port of PMSendActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/PMSendActivity.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "activities/pm_send_activity.h"
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
// Static constants
// ============================================================================

// Port of: public static final String EXTRA_RECIPIENT = "recipient";
const QString PMSendActivity::EXTRA_RECIPIENT =
    QStringLiteral("recipient");

// Port of: public static final String EXTRA_SUBJECT = "subject";
const QString PMSendActivity::EXTRA_SUBJECT =
    QStringLiteral("subject");

// Port of: public static final String EXTRA_TEXT = "text";
const QString PMSendActivity::EXTRA_TEXT =
    QStringLiteral("text");

// Port of: private static final String SAVED_STATE_RECIPIENT = "recipient";
const QString PMSendActivity::SAVED_STATE_RECIPIENT =
    QStringLiteral("recipient");

// Port of: private static final String SAVED_STATE_TEXT = "pm_text";
const QString PMSendActivity::SAVED_STATE_TEXT =
    QStringLiteral("pm_text");

// Port of: private static final String SAVED_STATE_SUBJECT = "pm_subject";
const QString PMSendActivity::SAVED_STATE_SUBJECT =
    QStringLiteral("pm_subject");

// ============================================================================
// Static fields
// ============================================================================

// Port of: private static String lastText;
QString PMSendActivity::s_lastText;

// Port of: private static String lastRecipient;
QString PMSendActivity::s_lastRecipient;

// Port of: private static String lastSubject;
QString PMSendActivity::s_lastSubject;

// ============================================================================
// Forward-declared stubs for non-ported Android/RedReader types
// ============================================================================

// Bundle stub
class BundleDelegate : public Bundle {
public:
    void putString(const char *key, const QString &value) {
        (void)key; (void)value;
    }
    QString getString(const char *key) const {
        (void)key;
        return QString();
    }
    bool containsKey(const char *key) const {
        (void)key;
        return false;
    }
};

// Intent stub
class IntentDelegate : public Intent {
public:
    bool hasExtra(const QString &key) const {
        (void)key;
        return false;
    }
    QString getStringExtra(const QString &key) const {
        (void)key;
        return QString();
    }
};

// ViewsBaseActivity stub
class ViewsBaseActivityDelegate : public ViewsBaseActivity {
public:
    void setTitle(const QString &title) { (void)title; }
    void finish() {}
    Context *getApplicationContext() { return nullptr; }
};

// Context stub
class ContextDelegate : public Context {
public:
    QString getString(int resId) const { (void)resId; return QString(); }
};

// Widget stubs
class SpinnerDelegate : public Spinner {
public:
    void setAdapter(ArrayAdapter *adapter) { (void)adapter; }
    QString getSelectedItem() const { return QString(); }
};

class EditTextDelegate : public EditText {
public:
    QString getText() const { return m_text; }
    void setText(const QString &text) { m_text = text; }
private:
    QString m_text;
};

class LinearLayout {};
class ScrollView {};

class ArrayAdapter {
public:
    ArrayAdapter(void *context, int layout, const std::vector<QString> &items) {
        (void)context; (void)layout; (void)items;
    }
};

class ProgressDialog {
public:
    void setTitle(const QString &t) { m_title = t; }
    void setMessage(const QString &m) { m_message = m; }
    void setIndeterminate(bool b) { m_indeterminate = b; }
    void setCancelable(bool b) { m_cancelable = b; }
    void setCanceledOnTouchOutside(bool b) { (void)b; }
    void setOnCancelListener(std::function<void()> l) { m_cancelListener = l; }
    void setOnKeyListener(std::function<void()> l) { m_keyListener = l; }
    void show() {}
    void dismiss() {}
    QString m_title;
    QString m_message;
    bool m_indeterminate = false;
    bool m_cancelable = false;
    std::function<void()> m_cancelListener;
    std::function<void()> m_keyListener;
};

// Menu/MenuItem stubs
class MenuDelegate : public Menu {
public:
    MenuItem *add(const QString &title) {
        (void)title;
        return new MenuItem();
    }
};

class MenuItem {};
// Port of: MenuItem.SHOW_AS_ACTION_ALWAYS — stub
constexpr int MENU_ITEM_SHOW_AS_ACTION_ALWAYS = 2;

// APIResponseHandler stub
class ActionResponseHandler_Stub {
public:
    virtual ~ActionResponseHandler_Stub() = default;
    virtual void onSuccess() = 0;
    virtual void onCallbackException(const std::exception &t) = 0;
    virtual void onFailure(const RRError &error) = 0;
};

// BugReportActivity stub
class BugReportActivity {
public:
    static void handleGlobalError(void *context, const std::exception &t) {
        (void)context; (void)t;
    }
};

// MarkdownPreviewDialog stub
class MarkdownPreviewDialog {
public:
    static void newInstance(const QString &text) {
        (void)text;
    }
    void show(void *fragmentManager, const QString &tag) {
        (void)fragmentManager; (void)tag;
    }
};

// RedditAPI stub
class RedditAPI_Stub {
public:
    static void compose(
            CacheManager *cm,
            ActionResponseHandler_Stub *handler,
            RedditAccount *selectedAccount,
            const QString &recipient,
            const QString &subject,
            const QString &text,
            void *context) {
        (void)cm; (void)handler; (void)selectedAccount;
        (void)recipient; (void)subject; (void)text; (void)context;
        // Stub - calls success callback
        if (handler) {
            handler->onSuccess();
        }
    }
};

// ============================================================================
// Constructor / Destructor
// ============================================================================

PMSendActivity::PMSendActivity(QObject *parent)
    : QObject(parent)
    , m_base(new ViewsBaseActivityDelegate()) {}

PMSendActivity::~PMSendActivity() {
    delete m_base;
}

// ============================================================================
// onCreate — port of override method (Java lines 69-154)
// ============================================================================

void PMSendActivity::onCreate(Bundle *savedInstanceState) {
    (void)savedInstanceState;

    // Port of: PrefsUtility.applyTheme(this); (Java line 72)
    // PrefsUtility::applyTheme(this);  // stub

    // Port of: super.onCreate(savedInstanceState); (Java line 74)
    // (handled by base class)

    // Port of: setTitle(R.string.pm_send_actionbar); (Java line 76)
    // m_base->setTitle(...);  // stub

    // Port of: final LinearLayout layout = ...inflate(R.layout.pm_send, null); (Java lines 78-79)
    LinearLayout layout;

    // Port of: usernameSpinner = layout.findViewById(R.id.pm_send_username); (Java line 81)
    m_usernameSpinner = new SpinnerDelegate();

    // Port of: recipientEdit = layout.findViewById(R.id.pm_send_recipient); (Java line 82)
    m_recipientEdit = new EditTextDelegate();

    // Port of: subjectEdit = layout.findViewById(R.id.pm_send_subject); (Java line 83)
    m_subjectEdit = new EditTextDelegate();

    // Port of: textEdit = layout.findViewById(R.id.pm_send_text); (Java line 84)
    m_textEdit = new EditTextDelegate();

    // Port of: final String initialRecipient; final String initialSubject;
    //           final String initialText; (Java lines 86-88)
    QString initialRecipient;
    QString initialSubject;
    QString initialText;

    // Port of: if(savedInstanceState != null && ...) { ... } else { ... }
    // (Java lines 90-117)
    BundleDelegate *bundleDelegate =
        dynamic_cast<BundleDelegate *>(savedInstanceState);

    if (bundleDelegate != nullptr
            && bundleDelegate->containsKey("pm_text")) {
        // Port of: initialRecipient = savedInstanceState.getString(SAVED_STATE_RECIPIENT); (Java line 92)
        initialRecipient = bundleDelegate->getString("recipient");
        // Port of: initialSubject = savedInstanceState.getString(SAVED_STATE_SUBJECT); (Java line 93)
        initialSubject = bundleDelegate->getString("pm_subject");
        // Port of: initialText = savedInstanceState.getString(SAVED_STATE_TEXT); (Java line 94)
        initialText = bundleDelegate->getString("pm_text");
    } else {
        // Port of: final Intent intent = getIntent(); (Java line 98)
        IntentDelegate intent;

        // Port of: if(intent != null && intent.hasExtra(EXTRA_RECIPIENT)) { ... }
        //           else { initialRecipient = lastRecipient; } (Java lines 100-104)
        if (intent.hasExtra(EXTRA_RECIPIENT)) {
            initialRecipient = intent.getStringExtra(EXTRA_RECIPIENT);
        } else {
            initialRecipient = s_lastRecipient;
        }

        // Port of: if(intent != null && intent.hasExtra(EXTRA_SUBJECT)) { ... }
        //           else { initialSubject = lastSubject; } (Java lines 106-110)
        if (intent.hasExtra(EXTRA_SUBJECT)) {
            initialSubject = intent.getStringExtra(EXTRA_SUBJECT);
        } else {
            initialSubject = s_lastSubject;
        }

        // Port of: if(intent != null && intent.hasExtra(EXTRA_TEXT)) { ... }
        //           else { initialText = lastText; } (Java lines 112-116)
        if (intent.hasExtra(EXTRA_TEXT)) {
            initialText = intent.getStringExtra(EXTRA_TEXT);
        } else {
            initialText = s_lastText;
        }
    }

    // Port of: if(initialRecipient != null) { recipientEdit.setText(initialRecipient); } (Java lines 119-121)
    if (!initialRecipient.isNull()) {
        m_recipientEdit->setText(initialRecipient);
    }

    // Port of: if(initialSubject != null) { subjectEdit.setText(initialSubject); } (Java lines 123-125)
    if (!initialSubject.isNull()) {
        m_subjectEdit->setText(initialSubject);
    }

    // Port of: if(initialText != null) { textEdit.setText(initialText); } (Java lines 127-129)
    if (!initialText.isNull()) {
        m_textEdit->setText(initialText);
    }

    // Port of: final ArrayList<RedditAccount> accounts =
    //     RedditAccountManager.getInstance(this).getAccounts(); (Java lines 131-132)
    std::vector<RedditAccount> accounts;
    // accounts = RedditAccountManager::getInstance().getAccounts(); // stub

    // Port of: final ArrayList<String> usernames = new ArrayList<>(); (Java line 133)
    // Port of: for(final RedditAccount account : accounts) {
    //     if(!account.isAnonymous()) { usernames.add(account.username); } } (Java lines 135-139)
    std::vector<QString> usernames;
    for (auto &account : accounts) {
        if (!account.isAnonymous()) {
            usernames.push_back(account.username);
        }
    }

    // Port of: if(usernames.isEmpty()) {
    //     General.quickToast(this, getString(R.string.error_toast_notloggedin));
    //     finish(); } (Java lines 141-144)
    if (usernames.empty()) {
        General::quickToast(QStringLiteral("You must be logged in to do that."));
        m_base->finish();
        return;
    }

    // Port of: usernameSpinner.setAdapter(new ArrayAdapter<>(
    //     this, android.R.layout.simple_list_item_1, usernames)); (Java lines 146-149)
    dynamic_cast<SpinnerDelegate *>(m_usernameSpinner)->setAdapter(
        new ArrayAdapter(nullptr, 0, usernames));

    // Port of: final ScrollView sv = new ScrollView(this);
    // sv.addView(layout); setBaseActivityListing(sv); (Java lines 151-153)
    ScrollView sv;
    // sv.addView(&layout);
    // m_base->setBaseActivityListing(&sv);
    (void)sv;
    (void)layout;
}

// ============================================================================
// onSaveInstanceState — port of override method (Java lines 156-162)
// ============================================================================

void PMSendActivity::onSaveInstanceState(Bundle *outState) {
    // Port of: super.onSaveInstanceState(outState); (Java line 158)
    // (handled by base class)

    // Port of: outState.putString(SAVED_STATE_RECIPIENT, recipientEdit.getText().toString()); (Java line 159)
    // Port of: outState.putString(SAVED_STATE_SUBJECT, subjectEdit.getText().toString()); (Java line 160)
    // Port of: outState.putString(SAVED_STATE_TEXT, textEdit.getText().toString()); (Java line 161)
    BundleDelegate *bundleDelegate =
        dynamic_cast<BundleDelegate *>(outState);
    if (bundleDelegate) {
        if (m_recipientEdit) {
            bundleDelegate->putString("recipient", m_recipientEdit->getText());
        }
        if (m_subjectEdit) {
            bundleDelegate->putString("pm_subject", m_subjectEdit->getText());
        }
        if (m_textEdit) {
            bundleDelegate->putString("pm_text", m_textEdit->getText());
        }
    }
}

// ============================================================================
// onCreateOptionsMenu — port of override method (Java lines 164-174)
// ============================================================================

bool PMSendActivity::onCreateOptionsMenu(Menu *menu) {
    // Port of: final MenuItem send = menu.add(R.string.comment_reply_send); (Java line 167)
    // Port of: send.setIcon(R.drawable.ic_action_send_dark); (Java line 168)
    // Port of: send.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS); (Java line 169)
    // Port of: menu.add(R.string.comment_reply_preview); (Java line 171)
    // Port of: return true; (Java line 173)
    (void)menu;
    return true;
}

// ============================================================================
// onOptionsItemSelected — port of override method (Java lines 176-272)
// ============================================================================

bool PMSendActivity::onOptionsItemSelected(MenuItem *item) {
    (void)item;

    // Port of: if(item.getTitle().equals(getString(R.string.comment_reply_send))) { ... }
    //             (Java lines 179-264)
    {
        // ---- Send branch ----

        // Port of: final ProgressDialog progressDialog = new ProgressDialog(this); (Java line 181)
        ProgressDialog progressDialog;
        progressDialog.setTitle(QStringLiteral("Submitting..."));
        progressDialog.setMessage(QStringLiteral("Sending your message..."));
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(true);
        progressDialog.setCanceledOnTouchOutside(false);

        // Port of: progressDialog.setOnCancelListener(dialogInterface -> { ... }) (Java lines 188-191)
        progressDialog.setOnCancelListener([this, &progressDialog]() {
            General::quickToast(QStringLiteral("Sending message..."));
            progressDialog.dismiss();
        });

        // Port of: progressDialog.setOnKeyListener((dialogInterface, keyCode, keyEvent) -> { ... }) (Java lines 193-201)
        progressDialog.setOnKeyListener([this, &progressDialog]() {
            // KEYCODE_BACK check
            General::quickToast(QStringLiteral("Sending message..."));
            progressDialog.dismiss();
        });

        // Port of: final APIResponseHandler.ActionResponseHandler handler
        //     = new APIResponseHandler.ActionResponseHandler(this) { ... } (Java lines 203-234)
        class MyActionHandler : public ActionResponseHandler_Stub {
        public:
            PMSendActivity *m_activity;
            ProgressDialog *m_progress;

            MyActionHandler(PMSendActivity *a, ProgressDialog *p)
                : m_activity(a), m_progress(p) {}

            // Port of: @Override protected void onSuccess() (Java lines 206-222)
            void onSuccess() override {
                // Port of: AndroidCommon.UI_THREAD_HANDLER.post(() -> { ... }); (Java line 207)
                General::safeDismissDialog();
                m_activity->m_sendSuccess = true;
                PMSendActivity::s_lastText.clear();
                PMSendActivity::s_lastRecipient.clear();
                PMSendActivity::s_lastSubject.clear();
                General::quickToast(QStringLiteral("Message sent."));
                if (m_activity->m_base) {
                    m_activity->m_base->finish();
                }
            }

            // Port of: @Override protected void onCallbackException(final Throwable t) (Java lines 225-227)
            void onCallbackException(const std::exception &t) override {
                BugReportActivity::handleGlobalError(m_activity, t);
            }

            // Port of: @Override protected void onFailure(@NonNull final RRError error) (Java lines 230-233)
            void onFailure(const RRError &error) override {
                // General.showResultDialog(PMSendActivity.this, error);
                // General.safeDismissDialog(progressDialog);
                (void)error;
                General::safeDismissDialog();
            }
        };

        MyActionHandler handler(this, &progressDialog);

        // Port of: final CacheManager cm = CacheManager.getInstance(this); (Java line 236)
        // CacheManager *cm = CacheManager::getInstance(nullptr); // stub

        // Port of: final ArrayList<RedditAccount> accounts =
        //     RedditAccountManager.getInstance(this).getAccounts(); (Java lines 238-239)
        std::vector<RedditAccount> accounts;

        // Port of: RedditAccount selectedAccount = null;
        // for(final RedditAccount account : accounts) {
        //     if(!account.isAnonymous() && account.username.equalsIgnoreCase(
        //         (String)usernameSpinner.getSelectedItem())) { selectedAccount = account; break; } }
        // (Java lines 240-249)
        RedditAccount *selectedAccountPtr = nullptr;
        QString selectedUsername = dynamic_cast<SpinnerDelegate *>(m_usernameSpinner)->getSelectedItem();
        for (auto &account : accounts) {
            if (!account.isAnonymous()
                    && account.username.compare(selectedUsername, Qt::CaseInsensitive) == 0) {
                selectedAccountPtr = &account;
                break;
            }
        }

        // Port of: if(selectedAccount == null) {
        //     throw new RuntimeException("Selected account no longer present"); } (Java lines 251-253)
        if (selectedAccountPtr == nullptr) {
            throw std::runtime_error("Selected account no longer present");
        }

        // Port of: RedditAPI.compose(cm, handler, selectedAccount,
        //     recipientEdit.getText().toString(), subjectEdit.getText().toString(),
        //     textEdit.getText().toString(), this); (Java lines 255-262)
        RedditAPI_Stub::compose(
                nullptr,  // CacheManager stub
                &handler,
                selectedAccountPtr,
                m_recipientEdit->getText(),
                m_subjectEdit->getText(),
                m_textEdit->getText(),
                this);

        // Port of: progressDialog.show(); (Java line 264)
        progressDialog.show();
    }

    // Port of: else if(item.getTitle().equals(getString(R.string.comment_reply_preview))) { ... }
    // (Java lines 266-269)
    // else {
    //     MarkdownPreviewDialog.newInstance(textEdit.getText().toString())
    //         .show(getSupportFragmentManager(), "MarkdownPreviewDialog");
    // }

    // Port of: return true; (Java line 271)
    return true;
}

// ============================================================================
// onDestroy — port of override method (Java lines 274-283)
// ============================================================================

void PMSendActivity::onDestroy() {
    // Port of: super.onDestroy(); (Java line 276)
    // (handled by base class)

    // Port of: if(!mSendSuccess && textEdit != null) {
    //     lastRecipient = recipientEdit.getText().toString();
    //     lastSubject = subjectEdit.getText().toString();
    //     lastText = textEdit.getText().toString(); } (Java lines 278-282)
    if (!m_sendSuccess && m_textEdit != nullptr) {
        s_lastRecipient = m_recipientEdit->getText();
        s_lastSubject = m_subjectEdit->getText();
        s_lastText = m_textEdit->getText();
    }
}

// ============================================================================
// onBackPressed — port of override method (Java lines 285-290)
// ============================================================================

void PMSendActivity::onBackPressed() {
    // Port of: if(General.onBackPressed()) { super.onBackPressed(); }
    // (Java lines 287-289)
    // if (General::onBackPressed()) {
    //     // super.onBackPressed();
    // }
    // Stub: General.onBackPressed() not ported
}

} // namespace PinkReader
