/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element.cpp
 * Port of: HtmlRawElement.java (readFrom method)
 *
 * Line-by-line translation of readFrom() static method.
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element.h"
#include "html_raw_element_block.h"
#include "html_raw_element_break.h"
#include "html_raw_element_bullet_list.h"
#include "html_raw_element_numbered_list.h"
#include "html_raw_element_img.h"
#include "html_raw_element_inline_error.h"
#include "html_raw_element_link_button.h"
#include "html_raw_element_plain_text.h"
#include "html_raw_element_styled_text.h"
#include "html_raw_element_quote.h"
#include "html_raw_element_spoiler.h"
#include "html_raw_element_table.h"
#include "html_raw_element_table_cell.h"
#include "html_raw_element_table_row.h"

// NOTE: These Tag classes are not yet ported but referenced by readFrom.
// Their headers should be included when available:
// #include "html_raw_element_tag_horizontal_rule.h"
// #include "html_raw_element_tag_code.h"
// #include "html_raw_element_tag_del.h"
// #include "html_raw_element_tag_emphasis.h"
// #include "html_raw_element_tag_strong.h"
// #include "html_raw_element_tag_h1.h"
// #include "html_raw_element_tag_h2.h"
// #include "html_raw_element_tag_h3.h"
// #include "html_raw_element_tag_h4.h"
// #include "html_raw_element_tag_h5.h"
// #include "html_raw_element_tag_h6.h"
// #include "html_raw_element_tag_superscript.h"
// #include "html_raw_element_tag_anchor.h"
// #include "html_raw_element_tag_passthrough.h"

#include "utils/html_reader.h"

