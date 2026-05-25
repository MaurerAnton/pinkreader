// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/MoreCommentsListingActivity.java

#include "more_comments_listing_activity.h"
#include <stdexcept>

namespace PinkReader {

MoreCommentsListingActivity::MoreCommentsListingActivity(QObject* parent)
	: RefreshableActivity(parent)
{
	mUrls.reserve(32);
}

// Port of: @Override protected boolean baseActivityAllowToolbarHideOnScroll()
bool MoreCommentsListingActivity::baseActivityAllowToolbarHideOnScroll() {
	return true;
}

// Port of: @Override public void onCreate(final Bundle savedInstanceState)
void MoreCommentsListingActivity::onCreate(Bundle* savedInstanceState) {
	// Port of: PrefsUtility.applyTheme(this);
	// Port of: super.onCreate(savedInstanceState);
	RefreshableActivity::onCreate(savedInstanceState);

	// Port of: setTitle(R.string.app_name);
	setTitle("More Comments");

	// Port of: final View layout = getLayoutInflater().inflate(R.layout.main_single, null);
	// setBaseActivityListing(layout);
	// RedditAccountManager.getInstance(this).addUpdateListener(this);
	//
	// if(getIntent() != null) {
	//   final Intent intent = getIntent();
	//   mSearchString = intent.getStringExtra(EXTRA_SEARCH_STRING);
	//   final ArrayList<String> commentIds = intent.getStringArrayListExtra("commentIds");
	//   final String postId = intent.getStringExtra("postId");
	//   for(final String commentId : commentIds) {
	//     mUrls.add(PostCommentListingURL.forPostId(postId).commentId(commentId));
	//   }
	//   doRefresh(RefreshableFragment.COMMENTS, false, null);
	// } else {
	//   throw new RuntimeException("Nothing to show! (should load from bundle)");
	// }
	doRefresh(RefreshableFragment::COMMENTS, false, nullptr);
}

bool MoreCommentsListingActivity::onCreateOptionsMenu(Menu* menu) {
	// Port of: OptionsMenuUtility.prepare(this, menu, false, false, true, ...);
	// if(mFragment != null) { mFragment.onCreateOptionsMenu(menu); }
	return true;
}

void MoreCommentsListingActivity::onRedditAccountChanged() {
	// Port of: requestRefresh(RefreshableFragment.ALL, false);
	requestRefresh(RefreshableFragment::ALL, false);
}

void MoreCommentsListingActivity::doRefresh(RefreshableFragment which, bool force, Bundle* savedInstanceState) {
	// Port of: mFragment = new CommentListingFragment(this, savedInstanceState, mUrls,
	//   null, mSearchString, force);
	// mFragment.setBaseActivityContent(this);
	setTitle("More Comments");
}

void MoreCommentsListingActivity::onRefreshComments() {
	requestRefresh(RefreshableFragment::COMMENTS, true);
}

void MoreCommentsListingActivity::onPastComments() {
	// No-op
}

void MoreCommentsListingActivity::onSortSelected(const PostCommentSort* order) {
	// No-op
}

void MoreCommentsListingActivity::onSortSelected(const UserCommentSort* order) {
	// No-op
}

void MoreCommentsListingActivity::onSearchComments() {
	// Port of: DialogUtils.showSearchDialog(this, query -> {
	//   final Intent searchIntent = getIntent();
	//   searchIntent.putExtra(EXTRA_SEARCH_STRING, query);
	//   startActivity(searchIntent);
	// });
}

bool MoreCommentsListingActivity::onOptionsItemSelected(MenuItem* item) {
	// Port of: if(mFragment != null && mFragment.onOptionsItemSelected(item)) {
	//   return true; }
	// return super.onOptionsItemSelected(item);
	return false;
}

void MoreCommentsListingActivity::onPostSelected(RedditPreparedPost* post) {
	// Port of: LinkHandler.onLinkClicked(this, post.src.getUrl(), false, post.src.getSrc());
}

void MoreCommentsListingActivity::onPostCommentsSelected(RedditPreparedPost* post) {
	// Port of: LinkHandler.onLinkClicked(this,
	//   PostCommentListingURL.forPostId(post.src.getIdAlone()).toUriString(), false);
}

void MoreCommentsListingActivity::onBackPressed() {
	// Port of: if(General.onBackPressed()) { super.onBackPressed(); }
}

int MoreCommentsListingActivity::getCommentSort() {
	return -1; // null equivalent
}

const PostCommentSort* MoreCommentsListingActivity::getSuggestedCommentSort() {
	return nullptr;
}

} // namespace PinkReader
