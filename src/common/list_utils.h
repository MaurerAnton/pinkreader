// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ListUtils.java
#pragma once

#include <cstddef>
#include <vector>

namespace PinkReader {

/**
 * Utility methods for lists.
 * Equivalent to: org.quantumbadger.redreader.common.ListUtils
 */
class ListUtils {
public:
	ListUtils() = delete;

	/**
	 * Returns the last element of the list, or nullptr if the list is empty.
	 * Equivalent to: ListUtils.getLast(List<E>)
	 *
	 * @tparam E element type
	 * @param list the list
	 * @return the last element, or nullptr if empty
	 */
	template<typename E>
	static E* getLast(std::vector<E*>& list) {
		if (list.empty()) {
			return nullptr;
		}
		return list[list.size() - 1];
	}

	/**
	 * Returns the last element of the list (const version), or nullptr if empty.
	 */
	template<typename E>
	static const E* getLast(const std::vector<E*>& list) {
		if (list.empty()) {
			return nullptr;
		}
		return list[list.size() - 1];
	}
};

} // namespace PinkReader
