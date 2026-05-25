// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/ShareOrderAdapter.java

#include "share_order_adapter.h"

namespace PinkReader {

// Constructor — Java lines 41-49
ShareOrderAdapter::ShareOrderAdapter(
	const QVector<QObject*>& appList,
	AppCompatDialogFragment* fragment,
	QObject* parent)
	: QObject(parent)
	, m_fragment(fragment)
	, m_appList(appList)
{
}

// Port of: @Override public int getCount() { return appList.size(); }
int ShareOrderAdapter::getCount() const {
	return m_appList.size();
}

// Port of: @Override public Object getItem(final int position)
//   return appList.get(position);
QObject* ShareOrderAdapter::getItem(int position) const {
	if (position >= 0 && position < m_appList.size()) {
		return m_appList.at(position);
	}
	return nullptr;
}

// Port of: @Override public long getItemId(final int position)
//   return position;
long ShareOrderAdapter::getItemId(int position) const {
	return static_cast<long>(position);
}

// Port of: @Override public View getView(final int position,
//   final View convertView, final ViewGroup parent)
QObject* ShareOrderAdapter::getView(int position, QObject* convertView, QObject* parent) {
	// Port of: final LayoutInflater inflater = (LayoutInflater)context
	//   .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	// View rowView = null;
	// if(inflater != null) {
	//   rowView = inflater.inflate(R.layout.list_item_share_dialog, parent, false);
	//   final TextView label = rowView.findViewById(R.id.list_item_share_dialog_text);
	//   label.setText(appList.get(position).loadLabel(packageManager).toString());
	//   final ImageView icon = rowView.findViewById(R.id.list_item_share_dialog_icon);
	//   icon.setImageDrawable(appList.get(position).loadIcon(packageManager));
	//   final View divider = rowView.findViewById(R.id.list_item_share_dialog_divider);
	//   divider.setVisibility(View.INVISIBLE);
	//   rowView.setOnClickListener(v -> {
	//     ((ShareOrderCallbackListener)fragment).onSelectedIntent(position);
	//     fragment.dismiss();
	//   });
	// }
	// return rowView;

	// In C++ port, view creation and binding is handled by UI framework
	emit shareTargetSelected(position);
	return convertView;
}

} // namespace PinkReader
