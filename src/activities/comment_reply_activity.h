/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: comment_reply_activity.h - Port of RedReader's CommentReplyActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/CommentReplyActivity.java
 *
 * Original: public class CommentReplyActivity extends ViewsBaseActivity
 *
 * Every field, method, constant, and inner class ported exactly.
 * Android-specific classes (Activity, Fragment, View, Spinner, EditText,
 * CheckBox, Menu, MenuItem, etc.) are used as forward declarations and stubs.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <memory>
#include <optional>
#include <vector>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

class Bundle;                         // Android os.Bundle stub
class Intent;                         // Android content.Intent stub
class Menu;                           // Android view.Menu stub
class MenuItem;                       // Android view.MenuItem stub
class ViewsBaseActivity;              // Base activity class
class Context;                        // Android content.Context stub

// Widget stubs
class Spinner;
class EditText;
class CheckBox;
class Button;
class LinearLayout;
class ScrollView;
class TextView;
class ProgressDialog;
class ArrayAdapter;
class Toast;

// RedReader types
class UriString;                      // Port of common.UriString
class RedditIdAndType;                // Port of reddit.kthings.RedditIdAndType
class RedditAccount;                   // Port of account.RedditAccount
class RedditAccountManager;           // Port of account.RedditAccountManager
class RRError;                         // Port of common.RRError
class CacheManager;                   // Port of cache.CacheManager
class Optional_String;                 // Port of common.Optional<String>

// API response handlers
class APIResponseHandler_SubmitResponseHandler;
class APIResponseHandler_ActionResponseHandler;

// ============================================================================
// CommentReplyActivity — port of Java class (Java line 63)
//
// Port of: org.quantumbadger.redreader.activities.CommentReplyActivity
//
// Handles both comment replies and private message replies.
// Extends ViewsBaseActivity.
// ============================================================================

class CommentReplyActivity : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // ParentType — port of inner enum (Java lines 65-67)
    //
    // Port of:
    //   private enum ParentType {
    //       MESSAGE, COMMENT_OR_POST
    //   }
    // ========================================================================
    enum class ParentType {
        MESSAGE,
        COMMENT_OR_POST
    };

    // ========================================================================
    // Static constants — port of Java fields (Java lines 80-85)
    // ========================================================================

    // Port of: public static final String PARENT_TYPE = "parentType"; (Java line 80)
    static const QString PARENT_TYPE;

    // Port of: public static final String PARENT_TYPE_MESSAGE = "parentTypeMessage";
    // (Java line 81)
    static const QString PARENT_TYPE_MESSAGE;

    // Port of: public static final String PARENT_ID_AND_TYPE_KEY = "parentIdAndType";
    // (Java line 83)
    static const QString PARENT_ID_AND_TYPE_KEY;

    // Port of: public static final String PARENT_MARKDOWN_KEY = "parent_markdown";
    // (Java line 84)
    static const QString PARENT_MARKDOWN_KEY;

    // ========================================================================
    // Private static field — port of Java field (Java line 85)
    // ========================================================================
    // Port of: private static final String COMMENT_TEXT_KEY = "comment_text";
    // (Java line 85)
    static const QString COMMENT_TEXT_KEY;

    // ========================================================================
    // Constructor / Lifecycle
    // ========================================================================
    explicit CommentReplyActivity(QObject *parent = nullptr);
    virtual ~CommentReplyActivity();

    // ========================================================================
    // onCreate — port of override method (Java lines 87-184)
    //
    // Port of:
    //   @Override protected void onCreate(final Bundle savedInstanceState)
    //
    // Initializes the comment reply form:
    //   - Determines parent type (message vs comment/post)
    //   - Sets up username spinner, text edit, inbox checkbox
    //   - Restores draft text from savedInstanceState or lastText
    //   - Populates username spinner from Reddit accounts
    // ========================================================================
    void onCreate(Bundle *savedInstanceState);

    // ========================================================================
    // onSaveInstanceState — port of override method (Java lines 186-191)
    //
    // Port of:
    //   @Override protected void onSaveInstanceState(@NonNull Bundle outState)
    //
    // Saves current text and parent ID/type for state restoration.
    // ========================================================================
    void onSaveInstanceState(Bundle *outState);

    // ========================================================================
    // onCreateOptionsMenu — port of override method (Java lines 193-203)
    //
    // Port of:
    //   @Override public boolean onCreateOptionsMenu(final Menu menu)
    //
    // Adds "Send" and "Preview" menu items.
    // ========================================================================
    bool onCreateOptionsMenu(Menu *menu);

    // ========================================================================
    // onOptionsItemSelected — port of override method (Java lines 205-358)
    //
    // Port of:
    //   @Override public boolean onOptionsItemSelected(final MenuItem item)
    //
    // Handles Send (submits reply via RedditAPI.comment) or Preview.
    // ========================================================================
    bool onOptionsItemSelected(MenuItem *item);

    // ========================================================================
    // onDestroy — port of override method (Java lines 360-368)
    //
    // Port of:
    //   @Override protected void onDestroy()
    //
    // Saves draft text and parent ID for restoration if not reset.
    // ========================================================================
    void onDestroy();

    // ========================================================================
    // onBackPressed — port of override method (Java lines 370-375)
    //
    // Port of:
    //   @Override public void onBackPressed()
    //
    // Uses General.onBackPressed() for double-back-to-exit.
    // ========================================================================
    void onBackPressed();

    // ========================================================================
    // Accessors for UI widgets (needed by submit logic)
    // ========================================================================
    Spinner *getUsernameSpinner() const { return m_usernameSpinner; }
    EditText *getTextEdit() const { return m_textEdit; }
    CheckBox *getInboxReplies() const { return m_inboxReplies; }
    RedditIdAndType *getParentIdAndType() const { return m_parentIdAndType; }
    ParentType getParentType() const { return m_parentType; }

signals:
    // Qt signal for finishing activity
    void finished();

private:
    // ========================================================================
    // Private fields — port of Java fields (Java lines 69-78)
    // ========================================================================

    // Port of: private Spinner usernameSpinner; (Java line 69)
    Spinner *m_usernameSpinner = nullptr;

    // Port of: private EditText textEdit; (Java line 70)
    EditText *m_textEdit = nullptr;

    // Port of: private CheckBox inboxReplies; (Java line 71)
    CheckBox *m_inboxReplies = nullptr;

    // Port of: private RedditIdAndType parentIdAndType = null; (Java line 72)
    RedditIdAndType *m_parentIdAndType = nullptr;

    // Port of: private ParentType mParentType; (Java line 74)
    ParentType m_parentType = ParentType::COMMENT_OR_POST;

    // Port of: private boolean mDraftReset = false; (Java line 76)
    bool m_draftReset = false;

    // Port of: private static String lastText; (Java line 77)
    static QString s_lastText;

    // Port of: private static RedditIdAndType lastParentIdAndType; (Java line 78)
    static RedditIdAndType *s_lastParentIdAndType;

    // ========================================================================
    // uploadPicture — port of private method (Java lines 377-393)
    //
    // Port of:
    //   private void uploadPicture()
    //
    // Starts ImgurUploadActivity and inserts returned URL into text edit.
    // ========================================================================
    void uploadPicture();

    // ========================================================================
    // Reference to ViewsBaseActivity base class
    // ========================================================================
    ViewsBaseActivity *m_base = nullptr;
};

} // namespace PinkReader
