// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementTable.java
// Ported to C++ for PinkReader

#include "body_element_table.h"

namespace PinkReader {

BodyElementTable::BodyElementTable(std::vector<BodyElement*> elements)
    : BodyElement(BlockType::TABLE)
    , m_elements(std::move(elements))
{
}

QWidget* BodyElementTable::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: Android TableLayout + HorizontalScrollView equivalent
    // final TableLayout table = new TableLayout(activity);
    // for (final BodyElement element : m_elements) {
    //     final View view = element->generateView(activity, textColor, textSize, showLinkButtons);
    //     table.addView(view);
    // }
    // table.setShowDividers(LinearLayout.SHOW_DIVIDER_MIDDLE);
    // table.setDividerDrawable(new ColorDrawable(Color.GRAY));
    // table.setLayoutParams(new ViewGroup.LayoutParams(WRAP_CONTENT, WRAP_CONTENT));
    // final HorizontalScrollView scrollView = new HorizontalScrollView(activity);
    // scrollView.addView(table);
    // return scrollView;

    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
