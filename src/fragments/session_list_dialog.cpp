// Origin: org/quantumbadger/redreader/fragments/SessionListDialog.java
#include "fragments/session_list_dialog.h"

namespace PinkReader {

std::unique_ptr<SessionListDialog> SessionListDialog::newInstance(
        const UriString& urlParam,
        const UUID& currentParam,
        SessionChangeType typeParam) {
    auto dialog = std::make_unique<SessionListDialog>();

    // final Bundle args = new Bundle(3);
    // args.putString("url", url.toString());
    // if(current != null) {
    //     args.putString("current", current.toString());
    // }
    // args.putString("type", type.name());
    // dialog.setArguments(args);

    dialog->type = typeParam;
    dialog->current = currentParam;
    // url handled in onCreate

    return dialog;
}

void SessionListDialog::onCreate(const class Bundle* /* savedInstanceState */) {
    // super.onCreate(savedInstanceState);
    // url = new UriString(getArguments().getString("url"));
    // if(getArguments().containsKey("current")) {
    //     current = UUID.fromString(getArguments().getString("current"));
    // } else {
    //     current = null;
    // }
    // type = SessionChangeListener.SessionChangeType.valueOf(getArguments().getString("type"));
}

std::unique_ptr<void> SessionListDialog::onCreateDialog(const class Bundle* /* savedInstanceState */) {
    // super.onCreateDialog(savedInstanceState);
    if (alreadyCreated) {
        // return getDialog();
        return nullptr;
    }
    alreadyCreated = true;

    // final Context context = getContext();
    // final MaterialAlertDialogBuilder builder = new MaterialAlertDialogBuilder(context);
    // builder.setTitle(context.getString(R.string.options_past));
    // rv = new RecyclerView(context);
    // builder.setView(rv);
    // rv.setLayoutManager(new LinearLayoutManager(context));
    // rv.setAdapter(new SessionListAdapter(context, url, current, type, this));
    // rv.setHasFixedSize(true);
    // RedditAccountManager.getInstance(context).addUpdateListener(this);
    // builder.setNeutralButton(context.getString(R.string.dialog_close), null);
    // return builder.create();

    return nullptr;
}

void SessionListDialog::onRedditAccountChanged() {
    // AndroidCommon.UI_THREAD_HANDLER.post(() -> rv.getAdapter().notifyDataSetChanged());
}

} // namespace PinkReader
