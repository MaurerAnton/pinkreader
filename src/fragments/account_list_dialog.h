// Origin: org/quantumbadger/redreader/fragments/AccountListDialog.java
#pragma once

#include <memory>

namespace PinkReader {

class AppCompatActivity;
class RecyclerView;

class AccountListDialog {
public:
    static void show(AppCompatActivity& activity);

    void onActivityResult(int requestCode, int resultCode, class Intent& data);

    std::unique_ptr<void> onCreateDialog(const class Bundle* savedInstanceState);

    void onRedditAccountChanged();

private:
    AccountListDialog() = default;

    AppCompatActivity* mActivity = nullptr;

    // Workaround for HoloEverywhere bug?
    volatile bool alreadyCreated = false;

    std::unique_ptr<RecyclerView> rv;
};

} // namespace PinkReader
