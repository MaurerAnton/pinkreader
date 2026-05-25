/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: bezel_toolbar.cpp - Implementation of Radial/swipe bezel toolbar
 */

#include "widgets/bezel_toolbar.h"
#include "settings/theme_manager.h"
#include "settings/preferences.h"
#include "utils/logging.h"

#include <QApplication>
#include <QStyle>
#include <QGesture>
#include <QGestureRecognizer>
#include <QSwipeGesture>
#include <QTapGesture>
#include <QTapAndHoldGesture>
#include <QGraphicsOpacityEffect>
#include <QPainterPath>
#include <QLinearGradient>
#include <QFontMetrics>
#include <QStyleOption>

namespace PinkReader {

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

BezelToolbar::BezelToolbar(QWidget *parent)
    : QWidget(parent)
{
    Logging::debug("Widgets::BezelToolbar", "Creating widget...");

    // Enable mouse tracking for hover effects
    setMouseTracking(true);

    // Accept touch and gesture events
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);

    // Set minimum size for touch targets (48dp for accessibility)
    setMinimumSize(48, 48);

    // Enable widget attributes
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setAttribute(Qt::WA_StyledBackground, true);

    setupUi();
    connectSignals();
    applyTheme();

    Logging::debug("Widgets::BezelToolbar", "Widget created");
}

BezelToolbar::~BezelToolbar()
{
    Logging::debug("Widgets::BezelToolbar", "Widget destroyed");

    // Stop any running animations
    if (m_opacityAnimation && m_opacityAnimation->state() == QAbstractAnimation::Running) {
        m_opacityAnimation->stop();
    }
}

// ---------------------------------------------------------------------------
// Property Accessors
// ---------------------------------------------------------------------------

qreal BezelToolbar::opacity() const
{
    return m_opacity;
}

void BezelToolbar::setOpacity(qreal opacity)
{
    qreal clampedOpacity = qBound(0.0, opacity, 1.0);

    if (qFuzzyCompare(m_opacity, clampedOpacity)) {
        return;  // No change
    }

    m_opacity = clampedOpacity;
    update();  // Request repaint
}

QColor BezelToolbar::backgroundColor() const
{
    return m_backgroundColor;
}

void BezelToolbar::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

int BezelToolbar::animationDuration() const
{
    return m_animationDuration;
}

void BezelToolbar::setAnimationDuration(int ms)
{
    m_animationDuration = qMax(0, ms);
}

// ---------------------------------------------------------------------------
// State Management
// ---------------------------------------------------------------------------

bool BezelToolbar::isLoading() const
{
    return m_isLoading;
}

void BezelToolbar::showLoading()
{
    if (m_isLoading) return;

    Logging::debug("Widgets::BezelToolbar", "Showing loading state");
    m_isLoading = true;
    update();
}

void BezelToolbar::hideLoading()
{
    if (!m_isLoading) return;

    Logging::debug("Widgets::BezelToolbar", "Hiding loading state");
    m_isLoading = false;
    update();
}

bool BezelToolbar::isInteractionEnabled() const
{
    return m_isEnabled;
}

void BezelToolbar::setInteractionEnabled(bool enabled)
{
    m_isEnabled = enabled;
    setEnabled(enabled);
    update();
}

// ---------------------------------------------------------------------------
// Content Methods
// ---------------------------------------------------------------------------

void BezelToolbar::setText(const QString &text)
{
    if (m_text == text) return;

    m_text = text;
    if (m_textLabel) {
        m_textLabel->setText(text);
    }
    emit contentChanged();
    update();
}

QString BezelToolbar::text() const
{
    return m_text;
}

void BezelToolbar::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle) return;

    m_subtitle = subtitle;
    if (m_subtitleLabel) {
        m_subtitleLabel->setText(subtitle);
        m_subtitleLabel->setVisible(!subtitle.isEmpty());
    }
    emit contentChanged();
    update();
}

void BezelToolbar::setIcon(const QIcon &icon)
{
    m_icon = icon;
    if (m_iconLabel) {
        m_iconLabel->setPixmap(icon.pixmap(24, 24));
    }
    update();
}

