// Origin: org/quantumbadger/redreader/fragments/UserPropertiesDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class RedditUser;
class Context;
class BaseActivity;
class LinearLayout;

class UserPropertiesDialog {
public:
    static std::unique_ptr<UserPropertiesDialog> newInstance(const RedditUser& user);

protected:
    std::string getTitle(Context& context);
    void prepare(BaseActivity& context, LinearLayout& items);

private:
    const RedditUser* mUser = nullptr;
};

} // namespace PinkReader
