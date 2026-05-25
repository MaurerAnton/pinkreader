// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElement.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <QWidget>

#include "block_type.h"

namespace PinkReader {

// Forward declaration
class BaseActivity;

class BodyElement {
public:
    explicit BodyElement(BlockType type);
    virtual ~BodyElement() = default;

    BlockType getType() const;

    virtual QWidget* generateView(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) = 0;

private:
    const BlockType m_type;
};

} // namespace PinkReader
