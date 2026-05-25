// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementNumberedListElement.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <vector>
#include <QWidget>

#include "body_element.h"

namespace PinkReader {

class BodyElementNumberedListElement : public BodyElement {
public:
    BodyElementNumberedListElement(
            int listIndex,
            std::vector<BodyElement*> elements);

    QWidget* generateView(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) override;

private:
    const int m_listIndex;
    std::vector<BodyElement*> m_elements;
};

} // namespace PinkReader
