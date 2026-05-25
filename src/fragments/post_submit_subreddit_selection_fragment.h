// Origin: org/quantumbadger/redreader/fragments/postsubmit/PostSubmitSubredditSelectionFragment.java
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace PinkReader {

class SubredditCanonicalId;
class Bundle;
class View;
class ViewGroup;
class LayoutInflater;
class Context;
class RecyclerView;
class TextInputEditText;
class MaterialAutoCompleteTextView;

class PostSubmitSubredditSelectionFragment {
public:
    // Static inner class Args
    class Args {
    public:
        static constexpr const char* KEY_SUBREDDIT = "subreddit";

        const SubredditCanonicalId* subreddit = nullptr;

        explicit Args(const SubredditCanonicalId* subreddit = nullptr);

        std::unique_ptr<Bundle> toBundle() const;
        static Args fromBundle(const Bundle& bundle);
    };

    // Interface Listener
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void onSubredditSelected(
                const std::string& username,
                const SubredditCanonicalId& subreddit) = 0;
        virtual void onNotLoggedIn() = 0;
    };

    // Private static class AutocompleteEntry
    struct AutocompleteEntry {
        int64_t listId;
        std::string nameWithoutPrefix;

        AutocompleteEntry(int64_t listId, std::string nameWithoutPrefix);
    };

    // Private inner class AutocompleteAdapter
    class AutocompleteAdapter {
    public:
        explicit AutocompleteAdapter(Context& context);

        void updateSuggestions();

        // RecyclerView.Adapter methods (placeholder)
        void* onCreateViewHolder(ViewGroup& viewGroup, int viewType);
        void onBindViewHolder(void* viewHolder, int position);
        int getItemCount() const;
        int64_t getItemId(int position) const;

    private:
        std::vector<AutocompleteEntry> mAllSuggestions;
        std::vector<AutocompleteEntry> mCurrentSuggestions;
        TextInputEditText* mSubredditBox = nullptr;
    };

    // Fragment lifecycle methods
    void onResume();

    std::unique_ptr<View> onCreateView(
            LayoutInflater& inflater,
            ViewGroup* container,
            const Bundle* savedInstanceState);

private:
    void scrollToTop();

    MaterialAutoCompleteTextView* mUsernameSpinner = nullptr;
    TextInputEditText* mSubredditBox = nullptr;
    RecyclerView* mAutocompleteSuggestions = nullptr;
    void* mAutocompleteSuggestionsLayout = nullptr;
};

} // namespace PinkReader
