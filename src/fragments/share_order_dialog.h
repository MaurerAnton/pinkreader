// Origin: org/quantumbadger/redreader/fragments/ShareOrderDialog.java
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>

namespace PinkReader {

class Intent;
class Context;
class ActivityInfo;
class ResolveInfo;
class PackageManager;

class ShareOrderDialog {
public:
    static std::unique_ptr<ShareOrderDialog> newInstance(Intent& shareIntent);

    void onCreate(const class Bundle* savedInstanceState);

    std::unique_ptr<void> onCreateDialog(const class Bundle* savedInstanceState);

    void onSelectedIntent(int position);

private:
    static const int amountOfPrioritizedApps = 3;

    std::vector<ResolveInfo> prioritizeTopApps(const std::vector<ResolveInfo>& unorderedList);

    void persistPriority(const ActivityInfo& selectedApplication);

    PackageManager* packageManager = nullptr;
    Intent* shareIntent = nullptr;
    std::vector<ResolveInfo> orderedAppList;
    Context* context = nullptr;
};

} // namespace PinkReader