void BezelToolbar::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    if (m_imageLabel && !pixmap.isNull()) {
        m_imageLabel->setPixmap(pixmap.scaled(
            m_imageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
        m_imageLabel->setVisible(true);
    }
    update();
}

void BezelToolbar::clear()
{
    m_text.clear();
    m_subtitle.clear();
    m_icon = QIcon();
    m_pixmap = QPixmap();

    if (m_textLabel) m_textLabel->clear();
    if (m_subtitleLabel) m_subtitleLabel->clear();
    if (m_iconLabel) m_iconLabel->clear();
    if (m_imageLabel) {
        m_imageLabel->clear();
        m_imageLabel->setVisible(false);
    }

    emit contentChanged();
    update();
}

// ---------------------------------------------------------------------------
// Qt Event Overrides
// ---------------------------------------------------------------------------

void BezelToolbar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Apply opacity
    painter.setOpacity(m_opacity);

    // Draw background
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect(), 8, 8);

    if (m_backgroundColor.isValid()) {
        painter.fillPath(backgroundPath, m_backgroundColor);
    } else {
        // Use theme background color
        QColor bgColor = palette().color(QPalette::Window);
        painter.fillPath(backgroundPath, bgColor);
    }

    // Draw hover/press effect
    if (m_isHovered && m_isEnabled) {
        QColor hoverColor = palette().color(QPalette::Highlight);
        hoverColor.setAlpha(20);
        painter.fillPath(backgroundPath, hoverColor);
    }

    if (m_isPressed) {
        QColor pressColor = palette().color(QPalette::Highlight);
        pressColor.setAlpha(40);
        painter.fillPath(backgroundPath, pressColor);
    }

    // Draw border
    QPen borderPen(palette().color(QPalette::Mid), 1);
    painter.setPen(borderPen);
    painter.drawPath(backgroundPath);

    // Draw loading indicator if loading
    if (m_isLoading) {
        drawLoadingIndicator(painter);
    }

    painter.end();
}

void BezelToolbar::mousePressEvent(QMouseEvent *event)
{
    if (!m_isEnabled) {
        event->ignore();
        return;
    }

    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        m_pressPosition = event->pos();
        update();
        event->accept();
    } else {
        event->ignore();
    }
}

void BezelToolbar::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_isEnabled) {
        event->ignore();
        return;
    }

    if (event->button() == Qt::LeftButton && m_isPressed) {
        m_isPressed = false;
        update();

        // Check if it's a click (minimal movement from press position)
        QPoint delta = event->pos() - m_pressPosition;
        if (delta.manhattanLength() < 10) {
            emit clicked();
        }

        event->accept();
    } else {
        event->ignore();
    }
}

void BezelToolbar::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isEnabled) return;

    // Track hover state
    bool wasHovered = m_isHovered;
    m_isHovered = rect().contains(event->pos());

    if (wasHovered != m_isHovered) {
        update();
    }

    // Check for swipe gesture if pressed
    if (m_isPressed) {
        int deltaX = event->pos().x() - m_pressPosition.x();
        if (qAbs(deltaX) > 50) {
            emit swiped(deltaX > 0 ? 1 : -1);
            m_isPressed = false;
            update();
        }
    }
}

