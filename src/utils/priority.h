/*
 * PinkReader - GPLv3
 * File: priority.h
 * Exact port of Priority.kt (31 lines)
 *
 * Ports every constructor, field, and method: isHigherPriorityThan
 */

#pragma once

#include <cstdint>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.Priority (Kotlin data class)
 *
 * Port of:
 *   data class Priority @JvmOverloads constructor(
 *       @JvmField val primary: Int,
 *       @JvmField val secondary: Int = 0
 *   ) {
 *       fun isHigherPriorityThan(other: Priority): Boolean {
 *           return if (primary != other.primary) {
 *               primary < other.primary
 *           } else {
 *               secondary < other.secondary
 *           }
 *       }
 *   }
 *
 * Every field and method matches exactly.
 */
class Priority {
public:
    // Port of: @JvmField val primary: Int
    int primary;

    // Port of: @JvmField val secondary: Int = 0
    int secondary = 0;

    // Port of: constructor(primary: Int, secondary: Int = 0)
    explicit Priority(int primaryVal, int secondaryVal = 0)
        : primary(primaryVal), secondary(secondaryVal) {}

    // Port of: fun isHigherPriorityThan(other: Priority): Boolean
    // Returns true if this priority is HIGHER (lower numeric value) than other.
    // Primary comparison first; if primary == other.primary, secondary is compared.
    bool isHigherPriorityThan(const Priority &other) const {
        if (primary != other.primary) {
            return primary < other.primary;
        }
        return secondary < other.secondary;
    }
};

} // namespace PinkReader
