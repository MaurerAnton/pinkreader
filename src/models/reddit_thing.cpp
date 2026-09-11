/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: reddit_thing.cpp
 * Port of RedReader's RedditThing.java
 */

#include "models/reddit_thing.h"
#include "jsonwrap/json_object.h"
#include "models/reddit_subreddit.h"
#include "models/reddit_user.h"

namespace PinkReader {

const QString RedditThing::KIND_USER = QStringLiteral("t2");

// ============================================================================
// Static kinds map initialization (Java static initializer, lines 37-46)
// ============================================================================
QHash<QString, RedditThing::Kind> RedditThing::initKinds() {
    QHash<QString, Kind> map;
    map.insert(QStringLiteral("t1"), Kind::COMMENT);
    map.insert(KIND_USER, Kind::USER);
    map.insert(QStringLiteral("t3"), Kind::POST);
    map.insert(QStringLiteral("t4"), Kind::MESSAGE);
    map.insert(QStringLiteral("t5"), Kind::SUBREDDIT);
    map.insert(QStringLiteral("more"), Kind::MORE_COMMENTS);
    map.insert(QStringLiteral("Listing"), Kind::LISTING);
    return map;
}

QHash<QString, RedditThing::Kind> RedditThing::s_kinds =
        RedditThing::initKinds();

// ============================================================================
// getKind (Java lines 51-61)
// ============================================================================
RedditThing::Kind RedditThing::getKind() const {
    auto it = s_kinds.find(this->kind);

    if (it == s_kinds.end()) {
        throw std::runtime_error(
            std::string("Unknown thing type: ") + this->kind.toStdString());
    }

    return it.value();
}

// NOTE: asSubreddit/asUser omitted (see header note).

} // namespace PinkReader
