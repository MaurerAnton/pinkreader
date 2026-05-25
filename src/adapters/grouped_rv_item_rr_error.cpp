// Origin: org/quantumbadger/redreader/adapters/GroupedRecyclerViewItemRRError.java
#include "adapters/grouped_rv_item_rr_error.h"
#include <typeinfo>

namespace PinkReader {

// ErrorHolder inner class
GroupedRecyclerViewItemRRError::ErrorHolder::ErrorHolder(AppCompatActivity& activity)
    : mActivity(activity) {
    // super(new FrameLayout(mActivity));
    // In C++: create FrameLayout equivalent
    mItemView = std::make_unique<FrameLayout>(activity);
}

void GroupedRecyclerViewItemRRError::ErrorHolder::bind(const RRError& error) {
    // final FrameLayout itemView = (FrameLayout)this.itemView;
    // itemView.removeAllViews();
    // itemView.addView(new ErrorView(mActivity, error));
    mItemView->removeAllViews();
    // ErrorView creation: placeholder
    mItemView->addErrorView(error);
}

// GroupedRecyclerViewItemRRError
GroupedRecyclerViewItemRRError::GroupedRecyclerViewItemRRError(
        AppCompatActivity& activity,
        const RRError& error)
    : mActivity(activity)
    , mError(error) {
}

const std::type_info& GroupedRecyclerViewItemRRError::getViewType() const {
    return typeid(GroupedRecyclerViewItemRRError);
}

std::unique_ptr<GroupedRecyclerViewItemRRError::ErrorHolder>
GroupedRecyclerViewItemRRError::onCreateViewHolder(ViewGroup& /* viewGroup */) {
    return std::make_unique<ErrorHolder>(mActivity);
}

void GroupedRecyclerViewItemRRError::onBindViewHolder(ErrorHolder& viewHolder) {
    viewHolder.bind(mError);
}

bool GroupedRecyclerViewItemRRError::isHidden() const {
    return false;
}

} // namespace PinkReader
