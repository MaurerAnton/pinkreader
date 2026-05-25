// Origin: org/quantumbadger/redreader/reddit/PostSort.java
#include "post_sort.h"
#include <stdexcept>

namespace PinkReader {

// Static instances
static const PostSort s_hot(PostSort::Value::HOT, 0);
static const PostSort s_new(PostSort::Value::NEW, 0);
static const PostSort s_rising(PostSort::Value::RISING, 0);
static const PostSort s_top_hour(PostSort::Value::TOP_HOUR, 0);
static const PostSort s_top_day(PostSort::Value::TOP_DAY, 0);
static const PostSort s_top_week(PostSort::Value::TOP_WEEK, 0);
static const PostSort s_top_month(PostSort::Value::TOP_MONTH, 0);
static const PostSort s_top_year(PostSort::Value::TOP_YEAR, 0);
static const PostSort s_top_all(PostSort::Value::TOP_ALL, 0);
static const PostSort s_controversial_hour(PostSort::Value::CONTROVERSIAL_HOUR, 0);
static const PostSort s_controversial_day(PostSort::Value::CONTROVERSIAL_DAY, 0);
static const PostSort s_controversial_week(PostSort::Value::CONTROVERSIAL_WEEK, 0);
static const PostSort s_controversial_month(PostSort::Value::CONTROVERSIAL_MONTH, 0);
static const PostSort s_controversial_year(PostSort::Value::CONTROVERSIAL_YEAR, 0);
static const PostSort s_controversial_all(PostSort::Value::CONTROVERSIAL_ALL, 0);
static const PostSort s_best(PostSort::Value::BEST, 0);
static const PostSort s_relevance_hour(PostSort::Value::RELEVANCE_HOUR, 0);
static const PostSort s_relevance_day(PostSort::Value::RELEVANCE_DAY, 0);
static const PostSort s_relevance_week(PostSort::Value::RELEVANCE_WEEK, 0);
static const PostSort s_relevance_month(PostSort::Value::RELEVANCE_MONTH, 0);
static const PostSort s_relevance_year(PostSort::Value::RELEVANCE_YEAR, 0);
static const PostSort s_relevance_all(PostSort::Value::RELEVANCE_ALL, 0);
static const PostSort s_new_hour(PostSort::Value::NEW_HOUR, 0);
static const PostSort s_new_day(PostSort::Value::NEW_DAY, 0);
static const PostSort s_new_week(PostSort::Value::NEW_WEEK, 0);
static const PostSort s_new_month(PostSort::Value::NEW_MONTH, 0);
static const PostSort s_new_year(PostSort::Value::NEW_YEAR, 0);
static const PostSort s_new_all(PostSort::Value::NEW_ALL, 0);
static const PostSort s_comments_hour(PostSort::Value::COMMENTS_HOUR, 0);
static const PostSort s_comments_day(PostSort::Value::COMMENTS_DAY, 0);
static const PostSort s_comments_week(PostSort::Value::COMMENTS_WEEK, 0);
static const PostSort s_comments_month(PostSort::Value::COMMENTS_MONTH, 0);
static const PostSort s_comments_year(PostSort::Value::COMMENTS_YEAR, 0);
static const PostSort s_comments_all(PostSort::Value::COMMENTS_ALL, 0);
static const PostSort s_hot_hour(PostSort::Value::HOT_HOUR, 0);
static const PostSort s_hot_day(PostSort::Value::HOT_DAY, 0);
static const PostSort s_hot_week(PostSort::Value::HOT_WEEK, 0);
static const PostSort s_hot_month(PostSort::Value::HOT_MONTH, 0);
static const PostSort s_hot_year(PostSort::Value::HOT_YEAR, 0);
static const PostSort s_hot_all(PostSort::Value::HOT_ALL, 0);

const PostSort& PostSort::HOT_VAL() { return s_hot; }
const PostSort& PostSort::NEW_VAL() { return s_new; }
const PostSort& PostSort::RISING_VAL() { return s_rising; }
const PostSort& PostSort::TOP_HOUR_VAL() { return s_top_hour; }
const PostSort& PostSort::TOP_DAY_VAL() { return s_top_day; }
const PostSort& PostSort::TOP_WEEK_VAL() { return s_top_week; }
const PostSort& PostSort::TOP_MONTH_VAL() { return s_top_month; }
const PostSort& PostSort::TOP_YEAR_VAL() { return s_top_year; }
const PostSort& PostSort::TOP_ALL_VAL() { return s_top_all; }
const PostSort& PostSort::CONTROVERSIAL_HOUR_VAL() { return s_controversial_hour; }
const PostSort& PostSort::CONTROVERSIAL_DAY_VAL() { return s_controversial_day; }
const PostSort& PostSort::CONTROVERSIAL_WEEK_VAL() { return s_controversial_week; }
const PostSort& PostSort::CONTROVERSIAL_MONTH_VAL() { return s_controversial_month; }
const PostSort& PostSort::CONTROVERSIAL_YEAR_VAL() { return s_controversial_year; }
const PostSort& PostSort::CONTROVERSIAL_ALL_VAL() { return s_controversial_all; }
const PostSort& PostSort::BEST_VAL() { return s_best; }
const PostSort& PostSort::RELEVANCE_HOUR_VAL() { return s_relevance_hour; }
const PostSort& PostSort::RELEVANCE_DAY_VAL() { return s_relevance_day; }
const PostSort& PostSort::RELEVANCE_WEEK_VAL() { return s_relevance_week; }
const PostSort& PostSort::RELEVANCE_MONTH_VAL() { return s_relevance_month; }
const PostSort& PostSort::RELEVANCE_YEAR_VAL() { return s_relevance_year; }
const PostSort& PostSort::RELEVANCE_ALL_VAL() { return s_relevance_all; }
const PostSort& PostSort::NEW_HOUR_VAL() { return s_new_hour; }
const PostSort& PostSort::NEW_DAY_VAL() { return s_new_day; }
const PostSort& PostSort::NEW_WEEK_VAL() { return s_new_week; }
const PostSort& PostSort::NEW_MONTH_VAL() { return s_new_month; }
const PostSort& PostSort::NEW_YEAR_VAL() { return s_new_year; }
const PostSort& PostSort::NEW_ALL_VAL() { return s_new_all; }
const PostSort& PostSort::COMMENTS_HOUR_VAL() { return s_comments_hour; }
const PostSort& PostSort::COMMENTS_DAY_VAL() { return s_comments_day; }
const PostSort& PostSort::COMMENTS_WEEK_VAL() { return s_comments_week; }
const PostSort& PostSort::COMMENTS_MONTH_VAL() { return s_comments_month; }
const PostSort& PostSort::COMMENTS_YEAR_VAL() { return s_comments_year; }
const PostSort& PostSort::COMMENTS_ALL_VAL() { return s_comments_all; }
const PostSort& PostSort::HOT_HOUR_VAL() { return s_hot_hour; }
const PostSort& PostSort::HOT_DAY_VAL() { return s_hot_day; }
const PostSort& PostSort::HOT_WEEK_VAL() { return s_hot_week; }
const PostSort& PostSort::HOT_MONTH_VAL() { return s_hot_month; }
const PostSort& PostSort::HOT_YEAR_VAL() { return s_hot_year; }
const PostSort& PostSort::HOT_ALL_VAL() { return s_hot_all; }

std::string PostSort::asciiLowercase(const std::string& s) {
	std::string result = s;
	std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
	return result;
}

std::string PostSort::asciiUppercase(const std::string& s) {
	std::string result = s;
	std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::toupper(c); });
	return result;
}

