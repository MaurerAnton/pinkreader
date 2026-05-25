// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/HeaderRecyclerAdapter.java

#pragma once

#include <QObject>
#include <stdexcept>

namespace PinkReader {

// Forward declaration - ViewHolder is represented as a QObject* in C++ port
// In Android: RecyclerView.ViewHolder

/**
 * @brief Port of org.quantumbadger.redreader.adapters.HeaderRecyclerAdapter<VH>
 *
 * Abstract RecyclerView adapter with a single header item at position 0.
 * VH (ViewHolder) is represented as QObject* in this port.
 *
 * Every field, method, and constant from the original Java file.
 */
class HeaderRecyclerAdapter : public QObject {
	Q_OBJECT

protected:
	// Port of: private static final int TYPE_HEADER = 0;
	static constexpr int TYPE_HEADER = 0;
	// Port of: private static final int TYPE_CONTENT = 1;
	static constexpr int TYPE_CONTENT = 1;
	// Port of: protected static final int HEADER_SIZE = 1;
	static constexpr int HEADER_SIZE = 1;

public:
	using ViewHolder = QObject*;

	explicit HeaderRecyclerAdapter(QObject* parent = nullptr) : QObject(parent) {}
	virtual ~HeaderRecyclerAdapter() = default;

	// Port of: @NonNull @Override public VH onCreateViewHolder(
	//   @NonNull final ViewGroup parent, final int viewType)
	//   switch(viewType) { case TYPE_HEADER: ... case TYPE_CONTENT: ... default: throw }
	virtual ViewHolder onCreateViewHolder(QObject* parent, int viewType) {
		switch (viewType) {
			case TYPE_HEADER:
				return onCreateHeaderItemViewHolder(parent);
			case TYPE_CONTENT:
				return onCreateContentItemViewHolder(parent);
			default:
				throw std::logic_error("HeaderRecyclerAdapter: invalid viewType");
		}
	}

	// Port of: @Override public void onBindViewHolder(
	//   @NonNull final VH holder, final int position)
	//   if(position == 0) { onBindHeaderItemViewHolder(holder, position); }
	//   else { onBindContentItemViewHolder(holder, position - HEADER_SIZE); }
	virtual void onBindViewHolder(ViewHolder holder, int position) {
		if (position == 0) {
			onBindHeaderItemViewHolder(holder, position);
		} else {
			onBindContentItemViewHolder(holder, position - HEADER_SIZE);
		}
	}

	// Port of: @Override public int getItemCount()
	//   return getContentItemCount() + HEADER_SIZE;
	virtual int getItemCount() {
		return getContentItemCount() + HEADER_SIZE;
	}

	// Port of: @Override public int getItemViewType(final int position)
	//   return position == 0 ? TYPE_HEADER : TYPE_CONTENT;
	virtual int getItemViewType(int position) {
		return position == 0 ? TYPE_HEADER : TYPE_CONTENT;
	}

protected:
	// Port of: protected abstract VH onCreateHeaderItemViewHolder(ViewGroup parent);
	virtual ViewHolder onCreateHeaderItemViewHolder(QObject* parent) = 0;

	// Port of: protected abstract VH onCreateContentItemViewHolder(ViewGroup parent);
	virtual ViewHolder onCreateContentItemViewHolder(QObject* parent) = 0;

	// Port of: protected abstract void onBindHeaderItemViewHolder(VH holder, int position);
	virtual void onBindHeaderItemViewHolder(ViewHolder holder, int position) = 0;

	// Port of: protected abstract void onBindContentItemViewHolder(VH holder, int position);
	virtual void onBindContentItemViewHolder(ViewHolder holder, int position) = 0;

	// Port of: protected abstract int getContentItemCount();
	virtual int getContentItemCount() = 0;
};

} // namespace PinkReader
