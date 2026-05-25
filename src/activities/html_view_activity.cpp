// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/HtmlViewActivity.java

#include "html_view_activity.h"

namespace PinkReader {

HtmlViewActivity::HtmlViewActivity(QObject* parent)
	: ViewsBaseActivity(parent)
{
}

// Port of: public static void showAsset(final Context context, final String filename)
void HtmlViewActivity::showAsset(const QString& filename) {
	// Port of: try(InputStream asset = context.getAssets().open(filename)) {
	//   final ByteArrayOutputStream baos = new ByteArrayOutputStream(16_384);
	//   final byte[] buf = new byte[8192];
	//   int bytesRead;
	//   while((bytesRead = asset.read(buf)) > 0) { baos.write(buf, 0, bytesRead); }
	//   html = baos.toString("UTF-8");
	// } catch(final IOException e) {
	//   BugReportActivity.handleGlobalError(context, e);
	//   return;
	// }
	// final Intent intent = new Intent(context, HtmlViewActivity.class);
	// intent.putExtra("html", html);
	// context.startActivity(intent);
}

void HtmlViewActivity::onCreate(Bundle* savedInstanceState) {
	// Port of: PrefsUtility.applyTheme(this);
	ViewsBaseActivity::onCreate(savedInstanceState);

	// Port of: final Intent intent = getIntent();
	// final String html = intent.getStringExtra("html");
	// final String title = intent.getStringExtra("title");
	// setTitle(title);
	// if(html == null) { BugReportActivity.handleGlobalError(this, "No HTML"); }
	// webView = WebViewFragment.newInstanceHtml(html);
	// setBaseActivityListing(View.inflate(this, R.layout.main_single, null));
	// getSupportFragmentManager().beginTransaction()
	//   .add(R.id.main_single_frame, webView).commit();
}

void HtmlViewActivity::onBackPressed() {
	// Port of: if(General.onBackPressed() && !webView.onBackButtonPressed()) {
	//   super.onBackPressed();
	// }
}

} // namespace PinkReader
