// Origin: org/quantumbadger/redreader/adapters/GroupedRecyclerViewItemFrameLayout.java
#include "adapters/grouped_rv_item_frame_layout.h"
#include <typeinfo>

namespace PinkReader {

GroupedRecyclerViewItemFrameLayout::GroupedRecyclerViewItemFrameLayout(
        std::unique_ptr<View> childView)
    : mChildView(std::move(childView)) {
}

const std::type_info& GroupedRecyclerViewItemFrameLayout::getViewType() const {
    return typeid(GroupedRecyclerViewItemFrameLayout);
}

std::unique_ptr<void*> GroupedRecyclerViewItemFrameLayout::onCreateViewHolder(
        ViewGroup& viewGroup) {
    // General.setLayoutMatchWidthWrapHeight(viewGroup);
    // final FrameLayout frameLayout = new FrameLayout(viewGroup.getContext());
    // return new RecyclerView.ViewHolder(frameLayout) {};
    auto frameLayout = std::make_unique<FrameLayout>(viewGroup.getContext());
    return std::make_unique<void*>(static_cast<void*>(frameLayout.release()));
}

void GroupedRecyclerViewItemFrameLayout::onBindViewHolder(void* viewHolder) {
    // final FrameLayout view = (FrameLayout)viewHolder.itemView;
    // view.removeAllViews();
    // if(mParent != null && mChildView.getParent() == mParent) {
    //     mParent.removeAllViews();
    // }
    // mParent = view;
    // view.addView(mChildView);
    // General.setLayoutMatchWidthWrapHeight(mChildView);
    auto* view = static_cast<FrameLayout*>(viewHolder);
    view->removeAllViews();

    if (mParent != nullptr && mChildView->getParent() == mParent) {
        mParent->removeAllViews();
    }

    mParent = view;
    view->addView(mChildView.get());
    // setLayoutMatchWidthWrapHeight equivalent
}

bool GroupedRecyclerViewItemFrameLayout::isHidden() const {
    return mHidden;
}

void GroupedRecyclerViewItemFrameLayout::setHidden(bool hidden) {
    mHidden = hidden;
}

} // namespace PinkReader
