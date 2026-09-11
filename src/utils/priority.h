/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: priority.h - request priority value class
 *
 * Port of org.quantumbadger.redreader.common.Priority (Kotlin data class):
 *   data class Priority(primary: Int, secondary: Int = 0) with
 *   isHigherPriorityThan (lower numeric value wins, primary first).
 *
 * NOTE: this is intentionally NOT in the Constants::Priority namespace
 * (request priority constants in core/constants.h) — upstream keeps them
 * in separate Java packages; here the data class keeps the bare name.
 */

#pragma once

namespace PinkReader {

class Priority {
public:
    int primary;
    int secondary = 0;

    explicit Priority(int primaryVal, int secondaryVal = 0)
        : primary(primaryVal), secondary(secondaryVal) {}

    bool isHigherPriorityThan(const Priority &other) const {
        if (primary != other.primary) {
            return primary < other.primary;
        }
        return secondary < other.secondary;
    }
};

} // namespace PinkReader
