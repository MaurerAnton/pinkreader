// Origin: org/quantumbadger/redreader/views/liststatus/StatusListItemView.java
#include "status_list_item_view.h"

#include <QVBoxLayout>
#include <QScreen>

namespace PinkReader {

StatusListItemView::StatusListItemView(QWidget* parent)
	: QWidget(parent)
	, m_dpScale(1.0f)
	, m_contents(nullptr) {
	// Original: dpScale = context.getResources().getDisplayMetrics().density;
	// In Qt, we approximate using logical DPI / 160
	if(QScreen* screen = QGuiApplication::primaryScreen()) {
		m_dpScale = screen->logicalDotsPerInch() / 160.0f;
	}
	if(m_dpScale <= 0.0f) {
		m_dpScale = 1.0f;
	}
}

StatusListItemView::~StatusListItemView() = default;

void StatusListItemView::setContents(QWidget* contents) {
	if(m_contents != nullptr) {
		m_contents->setParent(nullptr); // Original: removeView(this.contents)
	}
	m_contents = contents;
	contents->setParent(this);
	// Original: General.setLayoutMatchWidthWrapHeight(contents);
	contents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void StatusListItemView::hideNoAnim() {
	// Original: setVisibility(GONE); removeAllViews(); contents = null; requestLayout();
	setVisible(false);
	// Remove all child widgets
	QLayout* layout = this->layout();
	if(layout) {
		QLayoutItem* item;
		while((item = layout->takeAt(0)) != nullptr) {
			if(item->widget()) {
				item->widget()->setParent(nullptr);
			}
			delete item;
		}
	}
	m_contents = nullptr;
	updateGeometry();
}

} // namespace PinkReader
