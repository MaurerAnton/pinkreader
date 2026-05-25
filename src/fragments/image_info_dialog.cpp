// Origin: org/quantumbadger/redreader/fragments/ImageInfoDialog.java
#include "fragments/image_info_dialog.h"

namespace PinkReader {

std::unique_ptr<ImageInfoDialog> ImageInfoDialog::newInstance(const ImageInfo& info) {
    auto pp = std::make_unique<ImageInfoDialog>();

    // final Bundle args = new Bundle();
    // args.putParcelable("info", info);
    // pp.setArguments(args);
    pp->mInfo = &info;

    return pp;
}

std::string ImageInfoDialog::getTitle(Context& context) {
    // return context.getString(R.string.props_image_title);
    return "";
}

void ImageInfoDialog::prepare(BaseActivity& context, LinearLayout& items) {
    // final ImageInfo info = Objects.requireNonNull(BundleCompat.getParcelable(
    //     requireArguments(), "info", ImageInfo.class));
    const auto& info = *mInfo;

    bool first = true;

    // if(info.title != null && !info.title.trim().isEmpty()) {
    //     items.addView(propView(context, R.string.props_title, info.title.trim(), first));
    //     first = false;
    // }
    //
    // if(info.caption != null && !info.caption.trim().isEmpty()) {
    //     items.addView(propView(context, R.string.props_caption, info.caption.trim(), first));
    //     first = false;
    // }
    //
    // items.addView(propView(context, R.string.props_url, info.original.url.value, first));
    //
    // if (info.original.size != null) {
    //     items.addView(propView(context, R.string.props_resolution,
    //         info.original.size.getWidth() + " x " + info.original.size.getHeight(), false));
    // }
}

} // namespace PinkReader
