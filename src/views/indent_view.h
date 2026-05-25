/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: indent_view.h - Port of IndentView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/IndentView.java
 *
 * Original: class IndentView extends View
 *
 * Every field, method, constant, and inner class ported exactly.
 *
 * Draws the left margin for comments based on the RedditPreparedComment#indentation number.
 */

#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <vector>

namespace PinkReader {

// ============================================================================
// Forward declarations
// ============================================================================

class Context;
class AttributeSet;

// ============================================================================
// View base class stub (port of android.view.View)
// ============================================================================

class View : public QWidget {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    View(Context *context, QWidget *parent = nullptr);
    View(Context *context, AttributeSet *attrs, int defStyleAttr, QWidget *parent = nullptr);

    // Port of: setBackgroundColor(int color)
    void setBackgroundColor(int color);

    // Port of: getMeasuredHeight()
    int getMeasuredHeight() const;

    // Port of: getWidth()
    int getWidth() const override;

    // Port of: getHeight()
    int getHeight() const override;

    // Port of: invalidate()
    void invalidateView();

    // Port of: requestLayout()
    void requestLayout();

    // Port of: getLayoutParams()
    class ViewGroupLayoutParams *getLayoutParams() const;
    void setLayoutParams(ViewGroupLayoutParams *params);

protected:
    void paintEvent(QPaintEvent *event) override;

    int m_measuredHeight = 0;
    int m_backgroundColor = 0;
    ViewGroupLayoutParams *m_layoutParams = nullptr;
    QColor m_bgColor;

signals:
    void needsRedraw();
    void layoutRequested();
};

// LayoutParams stub
class ViewGroupLayoutParams {
public:
    int width = 0;
    int height = 0;
};

// ============================================================================
// IndentView — port of Java class (Java lines 34-130)
//
// Port of: org.quantumbadger.redreader.views.IndentView
//
// Extends View. Draws indent lines for comment threads.
// Every field, method, constant ported exactly.
// ============================================================================

class IndentView : public View {
    Q_OBJECT

public:
    // ========================================================================
    // Constructors (Java lines 46-85)
    // ========================================================================

    // Port of: public IndentView(final Context context) (Java lines 46-48)
    //     this(context, null);
    explicit IndentView(Context *context, QWidget *parent = nullptr);

    // Port of: public IndentView(final Context context, @Nullable final AttributeSet attrs) (Java lines 50-52)
    //     this(context, attrs, 0);
    IndentView(Context *context, AttributeSet *attrs, QWidget *parent = nullptr);

    // Port of: public IndentView(final Context context, @Nullable final AttributeSet attrs,
    //     final int defStyleAttr) (Java lines 54-85)
    IndentView(Context *context, AttributeSet *attrs, int defStyleAttr, QWidget *parent = nullptr);

    virtual ~IndentView();

    // ========================================================================
    // Public methods
    // ========================================================================

    // Port of: public void setIndentation(final int indent) (Java lines 119-129)
    void setIndentation(int indent);

protected:
    // ========================================================================
    // Override (Java lines 87-112)
    //
    // Port of: @Override protected void onDraw(final Canvas canvas)
    // ========================================================================
    void onDraw(QPainter *painter);

private:
    // ========================================================================
    // Private fields — exact match (Java lines 36-44)
    // ========================================================================

    // Port of: private final Paint mPaint = new Paint(); (Java line 36)
    QPen m_pen;
    QBrush m_brush;

    // Port of: private int mIndent; (Java line 37)
    int m_indent = 0;

    // Port of: private final int mPixelsPerIndent; (Java line 39)
    int m_pixelsPerIndent = 0;

    // Port of: private final int mHalfALine; (Java line 40)
    int m_halfALine = 0;

    // Port of: private final boolean mPrefDrawLines; (Java line 42)
    bool m_prefDrawLines = false;

    // Port of: private float[] mLineBuffer; (Java line 44)
    std::vector<float> m_lineBuffer;

    // Port of: Paint color from attr (Java lines 65-78)
    int m_indentBackgroundCol = 0;
    int m_indentLineCol = 0;

    // ========================================================================
    // Common init logic extracted from constructors (Java lines 61-85)
    // ========================================================================
    void commonInit(Context *context);
};

} // namespace PinkReader
