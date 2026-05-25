// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/DialogUtils.java (lines 1-158)
// Implementation

#include "dialog_utils.h"
#include <algorithm>

namespace PinkReader {

// line 44-88: showSearchDialog variants
void DialogUtils::showSearchDialog(
        const std::string& title,
        OnSearchListener listener) {
    // Origin: creates MaterialAlertDialogBuilder with R.layout.dialog_editbox
    // editText.setImeOptions(EditorInfo.IME_ACTION_SEARCH)
    // editText.setOnEditorActionListener(onEnter)
    // Simplified: stub for non-Android C++ port
    (void)title;
    (void)listener;
}

// line 90-99: performSearch
void DialogUtils::performSearch(
        const std::string& query,
        const OnSearchListener& listener) {
    // Origin line 93: final String query = editText.getText().toString().trim();
    std::string trimmed = query;
    // Trim
    size_t start = trimmed.find_first_not_of(" \t\n\r");
    size_t end = trimmed.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) {
        trimmed.clear();
    } else {
        trimmed = trimmed.substr(start, end - start + 1);
    }

    // Origin line 94-98:
    // if(StringUtils.isEmpty(query)) listener.onSearch(null);
    // else listener.onSearch(query);
    if (trimmed.empty()) {
        listener(std::nullopt);
    } else {
        listener(trimmed);
    }
}

// line 101-123: showDialogPositiveNegative
void DialogUtils::showDialogPositiveNegative(const PositiveNegativeDialog& dialog) {
    // Origin line 110: AndroidCommon.runOnUiThread(() -> { ... })
    // Origin lines 111-121: new MaterialAlertDialogBuilder(activity)
    //     .setTitle(title).setMessage(message)
    //     .setPositiveButton(positiveText, (d,w) -> positiveAction.run())
    //     .setNegativeButton(negativeText, (d,w) -> negativeAction.run())
    //     .create().show();
    // Simplified: stub for non-Android C++ port, actions are invoked directly
    if (dialog.positiveAction) {
        dialog.positiveAction();
    }
}

// line 125-139: showDialog(String, String)
void DialogUtils::showDialog(
        const std::string& title,
        const std::string& message) {
    // Origin line 130: AndroidCommon.runOnUiThread(() -> { ... })
    // Origin lines 130-138: new MaterialAlertDialogBuilder(activity)
    //     .setTitle(title).setMessage(message)
    //     .setNeutralButton(R.string.dialog_close, (d,w) -> {})
    //     .create().show();
    // Simplified: stub for non-Android C++ port
    (void)title;
    (void)message;
}

} // namespace PinkReader
