// Origin: org/quantumbadger/redreader/reddit/ (OptionsMenuUtility.Sort interface equivalent)
#pragma once

namespace PinkReader {

class Sort {
public:
	virtual ~Sort() = default;
	virtual int getMenuTitle() = 0;
	virtual void onSortSelected(void* activity) = 0;
};

} // namespace PinkReader
