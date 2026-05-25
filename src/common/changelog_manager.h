// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ChangelogManager.java (lines 1-131)
// Ported from Java to C++

#pragma once

#include <string>
#include <functional>

namespace PinkReader {

// ChangelogManager.java line 36: public class ChangelogManager
class ChangelogManager {
public:
    // line 38-130: generateViews
    // Origin: public static void generateViews(final AppCompatActivity context, final LinearLayout items, final boolean showAll)
    // Simplified: non-Android C++ port returns parsed changelog data
    struct ChangelogEntry {
        std::string versionName;
        std::vector<std::string> items;
    };

    // Returns parsed changelog entries from a text asset
    // filename: "changelog.txt" or "changelog-alpha.txt"
    // showAll: if false, returns at most 10 sections
    static std::vector<ChangelogEntry> parseChangelog(
            const std::string& changelogText,
            bool showAll);

private:
    ChangelogManager() = default;
};

} // namespace PinkReader
