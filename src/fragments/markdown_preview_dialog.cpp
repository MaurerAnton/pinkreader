// Origin: org/quantumbadger/redreader/fragments/MarkdownPreviewDialog.java
#include "fragments/markdown_preview_dialog.h"

namespace PinkReader {

std::unique_ptr<MarkdownPreviewDialog> MarkdownPreviewDialog::newInstance(
        const std::string& markdown) {
    auto dialog = std::make_unique<MarkdownPreviewDialog>();

    // final Bundle args = new Bundle(1);
    // args.putString("markdown", markdown);
    // dialog.setArguments(args);
    dialog->mMarkdown = markdown;

    return dialog;
}

std::string MarkdownPreviewDialog::getTitle(Context& context) {
    // return context.getString(R.string.comment_reply_preview);
    return "";
}

void MarkdownPreviewDialog::prepare(BaseActivity& activity, LinearLayout& items) {
    // final MarkdownParagraphGroup parsedGen
    //     = MarkdownParser.parse(getArguments().getString("markdown").toCharArray());
    // final ViewGroup parsed = parsedGen.buildView(activity, null, 14f, false);
    // final int paddingPx = General.dpToPixels(activity, 10);
    // parsed.setPadding(paddingPx, paddingPx, paddingPx, paddingPx);
    // items.addView(parsed);
}

} // namespace PinkReader
