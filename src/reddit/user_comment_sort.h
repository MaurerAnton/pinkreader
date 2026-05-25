// Origin: org/quantumbadger/redreader/reddit/UserCommentSort.java
#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include "sort.h"

namespace PinkReader {

class UserCommentSort : public Sort {
public:
	enum Value {
		NEW,
		HOT,
		CONTROVERSIAL_HOUR,
		CONTROVERSIAL_DAY,
		CONTROVERSIAL_WEEK,
		CONTROVERSIAL_MONTH,
		CONTROVERSIAL_YEAR,
		CONTROVERSIAL_ALL,
		TOP_HOUR,
		TOP_DAY,
		TOP_WEEK,
		TOP_MONTH,
		TOP_YEAR,
		TOP_ALL
	};

	const Value value;
	const int menuTitle;

	static const UserCommentSort& NEW_VAL();
	static const UserCommentSort& HOT_VAL();
	static const UserCommentSort& CONTROVERSIAL_HOUR_VAL();
	static const UserCommentSort& CONTROVERSIAL_DAY_VAL();
	static const UserCommentSort& CONTROVERSIAL_WEEK_VAL();
	static const UserCommentSort& CONTROVERSIAL_MONTH_VAL();
	static const UserCommentSort& CONTROVERSIAL_YEAR_VAL();
	static const UserCommentSort& CONTROVERSIAL_ALL_VAL();
	static const UserCommentSort& TOP_HOUR_VAL();
	static const UserCommentSort& TOP_DAY_VAL();
	static const UserCommentSort& TOP_WEEK_VAL();
	static const UserCommentSort& TOP_MONTH_VAL();
	static const UserCommentSort& TOP_YEAR_VAL();
	static const UserCommentSort& TOP_ALL_VAL();

	static std::string nameOf(Value v);
	static std::string asciiLowercase(const std::string& s);

	// parse(sort, t)
	static const UserCommentSort* parse(const std::string* sort, const std::string* t);

	// addToUserCommentListingUri
	void addToUserCommentListingUri(void* builder) const;

	int getMenuTitle() override { return menuTitle; }

	void onSortSelected(void* activity) override {
		// ((OptionsMenuUtility.OptionsMenuCommentsListener)activity).onSortSelected(this);
	}

	UserCommentSort(Value v, int menu) : value(v), menuTitle(menu) {}
};

} // namespace PinkReader
