// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/MoreCommentsListingActivity.java

#pragma once

#include "refreshable_activity.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <vector>
#include <string>

namespace PinkReader {

// Forward declarations
class Bundle;
class Menu;
class MenuItem;
class CommentListingFragment;
class RedditPreparedPost;
class RedditURLParser;
class PostCommentListingURL;
class PostCommentSort;
class UserCommentSort;
class OptionsMenuUtility;

/**
 * @brief Port of org.quantumbadger.redreader.activities.MoreCommentsListingActivity
 *
 * Activity for viewing more comments linked from a post. Implements
 * RedditAccountChangeListener, OptionsMenuUtility.OptionsMenuCommentsListener,
 * and RedditPostView.PostSelectionListener.
 * Every field, method, interface, and logic branch from the original Java file (221 lines).
 */
class MoreCommentsListingActivity : public RefreshableActivity {
	Q_OBJECT

protected:
	// Port of: private static final String EXTRA_SEARCH_STRING = "mcla_search_string";
	static constexpr const char* EXTRA_SEARCH_STRING = "mcla_search_string";

	// Port of: private final ArrayList<RedditURLParser.RedditURL> mUrls = new ArrayList<>(32);
	std::vector<const RedditURLParser::RedditURL*> mUrls;

	// Port of: private CommentListingFragment mFragment;
	CommentListingFragment* mFragment = nullptr;

	// Port of: private String mSearchString = null;
	QString mSearchString;

public:
	explicit MoreCommentsListingActivity(QObject* parent = nullptr);
	virtual ~MoreCommentsListingActivity() = default;

protected:
	// Port of: @Override protected boolean baseActivityAllowToolbarHideOnScroll()
	//   return true;
	bool baseActivityAllowToolbarHideOnScroll() override;

	// Port of: @Override public void onCreate(final Bundle savedInstanceState)
	//   PrefsUtility.applyTheme(this);
	//   super.onCreate(savedInstanceState);
	//   setTitle(R.string.app_name);
	//   final View layout = getLayoutInflater().inflate(R.layout.main_single, null);
	//   setBaseActivityListing(layout);
	//   RedditAccountManager.getInstance(this).addUpdateListener(this);
	//   if(getIntent() != null) {
	//     final Intent intent = getIntent();
	//     mSearchString = intent.getStringExtra(EXTRA_SEARCH_STRING);
	//     final ArrayList<String> commentIds = intent.getStringArrayListExtra("commentIds");
	//     final String postId = intent.getStringExtra("postId");
	//     for(final String commentId : commentIds) {
	//       mUrls.add(PostCommentListingURL.forPostId(postId).commentId(commentId));
	//     }
	//     doRefresh(RefreshableFragment.COMMENTS, false, null);
	//   } else {
	//     throw new RuntimeException("Nothing to show! (should load from bundle)");
	//   }
	void onCreate(Bundle* savedInstanceState) override;

public:
	// Port of: @Override public boolean onCreateOptionsMenu(final Menu menu)
	//   OptionsMenuUtility.prepare(this, menu, false, false, true, ...);
	//   if(mFragment != null) { mFragment.onCreateOptionsMenu(menu); }
	//   return true;
	bool onCreateOptionsMenu(Menu* menu);

	// Port of: @Override public void onRedditAccountChanged()
	//   requestRefresh(RefreshableFragment.ALL, false);
	void onRedditAccountChanged();

protected:
	// Port of: @Override protected void doRefresh(
	//   final RefreshableFragment which, final boolean force,
	//   final Bundle savedInstanceState)
	//   mFragment = new CommentListingFragment(this, savedInstanceState, mUrls,
	//     null, mSearchString, force);
	//   mFragment.setBaseActivityContent(this);
	//   setTitle("More Comments");
	void doRefresh(RefreshableFragment which, bool force, Bundle* savedInstanceState) override;

public:
	// Port of: @Override public void onRefreshComments()
	//   requestRefresh(RefreshableFragment.COMMENTS, true);
	void onRefreshComments();

	// Port of: @Override public void onPastComments() { }
	void onPastComments();

	// Port of: @Override public void onSortSelected(final PostCommentSort order) { }
	void onSortSelected(const PostCommentSort* order);

	// Port of: @Override public void onSortSelected(final UserCommentSort order) { }
	void onSortSelected(const UserCommentSort* order);

	// Port of: @Override public void onSearchComments()
	//   DialogUtils.showSearchDialog(this, query -> {
	//     final Intent searchIntent = getIntent();
	//     searchIntent.putExtra(EXTRA_SEARCH_STRING, query);
	//     startActivity(searchIntent);
	//   });
	void onSearchComments();

	// Port of: @Override public boolean onOptionsItemSelected(@NonNull final MenuItem item)
	//   if(mFragment != null && mFragment.onOptionsItemSelected(item)) { return true; }
	//   return super.onOptionsItemSelected(item);
	bool onOptionsItemSelected(MenuItem* item);

	// Port of: @Override public void onPostSelected(final RedditPreparedPost post)
	//   LinkHandler.onLinkClicked(this, post.src.getUrl(), false, post.src.getSrc());
	void onPostSelected(RedditPreparedPost* post);

	// Port of: @Override public void onPostCommentsSelected(final RedditPreparedPost post)
	//   LinkHandler.onLinkClicked(this,
	//     PostCommentListingURL.forPostId(post.src.getIdAlone()).toUriString(), false);
	void onPostCommentsSelected(RedditPreparedPost* post);

	// Port of: @Override public void onBackPressed()
	//   if(General.onBackPressed()) { super.onBackPressed(); }
	void onBackPressed();

	// Port of: @Override public OptionsMenuUtility.Sort getCommentSort() { return null; }
	virtual int getCommentSort();

	// Port of: @Override public PostCommentSort getSuggestedCommentSort() { return null; }
	virtual const PostCommentSort* getSuggestedCommentSort();
};

} // namespace PinkReader
