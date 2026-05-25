// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/BugReportActivity.java

#pragma once

#include "views_base_activity.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QStringBuilder>

namespace PinkReader {

// Forward declarations
class RRError;
class Bundle;

/**
 * @brief Port of org.quantumbadger.redreader.activities.BugReportActivity
 *
 * Activity for viewing and sending bug reports. Collects global errors,
 * formats them for email, and launches email intent. Every field, method,
 * and logic branch from the original Java file (223 lines).
 */
class BugReportActivity : public ViewsBaseActivity {
	Q_OBJECT

public:
	// Port of: private static final ArrayList<RRError> errors = new ArrayList<>();
	// Static error list shared across the app
	static QVector<RRError*> s_errors;

	// Port of: public static synchronized void addGlobalError(final RRError error)
	//   errors.add(error);
	static void addGlobalError(RRError* error);

	// Port of: public static synchronized void handleGlobalError(
	//   final Context context, final String text)
	//   handleGlobalError(context, new RRError(text, null, true, new RuntimeException()));
	static void handleGlobalError(const QString& text);

	// Port of: public static synchronized void handleGlobalError(
	//   final Context context, final Throwable t)
	//   if(t != null) { Log.e("BugReportActivity", "Handling exception", t); }
	//   handleGlobalError(context, new RRError(null, null, true, t));
	static void handleGlobalErrorFromThrowable(void* throwable);

	// Port of: public static synchronized void handleGlobalError(
	//   final Context context, final RRError error)
	//   addGlobalError(error);
	//   AndroidCommon.UI_THREAD_HANDLER.post(() -> {
	//     final Intent intent = new Intent(context, BugReportActivity.class);
	//     intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	//     context.startActivity(intent);
	//   });
	static void handleGlobalError(RRError* error);

	// Port of: private static synchronized LinkedList<RRError> getErrors()
	//   final LinkedList<RRError> result = new LinkedList<>(errors);
	//   errors.clear();
	//   return result;
	static QVector<RRError*> getErrors();

	// Port of: public static void sendBugReport(
	//   @NonNull final Context context, @NonNull final RRError error)
	//   sendBugReport(context, General.listOfOne(error));
	static void sendBugReport(RRError* error);

	// Port of: public static void sendBugReport(
	//   @NonNull final Context context, @NonNull final Iterable<RRError> errors)
	//   final StringBuilder sb = new StringBuilder(1024);
	//   sb.append("Error report -- RedReader v")
	//     .append(Constants.version(context)).append("\r\n\r\n");
	//   sb.append("Manufacturer: ").append(Build.MANUFACTURER).append("\r\n");
	//   sb.append("Model: ").append(Build.MODEL).append("\r\n");
	//   sb.append("Product: ").append(Build.PRODUCT).append("\r\n");
	//   sb.append("Android release: ").append(Build.VERSION.RELEASE).append("\r\n");
	//   sb.append("Android SDK: ").append(Build.VERSION.SDK_INT).append("\r\n");
	//   for(final RRError error : errors) { ... append error details ... }
	//   final Intent intent = new Intent(Intent.ACTION_SENDTO);
	//   intent.putExtra(Intent.EXTRA_EMAIL, new String[]{"bugreports@redreader.org"});
	//   intent.putExtra(Intent.EXTRA_SUBJECT, "Bug Report");
	//   intent.putExtra(Intent.EXTRA_TEXT, sb.toString());
	//   ...
	static void sendBugReport(const QVector<RRError*>& errors);

	// Port of: public static void appendException(
	//   final StringBuilder sb, final Throwable t, final int recurseLimit)
	//   if(t != null) {
	//     sb.append("Exception: ");
	//     sb.append(t.getClass().getCanonicalName()).append("\r\n");
	//     sb.append(t.getMessage()).append("\r\n");
	//     for(final StackTraceElement elem : t.getStackTrace()) {
	//       sb.append("  ").append(elem.toString()).append("\r\n");
	//     }
	//     if(recurseLimit > 0 && t.getCause() != null) {
	//       sb.append("Caused by: ");
	//       appendException(sb, t.getCause(), recurseLimit - 1);
	//     }
	//   }
	static void appendException(QString& sb, void* throwable, int recurseLimit);

	explicit BugReportActivity(QObject* parent = nullptr);
	virtual ~BugReportActivity() = default;

protected:
	// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
	//   super.onCreate(savedInstanceState);
	//   final LinearLayout layout = new LinearLayout(this);
	//   layout.setOrientation(LinearLayout.VERTICAL);
	//   final TextView title = new TextView(this);
	//   title.setText(R.string.bug_title);
	//   layout.addView(title); title.setTextSize(20.0f);
	//   final TextView text = new TextView(this);
	//   text.setText(R.string.bug_message);
	//   layout.addView(text); text.setTextSize(15.0f);
	//   final int paddingPx = General.dpToPixels(this, 20);
	//   title.setPadding(paddingPx, ...);
	//   final Button send = new Button(this);
	//   send.setText(R.string.bug_button_send);
	//   send.setOnClickListener(v -> { sendBugReport(this, getErrors()); finish(); });
	//   final Button ignore = new Button(this);
	//   ignore.setText(R.string.bug_button_ignore);
	//   ignore.setOnClickListener(v -> finish());
	//   layout.addView(send); layout.addView(ignore);
	//   final ScrollView sv = new ScrollView(this);
	//   sv.addView(layout);
	//   setBaseActivityListing(sv);
	void onCreate(Bundle* savedInstanceState) override;
};

} // namespace PinkReader
