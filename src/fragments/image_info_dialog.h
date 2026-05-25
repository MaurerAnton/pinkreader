// Origin: org/quantumbadger/redreader/fragments/ImageInfoDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class ImageInfo;
class Context;
class BaseActivity;
class LinearLayout;

class ImageInfoDialog {
public:
    static std::unique_ptr<ImageInfoDialog> newInstance(const ImageInfo& info);

protected:
    std::string getTitle(Context& context);
    void prepare(BaseActivity& context, LinearLayout& items);

private:
    const ImageInfo* mInfo = nullptr;
};

} // namespace PinkReader
