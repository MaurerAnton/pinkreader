// Origin: org/quantumbadger/redreader/fragments/MarkdownPreviewDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class Context;
class BaseActivity;
class LinearLayout;

class MarkdownPreviewDialog {
public:
    static std::unique_ptr<MarkdownPreviewDialog> newInstance(const std::string& markdown);

protected:
    std::string getTitle(Context& context);
    void prepare(BaseActivity& activity, LinearLayout& items);

private:
    std::string mMarkdown;
};

} // namespace PinkReader
