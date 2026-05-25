// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/Refreshable.java
#pragma once

namespace PinkReader {

class Refreshable {
public:
	virtual ~Refreshable() = default;
	virtual void refresh() = 0;
};

} // namespace PinkReader
