// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementRRError.java
// Ported to C++ for PinkReader

#include "body_element_rr_error.h"

// Placeholder: #include "rr_error.h"
// Placeholder: #include "error_view.h"

namespace PinkReader {

BodyElementRRError::BodyElementRRError(const RRError& error)
    : BodyElement(BlockType::ERROR)
    , m_error(error)
{
}

QWidget* BodyElementRRError::generateView(
        BaseActivity& activity,
        std::optional<int> textColor,
        std::optional<float> textSize,
        bool showLinkButtons)
{
    // Placeholder: return new ErrorView(activity, m_error);
    auto* placeholder = new QWidget(&activity);
    return placeholder;
}

} // namespace PinkReader
