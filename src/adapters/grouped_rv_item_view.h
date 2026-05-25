// Origin: org/quantumbadger/redreader/adapters/GroupedRecyclerViewItemView.java
#pragma once

#include <functional>
#include <memory>

namespace PinkReader {

class ViewGroup;
class View;

template<typename T>
class GroupedRecyclerViewAdapterItem {
public:
    virtual ~GroupedRecyclerViewAdapterItem() = default;
    virtual const std::type_info& getViewType() const = 0;
    virtual T onCreateViewHolder(ViewGroup& viewGroup) = 0;
    virtual void onBindViewHolder(T& viewHolder) = 0;
    virtual bool isHidden() const = 0;
};

// FunctionOneArgWithReturn<ViewGroup, View> equivalent
using ViewFactory = std::function<std::unique_ptr<View>(ViewGroup&)>;

class GroupedRecyclerViewItemView : public GroupedRecyclerViewAdapterItem<std::unique_ptr<RecyclerViewViewHolder>> {
public:
    GroupedRecyclerViewItemView(const std::type_info& viewType, ViewFactory factory);

    const std::type_info& getViewType() const override;
    std::unique_ptr<RecyclerViewViewHolder> onCreateViewHolder(ViewGroup& viewGroup) override;
    void onBindViewHolder(std::unique_ptr<RecyclerViewViewHolder>& viewHolder) override;
    bool isHidden() const override;

    void setHidden(bool hidden);

private:
    ViewFactory mFactory;
    const std::type_info& mViewType;  // reference to type_info for comparison
    bool mHidden = false;
};

} // namespace PinkReader