std::string PostSort::nameOf(Value v) {
	switch (v) {
		case Value::HOT: return "HOT";
		case Value::NEW: return "NEW";
		case Value::RISING: return "RISING";
		case Value::TOP_HOUR: return "TOP_HOUR";
		case Value::TOP_DAY: return "TOP_DAY";
		case Value::TOP_WEEK: return "TOP_WEEK";
		case Value::TOP_MONTH: return "TOP_MONTH";
		case Value::TOP_YEAR: return "TOP_YEAR";
		case Value::TOP_ALL: return "TOP_ALL";
		case Value::CONTROVERSIAL_HOUR: return "CONTROVERSIAL_HOUR";
		case Value::CONTROVERSIAL_DAY: return "CONTROVERSIAL_DAY";
		case Value::CONTROVERSIAL_WEEK: return "CONTROVERSIAL_WEEK";
		case Value::CONTROVERSIAL_MONTH: return "CONTROVERSIAL_MONTH";
		case Value::CONTROVERSIAL_YEAR: return "CONTROVERSIAL_YEAR";
		case Value::CONTROVERSIAL_ALL: return "CONTROVERSIAL_ALL";
		case Value::BEST: return "BEST";
		case Value::RELEVANCE_HOUR: return "RELEVANCE_HOUR";
		case Value::RELEVANCE_DAY: return "RELEVANCE_DAY";
		case Value::RELEVANCE_WEEK: return "RELEVANCE_WEEK";
		case Value::RELEVANCE_MONTH: return "RELEVANCE_MONTH";
		case Value::RELEVANCE_YEAR: return "RELEVANCE_YEAR";
		case Value::RELEVANCE_ALL: return "RELEVANCE_ALL";
		case Value::NEW_HOUR: return "NEW_HOUR";
		case Value::NEW_DAY: return "NEW_DAY";
		case Value::NEW_WEEK: return "NEW_WEEK";
		case Value::NEW_MONTH: return "NEW_MONTH";
		case Value::NEW_YEAR: return "NEW_YEAR";
		case Value::NEW_ALL: return "NEW_ALL";
		case Value::COMMENTS_HOUR: return "COMMENTS_HOUR";
		case Value::COMMENTS_DAY: return "COMMENTS_DAY";
		case Value::COMMENTS_WEEK: return "COMMENTS_WEEK";
		case Value::COMMENTS_MONTH: return "COMMENTS_MONTH";
		case Value::COMMENTS_YEAR: return "COMMENTS_YEAR";
		case Value::COMMENTS_ALL: return "COMMENTS_ALL";
		case Value::HOT_HOUR: return "HOT_HOUR";
		case Value::HOT_DAY: return "HOT_DAY";
		case Value::HOT_WEEK: return "HOT_WEEK";
		case Value::HOT_MONTH: return "HOT_MONTH";
		case Value::HOT_YEAR: return "HOT_YEAR";
		case Value::HOT_ALL: return "HOT_ALL";
	}
	return "UNKNOWN";
}

