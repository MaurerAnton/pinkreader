// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ChangelogManager.java (lines 1-131)
// Implementation

#include "changelog_manager.h"
#include <sstream>
#include <vector>

namespace PinkReader {

// line 38-130: generateViews
std::vector<ChangelogManager::ChangelogEntry> ChangelogManager::parseChangelog(
        const std::string& changelogText,
        bool showAll) {

    // Origin lines 48-54: determine filename based on package
    // filename = changelog-alpha.txt or changelog.txt
    // Already resolved to text content by caller

    std::vector<ChangelogEntry> entries;

    // Origin line 62: int itemsToShow = 10;
    int itemsToShow = 10;

    // Origin line 60: String curVersionName = null;
    std::string curVersionName;

    ChangelogEntry currentEntry;

    // Origin line 65: while((line = br.readLine()) != null)
    std::istringstream stream(changelogText);
    std::string line;

    while (std::getline(stream, line)) {
        // Origin line 67: if(line.isEmpty())
        if (line.empty()) {
            // Origin line 69: curVersionName = null;
            if (!curVersionName.empty()) {
                currentEntry.versionName = curVersionName;
                entries.push_back(std::move(currentEntry));
                currentEntry = ChangelogEntry{};
            }
            curVersionName.clear();

            // Origin lines 71-76: if(!showAll) { itemsToShow--; if(itemsToShow <= 0) break; }
            if (!showAll) {
                itemsToShow--;
                if (itemsToShow <= 0) {
                    break;
                }
            }

        // Origin line 78: } else if(curVersionName == null) {
        } else if (curVersionName.empty()) {
            // Origin line 80: final String[] lineSplit = line.split("/");
            size_t slashPos = line.find('/');
            if (slashPos != std::string::npos) {
                // Origin line 81: curVersionName = lineSplit[1];
                curVersionName = line.substr(slashPos + 1);
            }

            // Origin lines 83-104: create section header TextView
            // Simplified: just store the version name
            // header.setText(curVersionName); header.setTextColor(attr.colorAccent);
            // ViewCompat.setAccessibilityDelegate(header, ...);
            // items.addView(header);

        // Origin line 106: } else {
        } else {
            // Origin lines 108-122: create bullet item
            // Simplified: just store the bullet text
            // bullet.setText("•  "); text.setText(line);
            currentEntry.items.push_back("•  " + line);
        }
    }

    // Add last entry if there is one
    if (!curVersionName.empty()) {
        currentEntry.versionName = curVersionName;
        entries.push_back(std::move(currentEntry));
    }

    return entries;
}

} // namespace PinkReader
