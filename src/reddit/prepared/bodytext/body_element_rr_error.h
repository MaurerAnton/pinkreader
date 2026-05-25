// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementRRError.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <QWidget>

#include "body_element.h"

namespace PinkReader {

// Forward declaration
class RRError;

class BodyElementRRError : public BodyElement {
public:
    explicit BodyElementRRError(const RRError& error);

    QWidget* generateView(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) override;

private:
    const RRError& m_error;
};

} // namespace PinkReader
