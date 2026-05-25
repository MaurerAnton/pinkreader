/*******************************************************************************
 * This file is part of RedReader.
 *
 * RedReader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RedReader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RedReader.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/*
 * PinkReader C++ port of:
 *   org.quantumbadger.redreader.reddit.url.MultiredditPostListURL (lines 1-287)
 *
 * Every field, method, inner class, enum, constant, logic line.
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
 * @brief Port of org.quantumbadger.redreader.reddit.url.MultiredditPostListURL (Java lines 34-286)
 */
class MultiredditPostListURL : public PostListingURL {
public:
	// Port of: @Nullable public final String username; (Java line 65)
	QString username;

	// Port of: @NonNull public final String name; (Java line 66)
	const QString name;

	// Port of: @Nullable public final PostSort order; (Java line 68)
	PostSort *order;

	// Port of: @Nullable public final Integer limit; (Java line 69)
	std::optional<int> limit;

	// Port of: @Nullable public final String before; (Java line 70)
	QString before;

	// Port of: @Nullable public final RedditIdAndType after; (Java line 71)
	RedditIdAndType *after;

	// Port of: public static RedditURLParser.RedditURL getMultireddit(@NonNull final String name) (Java lines 36-47)
	static RedditURLParser::RedditURL *getMultireddit(const QString &name);

	// Port of: public static RedditURLParser.RedditURL getMultireddit(
	//            @NonNull final String username, @NonNull final String name) (Java lines 49-63)
	static RedditURLParser::RedditURL *getMultireddit(const QString &username, const QString &name);

	// Port of: private MultiredditPostListURL(...) (Java lines 73-87)
	MultiredditPostListURL(const QString &username, const QString &name,
			PostSort *order, std::optional<int> limit,
			const QString &before, RedditIdAndType *after);

	// Port of: @Override public MultiredditPostListURL after(final RedditIdAndType newAfter) (Java lines 89-92)
	PostListingURL *after(const RedditIdAndType &newAfter) override;

	// Port of: @Override public MultiredditPostListURL limit(final Integer newLimit) (Java lines 94-97)
	PostListingURL *limit(std::optional<int> newLimit) override;

	// Port of: public MultiredditPostListURL sort(final PostSort newOrder) (Java lines 99-101)
	MultiredditPostListURL *sort(PostSort *newOrder);

	// Port of: @Nullable @Override public PostSort getOrder() (Java lines 103-107)
	PostSort *getOrder() override;

	// Port of: @Override public Uri generateJsonUri() (Java lines 109-145)
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 147-151)
	int pathType() override;

	// Port of: public static MultiredditPostListURL parse(final Uri uri) (Java lines 153-246)
	static MultiredditPostListURL *parse(const PinkUri &uri);

	// Port of: @Override public String humanReadablePath() (Java lines 248-275)
	QString humanReadablePath() override;

	// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 277-286)
	QString humanReadableName(bool shorter) override;
};

} // namespace PinkReader
