// Origin: org/quantumbadger/redreader/adapters/GroupedRecyclerViewItemView.java
#include "adapters/grouped_rv_item_view.h"
#include <typeinfo>

namespace PinkReader {

GroupedRecyclerViewItemView::GroupedRecyclerViewItemView(
        const std::type_info& viewType,
        ViewFactory factory)
    : mFactory(std::move(factory))
    , mViewType(viewType) {
}

const std::type_info& GroupedRecyclerViewItemView::getViewType() const {
    return mViewType;
}

std::unique_ptr<RecyclerViewViewHolder> GroupedRecyclerViewItemView::onCreateViewHolder(
        ViewGroup& viewGroup) {
    auto view = mFactory(viewGroup);
    return std::make_unique<RecyclerViewViewHolder>(std::move(view));
}

void GroupedRecyclerViewItemView::onBindViewHolder(
        std::unique_ptr<RecyclerViewViewHolder>& /* viewHolder */) {
    // Nothing to do here
}

bool GroupedRecyclerViewItemView::isHidden() const {
    return mHidden;
}

void GroupedRecyclerViewItemView::setHidden(bool hidden) {
    mHidden = hidden;
}

} // namespace PinkReader
