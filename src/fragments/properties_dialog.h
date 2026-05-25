// Origin: org/quantumbadger/redreader/fragments/PropertiesDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class Context;
class BaseActivity;
class Bundle;
class MaterialAlertDialogBuilder;
class LinearLayout;
class AppCompatDialogFragment;

// Abstract base class for properties dialogs
class PropertiesDialog {
public:
    virtual ~PropertiesDialog() = default;

protected:
    int colorPrimary = 0;
    int rrCommentBodyCol = 0;

    // Workaround for HoloEverywhere bug?
    volatile bool alreadyCreated = false;

    virtual std::string getTitle(Context& context) = 0;

    virtual void prepare(BaseActivity& context, LinearLayout& items) = 0;

    std::unique_ptr<void> onCreateDialog(const Bundle* savedInstanceState);

    virtual void interceptBuilder(MaterialAlertDialogBuilder& builder);

    // propView overloads
    std::unique_ptr<LinearLayout> propView(
            Context& context,
            int titleRes,
            int textRes,
            bool firstInList);

    std::unique_ptr<LinearLayout> propView(
            Context& context,
            const std::string& title,
            const std::string* text,
            bool firstInList);

private:
    std::unique_ptr<LinearLayout> propViewImpl(
            Context& context,
            const std::string& title,
            const std::string& text,
            bool firstInList);
};

} // namespace PinkReader
