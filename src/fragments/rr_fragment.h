// Origin: org/quantumbadger/redreader/fragments/RRFragment.java
#pragma once

#include <memory>
#include <functional>

namespace PinkReader {

class AppCompatActivity;
class Bundle;
class Menu;
class MenuItem;
class View;

class RRFragment {
public:
    RRFragment(AppCompatActivity& parent, const Bundle* savedInstanceState);

    virtual ~RRFragment() = default;

    AppCompatActivity& getContext();
    AppCompatActivity& getActivity();
    std::string getString(int resource);

    void startActivity(class Intent& intent);
    void startActivityForResult(Intent& intent, int requestCode);

    virtual void onCreateOptionsMenu(Menu& menu);
    virtual bool onOptionsItemSelected(MenuItem& item);

    virtual std::unique_ptr<View> getListingView() = 0;

    virtual std::unique_ptr<View> getOverlayView();

    std::unique_ptr<View> createCombinedListingAndOverlayView();

    void setBaseActivityContent(class ViewsBaseActivity& baseActivity);

    virtual std::unique_ptr<Bundle> onSaveInstanceState() = 0;

private:
    AppCompatActivity& mParent;
};

} // namespace PinkReader
