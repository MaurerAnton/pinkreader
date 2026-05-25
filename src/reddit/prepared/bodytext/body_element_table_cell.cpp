// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementTableCell.java
// Ported to C++ for PinkReader

#include "body_element_table_cell.h"
#include "body_element_vertical_sequence.h"

// Placeholder: #include "general.h"

namespace PinkReader {

BodyElementTableCell::BodyElementTableCell(std::vector<BodyElement*> elements)
    : BodyElement(BlockType::TABLE_CELL)
    , m_elements(std::move(elements))
{
}

QWidget* BodyElementTableCell::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Don't show link buttons inside tables
    // Placeholder: FrameLayout-based padding
    // final View inner = BodyElementVerticalSequence(m_elements)
    //     .generateView(activity, textColor, textSize, false);
    // final FrameLayout padding = new FrameLayout(activity);
    // padding.addView(inner);
    // final int verticalPaddingPx = General::dpToPixels(activity, 2);
    // final int horizontalPaddingPx = General::dpToPixels(activity, 5);
    // padding.setPadding(horizontalPaddingPx, verticalPaddingPx,
    //     horizontalPaddingPx, verticalPaddingPx);
    // return padding;

    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
