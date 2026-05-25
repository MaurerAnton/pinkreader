// Origin: org/quantumbadger/redreader/reddit/PostSort.java
#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <memory>
#include "sort.h"

namespace PinkReader {

class PostSort : public Sort {
public:
	enum Value {
		HOT,
		NEW,
		RISING,
		TOP_HOUR,
		TOP_DAY,
		TOP_WEEK,
		TOP_MONTH,
		TOP_YEAR,
		TOP_ALL,
		CONTROVERSIAL_HOUR,
		CONTROVERSIAL_DAY,
		CONTROVERSIAL_WEEK,
		CONTROVERSIAL_MONTH,
		CONTROVERSIAL_YEAR,
		CONTROVERSIAL_ALL,
		BEST,
		// Sorts related to Search Listings
		RELEVANCE_HOUR,
		RELEVANCE_DAY,
		RELEVANCE_WEEK,
		RELEVANCE_MONTH,
		RELEVANCE_YEAR,
		RELEVANCE_ALL,
		NEW_HOUR,
		NEW_DAY,
		NEW_WEEK,
		NEW_MONTH,
		NEW_YEAR,
		NEW_ALL,
		COMMENTS_HOUR,
		COMMENTS_DAY,
		COMMENTS_WEEK,
		COMMENTS_MONTH,
		COMMENTS_YEAR,
		COMMENTS_ALL,
		HOT_HOUR,
		HOT_DAY,
		HOT_WEEK,
		HOT_MONTH,
		HOT_YEAR,
		HOT_ALL
	};

	const Value value;
	const int menuTitle;

	// All 39 instances
	static const PostSort& HOT_VAL();
	static const PostSort& NEW_VAL();
	static const PostSort& RISING_VAL();
	static const PostSort& TOP_HOUR_VAL();
	static const PostSort& TOP_DAY_VAL();
	static const PostSort& TOP_WEEK_VAL();
	static const PostSort& TOP_MONTH_VAL();
	static const PostSort& TOP_YEAR_VAL();
	static const PostSort& TOP_ALL_VAL();
	static const PostSort& CONTROVERSIAL_HOUR_VAL();
	static const PostSort& CONTROVERSIAL_DAY_VAL();
	static const PostSort& CONTROVERSIAL_WEEK_VAL();
	static const PostSort& CONTROVERSIAL_MONTH_VAL();
	static const PostSort& CONTROVERSIAL_YEAR_VAL();
	static const PostSort& CONTROVERSIAL_ALL_VAL();
	static const PostSort& BEST_VAL();
	static const PostSort& RELEVANCE_HOUR_VAL();
	static const PostSort& RELEVANCE_DAY_VAL();
	static const PostSort& RELEVANCE_WEEK_VAL();
	static const PostSort& RELEVANCE_MONTH_VAL();
	static const PostSort& RELEVANCE_YEAR_VAL();
	static const PostSort& RELEVANCE_ALL_VAL();
	static const PostSort& NEW_HOUR_VAL();
	static const PostSort& NEW_DAY_VAL();
	static const PostSort& NEW_WEEK_VAL();
	static const PostSort& NEW_MONTH_VAL();
	static const PostSort& NEW_YEAR_VAL();
	static const PostSort& NEW_ALL_VAL();
	static const PostSort& COMMENTS_HOUR_VAL();
	static const PostSort& COMMENTS_DAY_VAL();
	static const PostSort& COMMENTS_WEEK_VAL();
	static const PostSort& COMMENTS_MONTH_VAL();
	static const PostSort& COMMENTS_YEAR_VAL();
	static const PostSort& COMMENTS_ALL_VAL();
	static const PostSort& HOT_HOUR_VAL();
	static const PostSort& HOT_DAY_VAL();
	static const PostSort& HOT_WEEK_VAL();
	static const PostSort& HOT_MONTH_VAL();
	static const PostSort& HOT_YEAR_VAL();
	static const PostSort& HOT_ALL_VAL();

	static std::string nameOf(Value v);

	// valueOfOrNull
	static const PostSort* valueOfOrNull(const std::string& string);

	// parse(sort, t)
	static const PostSort* parse(const std::string* sort, const std::string* t);

	// parseSearch(sort, t)
	static const PostSort* parseSearch(const std::string* sort, const std::string* t);

	// Helper: lowercase a string
	static std::string asciiLowercase(const std::string& s);
	static std::string asciiUppercase(const std::string& s);

	// addToUserPostListingUri
	void addToUserPostListingUri(void* builder) const;

	// addToSubredditListingUri
	void addToSubredditListingUri(void* builder) const;

	int getMenuTitle() override { return menuTitle; }

	void onSortSelected(void* activity) override {
		// ((OptionsMenuUtility.OptionsMenuPostsListener)activity).onSortSelected(this);
	}

	PostSort(Value v, int menu) : value(v), menuTitle(menu) {}
};

} // namespace PinkReader
