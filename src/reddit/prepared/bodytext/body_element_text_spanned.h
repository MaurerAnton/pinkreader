// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementTextSpanned.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <memory>
#include <QWidget>

#include "body_element.h"
#include "dynamic_spanned.h"

namespace PinkReader {

// Forward declarations
class LinkifiedTextView;
class SpannableStringBuilder;

class BodyElementTextSpanned : public BodyElement, public DynamicSpanned {
public:
    BodyElementTextSpanned(
            BlockType blockType,
            const SpannableStringBuilder& spanned);

    void addSpanDynamic(
            void* what,
            int start,
            int end,
            int flags) override;

    QWidget* generateView(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) override;

private:
    const SpannableStringBuilder& m_spanned;
    LinkifiedTextView* m_textView = nullptr;
};

} // namespace PinkReader
