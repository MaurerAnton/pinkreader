/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: linkified_text_view.h - Port of LinkifiedTextView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/LinkifiedTextView.java
 *
 * Original: public class LinkifiedTextView extends AppCompatTextView
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QWidget>
#include <QString>
#include <QMouseEvent>
#include <memory>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

class AppCompatActivity;
class MotionEvent;
class Layout;
class ClickableSpan;

// Stub for AppCompatTextView (Android widget class)
// In C++, this maps to a QWidget with text display capabilities
class AppCompatTextView : public QWidget {
    Q_OBJECT

public:
    explicit AppCompatTextView(QWidget *parent = nullptr);

    // Port of: getText() — returns CharSequence
    QString getText() const;
    void setText(const QString &text);

    // Port of: getLayout()
    // In Android, Layout is the text layout engine. Returns a Layout-like object
    class TextLayout *getLayout() const;

    // Port of: getTotalPaddingLeft/Right/Top/Bottom
    int getTotalPaddingLeft() const;
    int getTotalPaddingTop() const;

    // Port of: getScrollX/Y
    int getScrollX() const;
    int getScrollY() const;

protected:
    QString m_text;

signals:
    void linkClicked(int offset);
};

// ============================================================================
// LinkifiedTextView — port of Java class (Java lines 30-97)
//
// Port of: org.quantumbadger.redreader.views.LinkifiedTextView
//
// Extends AppCompatTextView. Every field, method, constant ported exactly.
// ============================================================================

class LinkifiedTextView : public AppCompatTextView {
    Q_OBJECT

public:
    // ========================================================================
    // Constructor (Java lines 34-37)
    //
    // Port of: public LinkifiedTextView(final AppCompatActivity activity)
    //     super(activity); mActivity = activity;
    // ========================================================================
    explicit LinkifiedTextView(AppCompatActivity *activity, QWidget *parent = nullptr);

    virtual ~LinkifiedTextView();

    // ========================================================================
    // Public methods
    // ========================================================================

    // Port of: public AppCompatActivity getActivity() (Java lines 39-41)
    AppCompatActivity *getActivity() const;

protected:
    // ========================================================================
    // Override (Java lines 43-96)
    //
    // Port of: @SuppressLint("ClickableViewAccessibility")
    //     @Override public boolean onTouchEvent(final MotionEvent event)
    // ========================================================================
    bool onTouchEvent(MotionEvent *event);

private:
    // ========================================================================
    // Private fields (Java line 32)
    // ========================================================================

    // Port of: private final AppCompatActivity mActivity; (Java line 32)
    AppCompatActivity *m_activity = nullptr;
};

} // namespace PinkReader
