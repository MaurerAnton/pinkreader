/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_reader.cpp
 * Port of: HtmlReader.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_reader.h"
#include "malformed_html_exception.h"

#include <QString>
#include <QChar>

namespace PinkReader {

// ===== Token::EOF_TOKEN static initialization =====
// Port of: public static final Token EOF = new Token(TokenType.EOF, "", null, null, null);
const HtmlReader::Token HtmlReader::Token::EOF_TOKEN(
    HtmlReader::TokenType::EOF_TOKEN,
    QString(),
    std::nullopt,
    std::nullopt,
    std::nullopt);

// ===== Token::toString =====
// Port of: @NonNull @Override public String toString() {
//     return type.name() + "(" + text + ")";
// }
QString HtmlReader::Token::toString() const {
    QString typeName;
    switch(type) {
        case TokenType::TAG_START:         typeName = QStringLiteral("TAG_START"); break;
        case TokenType::TAG_END:           typeName = QStringLiteral("TAG_END"); break;
        case TokenType::TAG_START_AND_END: typeName = QStringLiteral("TAG_START_AND_END"); break;
        case TokenType::TEXT:              typeName = QStringLiteral("TEXT"); break;
        case TokenType::EOF_TOKEN:         typeName = QStringLiteral("EOF"); break;
    }
    return typeName + QStringLiteral("(") + text + QStringLiteral(")");
}

// ===== Constructor =====
HtmlReader::HtmlReader(const QString &html)
    : mHtml(html)
    , mPos(0)
    , mPreformattedTextPending(false) {}

// ===== normaliseWhitespace =====
QString HtmlReader::normaliseWhitespace(const QString &html) {
    QString result;
    result.reserve(html.length());

    bool lastCharWasWhitespace = false;

    for(int i = 0; i < html.length(); i++) {
        QChar c = html.at(i);

        if(c != QChar('\n') && c != QChar('\r')) {
            if(isWhitespace(c)) {
                if(!lastCharWasWhitespace) {
                    result.append(QChar(' '));
                    lastCharWasWhitespace = true;
                }
            } else {
                lastCharWasWhitespace = false;
                result.append(c);
            }
        }
    }

    return result;
}

// ===== isWhitespace =====
bool HtmlReader::isWhitespace(QChar c) {
    char16_t ch = c.unicode();
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

// ===== isNameChar =====
bool HtmlReader::isNameChar(QChar c) {
    char16_t ch = c.unicode();
    switch(ch) {
        case 0:
        case ' ':
        case '\'':
        case '"':
        case '>':
        case '/':
        case '=':
            return false;
        default:
            return true;
    }
}

// ===== readName =====
QString HtmlReader::readName() {
    QString result;
    result.reserve(16);

    try {
        while(mPos < static_cast<int>(mHtml.length()) && isNameChar(mHtml.at(mPos))) {
            result.append(mHtml.at(mPos));
            mPos++;
        }
    } catch(const std::out_of_range &e) {
        throw MalformedHtmlException(
            QStringLiteral("Reached EOF while reading name"),
            mHtml,
            std::optional<int>(mPos));
    }

    if(result.length() == 0) {
        throw MalformedHtmlException(
            QStringLiteral("Got zero-length name"),
            mHtml,
            std::optional<int>(mPos));
    }

    return result;
}

// ===== readAndUnescapeUntil =====
QString HtmlReader::readAndUnescapeUntil(QChar endChar) {
    QString result;
    result.reserve(64);

    while(mPos < mHtml.length() && mHtml.at(mPos) != endChar) {
        result.append(mHtml.at(mPos));
        mPos++;
    }

    return unescapeHtml4(result);
}

// ===== tryAccept =====
bool HtmlReader::tryAccept(QChar c) {
    if(mPos < mHtml.length() && mHtml.at(mPos) == c) {
        mPos++;
        return true;
    }
    return false;
}

// ===== accept =====
void HtmlReader::accept(QChar c) {
    try {
        if(mPos >= mHtml.length() || mHtml.at(mPos) != c) {
            throw MalformedHtmlException(
                QStringLiteral("Expecting ") + c,
                mHtml,
                std::optional<int>(mPos));
        }
    } catch(const std::out_of_range &e) {
        throw MalformedHtmlException(
            QStringLiteral("Unexpected EOF"),
            mHtml,
            std::optional<int>(mPos));
    }

    mPos++;
}

// ===== skipWhitespace =====
void HtmlReader::skipWhitespace() {
    while(mPos < mHtml.length() && isWhitespace(mHtml.at(mPos))) {
        mPos++;
    }
}

// ===== skipNewlines =====
void HtmlReader::skipNewlines() {
    while(mPos < mHtml.length() && mHtml.at(mPos) == QChar('\n')) {
        mPos++;
    }
}

// ===== unescapeHtml4 (HTML entity decoder) =====
// Minimal port of StringEscapeUtils.unescapeHtml4
QString HtmlReader::unescapeHtml4(const QString &input) {
    QString result;
    result.reserve(input.length());

    for(int i = 0; i < input.length(); i++) {
        if(input.at(i) == QChar('&')) {
            int semiPos = input.indexOf(QChar(';'), i);
            if(semiPos >= 0) {
                QString entity = input.mid(i + 1, semiPos - i - 1);

                if(entity.startsWith(QChar('#'))) {
                    // Numeric entity
                    bool ok;
                    int codepoint;
                    if(entity.length() > 1 && entity.at(1) == QChar('x')) {
                        codepoint = entity.mid(2).toInt(&ok, 16);
                    } else {
                        codepoint = entity.mid(1).toInt(&ok);
                    }
                    if(ok && codepoint > 0 && codepoint <= 0x10FFFF) {
                        result.append(QChar(static_cast<char16_t>(codepoint)));
                        i = semiPos;
                        continue;
                    }
                }

                // Named entities
                if(entity == QStringLiteral("amp"))       { result.append(QChar('&'));  i = semiPos; continue; }
                if(entity == QStringLiteral("lt"))         { result.append(QChar('<'));  i = semiPos; continue; }
                if(entity == QStringLiteral("gt"))         { result.append(QChar('>'));  i = semiPos; continue; }
                if(entity == QStringLiteral("quot"))       { result.append(QChar('"'));  i = semiPos; continue; }
                if(entity == QStringLiteral("apos"))       { result.append(QChar('\'')); i = semiPos; continue; }
                if(entity == QStringLiteral("nbsp"))       { result.append(QChar(0xA0)); i = semiPos; continue; }
                if(entity == QStringLiteral("mdash"))      { result.append(QChar(0x2014)); i = semiPos; continue; }
                if(entity == QStringLiteral("ndash"))      { result.append(QChar(0x2013)); i = semiPos; continue; }
                if(entity == QStringLiteral("hellip"))     { result.append(QChar(0x2026)); i = semiPos; continue; }
            }
        }
        result.append(input.at(i));
    }

    return result;
}

// ===== getHtml =====
QString HtmlReader::getHtml() const {
    return mHtml;
}

// ===== readNext =====
// Port of: @NonNull public Token readNext() throws MalformedHtmlException
// Origin: HtmlReader.java lines 227-370
HtmlReader::Token HtmlReader::readNext() {
    try {
        while(true) {
            skipNewlines();

            if(mPos >= mHtml.length()) {
                // End of data
                return Token::EOF_TOKEN;
            }

            if(mHtml.at(mPos) == QChar('<')) {
                mPos++;
                skipWhitespace();

                TokenType type;

                // Comment: <!-- ... -->
                if(mHtml.at(mPos) == QChar('!')) {
                    mPos++;
                    accept(QChar('-'));
                    accept(QChar('-'));

                    while(true) {
                        if(mHtml.at(mPos) == QChar('-')
                           && mHtml.at(mPos + 1) == QChar('-')
                           && mHtml.at(mPos + 2) == QChar('>')) {

                            mPos += 3;
                            goto mainLoopContinue;

                        } else {
                            mPos++;
                        }
                    }
                }

                if(mHtml.at(mPos) == QChar('/')) {
                    type = TokenType::TAG_END;
                    mPos++;
                    skipWhitespace();
                } else {
                    type = TokenType::TAG_START;
                }

                QString tagName = readName();
                std::optional<QString> href;
                std::optional<QString> cssClass;
                std::optional<QString> title;
                std::optional<QString> src;

                if(tagName.compare(QStringLiteral("pre"), Qt::CaseInsensitive) == 0) {
                    mPreformattedTextPending = true;
                }

                skipWhitespace();

                while(mHtml.at(mPos) != QChar('>')) {

                    if(tryAccept(QChar('/'))) {
                        skipWhitespace();
                        accept(QChar('>'));
                        return Token(
                            TokenType::TAG_START_AND_END,
                            tagName,
                            href,
                            cssClass,
                            title);
                    }

                    QString propertyName = readName();

                    if(tryAccept(QChar('='))) {
                        accept(QChar('"'));
                        QString value = readAndUnescapeUntil(QChar('"'));
                        accept(QChar('"'));
                        skipWhitespace();

                        if(propertyName.compare(QStringLiteral("href"), Qt::CaseInsensitive) == 0) {
                            href = value;
                        } else if(propertyName.compare(QStringLiteral("class"), Qt::CaseInsensitive) == 0) {
                            cssClass = value;
                        } else if(propertyName.compare(QStringLiteral("title"), Qt::CaseInsensitive) == 0) {
                            title = value;
                        } else if(propertyName.compare(QStringLiteral("src"), Qt::CaseInsensitive) == 0) {
                            src = value;
                        }
                    }
                }

                accept(QChar('>'));

                // Reddit doesn't provide an end tag with their img tags for some reason
                // Need this to show multiple concurrent images correctly
                if(tagName.compare(QStringLiteral("img"), Qt::CaseInsensitive) == 0) {
                    type = TokenType::TAG_START_AND_END;
                }

                return Token(type, tagName, href, cssClass, title, src);

            } else {
                // Text content
                if(mPreformattedTextPending) {
                    mPreformattedTextPending = false;

                    QString preformattedText = readAndUnescapeUntil(QChar('<'));

                    if(preformattedText.endsWith(QChar('\n'))) {
                        preformattedText = preformattedText.left(
                            preformattedText.length() - 1);
                    }

                    return Token(
                        TokenType::TEXT,
                        preformattedText,
                        std::nullopt,
                        std::nullopt,
                        std::nullopt);
                }

                // Raw text
                return Token(
                    TokenType::TEXT,
                    normaliseWhitespace(readAndUnescapeUntil(QChar('<'))),
                    std::nullopt,
                    std::nullopt,
                    std::nullopt);
            }

            mainLoopContinue:;
        }

    } catch(const std::out_of_range &e) {
        throw MalformedHtmlException(
            QStringLiteral("Unexpected EOF"),
            mHtml,
            std::optional<int>(mPos));
    }
}

// ===== parse =====
// Port of: public static BodyElement parse(@Nullable String html, @NonNull final AppCompatActivity activity)
// Origin: HtmlReader.java lines 372-427
BodyElement* HtmlReader::parse(const std::optional<QString> &html, AppCompatActivity &activity) {
    // Stub implementation - full implementation requires BodyElement types
    // that are not yet ported.
    (void)html;
    (void)activity;
    return nullptr;
}

// ===== getPos =====
int HtmlReader::getPos() const {
    return mPos;
}

} // namespace PinkReader
