// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementBullet.java
// Ported to C++ for PinkReader

#include "body_element_bullet.h"
#include "body_element_vertical_sequence.h"

// Placeholder: #include "general.h"

namespace PinkReader {

BodyElementBullet::BodyElementBullet(std::vector<BodyElement*> elements)
    : BodyElement(BlockType::LIST_ELEMENT)
    , m_elements(std::move(elements))
{
}

QWidget* BodyElementBullet::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: Qt LinearLayout equivalent
    // final LinearLayout bulletItem = new LinearLayout(activity);
    // final int paddingPx = General::dpToPixels(activity, 6);
    // bulletItem.setPadding(paddingPx, 0, paddingPx, 0);
    // final TextView bullet = new TextView(activity);
    // bullet.setText("\u2022   ");
    // if (textSize) { bullet.setTextSize(*textSize); }
    // bulletItem.addView(bullet);
    //
    // if (m_elements.size() == 1) {
    //     bulletItem.addView(m_elements[0]->generateView(...));
    // } else {
    //     bulletItem.addView(BodyElementVerticalSequence(m_elements).generateView(...));
    // }
    // General::setLayoutMatchWidthWrapHeight(bulletItem);
    // return bulletItem;

    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
