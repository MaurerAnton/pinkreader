// Origin: org/quantumbadger/redreader/fragments/ShareOrderDialog.java
#include "fragments/share_order_dialog.h"
#include <algorithm>
#include <list>
#include <sstream>
#include <string>
#include <vector>

namespace PinkReader {

std::unique_ptr<ShareOrderDialog> ShareOrderDialog::newInstance(Intent& shareIntent) {
    auto dialog = std::make_unique<ShareOrderDialog>();

    // final Bundle args = new Bundle(1);
    // args.putParcelable("intent", shareIntent);
    // dialog.setArguments(args);
    dialog->shareIntent = &shareIntent;

    return dialog;
}

void ShareOrderDialog::onCreate(const class Bundle* /* savedInstanceState */) {
    // super.onCreate(savedInstanceState);
    // context = getContext();
    // packageManager = getActivity().getPackageManager();
    // shareIntent = BundleCompat.getParcelable(requireArguments(), "intent", Intent.class);
}

std::unique_ptr<void> ShareOrderDialog::onCreateDialog(const class Bundle* /* savedInstanceState */) {
    // super.onCreate(savedInstanceState);
    // orderedAppList = prioritizeTopApps(packageManager.queryIntentActivities(shareIntent, 0));
    // final MaterialAlertDialogBuilder builder = new MaterialAlertDialogBuilder(context);
    // builder.setTitle(context.getString(R.string.pref_behaviour_sharing_share_dialog_dialogtitle));
    // final ListView listView = new ListView(context);
    // builder.setView(listView);
    // listView.setAdapter(new ShareOrderAdapter(context, orderedAppList, this));
    // return builder.create();
    return nullptr;
}

std::vector<ResolveInfo> ShareOrderDialog::prioritizeTopApps(
        const std::vector<ResolveInfo>& unorderedList) {
    // if(unorderedList.isEmpty()) {
    //     General.quickToast(context, R.string.error_toast_no_share_app_installed);
    //     dismiss();
    // }
    if (unorderedList.empty()) {
        // quickToast + dismiss
        return {};
    }

    // Make a copy of the list since the original is not modifiable
    std::list<ResolveInfo> orderedList(unorderedList.begin(), unorderedList.end());

    // final List<String> prioritizedAppNames
    //     = Arrays.asList(PrefsUtility.pref_behaviour_sharing_dialog_data_get().split(";"));
    std::vector<std::string> prioritizedAppNames;
    // Split semicolon-separated string
    {
        std::string data; // = PrefsUtility::pref_behaviour_sharing_dialog_data_get();
        std::istringstream iss(data);
        std::string token;
        while (std::getline(iss, token, ';')) {
            prioritizedAppNames.push_back(token);
        }
    }

    // final ResolveInfo[] prioritizedApps = new ResolveInfo[prioritizedAppNames.size()];
    std::vector<ResolveInfo*> prioritizedApps(prioritizedAppNames.size(), nullptr);

    // get the ResolveInfos for the available prioritized Apps and save them in order
    int count = 0;
    auto it = orderedList.begin();
    while (it != orderedList.end()) {
        const auto& currentApp = *it;
        // const String currentAppName = currentApp.activityInfo.name;
        std::string currentAppName; // = currentApp.activityInfo.name;

        auto found = std::find(prioritizedAppNames.begin(), prioritizedAppNames.end(), currentAppName);
        if (found != prioritizedAppNames.end()) {
            int index = static_cast<int>(std::distance(prioritizedAppNames.begin(), found));
            prioritizedApps[index] = &(*it);
            it = orderedList.erase(it);
            // Exit early if all apps matched
            if (++count >= static_cast<int>(prioritizedAppNames.size())) {
                break;
            }
        } else {
            ++it;
        }
    }

    // Combine the two lists in order, respecting unavailable apps (null values in the Array)
    for (int i = static_cast<int>(prioritizedApps.size()) - 1; i >= 0; i--) {
        if (prioritizedApps[i] != nullptr) {
            orderedList.push_front(*prioritizedApps[i]);
        }
    }

    return std::vector<ResolveInfo>(orderedList.begin(), orderedList.end());
}

void ShareOrderDialog::onSelectedIntent(int position) {
    // final ActivityInfo info = orderedAppList.get(position).activityInfo;
    // persistPriority(info);
    // shareIntent.addCategory(Intent.CATEGORY_LAUNCHER);
    // shareIntent.setClassName(info.applicationInfo.packageName, info.name);
    // shareIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
    // startActivity(shareIntent);
}

void ShareOrderDialog::persistPriority(const ActivityInfo& selectedApplication) {
    // final LinkedList<String> priorityAppList =
    //     new LinkedList<>(Arrays.asList(PrefsUtility.pref_behaviour_sharing_dialog_data_get().split(";")));
    // priorityAppList.remove(selectedApplication.name);
    // priorityAppList.add(0, selectedApplication.name);
    // if(priorityAppList.size() > amountOfPrioritizedApps) {
    //     priorityAppList.removeLast();
    // }
    // PrefsUtility.pref_behaviour_sharing_dialog_data_set(context, StringUtils.join(priorityAppList, ";"));
}

} // namespace PinkReader
