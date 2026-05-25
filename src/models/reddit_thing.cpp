/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: reddit_thing.cpp
 * Port of RedReader's RedditThing.java
 */

#include "models/reddit_thing.h"
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

// ============================================================================
// asSubreddit (Java lines 63-69)
// ============================================================================
RedditSubreddit RedditThing::asSubreddit() const {
    // In Java: return data.asObject(RedditSubreddit.class);
    // This calls JsonObject::asObject<T>() which does reflection-based
    // deserialization. The concrete implementation depends on the
    // JsonObject wrapper used by the project.
    if (!data) {
        throw std::runtime_error("RedditThing::asSubreddit: data is null");
    }
    return data->asObject<RedditSubreddit>();
}

// ============================================================================
// asUser (Java lines 71-77)
// ============================================================================
RedditUser RedditThing::asUser() const {
    // In Java: return data.asObject(RedditUser.class);
    if (!data) {
        throw std::runtime_error("RedditThing::asUser: data is null");
    }
    return data->asObject<RedditUser>();
}

} // namespace PinkReader
