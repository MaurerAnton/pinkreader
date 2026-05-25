// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementTableRow.java
// Ported to C++ for PinkReader

#include "body_element_table_row.h"

namespace PinkReader {

BodyElementTableRow::BodyElementTableRow(std::vector<BodyElement*> elements)
    : BodyElement(BlockType::TABLE_ROW)
    , m_elements(std::move(elements))
{
}

QWidget* BodyElementTableRow::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: Android TableRow equivalent
    // final TableRow result = new TableRow(activity);
    // for (final BodyElement element : m_elements) {
    //     final View view = element->generateView(activity, textColor, textSize, showLinkButtons);
    //     result.addView(view);
    //     final TableRow.LayoutParams layoutParams
    //         = (TableRow.LayoutParams)view.getLayoutParams();
    //     layoutParams.width = ViewGroup.LayoutParams.WRAP_CONTENT;
    //     layoutParams.height = ViewGroup.LayoutParams.WRAP_CONTENT;
    //     view.setLayoutParams(layoutParams);
    // }
    // return result;

    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
