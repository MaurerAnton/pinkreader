// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/HasUniqueId.java
#pragma once

#include <string>

namespace PinkReader {

/**
 * Interface for objects that have a unique string identifier.
 * Equivalent to: org.quantumbadger.redreader.common.HasUniqueId
 */
class HasUniqueId {
public:
	virtual ~HasUniqueId() = default;

	/** @return a non-null unique string identifier */
	virtual std::string getUniqueId() const = 0;
};

} // namespace PinkReader
