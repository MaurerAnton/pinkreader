// Origin: org/quantumbadger/redreader/adapters/GroupedRecyclerViewItemFrameLayout.java
#pragma once

#include <memory>

namespace PinkReader {

class View;
class ViewGroup;
class FrameLayout;

class GroupedRecyclerViewItemFrameLayout {
public:
    explicit GroupedRecyclerViewItemFrameLayout(std::unique_ptr<View> childView);

    const std::type_info& getViewType() const;
    std::unique_ptr<void*> onCreateViewHolder(ViewGroup& viewGroup);
    void onBindViewHolder(void* viewHolder);
    bool isHidden() const;
    void setHidden(bool hidden);

private:
    std::unique_ptr<View> mChildView;
    bool mHidden = false;
    FrameLayout* mParent = nullptr;
};

} // namespace PinkReader
