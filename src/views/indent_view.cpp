/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: indent_view.cpp - Port of IndentView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/IndentView.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "views/indent_view.h"

#include <QPainter>
#include <QPen>
#include <cmath>

namespace PinkReader {

// ============================================================================
// Stubs for non-ported types
// ============================================================================

// Context stub (Android context)
class Context {
public:
    // Port of: context.obtainStyledAttributes(int[])
    class TypedArray {
    public:
        // Port of: attr.getColor(index, defaultColor)
        int getColor(int index, int defaultColor) const {
            (void)index;
            return defaultColor;
        }

        // Port of: attr.recycle()
        void recycle() {}
    };

    TypedArray *obtainStyledAttributes(int *attrs) {
        (void)attrs;
        return new TypedArray();
    }
};

// AttributeSet stub
class AttributeSet {
};

// General stub (for dpToPixels)
class General {
public:
    // Port of: General.dpToPixels(context, dp)
    static int dpToPixels(Context *context, float dp) {
        (void)context;
        // Approximate: 1dp ≈ 1px on mdpi, scale by a typical factor
        return static_cast<int>(std::round(dp * 2.0f));
    }

    // Port of: General.COLOR_INVALID = Color.MAGENTA
    static constexpr int COLOR_INVALID = 0xFFFF00FF;  // Magenta
};

// PrefsUtility stub
class PrefsUtility {
public:
    // Port of: PrefsUtility.pref_appearance_indentlines()
    static bool pref_appearance_indentlines() {
        return true;  // Default: draw indent lines
    }
};

// R.attr stubs (Java: android resource attributes)
// Port of: R.attr.rrIndentBackgroundCol
constexpr int R_ATTR_RR_INDENT_BACKGROUND_COL = 0;
// Port of: R.attr.rrIndentLineCol
constexpr int R_ATTR_RR_INDENT_LINE_COL = 1;

// ============================================================================
// View base class implementations
// ============================================================================

View::View(QWidget *parent)
    : QWidget(parent)
    , m_layoutParams(new ViewGroupLayoutParams())
{
}

View::View(Context *context, QWidget *parent)
    : QWidget(parent)
    , m_layoutParams(new ViewGroupLayoutParams())
{
    (void)context;
}

View::View(Context *context, AttributeSet *attrs, int defStyleAttr, QWidget *parent)
    : QWidget(parent)
    , m_layoutParams(new ViewGroupLayoutParams())
{
    (void)context;
    (void)attrs;
    (void)defStyleAttr;
}

void View::setBackgroundColor(int color) {
    m_backgroundColor = color;
    m_bgColor = QColor(
            (color >> 16) & 0xFF,
            (color >> 8) & 0xFF,
            color & 0xFF,
            (color >> 24) & 0xFF);
    // Update the widget's background
    QPalette pal = palette();
    pal.setColor(QPalette::Window, m_bgColor);
    setPalette(pal);
    setAutoFillBackground(true);
}

int View::getMeasuredHeight() const {
    // Port of: getMeasuredHeight() (returns the measured height from the last layout pass)
    // In Qt, this maps to QWidget::height() after layout
    return height();
}

int View::getWidth() const {
    // Port of: getWidth() (returns the view's width)
    return QWidget::width();
}

int View::getHeight() const {
    // Port of: getHeight() (returns the view's height)
    return QWidget::height();
}

void View::invalidateView() {
    // Port of: invalidate() (triggers a redraw)
    update();
    emit needsRedraw();
}

void View::requestLayout() {
    // Port of: requestLayout() (requests a layout pass)
    updateGeometry();
    emit layoutRequested();
}

ViewGroupLayoutParams *View::getLayoutParams() const {
    return m_layoutParams;
}

void View::setLayoutParams(ViewGroupLayoutParams *params) {
    if (m_layoutParams) {
        delete m_layoutParams;
    }
    m_layoutParams = params;
}

void View::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

// ============================================================================
// IndentView — port of constructor (Java lines 46-48)
//
// Port of: public IndentView(final Context context) { this(context, null); }
// ============================================================================

IndentView::IndentView(Context *context, QWidget *parent)
    : View(context, parent)
{
    // Port of: this(context, null) → delegates to (context, null, 0)
    commonInit(context);
}

// ============================================================================
// IndentView — port of constructor (Java lines 50-52)
//
// Port of: public IndentView(final Context context, @Nullable final AttributeSet attrs)
//     { this(context, attrs, 0); }
// ============================================================================

IndentView::IndentView(Context *context, AttributeSet *attrs, QWidget *parent)
    : View(context, attrs, 0, parent)
{
    // Port of: this(context, attrs, 0)
    commonInit(context);
}

// ============================================================================
// IndentView — port of constructor (Java lines 54-85)
//
// Port of: public IndentView(final Context context, @Nullable final AttributeSet attrs,
//     final int defStyleAttr) { super(context, attrs, defStyleAttr); ... }
// ============================================================================

IndentView::IndentView(Context *context, AttributeSet *attrs, int defStyleAttr, QWidget *parent)
    : View(context, attrs, defStyleAttr, parent)
{
    // Port of: super(context, attrs, defStyleAttr); (Java line 59)
    // Already done in View initialization

    commonInit(context);
}

IndentView::~IndentView() = default;

// ============================================================================
// commonInit — shared initialization logic from constructor (Java lines 61-85)
// ============================================================================

void IndentView::commonInit(Context *context) {
    // Port of: mPixelsPerIndent = General.dpToPixels(context, 10.0f); (Java line 61)
    m_pixelsPerIndent = General::dpToPixels(context, 10.0f);

    // Port of: final int mPixelsPerLine = General.dpToPixels(context, 2); (Java line 62)
    const int pixelsPerLine = General::dpToPixels(context, 2);

    // Port of: mHalfALine = mPixelsPerLine / 2; (Java line 63)
    m_halfALine = pixelsPerLine / 2;

    // Port of: final int rrIndentBackgroundCol; (Java line 65)
    // Port of: final int rrIndentLineCol; (Java line 66)

    // Port of: final TypedArray attr = context.obtainStyledAttributes(new int[] {
    //     R.attr.rrIndentBackgroundCol, R.attr.rrIndentLineCol }); (Java lines 69-71)
    {
        int attrs[2] = {R_ATTR_RR_INDENT_BACKGROUND_COL, R_ATTR_RR_INDENT_LINE_COL};
        Context::TypedArray *attr = context->obtainStyledAttributes(attrs);

        // Port of: rrIndentBackgroundCol = attr.getColor(0, General.COLOR_INVALID); (Java line 74)
        m_indentBackgroundCol = attr->getColor(0, General::COLOR_INVALID);

        // Port of: rrIndentLineCol = attr.getColor(1, General.COLOR_INVALID); (Java line 75)
        m_indentLineCol = attr->getColor(1, General::COLOR_INVALID);

        // Port of: attr.recycle(); (Java line 77)
        attr->recycle();
        delete attr;
    }

    // Port of: this.setBackgroundColor(rrIndentBackgroundCol); (Java line 80)
    setBackgroundColor(m_indentBackgroundCol);

    // Port of: mPaint.setColor(rrIndentLineCol); (Java line 81)
    // In Qt, we use QPen with the color for line drawing
    m_pen.setColor(QColor(
            (m_indentLineCol >> 16) & 0xFF,
            (m_indentLineCol >> 8) & 0xFF,
            m_indentLineCol & 0xFF));

    // Port of: mPaint.setStrokeWidth(mPixelsPerLine); (Java line 82)
    m_pen.setWidth(pixelsPerLine);

    // Port of: mPrefDrawLines = PrefsUtility.pref_appearance_indentlines(); (Java line 84)
    m_prefDrawLines = PrefsUtility::pref_appearance_indentlines();
}

// ============================================================================
// onDraw — port of method (Java lines 87-112)
//
// Port of: @Override protected void onDraw(final Canvas canvas)
// ============================================================================

void IndentView::onDraw(QPainter *painter) {
    // Port of: super.onDraw(canvas); (Java line 90)
    // (Base class draw already handled by Qt)

    // Port of: final int height = getMeasuredHeight(); (Java line 92)
    const int viewHeight = getMeasuredHeight();

    if (m_prefDrawLines) {
        // Port of: if(mPrefDrawLines) { ... canvas.drawLines(mLineBuffer, mPaint); } (Java lines 94-106)
        //
        // Port of: // i keeps track of indentation, and
        //     // l is to populate the float[] with line co-ordinates
        //     int l = 0;
        //     int i = 0;
        //     while(i < mIndent) {
        //         final float x = (mPixelsPerIndent * ++i) - mHalfALine;
        //         mLineBuffer[l++] = x;      // start-x
        //         mLineBuffer[l++] = 0;      // start-y
        //         mLineBuffer[l++] = x;      // stop-x
        //         mLineBuffer[l++] = height; // stop-y
        //     }
        //     canvas.drawLines(mLineBuffer, mPaint); (Java lines 95-106)

        // Ensure m_lineBuffer is large enough
        m_lineBuffer.resize(m_indent * 4);

        int l = 0;
        int i = 0;
        while (i < m_indent) {
            const float x = static_cast<float>(m_pixelsPerIndent * (++i)) - m_halfALine;
            m_lineBuffer[l++] = x;                       // start-x (Java line 101)
            m_lineBuffer[l++] = 0;                       // start-y (Java line 102)
            m_lineBuffer[l++] = x;                       // stop-x  (Java line 103)
            m_lineBuffer[l++] = static_cast<float>(viewHeight);  // stop-y  (Java line 104)
        }

        painter->setPen(m_pen);
        // Draw all lines at once (port of canvas.drawLines)
        for (int j = 0; j < l; j += 4) {
            painter->drawLine(
                    QPointF(m_lineBuffer[j], m_lineBuffer[j + 1]),
                    QPointF(m_lineBuffer[j + 2], m_lineBuffer[j + 3]));
        }

    } else {
        // Port of: else { final float rightLine = getWidth() - mHalfALine;
        //     canvas.drawLine(rightLine, 0, rightLine, getHeight(), mPaint); } (Java lines 108-111)

        const float rightLine = static_cast<float>(getWidth()) - m_halfALine;
        painter->setPen(m_pen);
        painter->drawLine(
                QPointF(rightLine, 0),
                QPointF(rightLine, static_cast<float>(getHeight())));
    }
}

// ============================================================================
// setIndentation — port of method (Java lines 119-129)
//
// Port of: public void setIndentation(final int indent)
// ============================================================================

void IndentView::setIndentation(int indent) {
    // Port of: getLayoutParams().width = (mPixelsPerIndent * indent); (Java line 120)
    ViewGroupLayoutParams *params = getLayoutParams();
    if (params != nullptr) {
        params->width = m_pixelsPerIndent * indent;
    }

    // Port of: mIndent = indent; (Java line 121)
    m_indent = indent;

    // Port of: if(mPrefDrawLines) { mLineBuffer = new float[mIndent * 4]; } (Java lines 123-125)
    if (m_prefDrawLines) {
        m_lineBuffer.resize(m_indent * 4);
    }

    // Port of: invalidate(); (Java line 127)
    invalidateView();

    // Port of: requestLayout(); (Java line 128)
    requestLayout();
}

} // namespace PinkReader
