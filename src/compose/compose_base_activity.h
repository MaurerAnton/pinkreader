// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/activity/ComposeBaseActivity.kt
#pragma once

#include <functional>
#include <string>

namespace PinkReader {

// Forward declarations for Android/Compose dependencies
class Bundle;
class ComposeView;

// Ported from: open class ComposeBaseActivity: BaseActivity()
// BaseActivity corresponds to org.quantumbadger.redreader.activities.BaseActivity
class ComposeBaseActivity /* : public BaseActivity */ {
public:
	// override fun onCreate(savedInstanceState: Bundle?)
	virtual void onCreate(Bundle* savedInstanceState);

protected:
	// protected fun setContentCompose(content: @Composable () -> Unit)
	// @Composable content lambda -> std::function in C++
	void setContentCompose(std::function<void()> content);

private:
	ComposeView* composeView_ = nullptr;
};

} // namespace PinkReader
