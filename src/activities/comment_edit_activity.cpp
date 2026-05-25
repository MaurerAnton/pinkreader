// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/CommentEditActivity.java

#include "comment_edit_activity.h"

namespace PinkReader {

CommentEditActivity::CommentEditActivity(QObject* parent)
	: ViewsBaseActivity(parent)
	, m_isSelfPost(false)
{
}

void CommentEditActivity::onCreate(Bundle* savedInstanceState) {
	// Port of: PrefsUtility.applyTheme(this);
	ViewsBaseActivity::onCreate(savedInstanceState);

	// Port of: if(getIntent() != null && getIntent().hasExtra("isSelfPost") ...)
	//   setTitle(R.string.edit_post_actionbar); isSelfPost = true;
	// } else { setTitle(R.string.edit_comment_actionbar); }
	if (m_isSelfPost) {
		setTitle("Edit Post");
	} else {
		setTitle("Edit Comment");
	}

	// Port of: textEdit = (EditText)getLayoutInflater().inflate(R.layout.comment_edit, null);
	// Intent extras and savedInstanceState handling
	// final ScrollView sv = new ScrollView(this);
	// sv.addView(textEdit);
	// setBaseActivityListing(sv);
}

void CommentEditActivity::onSaveInstanceState(Bundle* outState) {
	// Port of: super.onSaveInstanceState(outState);
	// outState.putString("commentText", textEdit.getText().toString());
	// outState.putParcelable("commentIdAndType", commentIdAndType);
}

bool CommentEditActivity::onCreateOptionsMenu(Menu* menu) {
	// Port of: final MenuItem send = menu.add(R.string.comment_edit_save);
	// send.setIcon(R.drawable.ic_action_save_dark);
	// send.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);
	// menu.add(R.string.comment_reply_preview);
	return true;
}

bool CommentEditActivity::onOptionsItemSelected(MenuItem* item) {
	// Port of: if(item.getTitle().equals(getString(R.string.comment_edit_save))) {
	//   ... ProgressDialog, API call ...
	//   RedditAPI.editComment(cm, handler, selectedAccount, commentIdAndType,
	//     textEdit.getText().toString(), this);
	//   progressDialog.show();
	// } else if(item.getTitle().equals(getString(R.string.comment_reply_preview))) {
	//   MarkdownPreviewDialog.newInstance(...).show(...);
	// }
	return true;
}

void CommentEditActivity::onBackPressed() {
	// Port of: if(General.onBackPressed()) { super.onBackPressed(); }
}

} // namespace PinkReader
