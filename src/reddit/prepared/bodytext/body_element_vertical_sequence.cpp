// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementVerticalSequence.java
// Ported to C++ for PinkReader

#include "body_element_vertical_sequence.h"

#include "block_type.h"

// Placeholder: #include "general.h"

namespace PinkReader {

BodyElementVerticalSequence::BodyElementVerticalSequence(
        std::vector<BodyElement*> elements)
    : BodyElement(BlockType::VERTICAL_SEQUENCE)
    , m_elements(std::move(elements))
{
}

QWidget* BodyElementVerticalSequence::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: Qt VBoxLayout equivalent
    // final LinearLayout result = new LinearLayout(activity);
    // result.setOrientation(LinearLayout.VERTICAL);
    // final float dpScale = activity.getResources().getDisplayMetrics().density;
    // final int paragraphSpacing = (int)(dpScale * 6);
    // BlockType lastBlock = null;
    //
    // for (final BodyElement element : m_elements) {
    //     final View view = element->generateView(activity, textColor, textSize, showLinkButtons);
    //     result.addView(view);
    //     final LinearLayout.LayoutParams layoutParams
    //         = (LinearLayout.LayoutParams)view.getLayoutParams();
    //     if (lastBlock.has_value() &&
    //         !(element->getType() == BlockType::LIST_ELEMENT
    //           && lastBlock == BlockType::LIST_ELEMENT)) {
    //         layoutParams.topMargin = paragraphSpacing;
    //     }
    //     view.setLayoutParams(layoutParams);
    //     lastBlock = element->getType();
    // }
    //
    // General::setLayoutMatchWidthWrapHeight(result);
    // return result;

    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
