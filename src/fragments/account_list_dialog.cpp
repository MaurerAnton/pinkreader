// Origin: org/quantumbadger/redreader/fragments/AccountListDialog.java
#include "fragments/account_list_dialog.h"

namespace PinkReader {

void AccountListDialog::show(AppCompatActivity& activity) {
    auto dialog = std::make_unique<AccountListDialog>();
    // dialog->show(activity.getSupportFragmentManager(), null);
    // In C++, equivalent: show dialog on activity
}

void AccountListDialog::onActivityResult(
        int requestCode,
        int resultCode,
        Intent& data) {
    // if(requestCode == 123 && requestCode == resultCode && data.hasExtra("url")) {
    //     final Uri uri = Uri.parse(data.getStringExtra("url"));
    //     RedditOAuth.completeLogin(mActivity, uri, RunnableOnce.DO_NOTHING);
    // }
    if (requestCode == 123 && requestCode == resultCode /* && data.hasExtra("url") */) {
        // Uri uri = ...;
        // RedditOAuth::completeLogin(*mActivity, uri, RunnableOnce::DO_NOTHING);
    }
}

std::unique_ptr<void> AccountListDialog::onCreateDialog(const Bundle* /* savedInstanceState */) {
    // super.onCreateDialog(savedInstanceState);
    if (alreadyCreated) {
        // return getDialog();
        return nullptr;
    }
    alreadyCreated = true;

    // mActivity = (AppCompatActivity)getActivity();
    // final MaterialAlertDialogBuilder builder = new MaterialAlertDialogBuilder(mActivity);
    // builder.setTitle(mActivity.getString(R.string.options_accounts_long));
    // rv = new RecyclerView(mActivity);
    // rv.setLayoutManager(new LinearLayoutManager(mActivity));
    // rv.setAdapter(new AccountListAdapter(mActivity, this));
    // rv.setHasFixedSize(true);
    // final int paddingPx = General.dpToPixels(mActivity, 16f);
    // rv.setPadding(paddingPx, paddingPx, paddingPx, 0);
    // RedditAccountManager.getInstance(mActivity).addUpdateListener(this);
    // builder.setNeutralButton(mActivity.getString(R.string.dialog_close), null);
    // builder.setView(rv);
    // return builder.create();
    return nullptr; // Placeholder - UI components
}

void AccountListDialog::onRedditAccountChanged() {
    // AndroidCommon.UI_THREAD_HANDLER.post(() -> {
    //     rv.setAdapter(new AccountListAdapter(mActivity, this));
    //     if(mActivity instanceof BaseActivity) {
    //         AndroidCommon.promptForNotificationPermission((BaseActivity) mActivity, null);
    //     }
    // });
}

} // namespace PinkReader