const PostSort* PostSort::valueOfOrNull(const std::string& string) {
	try {
		std::string upper = asciiUppercase(string);

		if (upper == "HOT") return &s_hot;
		if (upper == "NEW") return &s_new;
		if (upper == "RISING") return &s_rising;
		if (upper == "TOP_HOUR") return &s_top_hour;
		if (upper == "TOP_DAY") return &s_top_day;
		if (upper == "TOP_WEEK") return &s_top_week;
		if (upper == "TOP_MONTH") return &s_top_month;
		if (upper == "TOP_YEAR") return &s_top_year;
		if (upper == "TOP_ALL") return &s_top_all;
		if (upper == "CONTROVERSIAL_HOUR") return &s_controversial_hour;
		if (upper == "CONTROVERSIAL_DAY") return &s_controversial_day;
		if (upper == "CONTROVERSIAL_WEEK") return &s_controversial_week;
		if (upper == "CONTROVERSIAL_MONTH") return &s_controversial_month;
		if (upper == "CONTROVERSIAL_YEAR") return &s_controversial_year;
		if (upper == "CONTROVERSIAL_ALL") return &s_controversial_all;
		if (upper == "BEST") return &s_best;
		if (upper == "RELEVANCE_HOUR") return &s_relevance_hour;
		if (upper == "RELEVANCE_DAY") return &s_relevance_day;
		if (upper == "RELEVANCE_WEEK") return &s_relevance_week;
		if (upper == "RELEVANCE_MONTH") return &s_relevance_month;
		if (upper == "RELEVANCE_YEAR") return &s_relevance_year;
		if (upper == "RELEVANCE_ALL") return &s_relevance_all;
		if (upper == "NEW_HOUR") return &s_new_hour;
		if (upper == "NEW_DAY") return &s_new_day;
		if (upper == "NEW_WEEK") return &s_new_week;
		if (upper == "NEW_MONTH") return &s_new_month;
		if (upper == "NEW_YEAR") return &s_new_year;
		if (upper == "NEW_ALL") return &s_new_all;
		if (upper == "COMMENTS_HOUR") return &s_comments_hour;
		if (upper == "COMMENTS_DAY") return &s_comments_day;
		if (upper == "COMMENTS_WEEK") return &s_comments_week;
		if (upper == "COMMENTS_MONTH") return &s_comments_month;
		if (upper == "COMMENTS_YEAR") return &s_comments_year;
		if (upper == "COMMENTS_ALL") return &s_comments_all;
		if (upper == "HOT_HOUR") return &s_hot_hour;
		if (upper == "HOT_DAY") return &s_hot_day;
		if (upper == "HOT_WEEK") return &s_hot_week;
		if (upper == "HOT_MONTH") return &s_hot_month;
		if (upper == "HOT_YEAR") return &s_hot_year;
		if (upper == "HOT_ALL") return &s_hot_all;

		return nullptr;
	} catch (...) {
		return nullptr;
	}
}

