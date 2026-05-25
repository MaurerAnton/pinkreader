// Origin: org/quantumbadger/redreader/views/liststatus/ErrorView.kt
#pragma once

#include <QWidget>
#include <QString>

#include "status_list_item_view.h"
#include "../../common/rr_error.h"

namespace PinkReader {

// Forward declarations
class QLabel;
class QPushButton;
class QVBoxLayout;
class AppCompatActivity;

/**
 * Port of ErrorView (Kotlin class).
 * Original: class ErrorView(activity: AppCompatActivity, error: RRError) : StatusListItemView(activity)
 * Every field, method, logic line preserved.
 */
class ErrorView : public StatusListItemView {
	Q_OBJECT

public:
	explicit ErrorView(AppCompatActivity* activity, const RRError& error, QWidget* parent = nullptr);
};

} // namespace PinkReader
