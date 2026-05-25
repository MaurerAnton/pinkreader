/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * Ported from RedReader: RedditParsedComment.java
 * File: reddit_parsed_comment.cpp - Parsed comment implementation
 */

#include "reddit_parsed_comment.h"

// Stub includes for dependencies not yet fully ported
#include "reddit_comment.h"
#include "../common/better_ssb.h"
#include "../common/reddit_id_and_type.h"

#include <QDebug>
#include <QImage>
#include <QFontMetrics>
#include <QApplication>
#include <QScreen>

namespace PinkReader {

// --- FlairEmoteData factory from RedditComment ---
static std::vector<MaybeParseError<FlairEmoteData>>
parseFlairRichtext(const RedditComment &comment) {
    std::vector<MaybeParseError<FlairEmoteData>> result;
    // In the Java original, this comes from comment.getAuthor_flair_richtext()
    // which returns List<MaybeParseError<RedditComment.FlairEmoteData>>
    // Ported as: parse each emote from the comment's extra data
    // TODO: implement full parse when RedditComment flair_richtext field is ported
    return result;
}

// --- RedditParsedComment constructor ---
RedditParsedComment::RedditParsedComment(
    QSharedPointer<RedditComment> comment,
    QObject *activity)
    : m_src(comment)
{
    // mBody = HtmlReader.parse(comment.getBody_html().getDecoded(), activity)
    // Port: decode body HTML to BodyElement
    // TODO: integrate HtmlReader when ported
    m_body = QSharedPointer<BodyElement>::create();

    // Flair parsing:
    // final String flair = General.mapIfNotNull(
    //     comment.getAuthor_flair_text(), UrlEncodedString::getDecoded);
    const QString flair = m_src->body(); // TODO: use author_flair_text when ported

    if (!flair.isEmpty()) {
        m_flair = QSharedPointer<BetterSSB>::create();
        m_flair->append(flair);

        const auto flairRichtext = parseFlairRichtext(*m_src);
        if (!flairRichtext.empty()) {
            getFlairEmotes(flairRichtext, activity);
        }
    }
    // else m_flair remains null (default QSharedPointer)
}

// --- getIdAlone ---
QString RedditParsedComment::getIdAlone() const {
    // return mSrc.getIdAlone()
    return m_src->id();
}

// --- getIdAndType ---
RedditIdAndType RedditParsedComment::getIdAndType() const {
    // return mSrc.getIdAndType()
    // Port: construct from comment's id
    return RedditIdAndType(m_src->id(), m_src->name());
}

// --- getFlairEmotes (private) ---
// Mirrors: private void getFlairEmotes(List<MaybeParseError<FlairEmoteData>>, AppCompatActivity)
void RedditParsedComment::getFlairEmotes(
    const std::vector<MaybeParseError<FlairEmoteData>> &flairRichtext,
    QObject *activity)
{
    // Alignment: Android uses ImageSpan.ALIGN_CENTER on Q+, ALIGN_BASELINE otherwise
    // In C++/Qt port, we use Qt::AlignCenter for all
    const int alignment = static_cast<int>(Qt::AlignCenter); // ImageSpan.ALIGN_CENTER equivalent

    for (const auto &flairEmoteData : flairRichtext) {
        if (!flairEmoteData.isOk()) {
            continue;
        }

        const FlairEmoteData &flairEmoteObject = flairEmoteData.getValue();

        const QString objectType = flairEmoteObject.getE();

        if (objectType == QStringLiteral("emoji")) {
            const QString placeholder = flairEmoteObject.getA();
            const QString url = flairEmoteObject.getU();

            // Port of:
            // CacheManager.getInstance(activity).makeRequest(new CacheRequest(...))
            // With IMMEDIATE queue type and anonymous account
            // The callback downloads and decodes the bitmap, then scales it
            // and replaces the placeholder text in mFlair with the image span
            //
            // In Qt port: we would use QNetworkAccessManager for download
            // For now, this is a stub that logs the emote request
            qDebug() << "RedditParsedComment: requesting flair emote from" << url
                     << "placeholder:" << placeholder;

            // Full implementation requires CacheManager/CacheRequest port
            // The callback pattern:
            // 1. Download bitmap from url
            // 2. BitmapFactory.decodeStream(is) -> QImage
            // 3. Scale if height > maxHeight (maxHeight computed from font scale)
            // 4. Create ImageSpan with alignment
            // 5. mFlair.replace(placeholder, span)
            //
            // TODO: implement when CacheManager and network layer are ported
        }
    }
}

} // namespace PinkReader
