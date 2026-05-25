// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/LinkDispatchActivity.java

#pragma once

#include "base_activity.h"
#include <QObject>
#include <QString>

namespace PinkReader {

// Forward declarations
class Bundle;
class UriString;

/**
 * @brief Port of org.quantumbadger.redreader.activities.LinkDispatchActivity
 *
 * Entry activity for handling dispatched links (OAuth callbacks, shared links).
 * Extends AppCompatActivity directly (not ViewsBaseActivity).
 * Every field, method, and logic branch from the original Java file (77 lines).
 */
class LinkDispatchActivity : public BaseActivity {
	Q_OBJECT

public:
	// Port of: private static final String TAG = "LinkDispatchActivity";
	static constexpr const char* TAG = "LinkDispatchActivity";

	explicit LinkDispatchActivity(QObject* parent = nullptr);
	virtual ~LinkDispatchActivity() = default;

protected:
	// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
	//   super.onCreate(savedInstanceState);
	//   final View backgroundView = new View(this);
	//   backgroundView.setBackground(new GradientDrawable(
	//     GradientDrawable.Orientation.LEFT_RIGHT,
	//     new int[] {0xffd32f2f, 0xffb52626}));
	//   setContentView(backgroundView);
	//   General.setLayoutMatchParent(backgroundView);
	//   final Intent intent = getIntent();
	//   if(intent == null) { Log.e(TAG, "Got null intent"); finish(); return; }
	//   final Uri data = intent.getData();
	//   if(data == null) { Log.e(TAG, "Got null intent data"); finish(); return; }
	//   if(data.getScheme().equalsIgnoreCase("redreader")) {
	//     RedditOAuth.completeLogin(this, data, new RunnableOnce(this::finish));
	//   } else {
	//     LinkHandler.onLinkClicked(this, UriString.from(data), true, null, null, 0, true);
	//     finish();
	//   }
	void onCreate(Bundle* savedInstanceState) override;
};

} // namespace PinkReader
