// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementHorizontalRule.java
// Ported to C++ for PinkReader

#include "body_element_horizontal_rule.h"

// Placeholder: #include "general.h"

namespace PinkReader {

BodyElementHorizontalRule::BodyElementHorizontalRule()
    : BodyElement(BlockType::HORIZONTAL_RULE)
{
}

QWidget* BodyElementHorizontalRule::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: Android View equivalent
    // final int paddingPx = General::dpToPixels(activity, 3);
    // final int thicknessPx = General::dpToPixels(activity, 1);
    // final View divider = new View(activity);
    // final ViewGroup.MarginLayoutParams layoutParams
    //     = new ViewGroup.MarginLayoutParams(
    //         ViewGroup.LayoutParams.MATCH_PARENT, thicknessPx);
    // layoutParams.leftMargin = paddingPx;
    // layoutParams.rightMargin = paddingPx;
    // divider.setBackgroundColor(Color.GRAY);
    // divider.setLayoutParams(layoutParams);
    // return divider;

    auto* placeholder = new QWidget(&activity);
    placeholder->setFixedHeight(1);
    placeholder->setStyleSheet("background-color: gray;");
    return placeholder;
}

} // namespace PinkReader
