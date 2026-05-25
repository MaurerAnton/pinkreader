// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementNumberedListElement.java
// Ported to C++ for PinkReader

#include "body_element_numbered_list.h"
#include "body_element_vertical_sequence.h"

// Placeholder: #include "general.h"

namespace PinkReader {

BodyElementNumberedListElement::BodyElementNumberedListElement(
        int listIndex,
        std::vector<BodyElement*> elements)
    : BodyElement(BlockType::LIST_ELEMENT)
    , m_listIndex(listIndex)
    , m_elements(std::move(elements))
{
}

QWidget* BodyElementNumberedListElement::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: Qt layout equivalent
    // final LinearLayout outerLayout = new LinearLayout(activity);
    // final int paddingPx = General::dpToPixels(activity, 6);
    // outerLayout.setPadding(paddingPx, 0, paddingPx, 0);
    // final TextView number = new TextView(activity);
    // number.setText(m_listIndex + ".  ");
    // if (textSize) { number.setTextSize(*textSize); }
    // outerLayout.addView(number);
    //
    // if (m_elements.size() == 1) {
    //     outerLayout.addView(m_elements[0]->generateView(...));
    // } else {
    //     outerLayout.addView(BodyElementVerticalSequence(m_elements).generateView(...));
    // }
    // General::setLayoutMatchWidthWrapHeight(outerLayout);
    // return outerLayout;

    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
