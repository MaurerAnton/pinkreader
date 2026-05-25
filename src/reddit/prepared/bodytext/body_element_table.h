// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementTable.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <vector>
#include <QWidget>

#include "body_element.h"

namespace PinkReader {

class BodyElementTable : public BodyElement {
public:
    explicit BodyElementTable(std::vector<BodyElement*> elements);

    QWidget* generateView(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) override;

private:
    std::vector<BodyElement*> m_elements;
};

} // namespace PinkReader
