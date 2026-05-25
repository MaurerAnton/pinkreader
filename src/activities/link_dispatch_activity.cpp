// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/LinkDispatchActivity.java

#include "link_dispatch_activity.h"

namespace PinkReader {

LinkDispatchActivity::LinkDispatchActivity(QObject* parent)
	: BaseActivity(parent)
{
}

// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
void LinkDispatchActivity::onCreate(Bundle* savedInstanceState) {
	// Port of: super.onCreate(savedInstanceState);
	BaseActivity::onCreate(savedInstanceState);

	// Port of: final View backgroundView = new View(this);
	// backgroundView.setBackground(new GradientDrawable(
	//   GradientDrawable.Orientation.LEFT_RIGHT,
	//   new int[] {0xffd32f2f, 0xffb52626}));
	// setContentView(backgroundView);
	// General.setLayoutMatchParent(backgroundView);

	// Port of: final Intent intent = getIntent();
	// if(intent == null) { Log.e(TAG, "Got null intent"); finish(); return; }
	// final Uri data = intent.getData();
	// if(data == null) { Log.e(TAG, "Got null intent data"); finish(); return; }
	// if(data.getScheme().equalsIgnoreCase("redreader")) {
	//   RedditOAuth.completeLogin(this, data, new RunnableOnce(this::finish));
	// } else {
	//   LinkHandler.onLinkClicked(this, UriString.from(data), true, null, null, 0, true);
	//   finish();
	// }
}

} // namespace PinkReader
