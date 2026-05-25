// Origin: org/quantumbadger/redreader/fragments/RRFragment.java
#include "fragments/rr_fragment.h"

namespace PinkReader {

RRFragment::RRFragment(AppCompatActivity& parent, const Bundle* /* savedInstanceState */)
    : mParent(parent) {
}

AppCompatActivity& RRFragment::getContext() {
    return mParent;
}

AppCompatActivity& RRFragment::getActivity() {
    return mParent;
}

std::string RRFragment::getString(int resource) {
    // mParent.getApplicationContext().getString(resource)
    return ""; // Placeholder: resource lookup
}

void RRFragment::startActivity(Intent& intent) {
    mParent.startActivity(intent);
}

void RRFragment::startActivityForResult(Intent& intent, int requestCode) {
    mParent.startActivityForResult(intent, requestCode);
}

void RRFragment::onCreateOptionsMenu(Menu& /* menu */) {
    // Default empty
}

bool RRFragment::onOptionsItemSelected(MenuItem& /* item */) {
    return false;
}

std::unique_ptr<View> RRFragment::getOverlayView() {
    // Null by default
    return nullptr;
}

std::unique_ptr<View> RRFragment::createCombinedListingAndOverlayView() {
    // final FrameLayout outer = new FrameLayout(mParent);
    auto outer = std::make_unique<View>(/* FrameLayout equivalent */);

    {
        auto view = getListingView();
        // outer.addView(view);
        // General.setLayoutMatchParent(view);
        outer->addChild(std::move(view));
    }

    {
        auto overlayView = getOverlayView();
        if (overlayView) {
            // outer.addView(overlayView);
            // General.setLayoutMatchParent(overlayView);
            outer->addChild(std::move(overlayView));
        }
    }

    return outer;
}

void RRFragment::setBaseActivityContent(ViewsBaseActivity& baseActivity) {
    {
        auto view = getListingView();
        baseActivity.setBaseActivityListing(view.get());
        // General.setLayoutMatchParent(view);
        view.release(); // Ownership transferred
    }

    {
        auto overlayView = getOverlayView();
        if (overlayView) {
            baseActivity.setBaseActivityOverlay(overlayView.get());
            // General.setLayoutMatchParent(overlayView);
            overlayView.release(); // Ownership transferred
        }
    }
}

} // namespace PinkReader
