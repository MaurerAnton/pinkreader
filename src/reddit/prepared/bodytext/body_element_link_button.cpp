// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementLinkButton.java
// Ported to C++ for PinkReader

#include "body_element_link_button.h"

// Placeholder: #include "link_handler.h"

namespace PinkReader {

BodyElementLinkButton::BodyElementLinkButton(const LinkButtonDetails& details)
    : BodyElementBaseButton(
        details.getButtonTitle(),
        details.getButtonSubtitle(),
        true)
    , m_details(details)
{
}

QWidget::ClickListener BodyElementLinkButton::generateOnClickListener(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: return (button) -> LinkHandler::onLinkClicked(activity, m_details.url, false);
    return QWidget::ClickListener();
}

std::optional<QWidget::LongClickListener> BodyElementLinkButton::generateOnLongClickListener(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: return (button) -> { LinkHandler::onLinkLongClicked(activity, m_details.url); return true; };
    return std::nullopt;
}

} // namespace PinkReader
