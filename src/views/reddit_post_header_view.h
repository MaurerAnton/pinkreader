/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_post_header_view.h - Port of RedditPostHeaderView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RedditPostHeaderView.java
 *
 * Original: public class RedditPostHeaderView extends LinearLayout
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

// View base classes
class LinearLayout;

// Android framework stubs
class TextView;
class TypedArray;
class ImageButton;
class BaseActivity;
class Context;
class Resources;

// RedReader types
class RedditPreparedPost;
class RedditAccount;
class RedditAccountManager;
class RedditChangeDataManager;
class RedditPostActions;
class PrefsUtility;
class Fonts;
class LinkHandler;
class General;
class AccessibilityActionManager;

// ============================================================================
// RedditPostHeaderView — port of Java class (Java lines 41-253)
//
// Port of: org.quantumbadger.redreader.views.RedditPostHeaderView
//
// Extends LinearLayout. Every field, method, and inner access ported exactly.
// ============================================================================

class RedditPostHeaderView : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Constructor (Java lines 48-234)
    //
    // Port of:
    //   public RedditPostHeaderView(
    //       final BaseActivity activity,
    //       final RedditPreparedPost post)
    // ========================================================================
    RedditPostHeaderView(BaseActivity *activity,
                          RedditPreparedPost *post,
                          QObject *parent = nullptr);

    virtual ~RedditPostHeaderView();

    // ========================================================================
    // Lifecycle methods (Java lines 236-252)
    // ========================================================================

    // Port of: @Override protected void onAttachedToWindow() (Java lines 236-243)
    void onAttachedToWindow();

    // Port of: @Override protected void onDetachedFromWindow() (Java lines 245-252)
    void onDetachedFromWindow();

private:
    // ========================================================================
    // Private fields (Java lines 43-46)
    // ========================================================================

    // Port of: private final TextView subtitle; (Java line 43)
    TextView *m_subtitle = nullptr;

    // Port of: @Nullable private final Runnable mChangeListenerAddTask; (Java line 45)
    std::function<void()> m_changeListenerAddTask;

    // Port of: @Nullable private final Runnable mChangeListenerRemoveTask; (Java line 46)
    std::function<void()> m_changeListenerRemoveTask;

    // Activity and post references
    BaseActivity *m_activity = nullptr;
    RedditPreparedPost *m_post = nullptr;
};

} // namespace PinkReader
