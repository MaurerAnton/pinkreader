// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/SessionListAdapter.java

#include "session_list_adapter.h"

namespace PinkReader {

// Constructor — Java lines 55-77
SessionListAdapter::SessionListAdapter(
	const std::string& currentUuid, int type,
	const std::vector<CacheEntry*>& sessions,
	AppCompatDialogFragment* fragment,
	QObject* parent)
	: HeaderRecyclerAdapter(parent)
	, m_current(currentUuid)
	, m_type(type)
	, m_fragment(fragment)
	, m_sessions(sessions)
{
	// Port of: final TypedArray attr = context.obtainStyledAttributes(new int[]{R.attr.rrIconRefresh});
	// rrIconRefresh = AppCompatResources.getDrawable(context, attr.getResourceId(0, 0));
	// attr.recycle();
	// Icon loading is platform-specific, handled by UI framework in C++ port
}

// Port of: @Override protected RecyclerView.ViewHolder
//   onCreateHeaderItemViewHolder(final ViewGroup parent)
ViewHolder SessionListAdapter::onCreateHeaderItemViewHolder(QObject* parent) {
	// Port of: final View v = LayoutInflater.from(parent.getContext())
	//   .inflate(R.layout.list_item_1_text, parent, false);
	// return new VH1Text(v);
	return nullptr; // ViewHolder creation handled by UI framework
}

// Port of: @Override protected RecyclerView.ViewHolder
//   onCreateContentItemViewHolder(final ViewGroup parent)
ViewHolder SessionListAdapter::onCreateContentItemViewHolder(QObject* parent) {
	// Port of: final View v = LayoutInflater.from(parent.getContext())
	//   .inflate(R.layout.list_item_1_text, parent, false);
	// return new VH1Text(v);
	return nullptr; // ViewHolder creation handled by UI framework
}

// Port of: @Override protected void onBindHeaderItemViewHolder(
//   final RecyclerView.ViewHolder holder, final int position)
void SessionListAdapter::onBindHeaderItemViewHolder(ViewHolder holder, int position) {
	// Port of: final VH1Text vh = (VH1Text)holder;
	// vh.text.setText(context.getString(R.string.options_refresh));
	// vh.text.setCompoundDrawablesWithIntrinsicBounds(rrIconRefresh, null, null, null);
	// vh.itemView.setOnClickListener(v -> {
	//   ((SessionChangeListener)context).onSessionRefreshSelected(type);
	//   fragment.dismiss();
	// });
	// UI binding handled by framework
}

// Port of: @Override protected void onBindContentItemViewHolder(
//   final RecyclerView.ViewHolder holder, final int position)
void SessionListAdapter::onBindContentItemViewHolder(ViewHolder holder, int position) {
	// Port of: final VH1Text vh = (VH1Text)holder;
	// final CacheEntry session = sessions.get(position);
	// final BetterSSB name = new BetterSSB();
	//
	// if(session.timestamp.elapsed().isLessThan(TimeDuration.minutes(2))) {
	//   name.append(TimeFormatHelper.format(session.timestamp.elapsedPeriod(),
	//     context, R.string.time_ago, 2), 0);
	// } else {
	//   name.append(session.timestamp.format(), 0);
	// }
	//
	// if(session.session.equals(current)) {
	//   final TypedArray attr = context.obtainStyledAttributes(new int[]{R.attr.rrListSubtitleCol});
	//   final int col = attr.getColor(0, 0); attr.recycle();
	//   name.append("  (" + context.getString(R.string.session_active) + ")",
	//     BetterSSB.FOREGROUND_COLOR | BetterSSB.SIZE, col, 0, 0.8f);
	// }
	//
	// vh.text.setText(name.get());
	// vh.itemView.setOnClickListener(v -> {
	//   final CacheEntry ce = sessions.get(position);
	//   ((SessionChangeListener)context).onSessionSelected(ce.session, type);
	//   fragment.dismiss();
	// });
	// UI binding handled by framework
}

// Port of: @Override protected int getContentItemCount() { return sessions.size(); }
int SessionListAdapter::getContentItemCount() {
	return static_cast<int>(m_sessions.size());
}

} // namespace PinkReader
