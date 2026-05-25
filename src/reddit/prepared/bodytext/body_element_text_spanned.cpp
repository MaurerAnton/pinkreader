// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementTextSpanned.java
// Ported to C++ for PinkReader

#include "body_element_text_spanned.h"

// Placeholder: #include "android_common.h"
// Placeholder: #include "linkified_text_view.h"
// Placeholder: #include "prefs_utility.h"

namespace PinkReader {

BodyElementTextSpanned::BodyElementTextSpanned(
        BlockType blockType,
        const SpannableStringBuilder& spanned)
    : BodyElement(blockType)
    , m_spanned(spanned)
{
}

void BodyElementTextSpanned::addSpanDynamic(
        void* what,
        int start,
        int end,
        int flags)
{
    // Placeholder: AndroidCommon.runOnUiThread(() -> {
    //     m_spanned.setSpan(what, start, end, flags);
    //     if (m_textView != nullptr) {
    //         m_textView->setText(m_spanned);
    //     }
    // });

    // For now, directly apply changes
    // m_spanned.setSpan(what, start, end, flags);
    // if (m_textView != nullptr) {
    //     m_textView->setText(m_spanned);
    // }
}

QWidget* BodyElementTextSpanned::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: Qt label equivalent
    // m_textView = new LinkifiedTextView(activity);
    // if (textColor) { m_textView->setTextColor(*textColor); }
    // if (textSize) { m_textView->setTextSize(*textSize); }
    // m_textView->setText(m_spanned, LinkifiedTextView::BufferType::SPANNABLE);
    // if (PrefsUtility::pref_accessibility_separate_body_text_lines()) {
    //     m_textView->setFocusable(true);
    // }
    // return m_textView;

    m_textView = nullptr; // Will be set by actual implementation
    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
