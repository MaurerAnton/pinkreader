/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_reader.h
 * Port of: HtmlReader.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlReader.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include <QString>
#include <QChar>
#include <optional>
#include <string>
#include <vector>

namespace PinkReader {

// Forward declarations
class UriString;
class BodyElement;
class AppCompatActivity;  // stub for Android Activity

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlReader
 *
 * HTML tokenizer and parser for Reddit comment bodies.
 * Every field, method, inner class, and constant ported exactly.
 *
 * Origin: HtmlReader.java lines 37-437
 */
class HtmlReader {
public:
    // ===== Inner enum: TokenType =====
    // Port of: public enum TokenType (lines 39-45)
    // Origin: HtmlReader.java lines 39-45
    enum class TokenType {
        TAG_START,          // Origin: HtmlReader.java line 40
        TAG_END,            // Origin: HtmlReader.java line 41
        TAG_START_AND_END,  // Origin: HtmlReader.java line 42
        TEXT,               // Origin: HtmlReader.java line 43
        EOF_TOKEN           // Origin: HtmlReader.java line 44
    };

    // ===== Inner class: Token =====
    // Port of: public static class Token (lines 47-92)
    // Origin: HtmlReader.java lines 47-92
    class Token {
    public:
        // Port of: public static final Token EOF = new Token(TokenType.EOF, "", null, null, null);
        // Origin: HtmlReader.java line 49
        static const Token EOF_TOKEN;

        // Port of: @NonNull public final TokenType type; (line 51)
        TokenType type;

        // Port of: @NonNull public final String text; (line 52)
        QString text;

        // Port of: @Nullable public final String href; (line 53)
        std::optional<QString> href;

        // Port of: @Nullable public final String cssClass; (line 54)
        std::optional<QString> cssClass;

        // Port of: @Nullable public final String title; (line 55)
        std::optional<QString> title;

        // Port of: @Nullable public final UriString src; (line 56)
        std::optional<QString> src;  // UriString stored as QString proxy

        // Port of: constructor (lines 58-70) - 5-arg
        Token(TokenType _type,
              const QString &_text,
              const std::optional<QString> &_href,
              const std::optional<QString> &_cssClass,
              const std::optional<QString> &_title)
            : type(_type)
            , text(_text)
            , href(_href)
            , cssClass(_cssClass)
            , title(_title)
            , src(std::nullopt) {}

        // Port of: constructor (lines 72-85) - 6-arg with src
        Token(TokenType _type,
              const QString &_text,
              const std::optional<QString> &_href,
              const std::optional<QString> &_cssClass,
              const std::optional<QString> &_title,
              const std::optional<QString> &_src)
            : type(_type)
            , text(_text)
            , href(_href)
            , cssClass(_cssClass)
            , title(_title)
            , src(_src) {}

        // Default constructor for EOF_TOKEN
        Token()
            : type(TokenType::EOF_TOKEN)
            , text()
            , href(std::nullopt)
            , cssClass(std::nullopt)
            , title(std::nullopt)
            , src(std::nullopt) {}

        // Port of: @NonNull @Override public String toString() (lines 88-91)
        // returns type.name() + "(" + text + ")"
        QString toString() const;
    };

    // Port of: public HtmlReader(@NonNull final String html) (lines 99-101)
    // Origin: HtmlReader.java lines 99-101
    explicit HtmlReader(const QString &html);

    // Port of: @NonNull public Token readNext() throws MalformedHtmlException (lines 227-370)
    // Origin: HtmlReader.java lines 227-370
    Token readNext();

    // Port of: public static BodyElement parse(...) (lines 372-427)
    // Origin: HtmlReader.java lines 372-427
    static BodyElement* parse(const std::optional<QString> &html,
                              AppCompatActivity &activity);

    // Port of: @NonNull public String getHtml() (lines 430-432)
    // Origin: HtmlReader.java lines 430-432
    QString getHtml() const;

    // Port of: public int getPos() (lines 434-436)
    // Origin: HtmlReader.java lines 434-436
    int getPos() const;

private:
    // ===== Private fields =====
    // Port of: @NonNull private final String mHtml; (line 94)
    // Origin: HtmlReader.java line 94
    QString mHtml;

    // Port of: private int mPos = 0; (line 95)
    // Origin: HtmlReader.java line 95
    int mPos = 0;

    // Port of: private boolean mPreformattedTextPending = false; (line 97)
    // Origin: HtmlReader.java line 97
    bool mPreformattedTextPending = false;

    // ===== Private static methods =====
    // Port of: private static String normaliseWhitespace(@NonNull final String html) (lines 103-128)
    // Origin: HtmlReader.java lines 103-128
    static QString normaliseWhitespace(const QString &html);

    // Port of: private static boolean isWhitespace(final char c) (lines 130-132)
    // Origin: HtmlReader.java lines 130-132
    static bool isWhitespace(QChar c);

    // Port of: private static boolean isNameChar(final char c) (lines 134-149)
    // Origin: HtmlReader.java lines 134-149
    static bool isNameChar(QChar c);

    // ===== Private instance methods =====
    // Port of: private String readName() throws MalformedHtmlException (lines 151-174)
    // Origin: HtmlReader.java lines 151-174
    QString readName();

    // Port of: private String readAndUnescapeUntil(final char endChar) (lines 176-186)
    // Origin: HtmlReader.java lines 176-186
    QString readAndUnescapeUntil(QChar endChar);

    // Port of: private boolean tryAccept(final char c) (lines 188-196)
    // Origin: HtmlReader.java lines 188-196
    bool tryAccept(QChar c);

    // Port of: private void accept(final char c) throws MalformedHtmlException (lines 198-210)
    // Origin: HtmlReader.java lines 198-210
    void accept(QChar c);

    // Port of: private void skipWhitespace() (lines 212-217)
    // Origin: HtmlReader.java lines 212-217
    void skipWhitespace();

    // Port of: private void skipNewlines() (lines 219-224)
    // Origin: HtmlReader.java lines 219-224
    void skipNewlines();

    // HTML4 entity unescape helper (port of StringEscapeUtils.unescapeHtml4)
    static QString unescapeHtml4(const QString &input);
};

} // namespace PinkReader
