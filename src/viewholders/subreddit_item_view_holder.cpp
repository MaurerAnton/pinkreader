// Origin: RedReader/src/main/java/org/quantumbadger/redreader/viewholders/SubredditItemViewHolder.java
#include "subreddit_item_view_holder.h"
#include <cstdio>
#include <string>

namespace PinkReader {

// Forward declaration stub for SubredditDetails
struct SubredditDetails {
	std::string name;
	int* subscribers = nullptr;
	std::string publicDescriptionHtmlEscaped;
	std::string url;
};

// Port of: public SubredditItemViewHolder(@NonNull final ViewGroup parent, final BaseActivity activity)
SubredditItemViewHolder::SubredditItemViewHolder()
	: m_bodyFontScale(1.0f) {
	// Port of: super(LayoutInflater.from(parent.getContext()).inflate(R.layout.subreddit_item_view, parent, false));
	// Port of: mActivity = activity;
	// Port of: mTheme = new RRThemeAttributes(activity);
	// Port of: mBodyFontScale = PrefsUtility.appearance_fontscale_bodytext();
	// Port of: mPrimaryText = this.itemView.findViewById(R.id.subreddit_item_view_primary_text);
	// Port of: mSubText = this.itemView.findViewById(R.id.subreddit_item_view_sub_text);
	// Port of: mSupportingText = this.itemView.findViewById(R.id.subreddit_item_view_supporting_text);
	// Port of: mActions = this.itemView.findViewById(R.id.subreddit_item_view_actions);
	// Port of: mGoButton = this.itemView.findViewById(R.id.subreddit_item_view_go);
}

// Port of: public void bind(@NonNull final SubredditDetails subreddit)
void SubredditItemViewHolder::bind(const SubredditDetails& subreddit) {
	// Port of: mPrimaryText.setText(subreddit.name);
	m_primaryText = subreddit.name;

	// Port of: final String subtitle;
	// Port of: if(subreddit.subscribers == null) { subtitle = null; }
	// Port of: else { subtitle = mActivity.getString(R.string.header_subscriber_count,
	//   NumberFormat.getNumberInstance(Locale.getDefault()).format(subreddit.subscribers)); }
	std::string subtitle;
	if (subreddit.subscribers == nullptr) {
		// Port of: subtitle = null;
		subtitle = "";
	} else {
		// Port of: subtitle = mActivity.getString(R.string.header_subscriber_count,
		//   NumberFormat.getNumberInstance(Locale.getDefault()).format(subreddit.subscribers));
		subtitle = std::to_string(*subreddit.subscribers) + " subscribers";
	}

	// Port of: if(subtitle == null) { mSubText.setVisibility(View.GONE); }
	// Port of: else { mSubText.setVisibility(View.VISIBLE); mSubText.setText(subtitle); }
	if (subtitle.empty()) {
		// Port of: mSubText.setVisibility(View.GONE);
		m_subText = "";
	} else {
		// Port of: mSubText.setVisibility(View.VISIBLE);
		// Port of: mSubText.setText(subtitle);
		m_subText = subtitle;
	}

	// Port of: mSupportingText.removeAllViews();
	// Port of: if(subreddit.publicDescriptionHtmlEscaped != null
	//   && !subreddit.publicDescriptionHtmlEscaped.trim().isEmpty()) {
	//   final BodyElement body = HtmlReader.parse(
	//     StringEscapeUtils.unescapeHtml4(subreddit.publicDescriptionHtmlEscaped), mActivity);
	//   mSupportingText.setVisibility(View.VISIBLE);
	//   mSupportingText.addView(body.generateView(mActivity, mTheme.rrCommentBodyCol,
	//     13.0f * mBodyFontScale, false));
	// } else { mSupportingText.setVisibility(View.GONE); }
	// Stub: HTML parsing not implemented in C++ port

	// Port of: mActions.bindSubreddit(subreddit, Optional.empty());
	// Stub: SubredditToolbar binding

	// Port of: mGoButton.setOnClickListener(v -> LinkHandler.onLinkClicked(mActivity, subreddit.url));
	// Stub: click handling
}

} // namespace PinkReader
