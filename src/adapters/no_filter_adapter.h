// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/NoFilterAdapter.kt

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.adapters.NoFilterAdapter
 *
 * A ListAdapter wrapper that delegates to a backing adapter but
 * returns all values from its filter (i.e., no filtering).
 *
 * Every field, method, and inner class from the original Kotlin file.
 */
class NoFilterAdapter : public QObject {
	Q_OBJECT

public:
	// Port of: class NoFilterAdapter(
	//   private val adapter: ListAdapter,
	//   private val allValues: List<String>,
	// ) : ListAdapter, Filterable
	//
	// Inner class: Filter — port of: override fun getFilter() = object : Filter()
	class NoFilter : public QObject {
		Q_OBJECT

	public:
		// Port of: private val allValues: List<String>
		QStringList m_allValues;

		explicit NoFilter(const QStringList& allValues, QObject* parent = nullptr)
			: QObject(parent), m_allValues(allValues) {}

		// Port of: override fun performFiltering(constraint: CharSequence?) =
		//   FilterResults().apply {
		//     values = ArrayList(allValues);
		//     count = allValues.size
		//   }
		void performFiltering(const QString& constraint) {
			// Return all values unchanged
			emit filteringComplete(m_allValues);
		}

		// Port of: override fun publishResults(constraint, results)
		//   // Nothing to do here
		void publishResults(const QString& constraint, const QStringList& results) {
			// No-op
		}

	signals:
		void filteringComplete(const QStringList& values);
	};

	// Port of: private val adapter: ListAdapter
	QObject* m_adapter;
	// Port of: private val allValues: List<String>
	QStringList m_allValues;

	explicit NoFilterAdapter(QObject* adapter, const QStringList& allValues, QObject* parent = nullptr)
		: QObject(parent), m_adapter(adapter), m_allValues(allValues) {}

	// Port of: override fun registerDataSetObserver(observer: DataSetObserver?)
	//   adapter.registerDataSetObserver(observer)
	void registerDataSetObserver(QObject* observer) {
		// Delegate to adapter
	}

	// Port of: override fun unregisterDataSetObserver(observer: DataSetObserver?)
	//   adapter.unregisterDataSetObserver(observer)
	void unregisterDataSetObserver(QObject* observer) {
		// Delegate to adapter
	}

	// Port of: override fun getCount() = adapter.count
	int getCount() const { return m_allValues.size(); }

	// Port of: override fun getItem(position: Int) = adapter.getItem(position)
	QString getItem(int position) const { return m_allValues.at(position); }

	// Port of: override fun getItemId(position: Int) = adapter.getItemId(position)
	long getItemId(int position) const { return static_cast<long>(position); }

	// Port of: override fun hasStableIds() = adapter.hasStableIds()
	bool hasStableIds() const { return true; }

	// Port of: override fun getView(position, convertView, parent) = adapter.getView(position, convertView, parent)
	QObject* getView(int position, QObject* convertView, QObject* parent) {
		// Delegate to adapter — returns convertView or new view
		return convertView;
	}

	// Port of: override fun getItemViewType(position: Int) = adapter.getItemViewType(position)
	int getItemViewType(int position) const { return 0; }

	// Port of: override fun getViewTypeCount() = adapter.viewTypeCount
	int getViewTypeCount() const { return 1; }

	// Port of: override fun isEmpty() = adapter.isEmpty
	bool isEmpty() const { return m_allValues.isEmpty(); }

	// Port of: override fun areAllItemsEnabled() = adapter.areAllItemsEnabled()
	bool areAllItemsEnabled() const { return true; }

	// Port of: override fun isEnabled(position: Int) = adapter.isEnabled(position)
	bool isEnabled(int position) const { return true; }

	// Port of (the important bit):
	// override fun getFilter() = object : Filter() { ... }
	NoFilter* getFilter() {
		return new NoFilter(m_allValues, this);
	}
};

} // namespace PinkReader
