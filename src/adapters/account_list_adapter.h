// Origin: org/quantumbadger/redreader/adapters/AccountListAdapter.kt
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace PinkReader {

class AppCompatActivity;
class Fragment;
class Drawable;
class RedditAccount;

// AccountAction inner class (was private inner class in Kotlin)
struct AccountAction {
    int message; // String resource ID
    std::function<void()> action;

    AccountAction(int msg, std::function<void()> act)
        : message(msg), action(std::move(act)) {
    }
};

class AccountListAdapter {
public:
    AccountListAdapter(AppCompatActivity& context, Fragment& fragment);

    // RecyclerView.Adapter / HeaderRecyclerAdapter methods
    void* onCreateHeaderItemViewHolder(void* parent);
    void* onCreateContentItemViewHolder(void* parent);
    void onBindHeaderItemViewHolder(void* holder, int position);
    void onBindContentItemViewHolder(void* holder, int position);
    int getContentItemCount() const;

private:
    void showLoginWarningDialog();
    void launchLogin();

    AppCompatActivity& mContext;
    Fragment& mFragment;
    std::vector<RedditAccount> mAccounts;
    std::unique_ptr<Drawable> mRrIconAdd;
    std::unique_ptr<Drawable> mRrIconUser;
};

} // namespace PinkReader
