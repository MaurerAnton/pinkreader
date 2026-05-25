// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/HtmlViewActivity.java

#pragma once

#include "views_base_activity.h"
#include <QObject>
#include <QString>

namespace PinkReader {

// Forward declarations
class Bundle;
class WebViewFragment;

/**
 * @brief Port of org.quantumbadger.redreader.activities.HtmlViewActivity
 *
 * Displays HTML content from assets or intent extras in a WebView.
 * Every field, method, and logic branch from the original Java file (100 lines).
 */
class HtmlViewActivity : public ViewsBaseActivity {
	Q_OBJECT

protected:
	// Port of: private WebViewFragment webView;
	WebViewFragment* m_webView = nullptr;

public:
	explicit HtmlViewActivity(QObject* parent = nullptr);
	virtual ~HtmlViewActivity() = default;

	// Port of: public static void showAsset(final Context context, final String filename)
	//   final String html;
	//   try(InputStream asset = context.getAssets().open(filename)) {
	//     final ByteArrayOutputStream baos = new ByteArrayOutputStream(16_384);
	//     final byte[] buf = new byte[8192];
	//     int bytesRead;
	//     while((bytesRead = asset.read(buf)) > 0) { baos.write(buf, 0, bytesRead); }
	//     html = baos.toString("UTF-8");
	//   } catch(final IOException e) {
	//     BugReportActivity.handleGlobalError(context, e);
	//     return;
	//   }
	//   final Intent intent = new Intent(context, HtmlViewActivity.class);
	//   intent.putExtra("html", html);
	//   context.startActivity(intent);
	static void showAsset(const QString& filename);

protected:
	// Port of: @Override public void onCreate(final Bundle savedInstanceState)
	//   PrefsUtility.applyTheme(this);
	//   super.onCreate(savedInstanceState);
	//   final Intent intent = getIntent();
	//   final String html = intent.getStringExtra("html");
	//   final String title = intent.getStringExtra("title");
	//   setTitle(title);
	//   if(html == null) { BugReportActivity.handleGlobalError(this, "No HTML"); }
	//   webView = WebViewFragment.newInstanceHtml(html);
	//   setBaseActivityListing(View.inflate(this, R.layout.main_single, null));
	//   getSupportFragmentManager().beginTransaction()
	//     .add(R.id.main_single_frame, webView).commit();
	void onCreate(Bundle* savedInstanceState) override;

	// Port of: @Override public void onBackPressed()
	//   if(General.onBackPressed() && !webView.onBackButtonPressed()) {
	//     super.onBackPressed();
	//   }
	void onBackPressed();
};

} // namespace PinkReader
