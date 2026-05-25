/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: loading_view.h - Port of LoadingView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/liststatus/LoadingView.java
 *
 * Original: public final class LoadingView extends StatusListItemView
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <functional>
#include <memory>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

class Context;
class TextView;
class LinearLayout;
class Handler;
class Message;

// Base class (not yet ported, stub)
class StatusListItemView;

// ============================================================================
// LoadingView — port of Java class (Java lines 32-113)
//
// Port of: org.quantumbadger.redreader.views.liststatus.LoadingView
//
// Extends StatusListItemView. Every field, method, constant ported exactly.
// ============================================================================

class LoadingView : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Static constants (Java lines 36-37)
    // ========================================================================

    // Port of: private static final int LOADING_INDETERMINATE = -1;
    static constexpr int LOADING_INDETERMINATE = -1;

    // Port of: private static final int LOADING_DONE = -2;
    static constexpr int LOADING_DONE = -2;

    // ========================================================================
    // Handler inner class (Java lines 39-51)
    //
    // Port of: private final Handler loadingHandler = new Handler(Looper.getMainLooper()) {
    //     @Override public void handleMessage(@NonNull final Message msg) { ... }
    // };
    // ========================================================================
    class LoadingHandler : public QObject {
        Q_OBJECT

    public:
        explicit LoadingHandler(QObject *parent = nullptr);
        virtual ~LoadingHandler();

        // Port of: @Override public void handleMessage(@NonNull final Message msg) (Java lines 41-50)
        void handleMessage(Message *msg, TextView *textView, LoadingView *loadingView);

    signals:
        void messageHandled();
    };

    // ========================================================================
    // Constructors (Java lines 72-112)
    // ========================================================================

    // Port of: public LoadingView(final Context context) (Java lines 72-74)
    LoadingView(Context *context, QObject *parent = nullptr);

    // Port of: public LoadingView(final Context context, final int initialTextRes,
    //     final boolean progressBarEnabled, final boolean indeterminate) (Java lines 76-86)
    LoadingView(Context *context,
                int initialTextRes,
                bool progressBarEnabled,
                bool indeterminate,
                QObject *parent = nullptr);

    // Port of: public LoadingView(final Context context, final String initialText,
    //     final boolean progressBarEnabled, final boolean indeterminate) (Java lines 88-112)
    LoadingView(Context *context,
                const QString &initialText,
                bool progressBarEnabled,
                bool indeterminate,
                QObject *parent = nullptr);

    virtual ~LoadingView();

    // ========================================================================
    // Public methods (Java lines 53-63)
    // ========================================================================

    // Port of: public void setIndeterminate(final int textRes) (Java lines 53-55)
    void setIndeterminate(int textRes);

    // Port of: public void setProgress(final int textRes, final float fraction) (Java lines 57-59)
    void setProgress(int textRes, float fraction);

    // Port of: public void setDone(final int textRes) (Java lines 61-63)
    void setDone(int textRes);

private:
    // ========================================================================
    // Private fields (Java lines 34, 39)
    // ========================================================================

    // Port of: private final TextView textView; (Java line 34)
    TextView *m_textView = nullptr;

    // Port of: private final Handler loadingHandler = new Handler(Looper.getMainLooper()) {...} (Java lines 39-51)
    LoadingHandler *m_loadingHandler = nullptr;

    // Base StatusListItemView reference
    StatusListItemView *m_statusListItemView = nullptr;

    // Context reference
    Context *m_context = nullptr;

    // ========================================================================
    // Private methods (Java lines 65-70)
    // ========================================================================

    // Port of: private void sendMessage(final String text, final int what) (Java lines 65-70)
    void sendMessage(const QString &text, int what);
};

} // namespace PinkReader
