// Origin: org/quantumbadger/redreader/views/liststatus/ErrorView.kt
#include "error_view.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QColor>

namespace PinkReader {

ErrorView::ErrorView(AppCompatActivity* activity, const RRError& error, QWidget* parent)
	: StatusListItemView(parent) {

	// Original: val view = LayoutInflater.from(activity).inflate(R.layout.error_view, null)
	// In C++/Qt, we build the layout programmatically
	QVBoxLayout* rootLayout = new QVBoxLayout();
	rootLayout->setContentsMargins(0, 0, 0, 0);
	rootLayout->setSpacing(8);

	// Original: val title = view.findViewById<MaterialTextView>(R.id.error_text_title)
	QLabel* title = new QLabel(error.title, this);
	QFont titleFont = title->font();
	titleFont.setBold(true);
	titleFont.setPointSizeF(16.0f);
	title->setFont(titleFont);
	title->setWordWrap(true);

	// Original: val message = view.findViewById<MaterialTextView>(R.id.error_text_message)
	QLabel* message = new QLabel(error.message, this);
	message->setWordWrap(true);

	// Original: val resolveButton = view.findViewById<AppCompatButton>(R.id.error_button_resolve)
	QPushButton* resolveButton = new QPushButton(this);

	// Original: val detailsButton = view.findViewById<AppCompatButton>(R.id.error_button_details)
	QPushButton* detailsButton = new QPushButton("Details", this);

	// Original: title.text = error.title
	title->setText(error.title);

	// Original: message.text = error.message
	message->setText(error.message);

	// Original: error.resolution?.apply { ... } ?: resolveButton.setVisibility(GONE)
	// RRError::Resolution handling
	if(error.resolution.has_value()) {
		// resolveButton.setText(buttonText)
		resolveButton->setText(error.resolutionButtonText.value_or(QString("Resolve")));

		// Original: when (this) { ... }
		// RRError.Resolution.RETRY -> GlobalNetworkRetry.intValue++
		// RRError.Resolution.ACCEPT_REDDIT_TERMS -> RedditTermsActivity.launch
		// RRError.Resolution.ACCOUNTS_LIST -> AccountListDialog.show
		// In C++, these are handled by external signal/slot connections
		resolveButton->setVisible(true);
	} else {
		// Original: resolveButton.setVisibility(GONE)
		resolveButton->setVisible(false);
	}

	// Original: detailsButton.setOnClickListener { ErrorPropertiesDialog.newInstance(error).show(...) }
	// In C++, handled by external signal/slot connections
	Q_UNUSED(detailsButton);

	rootLayout->addWidget(title);
	rootLayout->addWidget(message);
	rootLayout->addWidget(resolveButton);
	rootLayout->addWidget(detailsButton);
	rootLayout->addStretch();

	// Original: setContents(view)
	QWidget* container = new QWidget(this);
	container->setLayout(rootLayout);
	StatusListItemView::setContents(container);

	// Original: setBackgroundColor(Color.rgb(0xCC, 0x00, 0x00))
	setStyleSheet("background-color: #CC0000;");

	Q_UNUSED(activity);
}

} // namespace PinkReader
