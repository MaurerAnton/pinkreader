// Origin: org/quantumbadger/redreader/fragments/postsubmit/PostSubmitSubredditSelectionFragment.java
#include "fragments/post_submit_subreddit_selection_fragment.h"
#include <algorithm>
#include <cctype>

namespace PinkReader {

// --- Args ---

PostSubmitSubredditSelectionFragment::Args::Args(const SubredditCanonicalId* subreddit)
    : subreddit(subreddit) {
}

std::unique_ptr<Bundle> PostSubmitSubredditSelectionFragment::Args::toBundle() const {
    // final Bundle result = new Bundle(1);
    // if(subreddit != null) {
    //     result.putParcelable(KEY_SUBREDDIT, subreddit);
    // }
    // return result;
    return std::make_unique<Bundle>();
}

PostSubmitSubredditSelectionFragment::Args
PostSubmitSubredditSelectionFragment::Args::fromBundle(const Bundle& /* bundle */) {
    // return new Args(BundleCompat.getParcelable(bundle, KEY_SUBREDDIT, SubredditCanonicalId.class));
    return Args(nullptr);
}

// --- AutocompleteEntry ---

PostSubmitSubredditSelectionFragment::AutocompleteEntry::AutocompleteEntry(
        int64_t listId,
        std::string nameWithoutPrefix)
    : listId(listId)
    , nameWithoutPrefix(std::move(nameWithoutPrefix)) {
}

// --- AutocompleteAdapter ---

PostSubmitSubredditSelectionFragment::AutocompleteAdapter::AutocompleteAdapter(
        Context& /* context */) {
    // super();
    // setHasStableIds(true);
    //
    // final ArrayList<SubredditCanonicalId> allSuggestions
    //     = RedditSubredditHistory.getSubredditsSorted(
    //         RedditAccountManager.getInstance(context).getDefaultAccount());
    //
    // for(int i = 0; i < allSuggestions.size(); i++) {
    //     mAllSuggestions.add(new AutocompleteEntry(
    //         i, allSuggestions.get(i).getDisplayNameLowercase()));
    // }
    //
    // mCurrentSuggestions.addAll(mAllSuggestions);
}

void PostSubmitSubredditSelectionFragment::AutocompleteAdapter::updateSuggestions() {
    // mCurrentSuggestions.clear();
    //
    // final String currentText = StringUtils.asciiLowercase(
    //     mSubredditBox.getText().toString().trim());
    //
    // final String searchString;
    // try {
    //     searchString = RedditSubreddit.stripRPrefix(currentText);
    // } catch(final InvalidSubredditNameException e) {
    //     mCurrentSuggestions.addAll(mAllSuggestions);
    //     notifyDataSetChanged();
    //     return;
    // }
    //
    // final ArrayList<AutocompleteEntry> possibleSuggestions
    //     = new ArrayList<>(mAllSuggestions);
    //
    // // First pass: starts-with matches
    // Iterator<AutocompleteEntry> it = possibleSuggestions.iterator();
    // while(it.hasNext()) {
    //     AutocompleteEntry entry = it.next();
    //     if(entry.nameWithoutPrefix.startsWith(searchString)) {
    //         mCurrentSuggestions.add(entry);
    //         it.remove();
    //     }
    // }
    //
    // // Second pass: contains matches
    // it = possibleSuggestions.iterator();
    // while(it.hasNext()) {
    //     AutocompleteEntry entry = it.next();
    //     if(entry.nameWithoutPrefix.contains(searchString)) {
    //         mCurrentSuggestions.add(entry);
    //         it.remove();
    //     }
    // }
    //
    // mCurrentSuggestions.addAll(possibleSuggestions);
    // notifyDataSetChanged();
    // scrollToTop();
}

void* PostSubmitSubredditSelectionFragment::AutocompleteAdapter::onCreateViewHolder(
        ViewGroup& /* viewGroup */,
        int /* viewType */) {
    // View view = LayoutInflater.from(viewGroup.getContext())
    //     .inflate(R.layout.list_item_1_text, viewGroup, false);
    // VH1Text result = new VH1Text(view);
    // view.setOnClickListener(v -> mSubredditBox.setText(result.text.getText()));
    // return result;
    return nullptr;
}

void PostSubmitSubredditSelectionFragment::AutocompleteAdapter::onBindViewHolder(
        void* /* viewHolder */,
        int /* position */) {
    // viewHolder.text.setText(mCurrentSuggestions.get(position).nameWithoutPrefix);
}

int PostSubmitSubredditSelectionFragment::AutocompleteAdapter::getItemCount() const {
    return static_cast<int>(mCurrentSuggestions.size());
}

int64_t PostSubmitSubredditSelectionFragment::AutocompleteAdapter::getItemId(
        int position) const {
    return mCurrentSuggestions[position].listId;
}

// --- PostSubmitSubredditSelectionFragment ---

void PostSubmitSubredditSelectionFragment::onResume() {
    // super.onResume();
    // final FragmentActivity activity = getActivity();
    // if(activity != null) {
    //     activity.setTitle(R.string.subreddit_selector_title);
    // }
}

std::unique_ptr<View> PostSubmitSubredditSelectionFragment::onCreateView(
        LayoutInflater& /* inflater */,
        ViewGroup* /* container */,
        const Bundle* /* savedInstanceState */) {
    // final Args args = Args.fromBundle(requireArguments());
    // final Context context = Objects.requireNonNull(container).getContext();
    // final View root = inflater.inflate(R.layout.subreddit_selection, container, false);
    //
    // mUsernameSpinner = root.findViewById(R.id.subreddit_selection_account);
    // mSubredditBox = root.findViewById(R.id.subreddit_selection_textbox);
    // mAutocompleteSuggestions = root.findViewById(R.id.subreddit_selection_autocomplete);
    // mAutocompleteSuggestionsLayout = new LinearLayoutManager(context, RecyclerView.VERTICAL, false);
    // mAutocompleteSuggestions.setLayoutManager(mAutocompleteSuggestionsLayout);
    //
    // final AutocompleteAdapter adapter = new AutocompleteAdapter(context);
    // mAutocompleteSuggestions.setAdapter(adapter);
    //
    // AndroidCommon.onTextChanged(mSubredditBox, adapter::updateSuggestions);
    // AndroidCommon.onTextChanged(mUsernameSpinner, adapter::updateSuggestions);
    //
    // final RedditAccountManager accountManager = RedditAccountManager.getInstance(context);
    // final ArrayList<String> usernames = new ArrayList<>();
    // Stream.from(accountManager.getAccounts())
    //     .filter(RedditAccount::isNotAnonymous)
    //     .forEach(account -> usernames.add(account.username));
    //
    // if(usernames.isEmpty()) {
    //     final FragmentActivity activity = getActivity();
    //     if(activity != null) {
    //         ((Listener)activity).onNotLoggedIn();
    //     }
    //     return null;
    // }
    //
    // AndroidCommon.setAutoCompleteTextViewItemsNoFilter(mUsernameSpinner, usernames);
    // mUsernameSpinner.setText(accountManager.getDefaultAccount().username);
    //
    // {
    //     final Button continueButton = root.findViewById(R.id.subreddit_selection_button_continue);
    //     continueButton.setOnClickListener(v -> {
    //         final FragmentActivity activity = getActivity();
    //         if(activity == null) { return; }
    //         final SubredditCanonicalId subreddit;
    //         try {
    //             subreddit = new SubredditCanonicalId(mSubredditBox.getText().toString());
    //         } catch(final InvalidSubredditNameException e) {
    //             final Context applicationContext = activity.getApplicationContext();
    //             General.showResultDialog((AppCompatActivity)activity, new RRError(
    //                 applicationContext.getString(R.string.invalid_subreddit_name),
    //                 applicationContext.getString(R.string.invalid_subreddit_name_message),
    //                 false, e));
    //             return;
    //         }
    //         ((Listener)activity).onSubredditSelected(
    //             mUsernameSpinner.getText().toString(), subreddit);
    //     });
    // }
    //
    // if(args.subreddit != null) {
    //     mSubredditBox.setText(args.subreddit.getDisplayNameLowercase());
    //     adapter.updateSuggestions();
    // }
    //
    // return root;
    return nullptr;
}

void PostSubmitSubredditSelectionFragment::scrollToTop() {
    // mAutocompleteSuggestionsLayout.smoothScrollToPosition(mAutocompleteSuggestions, null, 0);
}

} // namespace PinkReader
