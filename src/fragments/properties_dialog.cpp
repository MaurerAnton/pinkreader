// Origin: org/quantumbadger/redreader/fragments/PropertiesDialog.java
#include "fragments/properties_dialog.h"

namespace PinkReader {

std::unique_ptr<void> PropertiesDialog::onCreateDialog(const Bundle* /* savedInstanceState */) {
    // super.onCreateDialog(savedInstanceState);
    if (alreadyCreated) {
        // return getDialog();
        return nullptr; // Placeholder
    }
    alreadyCreated = true;

    // final BaseActivity activity = (BaseActivity)getActivity();
    // final TypedArray attr = activity.obtainStyledAttributes(new int[] {
    //     androidx.appcompat.R.attr.colorPrimary,
    //     R.attr.rrMainTextCol
    // });
    // colorPrimary = attr.getColor(0, 0);
    // rrCommentBodyCol = attr.getColor(1, 0);
    // attr.recycle();

    // final MaterialAlertDialogBuilder builder = new MaterialAlertDialogBuilder(activity);
    // final LinearLayout items = new LinearLayout(activity);
    // items.setOrientation(LinearLayout.VERTICAL);
    // final int hPaddingPx = General.dpToPixels(activity, 12);
    // items.setPadding(hPaddingPx, 0, hPaddingPx, 0);
    // prepare(activity, items);
    // builder.setTitle(getTitle(activity));
    // final ScrollView sv = new ScrollView(activity);
    // sv.addView(items);
    // builder.setView(sv);
    // builder.setNeutralButton(R.string.dialog_close, null);
    // interceptBuilder(builder);
    // return builder.create();

    return nullptr; // Placeholder
}

void PropertiesDialog::interceptBuilder(MaterialAlertDialogBuilder& /* builder */) {
    // Do nothing by default
}

std::unique_ptr<LinearLayout> PropertiesDialog::propView(
        Context& context,
        int titleRes,
        int textRes,
        bool firstInList) {
    // return propView(context, context.getString(titleRes), getString(textRes), firstInList);
    return propView(context, "", nullptr, firstInList); // Placeholder
}

std::unique_ptr<LinearLayout> PropertiesDialog::propView(
        Context& context,
        const std::string& title,
        const std::string* text,
        bool firstInList) {
    const std::string textStr = (text == nullptr) ? "<null>" : *text;
    return propViewImpl(context, title, textStr, firstInList);
}

std::unique_ptr<LinearLayout> PropertiesDialog::propViewImpl(
        Context& context,
        const std::string& title,
        const std::string& text,
        bool /* firstInList */) {
    // final int paddingPixels = General.dpToPixels(context, 12);
    // final LinearLayout prop = new LinearLayout(context);
    // prop.setOrientation(LinearLayout.VERTICAL);
    //
    // final TextView titleView = new MaterialTextView(context);
    // titleView.setText(title);
    // titleView.setTextColor(colorPrimary);
    // titleView.setTextSize(14.0f);
    // titleView.setPadding(paddingPixels, paddingPixels, paddingPixels, 0);
    // prop.addView(titleView);
    //
    // final TextView textView = new MaterialTextView(context);
    // textView.setText(text == null ? "<null>" : text);
    // textView.setTextColor(rrCommentBodyCol);
    // textView.setTextSize(16.0f);
    // textView.setPadding(paddingPixels, 0, paddingPixels, 0);
    // textView.setTextIsSelectable(true);
    // textView.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
    // prop.addView(textView);
    //
    // prop.setContentDescription(title + "\n" + text);
    // return prop;

    auto prop = std::make_unique<LinearLayout>(context);
    prop->setOrientation(LinearLayout::VERTICAL);

    int paddingPixels = 12; // dpToPixels(context, 12)

    prop->addTitleView(title, colorPrimary, 14.0f, paddingPixels);
    prop->addTextView(text, rrCommentBodyCol, 16.0f, paddingPixels);
    prop->setContentDescription(title + "\n" + text);

    return prop;
}

} // namespace PinkReader
