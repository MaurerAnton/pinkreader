// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/SessionListAdapter.java

#pragma once

#include "header_recycler_adapter.h"
#include <QString>
#include <QVector>
#include <vector>
#include <string>

namespace PinkReader {

// Forward declarations
class CacheEntry;
class AppCompatDialogFragment;
class UriString;
class Drawable;
class SessionChangeListener;

/**
 * @brief Port of org.quantumbadger.redreader.adapters.SessionListAdapter
 *
 * RecyclerView adapter showing cache sessions with a refresh header.
 * Every field, method, and logic branch from the original Java file.
 */
class SessionListAdapter : public HeaderRecyclerAdapter {
	Q_OBJECT

public:
	// Port of: private final Context context;
	// In C++ port: context is the parent QObject
	// Port of: private final UUID current;
	std::string m_current;
	// Port of: private final SessionChangeListener.SessionChangeType type;
	// Using SessionChangeType from SessionChangeListener
	int m_type; // 0=POSTS, 1=COMMENTS
	// Port of: private final AppCompatDialogFragment fragment;
	AppCompatDialogFragment* m_fragment = nullptr;

	// Port of: private final ArrayList<CacheEntry> sessions;
	std::vector<CacheEntry*> m_sessions;
	// Port of: private final Drawable rrIconRefresh;
	// In C++ port: icon resource represented as string/int

	// Port of: public SessionListAdapter(
	//   final Context context, final UriString url, final UUID current,
	//   final SessionChangeListener.SessionChangeType type,
	//   final AppCompatDialogFragment fragment)
	//   this.context = context; this.current = current;
	//   this.type = type; this.fragment = fragment;
	//   sessions = new ArrayList<>(CacheManager.getInstance(context)
	//     .getSessions(url, RedditAccountManager.getInstance(context)
	//       .getDefaultAccount()));
	//   final TypedArray attr = context.obtainStyledAttributes(new int[]{R.attr.rrIconRefresh});
	//   rrIconRefresh = AppCompatResources.getDrawable(context, attr.getResourceId(0,0));
	//   attr.recycle();
	SessionListAdapter(const std::string& currentUuid, int type,
	                   const std::vector<CacheEntry*>& sessions,
	                   AppCompatDialogFragment* fragment,
	                   QObject* parent = nullptr);

	virtual ~SessionListAdapter() = default;

protected:
	// Port of: @Override protected RecyclerView.ViewHolder
	//   onCreateHeaderItemViewHolder(final ViewGroup parent)
	//   final View v = LayoutInflater.from(parent.getContext())
	//     .inflate(R.layout.list_item_1_text, parent, false);
	//   return new VH1Text(v);
	ViewHolder onCreateHeaderItemViewHolder(QObject* parent) override;

	// Port of: @Override protected RecyclerView.ViewHolder
	//   onCreateContentItemViewHolder(final ViewGroup parent)
	//   final View v = LayoutInflater.from(parent.getContext())
	//     .inflate(R.layout.list_item_1_text, parent, false);
	//   return new VH1Text(v);
	ViewHolder onCreateContentItemViewHolder(QObject* parent) override;

	// Port of: @Override protected void onBindHeaderItemViewHolder(
	//   final RecyclerView.ViewHolder holder, final int position)
	//   final VH1Text vh = (VH1Text)holder;
	//   vh.text.setText(context.getString(R.string.options_refresh));
	//   vh.text.setCompoundDrawablesWithIntrinsicBounds(rrIconRefresh, null, null, null);
	//   vh.itemView.setOnClickListener(v -> {
	//     ((SessionChangeListener)context).onSessionRefreshSelected(type);
	//     fragment.dismiss();
	//   });
	void onBindHeaderItemViewHolder(ViewHolder holder, int position) override;

	// Port of: @Override protected void onBindContentItemViewHolder(
	//   final RecyclerView.ViewHolder holder, final int position)
	//   final VH1Text vh = (VH1Text)holder;
	//   final CacheEntry session = sessions.get(position);
	//   final BetterSSB name = new BetterSSB();
	//   if(session.timestamp.elapsed().isLessThan(TimeDuration.minutes(2))) {
	//     name.append(TimeFormatHelper.format(...), 0);
	//   } else { name.append(session.timestamp.format(), 0); }
	//   if(session.session.equals(current)) {
	//     final TypedArray attr = context.obtainStyledAttributes(...);
	//     final int col = attr.getColor(0,0); attr.recycle();
	//     name.append("  (" + context.getString(R.string.session_active) + ")",
	//       BetterSSB.FOREGROUND_COLOR | BetterSSB.SIZE, col, 0, 0.8f);
	//   }
	//   vh.text.setText(name.get());
	//   vh.itemView.setOnClickListener(v -> {
	//     final CacheEntry ce = sessions.get(position);
	//     ((SessionChangeListener)context).onSessionSelected(ce.session, type);
	//     fragment.dismiss();
	//   });
	void onBindContentItemViewHolder(ViewHolder holder, int position) override;

	// Port of: @Override protected int getContentItemCount()
	//   return sessions.size();
	int getContentItemCount() override;
};

} // namespace PinkReader