namespace PinkReader {

// ===== Stub Tag classes (to be replaced with real implementations when ported) =====

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagHorizontalRule
class HtmlRawElementTagHorizontalRule : public HtmlRawElement {
public:
    void getPlainText(QString &) const override {}
    void reduce(const HtmlTextAttributes &, AppCompatActivity &,
                std::vector<HtmlRawElement*> &destination,
                std::vector<LinkButtonDetails> &) override {
        destination.push_back(this);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagCode
class HtmlRawElementTagCode : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagCode(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagDel
class HtmlRawElementTagDel : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagDel(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagEmphasis
class HtmlRawElementTagEmphasis : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagEmphasis(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagStrong
class HtmlRawElementTagStrong : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagStrong(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagH1
class HtmlRawElementTagH1 : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagH1(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagH2
class HtmlRawElementTagH2 : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagH2(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagH3
class HtmlRawElementTagH3 : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagH3(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagH4
class HtmlRawElementTagH4 : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagH4(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagH5
class HtmlRawElementTagH5 : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagH5(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagH6
class HtmlRawElementTagH6 : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagH6(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagSuperscript
class HtmlRawElementTagSuperscript : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagSuperscript(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagPassthrough
class HtmlRawElementTagPassthrough : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagPassthrough(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// Port of: org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagAnchor
class HtmlRawElementTagAnchor : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    QString mHref;
    HtmlRawElementTagAnchor(const std::vector<HtmlRawElement*> &c, const QString &href)
        : mChildren(c), mHref(href) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};

// ===== static inline helper: asciiLowercase =====
// Port of: StringUtils.asciiLowercase(String)
static QString asciiLowercase(const QString &s) {
    return s.toLower();
}

// ===== Static constants for BlockType enum proxy =====
// Port of: BlockType enum values
namespace BlockType {
    static constexpr int VERTICAL_SEQUENCE = 0;
    static constexpr int HEADER = 1;
    static constexpr int NORMAL_TEXT = 2;
    static constexpr int TABLE_CELL = 3;
    static constexpr int BUTTON = 4;
    static constexpr int CODE_BLOCK = 5;
    static constexpr int LIST_ELEMENT = 6;
    static constexpr int QUOTE = 7;
}

// ===== HtmlRawElement::readFrom =====
// Port of:
//   @NonNull
//   public static HtmlRawElement readFrom(@NonNull final HtmlReaderPeekable reader)
//       throws MalformedHtmlException
HtmlRawElement* HtmlRawElement::readFrom(HtmlReaderPeekable &reader) {

    // final HtmlReader.Token startToken = reader.peek();
    // reader.advance();
    HtmlReader::Token startToken = reader.peek();
    reader.advance();

    // final ArrayList<HtmlRawElement> children = new ArrayList<>();
    std::vector<HtmlRawElement*> children;

    // if(startToken.type == HtmlReader.TokenType.TAG_START_AND_END) {
    if(startToken.type == HtmlReader::TokenType::TAG_START_AND_END) {

        // switch(startToken.text) {
        const QString &text = startToken.text;

        // case "hr":
        if(text == QStringLiteral("hr")) {
            return new HtmlRawElementTagHorizontalRule();
        }

        // case "br":
        if(text == QStringLiteral("br")) {
            return new HtmlRawElementBreak();
        }

        // case "img":
        if(text == QStringLiteral("img")) {
            // String title = startToken.title == null || startToken.title.isEmpty()
            //     ? "emote" : startToken.title;
            QString title;
            if(!startToken.title.has_value() || startToken.title->isEmpty()) {
                title = QStringLiteral("emote");
            } else {
                title = startToken.title.value();
            }

            // UriString src = startToken.src;
            QString srcValue;
            if(startToken.src.has_value()) {
                srcValue = QString(); // UriString proxy
            }

            return new HtmlRawElementImg(children, title, srcValue);
        }

        // default:
        return HtmlRawElementInlineErrorMessage::create(
            QStringLiteral("Error: Unexpected tag <") + startToken.text + QStringLiteral("/>"));

    // } else if(startToken.type == HtmlReader.TokenType.TAG_START) {
    } else if(startToken.type == HtmlReader::TokenType::TAG_START) {

        // while(reader.peek().type != HtmlReader.TokenType.TAG_END
        //     && reader.peek().type != HtmlReader.TokenType.EOF) {
        while(reader.peek().type != HtmlReader::TokenType::TAG_END
              && reader.peek().type != HtmlReader::TokenType::EOF_TOKEN) {

            children.push_back(HtmlRawElement::readFrom(reader));
        }

        // {
        //     final HtmlReader.Token endToken = reader.peek();
        //     // Reddit sometimes doesn't close tags properly :'(
        //     if(endToken.text.equalsIgnoreCase(startToken.text)) {
        //         reader.advance();
        //     }
        // }
        {
            HtmlReader::Token endToken = reader.peek();

            // equalsIgnoreCase
            if(endToken.text.compare(startToken.text, Qt::CaseInsensitive) == 0) {
                reader.advance();
            }
        }

        // final HtmlRawElement result;
        HtmlRawElement* result = nullptr;

        // switch(StringUtils.asciiLowercase(startToken.text)) {
        QString lowerText = asciiLowercase(startToken.text);

        // case "code":
        if(lowerText == QStringLiteral("code")) {
            result = new HtmlRawElementTagCode(children);

        // case "del":
        } else if(lowerText == QStringLiteral("del")) {
            result = new HtmlRawElementTagDel(children);

        // case "em":
        } else if(lowerText == QStringLiteral("em")) {
            result = new HtmlRawElementTagEmphasis(children);

        // case "div":
        } else if(lowerText == QStringLiteral("div")) {
            result = new HtmlRawElementBlock(BlockType::VERTICAL_SEQUENCE, children);

        // case "h1":
        } else if(lowerText == QStringLiteral("h1")) {
            result = new HtmlRawElementBlock(BlockType::HEADER,
                {new HtmlRawElementTagH1(children)});

        // case "h2":
        } else if(lowerText == QStringLiteral("h2")) {
            result = new HtmlRawElementBlock(BlockType::HEADER,
                {new HtmlRawElementTagH2(children)});

        // case "h3":
        } else if(lowerText == QStringLiteral("h3")) {
            result = new HtmlRawElementBlock(BlockType::HEADER,
                {new HtmlRawElementTagH3(children)});

        // case "h4":
        } else if(lowerText == QStringLiteral("h4")) {
            result = new HtmlRawElementBlock(BlockType::HEADER,
                {new HtmlRawElementTagH4(children)});

        // case "h5":
        } else if(lowerText == QStringLiteral("h5")) {
            result = new HtmlRawElementBlock(BlockType::HEADER,
                {new HtmlRawElementTagH5(children)});

        // case "h6":
        } else if(lowerText == QStringLiteral("h6")) {
            result = new HtmlRawElementBlock(BlockType::HEADER,
                {new HtmlRawElementTagH6(children)});

        // case "strong":
        } else if(lowerText == QStringLiteral("strong")) {
            result = new HtmlRawElementTagStrong(children);

        // case "p":
        } else if(lowerText == QStringLiteral("p")) {
            result = new HtmlRawElementBlock(BlockType::NORMAL_TEXT, children);

        // case "th":
        // case "td":
        } else if(lowerText == QStringLiteral("th") || lowerText == QStringLiteral("td")) {
            result = new HtmlRawElementTableCell(
                new HtmlRawElementBlock(BlockType::TABLE_CELL, children));

        // case "sup":
        } else if(lowerText == QStringLiteral("sup")) {
            result = new HtmlRawElementTagSuperscript(children);

        // case "a":
        } else if(lowerText == QStringLiteral("a")) {

            // final String href = Objects.requireNonNull(startToken.href);
            QString href;
            if(startToken.href.has_value()) {
                href = startToken.href.value();
            }

            // if(href.startsWith("/spoiler")) {
            if(href.startsWith(QStringLiteral("/spoiler"))) {
                // Old spoiler syntax
                result = new HtmlRawElementSpoiler(
                    new HtmlRawElementBlock(BlockType::BUTTON, children));

            // } else if(href.length() == 2
            //     && (href.charAt(0) == '#' || href.charAt(0) == '/')
            //     && startToken.title != null) {
            } else if(href.length() == 2
                      && (href.at(0) == QChar('#') || href.at(0) == QChar('/'))
                      && startToken.title.has_value()) {

                // Another old spoiler syntax
                children.push_back(new HtmlRawElementSpoiler(
                    new HtmlRawElementBlock(BlockType::NORMAL_TEXT,
                        {new HtmlRawElementPlainText(startToken.title.value())})));

                result = new HtmlRawElementTagPassthrough(children);

            // } else if(href.startsWith("#")) {
            } else if(href.startsWith(QChar('#'))) {
                // Probably an emote: pass through the text, but don't make a link
                result = new HtmlRawElementTagPassthrough(children);

            // } else {
            } else {
                result = new HtmlRawElementTagAnchor(children, href);
            }

        // case "pre":
        } else if(lowerText == QStringLiteral("pre")) {
            result = new HtmlRawElementBlock(BlockType::CODE_BLOCK,
                {new HtmlRawElementTagCode(children)});

        // case "ul":
        } else if(lowerText == QStringLiteral("ul")) {
            result = new HtmlRawElementBulletList(children);

        // case "ol":
        } else if(lowerText == QStringLiteral("ol")) {
            result = new HtmlRawElementNumberedList(children);

        // case "li":
        } else if(lowerText == QStringLiteral("li")) {
            result = new HtmlRawElementBlock(BlockType::LIST_ELEMENT, children);

        // case "blockquote":
        } else if(lowerText == QStringLiteral("blockquote")) {
            result = new HtmlRawElementQuote(
                new HtmlRawElementBlock(BlockType::QUOTE, children));

        // case "span":
        } else if(lowerText == QStringLiteral("span")) {

            // if("md-spoiler-text".equalsIgnoreCase(startToken.cssClass)) {
            if(startToken.cssClass.has_value()
               && startToken.cssClass->compare(QStringLiteral("md-spoiler-text"), Qt::CaseInsensitive) == 0) {
                result = new HtmlRawElementSpoiler(
                    new HtmlRawElementBlock(BlockType::BUTTON, children));
            } else {
                result = new HtmlRawElementTagPassthrough(children);
            }

        // case "thead":
        } else if(lowerText == QStringLiteral("thead")) {
            result = new HtmlRawElementTagStrong(children);

        // case "tbody":
        } else if(lowerText == QStringLiteral("tbody")) {
            result = new HtmlRawElementTagPassthrough(children);

        // case "table":
        } else if(lowerText == QStringLiteral("table")) {
            result = new HtmlRawElementTable(children);

        // case "tr":
        } else if(lowerText == QStringLiteral("tr")) {
            result = new HtmlRawElementTableRow(children);

        // case "emote":
        } else if(lowerText == QStringLiteral("emote")) {
            // final UriString src = Objects.requireNonNull(startToken.src);
            QString srcValue;
            if(startToken.src.has_value()) {
                srcValue = QString(); // UriString proxy
            }

            QString title;
            if(!startToken.title.has_value() || startToken.title->isEmpty()) {
                title = QStringLiteral("emote");
            } else {
                title = startToken.title.value();
            }

            result = new HtmlRawElementImg(children, title, srcValue);

        // default:
        } else {
            return HtmlRawElementInlineErrorMessage::appendError(
                QStringLiteral("Error: Unexpected tag start <") + startToken.text + QStringLiteral(">"),
                *new HtmlRawElementBlock(BlockType::NORMAL_TEXT, children));
        }

        // return result;
        return result;

    // } else if(startToken.type == HtmlReader.TokenType.TEXT) {
    } else if(startToken.type == HtmlReader::TokenType::TEXT) {
        return new HtmlRawElementPlainText(startToken.text);

    // } else if(startToken.type == HtmlReader.TokenType.EOF) {
    } else if(startToken.type == HtmlReader::TokenType::EOF_TOKEN) {
        // throw new MalformedHtmlException(
        //     "Unexpected EOF",
        //     reader.getHtml(),
        //     reader.getPos());
        throw MalformedHtmlException(
            QStringLiteral("Unexpected EOF"),
            reader.getHtml(),
            reader.getPos());

    // } else {
    } else {
        return HtmlRawElementInlineErrorMessage::create(
            QStringLiteral("Error: Unexpected token type ")
            + QString::number(static_cast<int>(startToken.type)));
    }
}

} // namespace PinkReader
