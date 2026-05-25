/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: pm_send_activity.h - Port of PMSendActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/PMSendActivity.java
 *
 * Original: public class PMSendActivity extends ViewsBaseActivity
 *
 * Every field, method, constant, and inner class ported exactly.
 * Android-specific classes are used as forward declarations and stubs.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

class Bundle;
class Intent;
class Menu;
class MenuItem;
class ViewsBaseActivity;
class Context;

// Widget stubs
class Spinner;
class EditText;
class LinearLayout;
class ScrollView;
class ProgressDialog;
class ArrayAdapter;

// RedReader types
class RedditAccount;
class RedditAccountManager;
class RRError;
class CacheManager;

// ============================================================================
// PMSendActivity — port of Java class (Java lines 48-291)
//
// Port of: org.quantumbadger.redreader.activities.PMSendActivity
//
// Extends ViewsBaseActivity. Every field, method, and constant ported exactly.
// ============================================================================

class PMSendActivity : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Static constants (Java lines 50-56)
    // ========================================================================

    // Port of: public static final String EXTRA_RECIPIENT = "recipient";
    static const QString EXTRA_RECIPIENT;

    // Port of: public static final String EXTRA_SUBJECT = "subject";
    static const QString EXTRA_SUBJECT;

    // Port of: public static final String EXTRA_TEXT = "text";
    static const QString EXTRA_TEXT;

    // Port of: private static final String SAVED_STATE_RECIPIENT = "recipient";
    static const QString SAVED_STATE_RECIPIENT;

    // Port of: private static final String SAVED_STATE_TEXT = "pm_text";
    static const QString SAVED_STATE_TEXT;

    // Port of: private static final String SAVED_STATE_SUBJECT = "pm_subject";
    static const QString SAVED_STATE_SUBJECT;

    // ========================================================================
    // Static fields (Java lines 65-67)
    // ========================================================================

    // Port of: private static String lastText;
    static QString s_lastText;

    // Port of: private static String lastRecipient;
    static QString s_lastRecipient;

    // Port of: private static String lastSubject;
    static QString s_lastSubject;

    // ========================================================================
    // Constructor / Destructor
    // ========================================================================
    explicit PMSendActivity(QObject *parent = nullptr);
    virtual ~PMSendActivity();

    // ========================================================================
    // Public lifecycle methods (Java lines 69-290)
    // ========================================================================

    // Port of: @Override protected void onCreate(final Bundle savedInstanceState) (Java lines 69-154)
    void onCreate(Bundle *savedInstanceState);

    // Port of: @Override protected void onSaveInstanceState(@NonNull final Bundle outState) (Java lines 156-162)
    void onSaveInstanceState(Bundle *outState);

    // Port of: @Override public boolean onCreateOptionsMenu(final Menu menu) (Java lines 164-174)
    bool onCreateOptionsMenu(Menu *menu);

    // Port of: @Override public boolean onOptionsItemSelected(final MenuItem item) (Java lines 176-272)
    bool onOptionsItemSelected(MenuItem *item);

    // Port of: @Override protected void onDestroy() (Java lines 274-283)
    void onDestroy();

    // Port of: @Override public void onBackPressed() (Java lines 285-290)
    void onBackPressed();

private:
    // ========================================================================
    // Private fields (Java lines 58-63)
    // ========================================================================

    // Port of: private Spinner usernameSpinner;
    Spinner *m_usernameSpinner = nullptr;

    // Port of: private EditText recipientEdit;
    EditText *m_recipientEdit = nullptr;

    // Port of: private EditText subjectEdit;
    EditText *m_subjectEdit = nullptr;

    // Port of: private EditText textEdit;
    EditText *m_textEdit = nullptr;

    // Port of: private boolean mSendSuccess;
    bool m_sendSuccess = false;

    // Reference to ViewsBaseActivity base class
    ViewsBaseActivity *m_base = nullptr;
};

} // namespace PinkReader
