// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/CommentEditActivity.java

#pragma once

#include "views_base_activity.h"
#include <QObject>
#include <QString>

namespace PinkReader {

// Forward declarations
class Bundle;
class Menu;
class MenuItem;
class RedditIdAndType;

/**
 * @brief Port of org.quantumbadger.redreader.activities.CommentEditActivity
 *
 * Activity for editing comments and self-posts. Supports save and preview.
 * Every field, method, and logic branch from the original Java file (195 lines).
 */
class CommentEditActivity : public ViewsBaseActivity {
	Q_OBJECT

protected:
	// Port of: private EditText textEdit;
	// In C++ port: text editor managed by UI framework
	QString m_textEditContent;

	// Port of: private RedditIdAndType commentIdAndType = null;
	RedditIdAndType* m_commentIdAndType = nullptr;

	// Port of: private boolean isSelfPost = false;
	bool m_isSelfPost = false;

public:
	explicit CommentEditActivity(QObject* parent = nullptr);
	virtual ~CommentEditActivity() = default;

protected:
	// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
	//   PrefsUtility.applyTheme(this);
	//   super.onCreate(savedInstanceState);
	//   if(getIntent() != null && getIntent().hasExtra("isSelfPost") ...) {
	//     setTitle(R.string.edit_post_actionbar); isSelfPost = true;
	//   } else { setTitle(R.string.edit_comment_actionbar); }
	//   textEdit = (EditText)getLayoutInflater().inflate(R.layout.comment_edit, null);
	//   if(getIntent() != null && getIntent().hasExtra("commentIdAndType")) {
	//     commentIdAndType = IntentCompat.getParcelableExtra(getIntent(), ...);
	//     textEdit.setText(getIntent().getStringExtra("commentText"));
	//   } else if(savedInstanceState != null && savedInstanceState.containsKey("commentIdAndType")) {
	//     textEdit.setText(savedInstanceState.getString("commentText"));
	//     commentIdAndType = BundleCompat.getParcelable(savedInstanceState, ...);
	//   }
	//   final ScrollView sv = new ScrollView(this);
	//   sv.addView(textEdit);
	//   setBaseActivityListing(sv);
	void onCreate(Bundle* savedInstanceState) override;

	// Port of: @Override protected void onSaveInstanceState(@NonNull final Bundle outState)
	//   super.onSaveInstanceState(outState);
	//   outState.putString("commentText", textEdit.getText().toString());
	//   outState.putParcelable("commentIdAndType", commentIdAndType);
	void onSaveInstanceState(Bundle* outState);

public:
	// Port of: @Override public boolean onCreateOptionsMenu(final Menu menu)
	//   final MenuItem send = menu.add(R.string.comment_edit_save);
	//   send.setIcon(R.drawable.ic_action_save_dark);
	//   send.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);
	//   menu.add(R.string.comment_reply_preview);
	//   return true;
	bool onCreateOptionsMenu(Menu* menu);

	// Port of: @Override public boolean onOptionsItemSelected(final MenuItem item)
	//   if(item.getTitle().equals(getString(R.string.comment_edit_save))) {
	//     final ProgressDialog progressDialog = new ProgressDialog(this);
	//     progressDialog.setTitle(getString(R.string.comment_reply_submitting_title));
	//     progressDialog.setMessage(getString(R.string.comment_reply_submitting_message));
	//     progressDialog.setIndeterminate(true);
	//     progressDialog.setCancelable(true);
	//     progressDialog.setCanceledOnTouchOutside(false);
	//     progressDialog.setOnCancelListener(dialogInterface -> { ... });
	//     progressDialog.setOnKeyListener((dialogInterface, keyCode, keyEvent) -> { ... });
	//     final APIResponseHandler.ActionResponseHandler handler = ...;
	//     final CacheManager cm = CacheManager.getInstance(this);
	//     final RedditAccount selectedAccount = RedditAccountManager.getInstance(this).getDefaultAccount();
	//     RedditAPI.editComment(cm, handler, selectedAccount, commentIdAndType,
	//       textEdit.getText().toString(), this);
	//     progressDialog.show();
	//   } else if(item.getTitle().equals(getString(R.string.comment_reply_preview))) {
	//     MarkdownPreviewDialog.newInstance(textEdit.getText().toString())
	//       .show(getSupportFragmentManager(), "MarkdownPreviewDialog");
	//   }
	//   return true;
	bool onOptionsItemSelected(MenuItem* item);

	// Port of: @Override public void onBackPressed()
	//   if(General.onBackPressed()) { super.onBackPressed(); }
	void onBackPressed();
};

} // namespace PinkReader
