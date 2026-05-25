// Origin: org/quantumbadger/redreader/reddit/UserCommentSort.java
#include "user_comment_sort.h"

namespace PinkReader {

static const UserCommentSort s_new(UserCommentSort::Value::NEW, 0);
static const UserCommentSort s_hot(UserCommentSort::Value::HOT, 0);
static const UserCommentSort s_controversial_hour(UserCommentSort::Value::CONTROVERSIAL_HOUR, 0);
static const UserCommentSort s_controversial_day(UserCommentSort::Value::CONTROVERSIAL_DAY, 0);
static const UserCommentSort s_controversial_week(UserCommentSort::Value::CONTROVERSIAL_WEEK, 0);
static const UserCommentSort s_controversial_month(UserCommentSort::Value::CONTROVERSIAL_MONTH, 0);
static const UserCommentSort s_controversial_year(UserCommentSort::Value::CONTROVERSIAL_YEAR, 0);
static const UserCommentSort s_controversial_all(UserCommentSort::Value::CONTROVERSIAL_ALL, 0);
static const UserCommentSort s_top_hour(UserCommentSort::Value::TOP_HOUR, 0);
static const UserCommentSort s_top_day(UserCommentSort::Value::TOP_DAY, 0);
static const UserCommentSort s_top_week(UserCommentSort::Value::TOP_WEEK, 0);
static const UserCommentSort s_top_month(UserCommentSort::Value::TOP_MONTH, 0);
static const UserCommentSort s_top_year(UserCommentSort::Value::TOP_YEAR, 0);
static const UserCommentSort s_top_all(UserCommentSort::Value::TOP_ALL, 0);

const UserCommentSort& UserCommentSort::NEW_VAL() { return s_new; }
const UserCommentSort& UserCommentSort::HOT_VAL() { return s_hot; }
const UserCommentSort& UserCommentSort::CONTROVERSIAL_HOUR_VAL() { return s_controversial_hour; }
const UserCommentSort& UserCommentSort::CONTROVERSIAL_DAY_VAL() { return s_controversial_day; }
const UserCommentSort& UserCommentSort::CONTROVERSIAL_WEEK_VAL() { return s_controversial_week; }
const UserCommentSort& UserCommentSort::CONTROVERSIAL_MONTH_VAL() { return s_controversial_month; }
const UserCommentSort& UserCommentSort::CONTROVERSIAL_YEAR_VAL() { return s_controversial_year; }
const UserCommentSort& UserCommentSort::CONTROVERSIAL_ALL_VAL() { return s_controversial_all; }
const UserCommentSort& UserCommentSort::TOP_HOUR_VAL() { return s_top_hour; }
const UserCommentSort& UserCommentSort::TOP_DAY_VAL() { return s_top_day; }
const UserCommentSort& UserCommentSort::TOP_WEEK_VAL() { return s_top_week; }
const UserCommentSort& UserCommentSort::TOP_MONTH_VAL() { return s_top_month; }
const UserCommentSort& UserCommentSort::TOP_YEAR_VAL() { return s_top_year; }
const UserCommentSort& UserCommentSort::TOP_ALL_VAL() { return s_top_all; }

std::string UserCommentSort::asciiLowercase(const std::string& s) {
	std::string result = s;
	std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
	return result;
}

std::string UserCommentSort::nameOf(Value v) {
	switch (v) {
		case Value::NEW: return "NEW";
		case Value::HOT: return "HOT";
		case Value::CONTROVERSIAL_HOUR: return "CONTROVERSIAL_HOUR";
		case Value::CONTROVERSIAL_DAY: return "CONTROVERSIAL_DAY";
		case Value::CONTROVERSIAL_WEEK: return "CONTROVERSIAL_WEEK";
		case Value::CONTROVERSIAL_MONTH: return "CONTROVERSIAL_MONTH";
		case Value::CONTROVERSIAL_YEAR: return "CONTROVERSIAL_YEAR";
		case Value::CONTROVERSIAL_ALL: return "CONTROVERSIAL_ALL";
		case Value::TOP_HOUR: return "TOP_HOUR";
		case Value::TOP_DAY: return "TOP_DAY";
		case Value::TOP_WEEK: return "TOP_WEEK";
		case Value::TOP_MONTH: return "TOP_MONTH";
		case Value::TOP_YEAR: return "TOP_YEAR";
		case Value::TOP_ALL: return "TOP_ALL";
	}
	return "UNKNOWN";
}

const UserCommentSort* UserCommentSort::parse(const std::string* sort, const std::string* t) {
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

void UserCommentSort::addToUserCommentListingUri(void* builder) const {
	(void)builder;
	// switch(this) {
	//     case HOT:
	//     case NEW:
	//         builder.appendQueryParameter("sort", asciiLowercase(nameOf(value)));
	//         break;
	//     case CONTROVERSIAL_* / TOP_*:
	//         std::string name = nameOf(value);
	//         auto parts = split(name, "_");
	//         builder.appendQueryParameter("sort", asciiLowercase(parts[0]));
	//         builder.appendQueryParameter("t", asciiLowercase(parts[1]));
	//         break;
	// }
}

} // namespace PinkReader
