/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element.h
 * Port of: HtmlRawElement.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElement.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include <QString>
#include <QStringBuilder>
#include <memory>
#include <optional>
#include <vector>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class UriString;
class BodyElement;
class BlockType;
class HtmlTextAttributes;
class HtmlReaderPeekable;
class MalformedHtmlException;
class HtmlRawElementPlainText;
class HtmlRawElementStyledText;
class HtmlRawElementBlock;
class HtmlRawElementImg;
class HtmlRawElementSpoiler;
class HtmlRawElementTagPassthrough;
class HtmlRawElementTagAnchor;
class HtmlRawElementTagCode;
class HtmlRawElementTagDel;
class HtmlRawElementTagEmphasis;
class HtmlRawElementTagStrong;
class HtmlRawElementTagH1;
class HtmlRawElementTagH2;
class HtmlRawElementTagH3;
class HtmlRawElementTagH4;
class HtmlRawElementTagH5;
class HtmlRawElementTagH6;
class HtmlRawElementTagSuperscript;
class HtmlRawElementTagHorizontalRule;
class HtmlRawElementBreak;
class HtmlRawElementBulletList;
class HtmlRawElementNumberedList;
class HtmlRawElementQuote;
class HtmlRawElementTable;
class HtmlRawElementTableCell;
class HtmlRawElementTableRow;
class HtmlRawElementLinkButton;
class HtmlRawElementInlineErrorMessage;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElement
 *
 * Abstract base class for all HTML raw elements.
 * Every field, method, inner class, and constant ported exactly.
 */
class HtmlRawElement {
public:
    // ===== Inner class: LinkButtonDetails =====
    // Port of: public static class LinkButtonDetails
    class LinkButtonDetails {
    public:
        // Port of: @Nullable public final String name;
        std::optional<QString> name;

        // Port of: @NonNull public final UriString url;
        // NOTE: UriString is forward-declared; stored as pointer or value
        // depending on UriString definition. Using optional<UriString> for now.
        std::optional<QString> urlValue;  // UriString stored as QString proxy

        // Port of: constructor LinkButtonDetails(@Nullable String name, @NonNull UriString url)
        LinkButtonDetails(const std::optional<QString> &_name,
                          const QString &_urlValue)
            : name(_name)
            , urlValue(_urlValue) {}

        // Port of: @NonNull public final String getButtonTitle()
        QString getButtonTitle() const {
            // if(name == null || name.isEmpty())
            if(!name.has_value() || name->isEmpty()) {
                return urlValue.value_or(QString());
            } else {
                return name.value();
            }
        }

        // Port of: @Nullable public final String getButtonSubtitle()
        std::optional<QString> getButtonSubtitle() const {
            // if(name == null || name.isEmpty())
            if(!name.has_value() || name->isEmpty()) {
                return std::nullopt;
            } else {
                return urlValue;
            }
        }
    };

    virtual ~HtmlRawElement() = default;

    // ===== Public methods =====

    // Port of: @NonNull public final String getPlainText()
    QString getPlainText() const {
        // final StringBuilder sb = new StringBuilder();
        // getPlainText(sb);
        // return sb.toString();
        QString sb;
        getPlainText(sb);
        return sb;
    }

    // Port of: public abstract void getPlainText(@NonNull final StringBuilder stringBuilder)
    virtual void getPlainText(QString &stringBuilder) const = 0;

    // Port of: public abstract void reduce(
    //     @NonNull HtmlTextAttributes activeAttributes,
    //     @NonNull AppCompatActivity activity,
    //     @NonNull ArrayList<HtmlRawElement> destination,
    //     @NonNull ArrayList<LinkButtonDetails> linkButtons);
    virtual void reduce(
            const HtmlTextAttributes &activeAttributes,
            AppCompatActivity &activity,
            std::vector<HtmlRawElement*> &destination,
            std::vector<LinkButtonDetails> &linkButtons) = 0;

    // Port of: public abstract void generate(
    //     @NonNull final AppCompatActivity activity,
    //     @NonNull ArrayList<BodyElement> destination);
    virtual void generate(
            AppCompatActivity &activity,
            std::vector<BodyElement*> &destination) = 0;

    // ===== Static methods =====

    // Port of: @NonNull public static HtmlRawElement readFrom(
    //     @NonNull final HtmlReaderPeekable reader) throws MalformedHtmlException
    static HtmlRawElement* readFrom(HtmlReaderPeekable &reader);
};

} // namespace PinkReader
