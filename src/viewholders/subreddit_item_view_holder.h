// Origin: RedReader/src/main/java/org/quantumbadger/redreader/viewholders/SubredditItemViewHolder.java
#pragma once

#include <string>

namespace PinkReader {

// Forward declarations
class SubredditDetails;
class UriString;

/**
 * @brief Port of org.quantumbadger.redreader.viewholders.SubredditItemViewHolder
 *
 * ViewHolder for subreddit list items. Original: extends RecyclerView.ViewHolder.
 * Every field and method from the original Java file.
 */
class SubredditItemViewHolder {
public:
	// Port of: public SubredditItemViewHolder(@NonNull final ViewGroup parent, final BaseActivity activity)
	//   super(LayoutInflater.from(parent.getContext()).inflate(R.layout.subreddit_item_view, parent, false));
	//   mActivity = activity;
	//   mTheme = new RRThemeAttributes(activity);
	//   mBodyFontScale = PrefsUtility.appearance_fontscale_bodytext();
	//   mPrimaryText = this.itemView.findViewById(R.id.subreddit_item_view_primary_text);
	//   mSubText = this.itemView.findViewById(R.id.subreddit_item_view_sub_text);
	//   mSupportingText = this.itemView.findViewById(R.id.subreddit_item_view_supporting_text);
	//   mActions = this.itemView.findViewById(R.id.subreddit_item_view_actions);
	//   mGoButton = this.itemView.findViewById(R.id.subreddit_item_view_go);
	SubredditItemViewHolder();

	// Port of: public void bind(@NonNull final SubredditDetails subreddit)
	//   mPrimaryText.setText(subreddit.name);
	//   final String subtitle; if(subreddit.subscribers == null) { subtitle = null; }
	//   else { subtitle = ... NumberFormat.getNumberInstance... }
	//   if(subtitle == null) { mSubText.setVisibility(View.GONE); } else { ... }
	//   mSupportingText.removeAllViews();
	//   if(subreddit.publicDescriptionHtmlEscaped != null && !...trim().isEmpty()) { ... }
	//   mActions.bindSubreddit(subreddit, Optional.empty());
	//   mGoButton.setOnClickListener(v -> LinkHandler.onLinkClicked(mActivity, subreddit.url));
	void bind(const SubredditDetails& subreddit);

private:
	// Port of: private final BaseActivity mActivity;
	// Port of: private final RRThemeAttributes mTheme;
	// Port of: private final float mBodyFontScale;
	float m_bodyFontScale;

	// Port of: private final TextView mPrimaryText;
	std::string m_primaryText;
	// Port of: private final TextView mSubText;
	std::string m_subText;
	// Port of: private final FrameLayout mSupportingText;
	// Port of: private final SubredditToolbar mActions;
	// Port of: private final View mGoButton;
};

} // namespace PinkReader