const PostSort* PostSort::parse(const std::string* sort, const std::string* t) {
	if (sort == nullptr) {
		return nullptr;
	}

	std::string sortLower = asciiLowercase(*sort);
	std::string tLower;
	if (t != nullptr) {
		tLower = asciiLowercase(*t);
	}
	const std::string* tPtr = t ? &tLower : nullptr;

	if (sortLower == "hot") {
		return &s_hot;

	} else if (sortLower == "new") {
		return &s_new;

	} else if (sortLower == "best") {
		return &s_best;

	} else if (sortLower == "controversial") {
		if (tPtr == nullptr) {
			return &s_controversial_all;
		} else if (*tPtr == "all") {
			return &s_controversial_all;
		} else if (*tPtr == "hour") {
			return &s_controversial_hour;
		} else if (*tPtr == "day") {
			return &s_controversial_day;
		} else if (*tPtr == "week") {
			return &s_controversial_week;
		} else if (*tPtr == "month") {
			return &s_controversial_month;
		} else if (*tPtr == "year") {
			return &s_controversial_year;
		} else {
			return &s_controversial_all;
		}

	} else if (sortLower == "rising") {
		return &s_rising;

	} else if (sortLower == "top") {
		if (tPtr == nullptr) {
			return &s_top_all;
		} else if (*tPtr == "all") {
			return &s_top_all;
		} else if (*tPtr == "hour") {
			return &s_top_hour;
		} else if (*tPtr == "day") {
			return &s_top_day;
		} else if (*tPtr == "week") {
			return &s_top_week;
		} else if (*tPtr == "month") {
			return &s_top_month;
		} else if (*tPtr == "year") {
			return &s_top_year;
		} else {
			return &s_top_all;
		}

	} else {
		return nullptr;
	}
}

