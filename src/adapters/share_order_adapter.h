// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/ShareOrderAdapter.java

#pragma once

#include <QObject>
#include <QVector>
#include <vector>
#include <memory>

namespace PinkReader {

// Forward declarations
class AppCompatDialogFragment;
class ShareOrderCallbackListener;

/**
 * @brief Port of org.quantumbadger.redreader.adapters.ShareOrderAdapter
 *
 * BaseAdapter for share order selection dialog. Lists apps that can
 * receive share intents. Every field, method, and logic branch from
 * the original Java file.
 */
class ShareOrderAdapter : public QObject {
	Q_OBJECT

public:
	// Port of: private final Context context;
	// In C++ port: context is parent QObject
	// Port of: private final List<ResolveInfo> appList;
	// Ported as a QVector of opaque app info objects
	// Port of: private final PackageManager packageManager;
	// In C++ port: package access handled by framework
	// Port of: private final AppCompatDialogFragment fragment;
	AppCompatDialogFragment* m_fragment = nullptr;

	// Port of: private final List<ResolveInfo> appList;
	// In C++ port: stored as opaque pointers
	QVector<QObject*> m_appList;

	// Port of: public ShareOrderAdapter(
	//   final Context context, final List<ResolveInfo> appList,
	//   final AppCompatDialogFragment fragment)
	//   this.context = context; this.appList = appList;
	//   this.packageManager = context.getPackageManager();
	//   this.fragment = fragment;
	explicit ShareOrderAdapter(const QVector<QObject*>& appList,
	                           AppCompatDialogFragment* fragment,
	                           QObject* parent = nullptr);

	virtual ~ShareOrderAdapter() = default;

	// Port of: @Override public int getCount() { return appList.size(); }
	int getCount() const;

	// Port of: @Override public Object getItem(final int position)
	//   return appList.get(position);
	QObject* getItem(int position) const;

	// Port of: @Override public long getItemId(final int position)
	//   return position;
	long getItemId(int position) const;

	// Port of: @Override public View getView(final int position,
	//   final View convertView, final ViewGroup parent)
	//   final LayoutInflater inflater = (LayoutInflater)context
	//     .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	//   View rowView = null;
	//   if(inflater != null) {
	//     rowView = inflater.inflate(R.layout.list_item_share_dialog, parent, false);
	//     final TextView label = rowView.findViewById(R.id.list_item_share_dialog_text);
	//     label.setText(appList.get(position).loadLabel(packageManager).toString());
	//     final ImageView icon = rowView.findViewById(R.id.list_item_share_dialog_icon);
	//     icon.setImageDrawable(appList.get(position).loadIcon(packageManager));
	//     final View divider = rowView.findViewById(R.id.list_item_share_dialog_divider);
	//     divider.setVisibility(View.INVISIBLE);
	//     rowView.setOnClickListener(v -> {
	//       ((ShareOrderCallbackListener)fragment).onSelectedIntent(position);
	//       fragment.dismiss();
	//     });
	//   }
	//   return rowView;
	QObject* getView(int position, QObject* convertView, QObject* parent);

signals:
	// Signal emitted when a share target is selected
	void shareTargetSelected(int position);
};

} // namespace PinkReader