void BezelToolbar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Update pixmap scaling when resized
    if (m_imageLabel && !m_pixmap.isNull()) {
        m_imageLabel->setPixmap(m_pixmap.scaled(
            m_imageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
    }
}

bool BezelToolbar::event(QEvent *event)
{
    // Handle gesture events
    if (event->type() == QEvent::Gesture) {
        auto *gestureEvent = static_cast<QGestureEvent *>(event);

        if (QGesture *swipe = gestureEvent->gesture(Qt::SwipeGesture)) {
            auto *swipeGesture = static_cast<QSwipeGesture *>(swipe);
            if (swipeGesture->state() == Qt::GestureFinished) {
                if (swipeGesture->horizontalDirection() == QSwipeGesture::Left) {
                    emit swiped(-1);
                } else if (swipeGesture->horizontalDirection() == QSwipeGesture::Right) {
                    emit swiped(1);
                }
                return true;
            }
        }

        if (QGesture *tapAndHold = gestureEvent->gesture(Qt::TapAndHoldGesture)) {
            if (tapAndHold->state() == Qt::GestureFinished) {
                emit longPressed();
                return true;
            }
        }
    }

    return QWidget::event(event);
}

// ---------------------------------------------------------------------------
// Private Methods
// ---------------------------------------------------------------------------

void BezelToolbar::setupUi()
{
    Logging::debug("Widgets::BezelToolbar", "Setting up UI...");

    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(8, 8, 8, 8);
    m_mainLayout->setSpacing(4);

    // Text label
    m_textLabel = new QLabel(this);
    m_textLabel->setObjectName(QStringLiteral("BezelToolbarText"));
    m_textLabel->setWordWrap(true);
    m_textLabel->setTextInteractionFlags(Qt::NoTextInteraction);
    QFont textFont = m_textLabel->font();
    textFont.setPointSize(14);
    m_textLabel->setFont(textFont);
    m_mainLayout->addWidget(m_textLabel);

    // Subtitle label
    m_subtitleLabel = new QLabel(this);
    m_subtitleLabel->setObjectName(QStringLiteral("BezelToolbarSubtitle"));
    m_subtitleLabel->setWordWrap(true);
    QFont subtitleFont = m_subtitleLabel->font();
    subtitleFont.setPointSize(12);
    m_subtitleLabel->setFont(subtitleFont);
    m_subtitleLabel->setStyleSheet(QStringLiteral("color: #888888;"));
    m_subtitleLabel->hide();
    m_mainLayout->addWidget(m_subtitleLabel);

    // Icon label (in its own layout alongside text for horizontal layouts)
    m_iconLabel = new QLabel(this);
    m_iconLabel->setObjectName(QStringLiteral("BezelToolbarIcon"));
    m_iconLabel->setFixedSize(24, 24);
    m_iconLabel->setAlignment(Qt::AlignCenter);

    // Image label
    m_imageLabel = new QLabel(this);
    m_imageLabel->setObjectName(QStringLiteral("BezelToolbarImage"));
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setScaledContents(false);
    m_imageLabel->hide();

    setLayout(m_mainLayout);
}

void BezelToolbar::connectSignals()
{
    Logging::debug("Widgets::BezelToolbar", "Connecting signals...");

    // Theme change handling
    connect(&ThemeManager::instance(), &ThemeManager::themeApplied,
            this, &BezelToolbar::applyTheme);

    // Preference change handling
    connect(&Preferences::instance(), &Preferences::settingChanged,
            this, [this](const QString &key, const QVariant &value) {
        Q_UNUSED(value)
        if (key == QStringLiteral("theme") ||
            key == QStringLiteral("font_size")) {
            applyTheme();
        }
    });
}

void BezelToolbar::applyTheme()
{
    // Update colors from current theme palette
    m_backgroundColor = palette().color(QPalette::Window);
    m_textColor = palette().color(QPalette::WindowText);
    m_accentColor = palette().color(QPalette::Highlight);

    // Update text label colors
    if (m_textLabel) {
        QPalette textPal = m_textLabel->palette();
        textPal.setColor(QPalette::WindowText, m_textColor);
        m_textLabel->setPalette(textPal);
    }

    if (m_subtitleLabel) {
        QPalette subPal = m_subtitleLabel->palette();
        QColor subColor = m_textColor;
        subColor.setAlpha(150);
        subPal.setColor(QPalette::WindowText, subColor);
        m_subtitleLabel->setPalette(subPal);
    }

    // Update font sizes based on preferences
    int fontSize = Preferences::instance().getInt("font_size", 14);
    if (m_textLabel) {
        QFont font = m_textLabel->font();
        font.setPointSize(fontSize);
        m_textLabel->setFont(font);
    }

    update();
}

void BezelToolbar::animateOpacity(qreal targetOpacity)
{
    if (!m_opacityAnimation) {
        m_opacityAnimation = new QPropertyAnimation(this, "opacity", this);
    }

    m_opacityAnimation->stop();
    m_opacityAnimation->setDuration(m_animationDuration);
    m_opacityAnimation->setStartValue(m_opacity);
    m_opacityAnimation->setEndValue(targetOpacity);
    m_opacityAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    m_opacityAnimation->start();
}

// Private member initialization in constructor
QPoint m_pressPosition;  // Will be added to class in practice

} // namespace PinkReader