const PostSort* PostSort::parseSearch(const std::string* sort, const std::string* t) {
	if (sort == nullptr) {
		return nullptr;
	}

	std::string sortLower = asciiLowercase(*sort);
	std::string tLower;
	if (t != nullptr) {
		tLower = asciiLowercase(*t);
	}
	const std::string* tPtr = t ? &tLower : nullptr;

	if (sortLower == "relevance") {
		if (tPtr == nullptr) {
			return &s_relevance_all;
		} else if (*tPtr == "all") {
			return &s_relevance_all;
		} else if (*tPtr == "hour") {
			return &s_relevance_hour;
		} else if (*tPtr == "day") {
			return &s_relevance_day;
		} else if (*tPtr == "week") {
			return &s_relevance_week;
		} else if (*tPtr == "month") {
			return &s_relevance_month;
		} else if (*tPtr == "year") {
			return &s_relevance_year;
		} else {
			return &s_relevance_all;
		}

	} else if (sortLower == "new") {
		if (tPtr == nullptr) {
			return &s_new_all;
		} else if (*tPtr == "all") {
			return &s_new_all;
		} else if (*tPtr == "hour") {
			return &s_new_hour;
		} else if (*tPtr == "day") {
			return &s_new_day;
		} else if (*tPtr == "week") {
			return &s_new_week;
		} else if (*tPtr == "month") {
			return &s_new_month;
		} else if (*tPtr == "year") {
			return &s_new_year;
		} else {
			return &s_new_all;
		}

	} else if (sortLower == "hot") {
		if (tPtr == nullptr) {
			return &s_hot_all;
		} else if (*tPtr == "all") {
			return &s_hot_all;
		} else if (*tPtr == "hour") {
			return &s_hot_hour;
		} else if (*tPtr == "day") {
			return &s_hot_day;
		} else if (*tPtr == "week") {
			return &s_hot_week;
		} else if (*tPtr == "month") {
			return &s_hot_month;
		} else if (*tPtr == "year") {
			return &s_hot_year;
		} else {
			return &s_hot_all;
		}

	} else if (sortLower == "top") {
		if (tPtr == nullptr) {
			return &s_top_all;
		} else if (*tPtr == "all") {
			return &s_top_all;
		} else if (*tPtr == "hour") {
			return &s_top_hour;
		} else if (*tPtr == "day") {
			return &s_top_day;
		} else if (*tPtr == "week") {
			return &s_top_week;
		} else if (*tPtr == "month") {
			return &s_top_month;
		} else if (*tPtr == "year") {
			return &s_top_year;
		} else {
			return &s_top_all;
		}

	} else if (sortLower == "comments") {
		if (tPtr == nullptr) {
			return &s_comments_all;
		} else if (*tPtr == "all") {
			return &s_comments_all;
		} else if (*tPtr == "hour") {
			return &s_comments_hour;
		} else if (*tPtr == "day") {
			return &s_comments_day;
		} else if (*tPtr == "week") {
			return &s_comments_week;
		} else if (*tPtr == "month") {
			return &s_comments_month;
		} else if (*tPtr == "year") {
			return &s_comments_year;
		} else {
			return &s_comments_all;
		}

	} else {
		return nullptr;
	}
}

void PostSort::addToUserPostListingUri(void* builder) const {
	(void)builder;
	// switch(this) {
	//     case HOT:
	//     case NEW:
	//         builder.appendQueryParameter("sort", asciiLowercase(nameOf(value)));
	//         break;
	//     case CONTROVERSIAL_HOUR/DAY/WEEK/MONTH/YEAR/ALL:
	//     case TOP_HOUR/DAY/WEEK/MONTH/YEAR/ALL:
	//         std::string name = nameOf(value);
	//         auto parts = split(name, "_");
	//         builder.appendQueryParameter("sort", asciiLowercase(parts[0]));
	//         builder.appendQueryParameter("t", asciiLowercase(parts[1]));
	//         break;
	// }
}

void PostSort::addToSubredditListingUri(void* builder) const {
	(void)builder;
	// switch(this) {
	//     case HOT/NEW/RISING/BEST:
	//         builder.appendEncodedPath(asciiLowercase(nameOf(value)));
	//         break;
	//     case CONTROVERSIAL_*/TOP_*:
	//         std::string name = nameOf(value);
	//         auto parts = split(name, "_");
	//         builder.appendEncodedPath(asciiLowercase(parts[0]));
	//         builder.appendQueryParameter("t", asciiLowercase(parts[1]));
	//         break;
	// }
}

} // namespace PinkReader
