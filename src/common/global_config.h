// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/GlobalConfig.kt (lines 1-27)
// Ported from Kotlin object to C++ struct with static fields

#pragma once

#include <string>
#include <optional>

namespace PinkReader {

// GlobalConfig.kt line 23: object GlobalConfig
struct GlobalConfig {
    // line 24: @JvmField @Keep var appName = BuildConfig.APPLICATION_ID
    static inline std::string appName;

    // line 25: @JvmField @Keep var appBuildType = BuildConfig.BUILD_TYPE
    static inline std::string appBuildType;

    // line 26: @JvmField @Keep var appId: String? = null
    static inline std::optional<std::string> appId = std::nullopt;
};

} // namespace PinkReader
