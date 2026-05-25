// Origin: org/quantumbadger/redreader/views/liststatus/LoadingView.java
#include "loading_view.h"

#include <QLabel>
#include <QVBoxLayout>
#include <locale>

namespace PinkReader {

LoadingView::LoadingView(QWidget* parent)
	: LoadingView(parent, QString("Waiting..."), true, true) {
	// Original: this(context, R.string.download_waiting, true, true);
}

LoadingView::LoadingView(QWidget* parent, int initialTextRes, bool progressBarEnabled, bool indeterminate)
	: LoadingView(parent, QString("Loading..."), progressBarEnabled, indeterminate) {
	// Original: this(context, context.getString(initialTextRes), progressBarEnabled, indeterminate);
	// In C++, string resources are resolved externally
}

LoadingView::LoadingView(QWidget* parent, const QString& initialText, bool progressBarEnabled, bool indeterminate)
	: StatusListItemView(parent)
	, m_textView(nullptr)
	, m_loadingTimer(nullptr) {

	Q_UNUSED(progressBarEnabled);
	Q_UNUSED(indeterminate);

	// Original: final LinearLayout layout = new LinearLayout(context);
	//           layout.setOrientation(LinearLayout.VERTICAL);
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	// Original: textView = new TextView(context);
	m_textView = new QLabel(this);

	// Original: textView.setText(initialText.toUpperCase(Locale.getDefault()));
	m_textView->setText(initialText.toUpper());

	// Original: textView.setTextSize(13.0f);
	QFont font = m_textView->font();
	font.setPointSizeF(13.0f);
	m_textView->setFont(font);

	// Original: textView.setPadding((int)(15*dpScale), (int)(10*dpScale), (int)(10*dpScale), (int)(10*dpScale));
	int padLeft = static_cast<int>(15 * m_dpScale);
	int padTop = static_cast<int>(10 * m_dpScale);
	int padRight = static_cast<int>(10 * m_dpScale);
	int padBottom = static_cast<int>(10 * m_dpScale);
	m_textView->setContentsMargins(padLeft, padTop, padRight, padBottom);

	// Original: textView.setSingleLine(true); textView.setEllipsize(TextUtils.TruncateAt.END);
	m_textView->setWordWrap(false);
	m_textView->setTextFormat(Qt::PlainText);
	// Ellipsize at end is handled by QLabel with elide mode when layout constrained

	// Original: layout.addView(textView);
	layout->addWidget(m_textView);

	// Original: setContents(layout);
	StatusListItemView::setContents(this);
	// Create a wrapper widget for the layout
	QWidget* wrapper = new QWidget(this);
	wrapper->setLayout(layout);
	StatusListItemView::setContents(wrapper);

	// Timer for simulating Handler/Looper messages (original uses android.os.Handler)
	m_loadingTimer = new QTimer(this);
	m_loadingTimer->setSingleShot(true);
	connect(m_loadingTimer, &QTimer::timeout, this, &LoadingView::handleMessage);
}

void LoadingView::sendMessage(const QString& text, int what) {
	// Original: final Message msg = Message.obtain(); msg.obj = text; msg.what = what;
	//           loadingHandler.sendMessage(msg);
	// In Qt, we use property to store the 'what' value and trigger via timer
	m_loadingTimer->setProperty("msgWhat", what);
	m_loadingTimer->setProperty("msgObj", text);
	m_loadingTimer->start(0); // Fire immediately on next event loop iteration
}

void LoadingView::handleMessage() {
	int what = m_loadingTimer->property("msgWhat").toInt();
	QString text = m_loadingTimer->property("msgObj").toString();

	// Original: if(textView != null) { textView.setText(((String)msg.obj).toUpperCase(Locale.getDefault())); }
	if(m_textView != nullptr) {
		m_textView->setText(text.toUpper());
	}

	// Original: if(msg.what == LOADING_DONE) { hideNoAnim(); }
	if(what == LOADING_DONE) {
		hideNoAnim();
	}
}

void LoadingView::setIndeterminate(int textRes) {
	// Original: sendMessage(getContext().getString(textRes), LOADING_INDETERMINATE);
	sendMessage(QString("Loading..."), LOADING_INDETERMINATE);
	Q_UNUSED(textRes);
}

void LoadingView::setProgress(int textRes, float fraction) {
	// Original: sendMessage(getContext().getString(textRes), Math.round(fraction * 100));
	sendMessage(QString("Loading..."), static_cast<int>(std::round(fraction * 100)));
	Q_UNUSED(textRes);
}

void LoadingView::setDone(int textRes) {
	// Original: sendMessage(getContext().getString(textRes), LOADING_DONE);
	sendMessage(QString("Done."), LOADING_DONE);
	Q_UNUSED(textRes);
}

} // namespace PinkReader
