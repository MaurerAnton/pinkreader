#pragma once

#include <QObject>
#include <QString>

namespace PinkReader {

class MarkdownParser : public QObject {
    Q_OBJECT
public:
    explicit MarkdownParser(QObject* parent = nullptr);

    Q_INVOKABLE QString toHtml(const QString& markdown) const;

private:
    QString escapeHtml(const QString& text) const;
    QString parseInline(const QString& text) const;
    QString parseBlocks(const QString& text) const;

    // Spoilered text
    static QString spoilerCss() { return
        "background:#333;color:#333;border-radius:4px;padding:0 4px;"
        ":hover{color:#fff;background:#555}";
    }
};

} // namespace PinkReader
