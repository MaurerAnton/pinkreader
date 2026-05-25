/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: inbox_listing_activity.h - Port of InboxListingActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/InboxListingActivity.java
 *
 * Original: public final class InboxListingActivity extends ViewsBaseActivity
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Activity base classes
class ViewsBaseActivity;

// Android framework stubs
class Bundle;
class Context;
class Intent;
class Menu;
class MenuItem;
class View;
class ViewGroup;
class LinearLayout;
class TextView;
class RecyclerView;
class LayoutInflater;

// RedReader models
class RedditAccount;
class RedditAccountManager;
class RedditChangeDataManager;
class RRThemeAttributes;
class SharedPrefsWrapper;

// RedReader cache
class CacheManager;
class CacheRequest;
class CacheRequestCallbacks;
class DownloadStrategyAlways;

// RedReader common
class General;
class RRError;
class UriString;
class SeekableInputStream;
class FailedRequestBody;
template<typename T> class GenericFactory;

// RedReader reddit
class RedditThing;
class RedditListing;
class RedditComment;
class RedditMessage;
class RedditFieldReplies;
class RedditParsedComment;
class RedditRenderableComment;
class RedditPreparedMessage;
class RedditRenderableInboxItem;
class MaybeParseError;

// RedReader views
class RedditInboxItemView;
class ScrollbarRecyclerViewManager;
class ErrorView;
class LoadingView;

// RedReader adapters
class GroupedRecyclerViewAdapter;

// RedReader API
class APIResponseHandler;
class RedditAPI;
class JsonUtils;

// Timestamp
class TimestampUTC;

// Handler
class Handler;
class Looper;
class Message;

// BugReport
class BugReportActivity;

/**
 * @brief Port of org.quantumbadger.redreader.activities.InboxListingActivity
 *
 * Displays inbox messages (inbox, sent, modmail). Every field, method,
 * constant, and inner class ported exactly.
 */
class InboxListingActivity : public QObject {
    Q_OBJECT

public:
    // ===== Enum: InboxType (Java line 92-94) =====
    enum class InboxType {
        INBOX,
        SENT,
        MODMAIL
    };

    /**
     * @brief Inner class: InboxItem (Java lines 118-163)
     * Extends GroupedRecyclerViewAdapter.Item
     */
    class InboxItem : public QObject {
        Q_OBJECT
    public:
        // Java lines 120-121: private final int mListPosition; private final RedditRenderableInboxItem mItem;
        int mListPosition;
        RedditRenderableInboxItem *mItem = nullptr;
        InboxListingActivity *mActivity = nullptr;
        RRThemeAttributes *mTheme = nullptr;
        RedditChangeDataManager *mChangeDataManager = nullptr;

        // Java lines 123-125: constructor
        InboxItem(int listPosition, RedditRenderableInboxItem *item,
                  InboxListingActivity *activity, RRThemeAttributes *theme,
                  RedditChangeDataManager *changeDataManager);

        // Java lines 128-131: getViewType()
        // Returns the view class type
        void *getViewType() const; // stub: returns RedditInboxItemView class indicator

        // Java lines 134-147: onCreateViewHolder(ViewGroup)
        void *onCreateViewHolder(ViewGroup *viewGroup);

        // Java lines 150-157: onBindViewHolder(RecyclerView.ViewHolder)
        void onBindViewHolder(void *viewHolder);

        // Java lines 160-162: isHidden()
        bool isHidden() const;
    };

    // ===== Static constants (mirrors Java exactly) =====

    // Java line 86: private static final String TAG = "InboxListingActivity";
    static const QString TAG;

    // Java line 88: private static final int OPTIONS_MENU_MARK_ALL_AS_READ = 0;
    static const int OPTIONS_MENU_MARK_ALL_AS_READ;

    // Java line 89: private static final int OPTIONS_MENU_SHOW_UNREAD_ONLY = 1;
    static const int OPTIONS_MENU_SHOW_UNREAD_ONLY;

    // Java line 90: private static final int OPTIONS_MENU_REFRESH = 2;
    static const int OPTIONS_MENU_REFRESH;

    // Java line 96: private static final String PREF_ONLY_UNREAD = "inbox_only_show_unread";
    static const QString PREF_ONLY_UNREAD;

    // ===== Constructor / Lifecycle =====
    explicit InboxListingActivity(QObject *parent = nullptr);
    virtual ~InboxListingActivity();

    // ===== Activity lifecycle (port of overrides) =====

    // Java lines 167-234: public void onCreate(final Bundle savedInstanceState)
    void onCreate(Bundle *savedInstanceState);

    // Java lines 451-456: public void onBackPressed()
    void onBackPressed();

    // Java lines 458-478: public boolean onCreateOptionsMenu(final Menu menu)
    bool onCreateOptionsMenu(Menu *menu);

    // Java lines 480-539: public boolean onOptionsItemSelected(final MenuItem item)
    bool onOptionsItemSelected(MenuItem *item);

    // ===== Public methods =====

    // Java lines 236-240: public void cancel()
    void cancel();

private:
    // ===== Private fields (Java lines 98-110) =====

    // Java line 98: private GroupedRecyclerViewAdapter adapter;
    GroupedRecyclerViewAdapter *adapter = nullptr;

    // Java line 100: private LoadingView loadingView;
    LoadingView *loadingView = nullptr;

    // Java line 101: private LinearLayout notifications;
    LinearLayout *notifications = nullptr;

    // Java line 103: private CacheRequest request;
    CacheRequest *request = nullptr;

    // Java line 105: private InboxType inboxType;
    InboxType inboxType = InboxType::INBOX;

    // Java line 106: private boolean mOnlyShowUnread;
    bool mOnlyShowUnread = false;

    // Java line 108: private RRThemeAttributes mTheme;
    RRThemeAttributes *mTheme = nullptr;

    // Java line 109: private RedditChangeDataManager mChangeDataManager;
    RedditChangeDataManager *mChangeDataManager = nullptr;

    // Java lines 111-116: private final Handler itemHandler
    // Stub: Handler/Looper not ported; functionality approximated with Qt signals
    // Original: new Handler(Looper.getMainLooper()) with handleMessage sending to adapter

    // ===== Private methods =====

    // Java lines 242-449: private void makeFirstRequest(final Context context)
    void makeFirstRequest(Context *context);

signals:
    // Qt signal for handler message forwarding (replaces itemHandler)
    void itemReady(int groupIndex, void *item);

    // Qt signal equivalent of invalidateOptionsMenu()
    void optionsMenuInvalidated();
};

} // namespace PinkReader
