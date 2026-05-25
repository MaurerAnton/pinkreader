/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: gif_view.h - Port of GIFView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/GIFView.java
 *
 * Original: public final class GIFView extends View
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <vector>
#include <cstdint>

namespace PinkReader {

// ============================================================================
// Movie stub (port of android.graphics.Movie)
//
// In Android, Movie is a GIF decoder. In Qt, we use QMovie or manual decoding.
// For this port, we use a simplified GIF representation.
// ============================================================================

struct GifFrame {
    QImage image;
    int delayMs = 100;  // Frame delay in milliseconds
};

class Movie {
public:
    // Port of: Movie.decodeByteArray(data, offset, length)
    static Movie *decodeByteArray(const std::vector<uint8_t> &data, int offset, int length);

    // Port of: movie.duration()
    int duration() const;

    // Port of: movie.width()
    int width() const;

    // Port of: movie.height()
    int height() const;

    // Port of: movie.setTime(relativeTimeMs)
    void setTime(int relativeTimeMs);

    // Port of: movie.draw(canvas, x, y, paint)
    void draw(QPainter *painter, float x, float y);

    // Get the current frame image
    QImage currentFrame() const;

private:
    std::vector<GifFrame> m_frames;
    int m_totalDurationMs = 0;
    int m_currentFrameIndex = 0;
    int m_width = 0;
    int m_height = 0;

    friend class GIFView;
};

// ============================================================================
// GIFView — port of Java class (Java lines 29-87)
//
// Port of: org.quantumbadger.redreader.views.GIFView
//
// Extends View. Renders an animated GIF.
// Every field, method, constant ported exactly.
// ============================================================================

class GIFView : public QWidget {
    Q_OBJECT

public:
    // ========================================================================
    // Static method (Java lines 36-48)
    //
    // Port of: public static Movie prepareMovie(@NonNull final byte[] data,
    //     final int offset, final int length)
    // ========================================================================
    static Movie *prepareMovie(const std::vector<uint8_t> &data, int offset, int length);

    // ========================================================================
    // Constructor (Java lines 51-60)
    //
    // Port of: public GIFView(final Context context, final Movie movie)
    // ========================================================================
    explicit GIFView(Movie *movie, QWidget *parent = nullptr);

    virtual ~GIFView();

protected:
    // ========================================================================
    // Override (Java lines 62-86)
    //
    // Port of: @Override protected void onDraw(final Canvas canvas)
    // ========================================================================
    void paintEvent(QPaintEvent *event) override;

private slots:
    // Timer-driven invalidation (port of this.invalidate() in Android onDraw)
    void onTimerTick();

private:
    // ========================================================================
    // Private fields — exact match (Java lines 31-34)
    // ========================================================================

    // Port of: private final Movie mMovie; (Java line 31)
    Movie *m_movie = nullptr;

    // Port of: private long movieStart; (Java line 32)
    int64_t m_movieStart = 0;

    // Port of: private final Paint paint = new Paint(); (Java lines 34)
    // Represented by QPainter settings
    bool m_antiAlias = true;
    bool m_filterBitmap = true;

    // Timer for animation invalidation
    QTimer *m_timer = nullptr;
};

} // namespace PinkReader
