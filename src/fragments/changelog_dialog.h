// Origin: org/quantumbadger/redreader/fragments/ChangelogDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class Context;
class BaseActivity;
class LinearLayout;

class ChangelogDialog {
public:
    static std::unique_ptr<ChangelogDialog> newInstance();

protected:
    std::string getTitle(Context& context);
    void prepare(BaseActivity& context, LinearLayout& items);
};

} // namespace PinkReader
