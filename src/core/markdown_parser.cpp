#include "markdown_parser.hpp"

#include <QRegularExpression>

namespace PinkReader {

namespace {

void closeInlines(QString& result, bool& inQuote, bool& inList, bool& inOrderedList) {
    if (inOrderedList) { result += "</ol>\n"; inOrderedList = false; }
    if (inList) { result += "</ul>\n"; inList = false; }
}

} // anonymous namespace

MarkdownParser::MarkdownParser(QObject* parent) : QObject(parent) {}

QString MarkdownParser::escapeHtml(const QString& text) const {
    QString result = text;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    return result;
}

QString MarkdownParser::toHtml(const QString& markdown) const {
    if (markdown.isEmpty()) return {};

    QString text = markdown;
    // Normalize line endings
    text.replace("\r\n", "\n");
    text.replace('\r', '\n');

    QString html = parseBlocks(text);
    return html.trimmed();
}

QString MarkdownParser::parseBlocks(const QString& text) const {
    QString result;
    QStringList lines = text.split('\n');
    bool inCodeBlock = false;
    bool inList = false;
    bool inOrderedList = false;
    bool inQuote = false;

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i];

        // Code block
        if (line.startsWith("```")) {
            if (inCodeBlock) {
                result += "</pre>\n";
                inCodeBlock = false;
            } else {
                result += "<pre><code>";
                inCodeBlock = true;
            }
            continue;
        }
        if (inCodeBlock) {
            result += escapeHtml(line) + '\n';
            continue;
        }

        // Horizontal rule
        QRegularExpression hrRe("^[-*_]{3,}\\s*$");
        if (hrRe.match(line).hasMatch()) {
            closeInlines(result, inQuote, inList, inOrderedList);
            if (inQuote) { result += "</blockquote>\n"; inQuote = false; }
            result += "<hr/>\n";
            continue;
        }

        // Quote
        if (line.startsWith("> ")) {
            closeInlines(result, inQuote, inList, inOrderedList);
            if (!inQuote) { result += "<blockquote>"; inQuote = true; }
            result += parseInline(line.mid(2)) + "<br/>";
            continue;
        }
        if (inQuote) { result += "</blockquote>\n"; inQuote = false; }

        // Unordered list
        QRegularExpression ulRe("^\\s*[*+-]\\s+(.+)");
        auto ulMatch = ulRe.match(line);
        if (ulMatch.hasMatch()) {
            closeInlines(result, inQuote, inList, inOrderedList);
            if (!inList) { result += "<ul>\n"; inList = true; }
            result += "<li>" + parseInline(ulMatch.captured(1)) + "</li>\n";
            continue;
        }

        // Ordered list
        QRegularExpression olRe("^\\s*\\d+\\.\\s+(.+)");
        auto olMatch = olRe.match(line);
        if (olMatch.hasMatch()) {
            closeInlines(result, inQuote, inList, inOrderedList);
            if (!inOrderedList) { result += "<ol>\n"; inOrderedList = true; }
            result += "<li>" + parseInline(olMatch.captured(1)) + "</li>\n";
            continue;
        }

        closeInlines(result, inQuote, inList, inOrderedList);
        if (inList) { result += "</ul>\n"; inList = false; }
        if (inOrderedList) { result += "</ol>\n"; inOrderedList = false; }

        // Heading
        QRegularExpression hRe("^(#{1,6})\\s+(.+)");
        auto hMatch = hRe.match(line);
        if (hMatch.hasMatch()) {
            int level = hMatch.captured(1).length();
            int hLevel = qMin(level + 1, 6);
            result += QString("<h%1>%2</h%1>\n").arg(hLevel).arg(parseInline(hMatch.captured(2)));
            continue;
        }

