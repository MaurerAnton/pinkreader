// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/DialogUtils.java (lines 1-158)
// Ported from Java to C++

#pragma once

#include <string>
#include <functional>
#include <optional>

namespace PinkReader {

// DialogUtils.java line 39: public class DialogUtils
class DialogUtils {
public:
    // line 40-42: public interface OnSearchListener
    using OnSearchListener = std::function<void(std::optional<std::string>)>;

    // line 44-47: showSearchDialog(Context, OnSearchListener)
    // line 50-88: showSearchDialog(Context, int, OnSearchListener)
    // Simplified: non-Android C++ port returns prompt data
    struct SearchDialogData {
        std::string title = "Search";
        std::string hint;
        OnSearchListener listener;
    };

    static void showSearchDialog(
            const std::string& title,
            OnSearchListener listener);

    // line 101-123: showDialogPositiveNegative
    struct PositiveNegativeDialog {
        std::string title;
        std::string message;
        std::string positiveText = "Yes";
        std::string negativeText = "No";
        std::function<void()> positiveAction;
        std::function<void()> negativeAction;
    };

    static void showDialogPositiveNegative(const PositiveNegativeDialog& dialog);

    // line 125-139: showDialog(Activity, String, String)
    // line 142-157: showDialog(Activity, int, int)
    static void showDialog(
            const std::string& title,
            const std::string& message);

private:
    // line 90-99: performSearch
    static void performSearch(
            const std::string& query,
            const OnSearchListener& listener);
};

} // namespace PinkReader
