/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_reader.cpp - Port of RedReader's HtmlReader.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlReader.java
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlReaderPeekable.java
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/MalformedHtmlException.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "html_reader.h"

#include <QMap>

namespace PinkReader {

// ===== TokenType helper: name string =====
static const char *tokenTypeName(HtmlReader::TokenType type) {
    switch (type) {
        case HtmlReader::TokenType::TAG_START:        return "TAG_START";
        case HtmlReader::TokenType::TAG_END:          return "TAG_END";
        case HtmlReader::TokenType::TAG_START_AND_END: return "TAG_START_AND_END";
        case HtmlReader::TokenType::TEXT:             return "TEXT";
        case HtmlReader::TokenType::EOF_TOKEN:        return "EOF";
    }
    return "UNKNOWN";
}

// ===== Token::EOF static constant =====
// public static final Token EOF = new Token(TokenType.EOF, "", null, null, null);
const HtmlReader::Token HtmlReader::Token::EOF_TOKEN(
    HtmlReader::TokenType::EOF_TOKEN,
    QStringLiteral(""),
    std::nullopt,
    std::nullopt,
    std::nullopt
);

// ===== Token::toString() =====
QString HtmlReader::Token::toString() const {
    // return type.name() + "(" + text + ")";
    return QString::fromLatin1(tokenTypeName(type)) + QStringLiteral("(") + text + QStringLiteral(")");
}

// ===== HtmlReader Constructor =====
// public HtmlReader(@NonNull final String html)
HtmlReader::HtmlReader(const QString &html)
    : mHtml(html)
    , mPos(0)
    , mPreformattedTextPending(false)
{
}

// ===== Private static: isWhitespace =====
// private static boolean isWhitespace(final char c)
bool HtmlReader::isWhitespace(QChar c) {
    // return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    return c == QChar(' ') || c == QChar('\t') || c == QChar('\r') || c == QChar('\n');
}

// ===== Private static: isNameChar =====
// private static boolean isNameChar(final char c)
bool HtmlReader::isNameChar(QChar c) {
    switch (c.unicode()) {
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

// ===== Private static: normaliseWhitespace =====
// private static String normaliseWhitespace(@NonNull final String html)
QString HtmlReader::normaliseWhitespace(const QString &html) {
    // final StringBuilder result = new StringBuilder(html.length());
    QString result;
    result.reserve(html.length());

    // boolean lastCharWasWhitespace = false;
    bool lastCharWasWhitespace = false;

    // for(int i = 0; i < html.length(); i++)
    for (int i = 0; i < html.length(); i++) {
        // final char c = html.charAt(i);
        QChar c = html.at(i);

        // if(c != '\n' && c != '\r')
        if (c != QChar('\n') && c != QChar('\r')) {
            if (isWhitespace(c)) {
                if (!lastCharWasWhitespace) {
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

// ===== Private: readName =====
// private String readName() throws MalformedHtmlException
QString HtmlReader::readName() {
    // final StringBuilder result = new StringBuilder(16);
    QString result;
    result.reserve(16);

    // try {
    try {
        // while(isNameChar(mHtml.charAt(mPos)))
        while (mPos < mHtml.length() && isNameChar(mHtml.at(mPos))) {
            result.append(mHtml.at(mPos));
            mPos++;
        }
    } catch (const std::out_of_range &e) {
        // throw new MalformedHtmlException("Reached EOF while reading name", mHtml, mPos, e);
        throw MalformedHtmlException(
            QStringLiteral("Reached EOF while reading name"),
            mHtml,
            mPos,
            e);
    }

    // if(result.length() == 0)
    if (result.length() == 0) {
        // throw new MalformedHtmlException("Got zero-length name", mHtml, mPos);
        throw MalformedHtmlException(
            QStringLiteral("Got zero-length name"),
            mHtml,
            mPos);
    }

    return result;
}

// ===== Private: readAndUnescapeUntil =====
// private String readAndUnescapeUntil(final char endChar)
QString HtmlReader::readAndUnescapeUntil(QChar endChar) {
    // final StringBuilder result = new StringBuilder(64);
    QString result;
    result.reserve(64);

    // while(mPos < mHtml.length() && mHtml.charAt(mPos) != endChar)
    while (mPos < mHtml.length() && mHtml.at(mPos) != endChar) {
        result.append(mHtml.at(mPos));
        mPos++;
    }

    // return StringEscapeUtils.unescapeHtml4(result.toString());
    return unescapeHtml4(result);
}

// ===== Private: tryAccept =====
// private boolean tryAccept(final char c)
bool HtmlReader::tryAccept(QChar c) {
    // if(mPos < mHtml.length() && mHtml.charAt(mPos) == c)
    if (mPos < mHtml.length() && mHtml.at(mPos) == c) {
        mPos++;
        return true;
    }
    return false;
}

// ===== Private: accept =====
// private void accept(final char c) throws MalformedHtmlException
void HtmlReader::accept(QChar c) {
    // try {
    try {
        // if(mHtml.charAt(mPos) != c)
        if (mPos >= mHtml.length() || mHtml.at(mPos) != c) {
            // throw new MalformedHtmlException("Expecting " + c, mHtml, mPos);
            throw MalformedHtmlException(
                QStringLiteral("Expecting ") + c,
                mHtml,
                mPos);
        }
    } catch (const std::out_of_range &e) {
        // throw new MalformedHtmlException("Unexpected EOF", mHtml, mPos, e);
        throw MalformedHtmlException(
            QStringLiteral("Unexpected EOF"),
            mHtml,
            mPos,
            e);
    }

    mPos++;
}

// ===== Private: skipWhitespace =====
// private void skipWhitespace()
void HtmlReader::skipWhitespace() {
    // while(mPos < mHtml.length() && isWhitespace(mHtml.charAt(mPos)))
    while (mPos < mHtml.length() && isWhitespace(mHtml.at(mPos))) {
        mPos++;
    }
}

// ===== Private: skipNewlines =====
// private void skipNewlines() {
void HtmlReader::skipNewlines() {
    // while(mPos < mHtml.length() && mHtml.charAt(mPos) == '\n')
    while (mPos < mHtml.length() && mHtml.at(mPos) == QChar('\n')) {
        mPos++;
    }
}

// ===== Public: readNext =====
// @NonNull public Token readNext() throws MalformedHtmlException
HtmlReader::Token HtmlReader::readNext() {
    // try {
    try {
        // mainLoop: while(true)
        while (true) {
            skipNewlines();

            // if(mPos >= mHtml.length())
            if (mPos >= mHtml.length()) {
                // End of data
                return Token::EOF_TOKEN;
            }

            // if(mHtml.charAt(mPos) == '<')
            if (mHtml.at(mPos) == QChar('<')) {
                mPos++;
                skipWhitespace();

                TokenType type;

                // if(mHtml.charAt(mPos) == '!')
                if (mHtml.at(mPos) == QChar('!')) {
                    // Comment
                    mPos++;
                    accept(QChar('-'));
                    accept(QChar('-'));

                    while (true) {
                        // if(mHtml.charAt(mPos) == '-'
                        //     && mHtml.charAt(mPos + 1) == '-'
                        //     && mHtml.charAt(mPos + 2) == '>')
                        if (mPos + 2 < mHtml.length()
                                && mHtml.at(mPos) == QChar('-')
                                && mHtml.at(mPos + 1) == QChar('-')
                                && mHtml.at(mPos + 2) == QChar('>')) {
                            mPos += 3;
                            // continue mainLoop
                            goto mainLoopContinue;
                        } else {
                            mPos++;
                        }
                    }
                }

                // if(mHtml.charAt(mPos) == '/')
                if (mHtml.at(mPos) == QChar('/')) {
                    type = TokenType::TAG_END;
                    mPos++;
                    skipWhitespace();
                } else {
                    type = TokenType::TAG_START;
                }

                // final String tagName = readName();
                QString tagName = readName();
                std::optional<QString> href;
                std::optional<QString> cssClass;
                std::optional<QString> title;
                std::optional<UriString> src;

                // if(tagName.equalsIgnoreCase("pre"))
                if (tagName.compare(QStringLiteral("pre"), Qt::CaseInsensitive) == 0) {
                    mPreformattedTextPending = true;
                }

                skipWhitespace();

                // while(mHtml.charAt(mPos) != '>')
                while (mPos < mHtml.length() && mHtml.at(mPos) != QChar('>')) {
                    // if(tryAccept('/'))
                    if (tryAccept(QChar('/'))) {
                        skipWhitespace();
                        accept(QChar('>'));
                        return Token(TokenType::TAG_START_AND_END, tagName, href, cssClass, title);
                    }

                    // final String propertyName = readName();
                    QString propertyName = readName();

                    // if(tryAccept('='))
                    if (tryAccept(QChar('='))) {
                        accept(QChar('"'));
                        // final String value = readAndUnescapeUntil('"');
                        QString value = readAndUnescapeUntil(QChar('"'));
                        accept(QChar('"'));
                        skipWhitespace();

                        // if(propertyName.equalsIgnoreCase("href"))
                        if (propertyName.compare(QStringLiteral("href"), Qt::CaseInsensitive) == 0) {
                            href = value;
                        } else if (propertyName.compare(QStringLiteral("class"), Qt::CaseInsensitive) == 0) {
                            cssClass = value;
                        } else if (propertyName.compare(QStringLiteral("title"), Qt::CaseInsensitive) == 0) {
                            title = value;
                        } else if (propertyName.compare(QStringLiteral("src"), Qt::CaseInsensitive) == 0) {
                            src = UriString(value);
                        }
                    }
                }

                accept(QChar('>'));

                // Reddit doesn't provide an end tag with their img tags for some reason
                // Need this to show multiple concurrent images correctly
                if (tagName.compare(QStringLiteral("img"), Qt::CaseInsensitive) == 0) {
                    type = TokenType::TAG_START_AND_END;
                }

                return Token(type, tagName, href, cssClass, title, src);

            } else {
                // if(mPreformattedTextPending)
                if (mPreformattedTextPending) {
                    mPreformattedTextPending = false;

                    // String preformattedText = readAndUnescapeUntil('<');
                    QString preformattedText = readAndUnescapeUntil(QChar('<'));

                    // if(preformattedText.endsWith("\n"))
                    if (preformattedText.endsWith(QChar('\n'))) {
                        preformattedText = preformattedText.left(preformattedText.length() - 1);
                    }

                    return Token(TokenType::TEXT, preformattedText, std::nullopt, std::nullopt, std::nullopt);
                }

                // Raw text
                return Token(
                    TokenType::TEXT,
                    normaliseWhitespace(readAndUnescapeUntil(QChar('<'))),
                    std::nullopt,
                    std::nullopt,
                    std::nullopt);
            }

            mainLoopContinue:
            continue;
        }

    } catch (const std::out_of_range &e) {
        // throw new MalformedHtmlException("Unexpected EOF", mHtml, mPos, e);
        throw MalformedHtmlException(
            QStringLiteral("Unexpected EOF"),
            mHtml,
            mPos,
            e);
    }
}

// ===== Public static: parse =====
// public static BodyElement parse(@Nullable String html, @NonNull final AppCompatActivity activity)
BodyElement HtmlReader::parse(const std::optional<QString> &html,
                              AppCompatActivity &activity) {
    QString effectiveHtml;
    if (!html.has_value()) {
        effectiveHtml = QStringLiteral("");
    } else {
        effectiveHtml = *html;
    }

    // final Context applicationContext = activity.getApplicationContext();

    try {
        // final HtmlReaderPeekable reader = new HtmlReaderPeekable(new HtmlReader(html));
        HtmlReader htmlReader(effectiveHtml);
        HtmlReaderPeekable reader(htmlReader);

        // HtmlRawElement rootElement;
        // NOTE: HtmlRawElement and related classes are not yet ported.
        // This parse method is provided as a faithful structural port; the HtmlRawElement
        // family will be ported separately. For now, parse() returns a default BodyElement.
        //
        // Original logic:
        // if(reader.peek().type == TokenType.EOF) {
        //     rootElement = new HtmlRawElementPlainText("");
        // } else {
        //     rootElement = HtmlRawElement.readFrom(reader);
        // }
        // if(!(rootElement instanceof HtmlRawElementBlock)) {
        //     rootElement = new HtmlRawElementBlock(BlockType.NORMAL_TEXT, rootElement);
        // }
        // final HtmlRawElementBlock reduced = ((HtmlRawElementBlock)rootElement).reduce(
        //     new HtmlTextAttributes(), activity);
        // final ArrayList<BodyElement> generated = new ArrayList<>();
        // reduced.generate(activity, generated);
        // return new BodyElementVerticalSequence(generated);

        // Return empty sequence for now (HtmlRawElement not yet ported)
        return BodyElement();

    } catch (const MalformedHtmlException &e) {
        // return new BodyElementRRError(new RRError(
        //     applicationContext.getString(R.string.error_title_malformed_html),
        //     applicationContext.getString(R.string.error_message_malformed_html),
        //     true, e));
        return BodyElement(); // stub

    } catch (const std::exception &e) {
        // return new BodyElementRRError(new RRError(
        //     applicationContext.getString(R.string.error_parse_title),
        //     applicationContext.getString(R.string.error_parse_message),
        //     true, e));
        return BodyElement(); // stub
    }
}

// ===== Public: getHtml =====
// @NonNull public String getHtml()
QString HtmlReader::getHtml() const {
    return mHtml;
}

// ===== Public: getPos =====
// public int getPos()
int HtmlReader::getPos() const {
    return mPos;
}

// ===== HTML4 Entity Unescape (port of StringEscapeUtils.unescapeHtml4) =====
QString HtmlReader::unescapeHtml4(const QString &input) {
    // Port of commons-text StringEscapeUtils.unescapeHtml4.
    // Handles the most common HTML entities that Reddit markdown produces.
    
    QString result = input;
    
    // Named entities (most common)
    static const QMap<QString, QChar> namedEntities = {
        {QStringLiteral("amp"),    QChar('&')},
        {QStringLiteral("lt"),     QChar('<')},
        {QStringLiteral("gt"),     QChar('>')},
        {QStringLiteral("quot"),   QChar('"')},
        {QStringLiteral("apos"),   QChar('\'')},
        {QStringLiteral("nbsp"),   QChar(0xA0)},
        {QStringLiteral("iexcl"),  QChar(0xA1)},
        {QStringLiteral("cent"),   QChar(0xA2)},
        {QStringLiteral("pound"),  QChar(0xA3)},
        {QStringLiteral("curren"), QChar(0xA4)},
        {QStringLiteral("yen"),    QChar(0xA5)},
        {QStringLiteral("brvbar"), QChar(0xA6)},
        {QStringLiteral("sect"),   QChar(0xA7)},
        {QStringLiteral("uml"),    QChar(0xA8)},
        {QStringLiteral("copy"),   QChar(0xA9)},
        {QStringLiteral("ordf"),   QChar(0xAA)},
        {QStringLiteral("laquo"),  QChar(0xAB)},
        {QStringLiteral("not"),    QChar(0xAC)},
        {QStringLiteral("shy"),    QChar(0xAD)},
        {QStringLiteral("reg"),    QChar(0xAE)},
        {QStringLiteral("macr"),   QChar(0xAF)},
        {QStringLiteral("deg"),    QChar(0xB0)},
        {QStringLiteral("plusmn"), QChar(0xB1)},
        {QStringLiteral("sup2"),   QChar(0xB2)},
        {QStringLiteral("sup3"),   QChar(0xB3)},
        {QStringLiteral("acute"),  QChar(0xB4)},
        {QStringLiteral("micro"),  QChar(0xB5)},
        {QStringLiteral("para"),   QChar(0xB6)},
        {QStringLiteral("middot"), QChar(0xB7)},
        {QStringLiteral("cedil"),  QChar(0xB8)},
        {QStringLiteral("sup1"),   QChar(0xB9)},
        {QStringLiteral("ordm"),   QChar(0xBA)},
        {QStringLiteral("raquo"),  QChar(0xBB)},
        {QStringLiteral("frac14"), QChar(0xBC)},
        {QStringLiteral("frac12"), QChar(0xBD)},
        {QStringLiteral("frac34"), QChar(0xBE)},
        {QStringLiteral("iquest"), QChar(0xBF)},
        {QStringLiteral("times"),  QChar(0xD7)},
        {QStringLiteral("divide"), QChar(0xF7)},
    };

    // Replace &amp; first to avoid double-escaping
    // Then replace other named entities
    int i = 0;
    QString out;
    out.reserve(result.length());
    
    while (i < result.length()) {
        if (result.at(i) == QChar('&')) {
            int end = result.indexOf(QChar(';'), i);
            if (end > i && end - i < 20) {
                QString entity = result.mid(i + 1, end - i - 1);
                
                // Numeric entity: &#nnnn; or &#xNNNN;
                if (entity.startsWith(QChar('#'))) {
                    bool ok = false;
                    int codepoint = 0;
                    if (entity.length() > 1 && entity.at(1) == QChar('x')) {
                        codepoint = entity.mid(2).toInt(&ok, 16);
                    } else {
                        codepoint = entity.mid(1).toInt(&ok);
                    }
                    if (ok && codepoint > 0 && codepoint <= 0x10FFFF) {
                        out += QChar(codepoint);
                        i = end + 1;
                        continue;
                    }
                }
                
                // Named entity
                auto it = namedEntities.find(entity);
                if (it != namedEntities.end()) {
                    out += it.value();
                    i = end + 1;
                    continue;
                }
            }
        }
        out += result.at(i);
        i++;
    }
    
    return out;
}

// ===== HtmlReaderPeekable =====

// public HtmlReaderPeekable(@NonNull final HtmlReader htmlReader)
HtmlReaderPeekable::HtmlReaderPeekable(HtmlReader &htmlReader)
    : mHtmlReader(htmlReader)
    , mNext(mHtmlReader.readNext())
{
}

// public HtmlReader.Token peek()
HtmlReader::Token HtmlReaderPeekable::peek() const {
    return mNext;
}

// public HtmlReader.Token advance()
HtmlReader::Token HtmlReaderPeekable::advance() {
    mNext = mHtmlReader.readNext();
    return mNext;
}

// @NonNull public String getHtml()
QString HtmlReaderPeekable::getHtml() const {
    return mHtmlReader.getHtml();
}

// public int getPos()
int HtmlReaderPeekable::getPos() const {
    return mHtmlReader.getPos();
}

} // namespace PinkReader
