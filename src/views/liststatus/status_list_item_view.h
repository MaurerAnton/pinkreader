// Origin: org/quantumbadger/redreader/views/liststatus/StatusListItemView.java
// Original extends FrameLayout (Android). In C++, ported as standalone widget class.
#pragma once

#include <QWidget>
#include <QString>

namespace PinkReader {

// Forward declarations
class QVBoxLayout;

/**
 * Port of StatusListItemView.
 * Original: public class StatusListItemView extends FrameLayout
 * Every field, method, logic line preserved.
 */
class StatusListItemView : public QWidget {
	Q_OBJECT

protected:
	float m_dpScale;

private:
	QWidget* m_contents;

public:
	explicit StatusListItemView(QWidget* parent = nullptr);
	~StatusListItemView() override;

	void setContents(QWidget* contents);

	void hideNoAnim();
};

} // namespace PinkReader
