// Origin: org/quantumbadger/redreader/reddit/PostCommentSort.java
#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include "sort.h"

namespace PinkReader {

// Forward declaration
class AppCompatActivity;

class PostCommentSort : public Sort {
public:
	enum Value {
		BEST,
		HOT,
		NEW,
		OLD,
		TOP,
		CONTROVERSIAL,
		QA
	};

	const std::string key;
	const int menuTitle;
	const int suggestedTitle;
	const Value value;

	static const PostCommentSort& BEST_VAL() { static PostCommentSort v(Value::BEST, "confidence", 0, 0); return v; }
	static const PostCommentSort& HOT_VAL() { static PostCommentSort v(Value::HOT, "hot", 0, 0); return v; }
	static const PostCommentSort& NEW_VAL() { static PostCommentSort v(Value::NEW, "new", 0, 0); return v; }
	static const PostCommentSort& OLD_VAL() { static PostCommentSort v(Value::OLD, "old", 0, 0); return v; }
	static const PostCommentSort& TOP_VAL() { static PostCommentSort v(Value::TOP, "top", 0, 0); return v; }
	static const PostCommentSort& CONTROVERSIAL_VAL() { static PostCommentSort v(Value::CONTROVERSIAL, "controversial", 0, 0); return v; }
	static const PostCommentSort& QA_VAL() { static PostCommentSort v(Value::QA, "qa", 0, 0); return v; }

	// Static lookup - matches PostCommentSort.lookup(String name)
	static const PostCommentSort* lookup(const std::string& name) {
		std::string upper = name;
		std::transform(upper.begin(), upper.end(), upper.begin(),
				[](unsigned char c) { return std::toupper(c); });

		if (upper == "CONFIDENCE") {
			return &BEST_VAL(); // oh, reddit...
		}

		if (upper == "BEST") return &BEST_VAL();
		if (upper == "HOT") return &HOT_VAL();
		if (upper == "NEW") return &NEW_VAL();
		if (upper == "OLD") return &OLD_VAL();
		if (upper == "TOP") return &TOP_VAL();
		if (upper == "CONTROVERSIAL") return &CONTROVERSIAL_VAL();
		if (upper == "QA") return &QA_VAL();

		return nullptr;
	}

	int getMenuTitle() override { return menuTitle; }
	int getSuggestedTitle() const { return suggestedTitle; }

	void onSortSelected(void* activity) override {
		// ((OptionsMenuUtility.OptionsMenuCommentsListener)activity).onSortSelected(this);
	}

private:
	PostCommentSort(Value v, std::string k, int menu, int suggested)
		: key(std::move(k)), menuTitle(menu), suggestedTitle(suggested), value(v) {}
};

} // namespace PinkReader
