// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementBaseButton.java
// Ported to C++ for PinkReader

#include "body_element_base_button.h"

// Placeholder includes for types not yet ported:
// #include "link_details_view.h"
// #include "general.h"

namespace PinkReader {

BodyElementBaseButton::BodyElementBaseButton(
        const QString& text,
        std::optional<QString> subtitle,
        bool isLinkButton)
    : BodyElement(BlockType::BUTTON)
    , m_text(text)
    , m_subtitle(subtitle)
    , m_isLinkButton(isLinkButton)
{
}

QWidget* BodyElementBaseButton::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    if (m_isLinkButton && !showLinkButtons) {
        // Don't show
        auto* result = new QWidget(&activity);
        result->setVisible(false);
        return result;
    }

    // Placeholder: LinkDetailsView not yet ported
    // final LinkDetailsView ldv = new LinkDetailsView(activity, m_text, m_subtitle);
    // final int linkMarginPx = General::dpToPixels(activity, 8);
    // ...
    // ldv->setOnClickListener(generateOnClickListener(activity, textColor, textSize, showLinkButtons));
    // auto longClickListener = generateOnLongClickListener(activity, textColor, textSize, showLinkButtons);
    // if (longClickListener) { ldv->setOnLongClickListener(*longClickListener); }
    // return ldv;

    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
