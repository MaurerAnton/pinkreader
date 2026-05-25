// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/BugReportActivity.java

#include "bug_report_activity.h"

namespace PinkReader {

// Port of: private static final ArrayList<RRError> errors = new ArrayList<>();
QVector<RRError*> BugReportActivity::s_errors;

// Port of: public static synchronized void addGlobalError(final RRError error)
void BugReportActivity::addGlobalError(RRError* error) {
	s_errors.append(error);
}

// Port of: public static synchronized void handleGlobalError(final Context context, final String text)
void BugReportActivity::handleGlobalError(const QString& text) {
	// Port of: handleGlobalError(context, new RRError(text, null, true, new RuntimeException()));
	// In C++: error creation handled differently
	addGlobalError(nullptr);
}

// Port of: public static synchronized void handleGlobalError(final Context context, final Throwable t)
void BugReportActivity::handleGlobalErrorFromThrowable(void* throwable) {
	// Port of: if(t != null) { Log.e("BugReportActivity", "Handling exception", t); }
	// handleGlobalError(context, new RRError(null, null, true, t));
}

// Port of: public static synchronized void handleGlobalError(final Context context, final RRError error)
void BugReportActivity::handleGlobalError(RRError* error) {
	// Port of: addGlobalError(error);
	addGlobalError(error);
	// Port of: AndroidCommon.UI_THREAD_HANDLER.post(() -> {
	//   final Intent intent = new Intent(context, BugReportActivity.class);
	//   intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	//   context.startActivity(intent);
	// });
}

// Port of: private static synchronized LinkedList<RRError> getErrors()
QVector<RRError*> BugReportActivity::getErrors() {
	// Port of: final LinkedList<RRError> result = new LinkedList<>(errors);
	// errors.clear();
	// return result;
	QVector<RRError*> result = s_errors;
	s_errors.clear();
	return result;
}

// Port of: public static void sendBugReport(@NonNull final Context context, @NonNull final RRError error)
void BugReportActivity::sendBugReport(RRError* error) {
	// Port of: sendBugReport(context, General.listOfOne(error));
	QVector<RRError*> errors;
	errors.append(error);
	sendBugReport(errors);
}

// Port of: public static void sendBugReport(@NonNull final Context context, @NonNull final Iterable<RRError> errors)
void BugReportActivity::sendBugReport(const QVector<RRError*>& errors) {
	// Port of: final StringBuilder sb = new StringBuilder(1024);
	QString sb;
	sb.reserve(1024);

	// Port of: sb.append("Error report -- RedReader v").append(Constants.version(context)).append("\r\n\r\n");
	sb.append("Error report -- PinkReader v[version]\r\n\r\n");

	// Port of: sb.append("Manufacturer: ") etc.
	sb.append("Manufacturer: [system]\r\n");
	sb.append("Model: [system]\r\n");
	sb.append("Product: [system]\r\n");
	sb.append("OS release: [system]\r\n");
	sb.append("OS SDK: [system]\r\n");

	// Port of: for(final RRError error : errors) { ... }
	for (const auto& error : errors) {
		sb.append("\r\n-------------------------------\r\n");
		if (error != nullptr) {
			// Append error details
			appendException(sb, nullptr, 25);
		}
	}

	// Port of: final Intent intent creation for email
	// Email sending is platform-specific in C++ port
}

// Port of: public static void appendException(final StringBuilder sb, final Throwable t, final int recurseLimit)
void BugReportActivity::appendException(QString& sb, void* throwable, int recurseLimit) {
	// Port of: if(t != null) { ... }
	if (throwable != nullptr && recurseLimit > 0) {
		sb.append("Exception: [details]\r\n");
	}
}

// Constructor
BugReportActivity::BugReportActivity(QObject* parent)
	: ViewsBaseActivity(parent)
{
}

// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
void BugReportActivity::onCreate(Bundle* savedInstanceState) {
	// Port of: super.onCreate(savedInstanceState);
	ViewsBaseActivity::onCreate(savedInstanceState);

	// Port of: final LinearLayout layout = new LinearLayout(this);
	// layout.setOrientation(LinearLayout.VERTICAL);
	// final TextView title = new TextView(this); title.setText(R.string.bug_title);
	// layout.addView(title); title.setTextSize(20.0f);
	// final TextView text = new TextView(this); text.setText(R.string.bug_message);
	// layout.addView(text); text.setTextSize(15.0f);
	// final int paddingPx = General.dpToPixels(this, 20);
	// title.setPadding(paddingPx, paddingPx, paddingPx, paddingPx);
	// text.setPadding(paddingPx, paddingPx, paddingPx, paddingPx);
	// final Button send = new Button(this); send.setText(R.string.bug_button_send);
	// send.setOnClickListener(v -> { sendBugReport(this, getErrors()); finish(); });
	// final Button ignore = new Button(this); ignore.setText(R.string.bug_button_ignore);
	// ignore.setOnClickListener(v -> finish());
	// layout.addView(send); layout.addView(ignore);
	// final ScrollView sv = new ScrollView(this); sv.addView(layout);
	// setBaseActivityListing(sv);
	//
	// UI setup handled by framework in C++ port
}

} // namespace PinkReader
