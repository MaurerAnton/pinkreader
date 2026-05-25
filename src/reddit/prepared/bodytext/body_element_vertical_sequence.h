// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementVerticalSequence.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <vector>
#include <QWidget>

#include "body_element.h"

namespace PinkReader {

class BodyElementVerticalSequence : public BodyElement {
public:
    explicit BodyElementVerticalSequence(std::vector<BodyElement*> elements);

    QWidget* generateView(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) override;

private:
    std::vector<BodyElement*> m_elements;
};

} // namespace PinkReader
