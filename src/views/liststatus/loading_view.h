// Origin: org/quantumbadger/redreader/views/liststatus/LoadingView.java
#pragma once

#include <QWidget>
#include <QString>
#include <QTimer>

#include "status_list_item_view.h"

namespace PinkReader {

// Forward declarations
class QLabel;
class QVBoxLayout;

/**
 * Port of LoadingView.
 * Original: public final class LoadingView extends StatusListItemView
 * Every field, method, constant, logic line preserved.
 */
class LoadingView : public StatusListItemView {
	Q_OBJECT

private:
	QLabel* m_textView;

	static constexpr int LOADING_INDETERMINATE = -1;
	static constexpr int LOADING_DONE = -2;

	QTimer* m_loadingTimer;

	void sendMessage(const QString& text, int what);

private slots:
	void handleMessage();

public:
	explicit LoadingView(QWidget* parent = nullptr);
	LoadingView(QWidget* parent, int initialTextRes, bool progressBarEnabled, bool indeterminate);
	LoadingView(QWidget* parent, const QString& initialText, bool progressBarEnabled, bool indeterminate);

	void setIndeterminate(int textRes);
	void setProgress(int textRes, float fraction);
	void setDone(int textRes);
};

} // namespace PinkReader
