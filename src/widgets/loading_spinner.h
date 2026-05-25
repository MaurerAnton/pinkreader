/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: loading_spinner.h - Loading/progress spinner widget
 */

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGestureEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QPixmap>
#include <QIcon>
#include <QFont>

namespace PinkReader {

/**
 * @brief Loading/progress spinner widget
 *
 * A custom UI widget providing loading/progress spinner widget.
 * Designed for optimal performance on mobile devices with
 * smooth animations, gesture support, and accessibility features.
 */
class LoadingSpinner : public QWidget
{
    Q_OBJECT

    // Qt properties for animation support
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)

public:
    /**
     * @brief Construct the LoadingSpinner widget
     * @param parent Parent widget (nullptr for top-level)
     */
    explicit LoadingSpinner(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LoadingSpinner() override;

    // -----------------------------------------------------------------------
    // Property Accessors
    // -----------------------------------------------------------------------

    /** @brief Current opacity value (0.0 - 1.0) */
    qreal opacity() const;

    /** @brief Set opacity with optional animation */
    void setOpacity(qreal opacity);

    /** @brief Current background color */
    QColor backgroundColor() const;

    /** @brief Set background color */
    void setBackgroundColor(const QColor &color);

    /** @brief Animation duration in milliseconds */
    int animationDuration() const;

    /** @brief Set animation duration */
    void setAnimationDuration(int ms);

    // -----------------------------------------------------------------------
    // State Management
    // -----------------------------------------------------------------------

    /** @brief Check if widget is in loading state */
    bool isLoading() const;

    /** @brief Show loading indicator */
    void showLoading();

    /** @brief Hide loading indicator */
    void hideLoading();

    /** @brief Check if widget is enabled for interaction */
    bool isInteractionEnabled() const;

    /** @brief Enable or disable user interaction */
    void setInteractionEnabled(bool enabled);

    // -----------------------------------------------------------------------
    // Content Methods
    // -----------------------------------------------------------------------

    /**
     * @brief Set the primary text content
     * @param text The text to display
     */
    void setText(const QString &text);

    /**
     * @brief Get the primary text content
     * @return Current text content
     */
    QString text() const;

    /**
     * @brief Set secondary/subtitle text
     * @param subtitle The subtitle to display
     */
    void setSubtitle(const QString &subtitle);

    /**
     * @brief Set an icon or image
     * @param icon The icon to display
     */
    void setIcon(const QIcon &icon);

    /**
     * @brief Set an image pixmap
     * @param pixmap The pixmap to display
     */
    void setPixmap(const QPixmap &pixmap);

    /**
     * @brief Clear all displayed content
     */
    void clear();

signals:
    /**
     * @brief Emitted when the widget is clicked/tapped
     */
    void clicked();

    /**
     * @brief Emitted when the widget is long-pressed
     */
    void longPressed();

    /**
     * @brief Emitted when the widget's content changes
     */
    void contentChanged();

    /**
     * @brief Emitted when a swipe gesture is detected
     * @param direction Swipe direction (positive = right, negative = left)
     */
    void swiped(int direction);

protected:
    // Qt event overrides
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

    // State flags
    bool m_isLoading = false;
    bool m_isPressed = false;
    bool m_isHovered = false;
    bool m_isEnabled = true;

    // Content
    QString m_text;
    QString m_subtitle;
    QIcon m_icon;
    QPixmap m_pixmap;

    // Animation
    qreal m_opacity = 1.0;
    int m_animationDuration = 200;
    QPropertyAnimation *m_opacityAnimation = nullptr;
    QPoint m_pressPosition;

    // Colors
    QColor m_backgroundColor;
    QColor m_textColor;
    QColor m_accentColor;

    // Layout
    QVBoxLayout *m_mainLayout = nullptr;
    QLabel *m_textLabel = nullptr;
    QLabel *m_subtitleLabel = nullptr;
    QLabel *m_iconLabel = nullptr;
    QLabel *m_imageLabel = nullptr;

private:
    /**
     * @brief Initialize the widget UI and layout
     */
    virtual void setupUi();

    /**
     * @brief Connect internal signals and slots
     */
    virtual void connectSignals();

    /**
     * @brief Apply the current theme colors
     */
    void applyTheme();

    /**
     * @brief Start opacity animation
     * @param targetOpacity The target opacity value
     */
    void animateOpacity(qreal targetOpacity);
};

} // namespace PinkReader
