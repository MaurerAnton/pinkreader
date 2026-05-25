// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementSpoilerButton.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <QWidget>

#include "body_element_base_button.h"

namespace PinkReader {

// Forward declaration
class AppCompatActivity;

class BodyElementSpoilerButton : public BodyElementBaseButton {
public:
    BodyElementSpoilerButton(
            AppCompatActivity& activity,
            const BodyElement& spoilerText);

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
    const BodyElement* m_spoilerText;
};

} // namespace PinkReader