        // Table (simple: |col1|col2|)
        if (line.startsWith('|') && line.endsWith('|') && line.length() > 2) {
            QString tableHtml = "<table border='1' cellpadding='4' cellspacing='0' style='border-collapse:collapse'>";
            // Header row
            tableHtml += "<tr>";
            QStringList cells = line.split('|', Qt::SkipEmptyParts);
            for (const auto& cell : cells) {
                tableHtml += "<th>" + parseInline(cell.trimmed()) + "</th>";
            }
            tableHtml += "</tr>";

            // Next line is separator (|---|---|)
            if (i + 1 < lines.size() && lines[i + 1].contains("---")) {
                i++; // skip separator
                // Data rows
                for (i = i + 1; i < lines.size(); ++i) {
                    if (!lines[i].startsWith('|')) break;
                    tableHtml += "<tr>";
                    QStringList dataCells = lines[i].split('|', Qt::SkipEmptyParts);
                    for (const auto& cell : dataCells) {
                        tableHtml += "<td>" + parseInline(cell.trimmed()) + "</td>";
                    }
                    tableHtml += "</tr>";
                }
            }
            tableHtml += "</table>\n";
            result += tableHtml;
            continue;
        }

        // Empty line = paragraph break
        if (line.trimmed().isEmpty()) {
            result += "<br/>\n";
            continue;
        }

        // Regular paragraph
        result += "<p>" + parseInline(line) + "</p>\n";
    }

    // Close remaining
    closeInlines(result, inQuote, inList, inOrderedList);
    if (inQuote) result += "</blockquote>\n";
    if (inList) result += "</ul>\n";
    if (inOrderedList) result += "</ol>\n";
    if (inCodeBlock) result += "</code></pre>\n";

    return result;
}

QString MarkdownParser::parseInline(const QString& text) const {
    QString result = escapeHtml(text);

    // Spoiler: >!text!<
    QRegularExpression spoilRe(">!(.+?)!<");
    result.replace(spoilRe, "<span style='" + spoilerCss() + "'>\\1</span>");

    // Bold + Italic: ***text***
    QRegularExpression boldItalicRe("\\*\\*\\*(.+?)\\*\\*\\*");
    result.replace(boldItalicRe, "<b><i>\\1</i></b>");

    // Bold: **text**
    QRegularExpression boldRe("\\*\\*(.+?)\\*\\*");
    result.replace(boldRe, "<b>\\1</b>");

    // Italic: *text*
    QRegularExpression italicRe("(?<!\\*)\\*([^*\\n]+)\\*(?!\\*)");
    result.replace(italicRe, "<i>\\1</i>");

    // Strikethrough: ~~text~~
    QRegularExpression strikeRe("~~(.+?)~~");
    result.replace(strikeRe, "<s>\\1</s>");

    // Superscript: ^(text)
    QRegularExpression superRe("\\^\\((.+?)\\)");
    result.replace(superRe, "<sup>\\1</sup>");

    // Inline code: `code`
    QRegularExpression codeRe("`([^`\\n]+)`");
    result.replace(codeRe, "<code style='background:#2a2a3a;padding:1px 4px;border-radius:3px;font-family:monospace'>\\1</code>");

    // Images: ![alt](url) — must be BEFORE links
    QRegularExpression imgRe("!\\[([^\\]]*)\\]\\(([^\\)]+)\\)");
    result.replace(imgRe, "<img src='\\2' alt='\\1' style='max-width:100%'>");

    // Links: [text](url)
    QRegularExpression linkRe("\\[([^\\]]+)\\]\\(([^\\)]+)\\)");
    result.replace(linkRe, "<a href='\\2'>\\1</a>");

    // Reddit subreddit links: /r/subreddit (not inside href)
    QRegularExpression subRe("(?<!['\"=])(/r/(\\w+))");
    result.replace(subRe, "<a href='https://reddit.com\\1'>\\1</a>");

    // Reddit user links: /u/username
    QRegularExpression userRe("(?<!['\"=])(/u/(\\w+))");
    result.replace(userRe, "<a href='https://reddit.com\\1'>\\1</a>");

    // Auto-link plain URLs (not inside href/src attributes)
    QRegularExpression urlRe("(?<!['\"=])(https?://[^\\s<>\"{}|\\\\^`\\[\\]]+)");
    result.replace(urlRe, "<a href='\\1'>\\1</a>");

    return result;
}

} // namespace PinkReader
