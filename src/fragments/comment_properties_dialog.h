// Origin: org/quantumbadger/redreader/fragments/CommentPropertiesDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class RedditComment;
class Context;
class BaseActivity;
class LinearLayout;

class CommentPropertiesDialog {
public:
    static std::unique_ptr<CommentPropertiesDialog> newInstance(const RedditComment& comment);

protected:
    std::string getTitle(Context& context);
    void prepare(BaseActivity& context, LinearLayout& items);

private:
    // comment stored in Bundle args - in C++ we store directly
    const RedditComment* mComment = nullptr;
};

} // namespace PinkReader
