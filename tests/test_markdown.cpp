#include <QtTest>
#include "../src/core/markdown_parser.hpp"

class TestMarkdownParser : public QObject {
    Q_OBJECT

private slots:
    void testEmpty() {
        PinkReader::MarkdownParser parser;
        QCOMPARE(parser.toHtml(""), QString{});
    }

    void testBold() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("hello **world** test");
        QVERIFY(result.contains("<b>world</b>"));
    }

    void testItalic() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("hello *world* test");
        QVERIFY(result.contains("<i>world</i>"));
    }

    void testStrikethrough() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("hello ~~world~~ test");
        QVERIFY(result.contains("<s>world</s>"));
    }

    void testCode() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("use `code` here");
        QVERIFY(result.contains("<code"));
        QVERIFY(result.contains("code</code>"));
    }

    void testLink() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("[click](https://example.com)");
        QVERIFY(result.contains("<a href='https://example.com'>click</a>"));
    }

    void testSpoiler() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("secret >!spoiler!< text");
        QVERIFY(result.contains("spoiler"));
    }

    void testHeading() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("# Title");
        QVERIFY(result.contains("<h2>Title</h2>"));
    }

    void testUnorderedList() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("* item1\n* item2");
        QVERIFY(result.contains("<ul>"));
        QVERIFY(result.contains("<li>item1</li>"));
        QVERIFY(result.contains("<li>item2</li>"));
        QVERIFY(result.contains("</ul>"));
    }

    void testOrderedList() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("1. first\n2. second");
        QVERIFY(result.contains("<ol>"));
        QVERIFY(result.contains("<li>first</li>"));
        QVERIFY(result.contains("<li>second</li>"));
        QVERIFY(result.contains("</ol>"));
    }

    void testQuote() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("> quoted text");
        QVERIFY(result.contains("<blockquote>"));
        QVERIFY(result.contains("quoted text"));
        QVERIFY(result.contains("</blockquote>"));
    }

    void testHorizontalRule() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("---");
        QVERIFY(result.contains("<hr"));
    }

    void testImage() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("![alt](https://img.com/pic.jpg)");
        QVERIFY(result.contains("<img"));
    }

    void testAutoLink() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("visit https://example.com/page now");
        QVERIFY(result.contains("href='https://example.com/page'"));
    }

    void testSubredditLink() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("see /r/programming for more");
        QVERIFY(result.contains("href='https://reddit.com/r/programming'"));
    }

    void testUserLink() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("ask /u/spez about it");
        QVERIFY(result.contains("href='https://reddit.com/u/spez'"));
    }

    void testCodeBlock() {
        PinkReader::MarkdownParser parser;
        QString result = parser.toHtml("```\nint x = 1;\n```");
        QVERIFY(result.contains("<pre><code>"));
        QVERIFY(result.contains("int x = 1;"));
    }
};

QTEST_MAIN(TestMarkdownParser)
#include "test_markdown.moc"
