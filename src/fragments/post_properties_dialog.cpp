// Origin: org/quantumbadger/redreader/fragments/PostPropertiesDialog.java
#include "fragments/post_properties_dialog.h"

namespace PinkReader {

std::unique_ptr<PostPropertiesDialog> PostPropertiesDialog::newInstance(const RedditPost& post) {
    auto pp = std::make_unique<PostPropertiesDialog>();

    // final Bundle args = new Bundle();
    // args.putParcelable("post", post);
    // pp.setArguments(args);
    pp->mPost = &post;

    return pp;
}

std::string PostPropertiesDialog::getTitle(Context& context) {
    // return context.getString(R.string.props_post_title);
    return "";
}

void PostPropertiesDialog::prepare(BaseActivity& context, LinearLayout& items) {
    // final RedditPost post = Objects.requireNonNull(
    //     BundleCompat.getParcelable(requireArguments(), "post", RedditPost.class));
    const auto& post = *mPost;

    // TODO nullability

    // items.addView(propView(context, R.string.props_title, post.getTitle().getDecoded().trim(), true));
    // items.addView(propView(context, R.string.props_author, post.getAuthor().getDecoded(), false));
    // items.addView(propView(context, R.string.props_url, post.getUrl().getDecoded(), false));
    // items.addView(propView(context, R.string.props_created, post.getCreated_utc().getValue().format(), false));
    //
    // if(post.getEdited() instanceof RedditFieldEdited.Timestamp) {
    //     items.addView(propView(context, R.string.props_edited,
    //         ((RedditFieldEdited.Timestamp)post.getEdited()).getValue().getValue().format(), false));
    // } else {
    //     items.addView(propView(context, R.string.props_edited, R.string.props_never, false));
    // }
    //
    // items.addView(propView(context, R.string.props_subreddit, post.getSubreddit().getDecoded(), false));
    // items.addView(propView(context, R.string.props_score, String.valueOf(post.getScore()), false));
    // items.addView(propView(context, R.string.props_num_comments, String.valueOf(post.getNum_comments()), false));
    //
    // if(post.getSelftext() != null && !post.getSelftext().getDecoded().isEmpty()) {
    //     items.addView(propView(context, R.string.props_self_markdown, post.getSelftext().getDecoded(), false));
    //     if(post.getSelftext_html() != null) {
    //         items.addView(propView(context, R.string.props_self_html, post.getSelftext_html().getDecoded(), false));
    //     }
    // }
}

} // namespace PinkReader
