// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementHorizontalRule.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <QWidget>

#include "body_element.h"

namespace PinkReader {

class BodyElementHorizontalRule : public BodyElement {
public:
    BodyElementHorizontalRule();

    QWidget* generateView(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) override;
};

} // namespace PinkReader
