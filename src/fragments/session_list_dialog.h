// Origin: org/quantumbadger/redreader/fragments/SessionListDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class UriString;
class RecyclerView;

// UUID placeholder
using UUID = std::string;

enum class SessionChangeType {
    // Enum values from SessionChangeListener.SessionChangeType
    // Placeholder
};

class SessionListDialog {
public:
    static std::unique_ptr<SessionListDialog> newInstance(
            const UriString& url,
            const UUID& current,
            SessionChangeType type);

    void onCreate(const class Bundle* savedInstanceState);

    std::unique_ptr<void> onCreateDialog(const class Bundle* savedInstanceState);

    void onRedditAccountChanged();

private:
    UriString* url = nullptr;
    UUID current;
    SessionChangeType type;

    std::unique_ptr<RecyclerView> rv;

    // Workaround for HoloEverywhere bug?
    volatile bool alreadyCreated = false;
};

} // namespace PinkReader
