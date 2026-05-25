// Origin: org/quantumbadger/redreader/reddit/Hideable.java
#pragma once

namespace PinkReader {

class Hideable {
public:
	virtual ~Hideable() = default;
	virtual bool isVisible() = 0;
};

} // namespace PinkReader
