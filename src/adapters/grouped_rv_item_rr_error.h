// Origin: org/quantumbadger/redreader/adapters/GroupedRecyclerViewItemRRError.java
#pragma once

#include <memory>

namespace PinkReader {

class AppCompatActivity;
class RRError;
class ViewGroup;
class FrameLayout;

class GroupedRecyclerViewItemRRError {
public:
    // Inner class ErrorHolder
    class ErrorHolder {
    public:
        explicit ErrorHolder(AppCompatActivity& activity);
        void bind(const RRError& error);

    private:
        AppCompatActivity& mActivity;
        std::unique_ptr<FrameLayout> mItemView;
    };

    GroupedRecyclerViewItemRRError(AppCompatActivity& activity, const RRError& error);

    const std::type_info& getViewType() const;
    std::unique_ptr<ErrorHolder> onCreateViewHolder(ViewGroup& viewGroup);
    void onBindViewHolder(ErrorHolder& viewHolder);
    bool isHidden() const;

private:
    AppCompatActivity& mActivity;
    const RRError& mError;
};

} // namespace PinkReader
