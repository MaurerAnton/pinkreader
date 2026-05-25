// Origin: RedReader - org/quantumbadger/redreader/reddit/prepared/bodytext/BodyElementBaseButton.java
// Ported to C++ for PinkReader

#pragma once

#include <optional>
#include <QString>
#include <QWidget>

#include "body_element.h"

namespace PinkReader {

// Forward declarations
class BaseActivity;
class LinkDetailsView;

class BodyElementBaseButton : public BodyElement {
public:
    BodyElementBaseButton(
            const QString& text,
            std::optional<QString> subtitle,
            bool isLinkButton);

    QWidget* generateView(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) final;

protected:
    virtual QWidget::ClickListener generateOnClickListener(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) = 0;

    virtual std::optional<QWidget::LongClickListener> generateOnLongClickListener(
            BaseActivity& activity,
            std::optional<int> textColor,
            std::optional<float> textSize,
            bool showLinkButtons) = 0;

    const QString& text() const { return m_text; }
    const std::optional<QString>& subtitle() const { return m_subtitle; }
    bool isLinkButton() const { return m_isLinkButton; }

private:
    const QString m_text;
    const std::optional<QString> m_subtitle;
    const bool m_isLinkButton;
};

} // namespace PinkReader
