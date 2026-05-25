/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_reader.h - Port of RedReader's HtmlReader.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlReader.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QString>
#include <QChar>
#include <QStringBuilder>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace PinkReader {

// Forward declarations
class BodyElement;
class AppCompatActivity;  // stub for Android class
class UriString;

/**
 * @brief Port of MalformedHtmlException (inner exception used by HtmlReader)
 * Original: MalformedHtmlException.java
 */
class MalformedHtmlException : public std::exception {
public:
    QString html;
    std::optional<int> charPosition;

    MalformedHtmlException(const QString &message,
                           const QString &html,
                           const std::optional<int> &charPosition)
        : std::exception()
        , m_message(message.toStdString())
        , html(html)
        , charPosition(charPosition) {}

    MalformedHtmlException(const QString &message,
                           const QString &html,
                           const std::optional<int> &charPosition,
                           const std::exception &cause)
        : std::exception()
        , m_message(message.toStdString())
        , html(html)
        , charPosition(charPosition)
        , m_causeMessage(cause.what()) {}

    const char *what() const noexcept override {
        return m_message.c_str();
    }

    const std::string &causeMessage() const { return m_causeMessage; }

private:
    std::string m_message;
    std::string m_causeMessage;
};

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlReader
 *
 * HTML tokenizer and parser for Reddit comment bodies.
 * Every field, method, inner class, and constant ported exactly.
 */
class HtmlReader {
public:
    // ===== Inner enum: TokenType (port of HtmlReader.TokenType) =====
    enum class TokenType {
        TAG_START,
        TAG_END,
        TAG_START_AND_END,
        TEXT,
        EOF_TOKEN
    };

    // ===== Inner class: Token (port of HtmlReader.Token) =====
    class Token {
    public:
        // public static final Token EOF = new Token(TokenType.EOF, "", null, null, null);
        static const Token EOF_TOKEN;

        TokenType type;
        QString text;
        std::optional<QString> href;
        std::optional<QString> cssClass;
        std::optional<QString> title;
        std::optional<UriString> src;

        // Token(TokenType, String text, String href, String cssClass, String title)
        Token(TokenType type,
              const QString &text,
              const std::optional<QString> &href,
              const std::optional<QString> &cssClass,
              const std::optional<QString> &title)
            : type(type)
            , text(text)
            , href(href)
            , cssClass(cssClass)
            , title(title)
            , src(std::nullopt) {}

        // Token(TokenType, String text, String href, String cssClass, String title, UriString src)
        Token(TokenType type,
              const QString &text,
              const std::optional<QString> &href,
              const std::optional<QString> &cssClass,
              const std::optional<QString> &title,
              const std::optional<UriString> &src)
            : type(type)
            , text(text)
            , href(href)
            , cssClass(cssClass)
            , title(title)
            , src(src) {}

        // toString() -> type.name() + "(" + text + ")"
        QString toString() const;
    };

    // ===== Constructor =====
    // public HtmlReader(@NonNull final String html)
    explicit HtmlReader(const QString &html);

    // ===== Public methods =====
    // @NonNull public Token readNext() throws MalformedHtmlException
    Token readNext();

    // public static BodyElement parse(@Nullable String html, @NonNull final AppCompatActivity activity)
    static BodyElement parse(const std::optional<QString> &html,
                             AppCompatActivity &activity);

    // @NonNull public String getHtml()
    QString getHtml() const;

    // public int getPos()
    int getPos() const;

private:
    // ===== Private fields =====
    // @NonNull private final String mHtml;
    QString mHtml;

    // private int mPos = 0;
    int mPos = 0;

    // private boolean mPreformattedTextPending = false;
    bool mPreformattedTextPending = false;

    // ===== Private static methods (port of static methods) =====
    // private static String normaliseWhitespace(@NonNull final String html)
    static QString normaliseWhitespace(const QString &html);

    // private static boolean isWhitespace(final char c)
    static bool isWhitespace(QChar c);

    // private static boolean isNameChar(final char c)
    static bool isNameChar(QChar c);

    // ===== Private instance methods =====
    // private String readName() throws MalformedHtmlException
    QString readName();

    // private String readAndUnescapeUntil(final char endChar)
    QString readAndUnescapeUntil(QChar endChar);

    // private boolean tryAccept(final char c)
    bool tryAccept(QChar c);

    // private void accept(final char c) throws MalformedHtmlException
    void accept(QChar c);

    // private void skipWhitespace()
    void skipWhitespace();

    // private void skipNewlines()
    void skipNewlines();

    // Helper: HTML4 entity unescape (port of StringEscapeUtils.unescapeHtml4)
    static QString unescapeHtml4(const QString &input);
};

/**
 * @brief Port of HtmlReaderPeekable.java
 * Wraps HtmlReader with peek/advance semantics.
 */
class HtmlReaderPeekable {
public:
    // HtmlReaderPeekable(@NonNull final HtmlReader htmlReader) throws MalformedHtmlException
    explicit HtmlReaderPeekable(HtmlReader &htmlReader);

    // public HtmlReader.Token peek()
    HtmlReader::Token peek() const;

    // public HtmlReader.Token advance() throws MalformedHtmlException
    HtmlReader::Token advance();

    // @NonNull public String getHtml()
    QString getHtml() const;

    // public int getPos()
    int getPos() const;

private:
    // @NonNull private final HtmlReader mHtmlReader;
    HtmlReader &mHtmlReader;

    // @NonNull private HtmlReader.Token mNext;
    HtmlReader::Token mNext;
};

} // namespace PinkReader
