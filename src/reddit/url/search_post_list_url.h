/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: search_post_list_url.h - Port of RedReader's SearchPostListURL
 *
 * Line-by-line translation of:
 *   redreader/.../reddit/url/SearchPostListURL.java
 */

#pragma once

#include <QString>
#include <optional>
#include "post_listing_url.h"

namespace PinkReader {

// Forward
class RedditIdAndType;
class PostSort;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.SearchPostListURL
 */
class SearchPostListURL : public PostListingURL {
public:
	// Port of: public enum Type { SUB_OR_SUB_COMBO, MULTI }
	enum class Type {
		SUB_OR_SUB_COMBO,
		MULTI
	};

	// Port of: @Nullable public final Type type;
	const Type type;

	// Port of: @Nullable public final String subreddit;
	const QString subreddit;

	// Port of: @Nullable public final String username;
	const QString username;

	// Port of: @Nullable public final String name;
	const QString name;

	// Port of: @Nullable public final String query;
	const QString query;

	// Port of: @Nullable public final PostSort order;
	PostSort *order;

	// Port of: @Nullable public final Integer limit;
	std::optional<int> limit;

	// Port of: @Nullable public final String before;
	const QString before;

	// Port of: @Nullable public final RedditIdAndType after;
	RedditIdAndType *after;

	// Port of: SearchPostListURL(String, String, PostSort, Integer, String, RedditIdAndType)
	SearchPostListURL(const QString &subreddit, const QString &query,
			PostSort *order, std::optional<int> limit,
			const QString &before, RedditIdAndType *after);

	// Port of: SearchPostListURL(String, String, Integer, String, RedditIdAndType)
	// (delegates with RELEVANCE_ALL)
	SearchPostListURL(const QString &subreddit, const QString &query,
			std::optional<int> limit, const QString &before,
			RedditIdAndType *after);

	// Port of: SearchPostListURL(String username, String name, String query,
	//     PostSort, Integer, String, RedditIdAndType) — multi-reddit search
	SearchPostListURL(const QString &username, const QString &name,
			const QString &query, PostSort *order, std::optional<int> limit,
			const QString &before, RedditIdAndType *after);

	// Port of: SearchPostListURL(String username, String name, String query,
	//     Integer, String, RedditIdAndType) (delegates with RELEVANCE_ALL)
	SearchPostListURL(const QString &username, const QString &name,
			const QString &query, std::optional<int> limit,
			const QString &before, RedditIdAndType *after);

	// Port of: public static SearchPostListURL build(String location, String query)
	static SearchPostListURL *build(const QString &location, const QString &query);

	// Port of: @Override public SearchPostListURL after(final RedditIdAndType newAfter)
	PostListingURL *after(const RedditIdAndType &newAfter) override;

	// Port of: @Override public SearchPostListURL limit(final Integer newLimit)
	PostListingURL *limit(std::optional<int> newLimit) override;

	// Port of: public SearchPostListURL sort(final PostSort newOrder)
	SearchPostListURL *sort(PostSort *newOrder);

	// Port of: @Nullable @Override public PostSort getOrder()
	PostSort *getOrder() override;

	// Port of: @Override public Uri generateJsonUri()
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType()
	int pathType() override;

	// Port of: @Override public String humanReadableName(final Context context, final boolean shorter)
	// (Context dropped like the sibling port: strings are inlined)
	QString humanReadableName(bool shorter) override;

	// Port of: @Override public String humanReadablePath()
	QString humanReadablePath() override;
};

} // namespace PinkReader
