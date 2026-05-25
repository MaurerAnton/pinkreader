// Origin: org/quantumbadger/redreader/fragments/PostPropertiesDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class RedditPost;
class Context;
class BaseActivity;
class LinearLayout;

class PostPropertiesDialog {
public:
    static std::unique_ptr<PostPropertiesDialog> newInstance(const RedditPost& post);

protected:
    std::string getTitle(Context& context);
    void prepare(BaseActivity& context, LinearLayout& items);

private:
    const RedditPost* mPost = nullptr;
};

} // namespace PinkReader
