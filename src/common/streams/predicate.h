// Origin: org/quantumbadger/redreader/common/streams/Predicate.java
#pragma once

namespace PinkReader {

template<typename E>
class Predicate {
public:
	virtual ~Predicate() = default;

	virtual bool matches(const E& value) = 0;
};

} // namespace PinkReader
