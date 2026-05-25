// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementLinkButton.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <QWidget>

#include "body_element_base_button.h"

namespace PinkReader {

// Forward declaration
struct LinkButtonDetails;

class BodyElementLinkButton : public BodyElementBaseButton {
public:
    explicit BodyElementLinkButton(const LinkButtonDetails& details);

protected:
    QWidget::ClickListener generateOnClickListener(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) override;

    std::optional<QWidget::LongClickListener> generateOnLongClickListener(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) override;

private:
    const LinkButtonDetails& m_details;
};

} // namespace PinkReader
