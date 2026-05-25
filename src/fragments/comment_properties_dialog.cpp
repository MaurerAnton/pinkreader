// Origin: org/quantumbadger/redreader/fragments/CommentPropertiesDialog.java
#include "fragments/comment_properties_dialog.h"

namespace PinkReader {

std::unique_ptr<CommentPropertiesDialog> CommentPropertiesDialog::newInstance(
        const RedditComment& comment) {
    auto pp = std::make_unique<CommentPropertiesDialog>();

    // final Bundle args = new Bundle();
    // args.putParcelable("comment", comment);
    // pp.setArguments(args);
    pp->mComment = &comment;

    return pp;
}

std::string CommentPropertiesDialog::getTitle(Context& context) {
    // return context.getString(R.string.props_comment_title);
    return ""; // Placeholder
}

void CommentPropertiesDialog::prepare(BaseActivity& context, LinearLayout& items) {
    // final RedditComment comment = Objects.requireNonNull(
    //     BundleCompat.getParcelable(requireArguments(), "comment", RedditComment.class));
    const auto& comment = *mComment;

    // items.addView(propView(context, "ID", comment.getName().getValue(), true));
    // items.addView(propView(context, R.string.props_author, comment.getAuthor().getDecoded(), false));
    // if(comment.getAuthor_flair_text() != null && !comment.getAuthor_flair_text().getDecoded().isEmpty()) {
    //     items.addView(propView(context, R.string.props_author_flair, comment.getAuthor_flair_text().getDecoded(), false));
    // }
    // items.addView(propView(context, R.string.props_created, comment.getCreated_utc().getValue().format(), false));
    // if(comment.getEdited() instanceof RedditFieldEdited.Timestamp) {
    //     items.addView(propView(context, R.string.props_edited, ((RedditFieldEdited.Timestamp)comment.getEdited()).getValue().getValue().format(), false));
    // } else {
    //     items.addView(propView(context, R.string.props_edited, R.string.props_never, false));
    // }
    // items.addView(propView(context, R.string.props_score, String.valueOf(comment.getUps() - comment.getDowns()), false));
    // items.addView(propView(context, R.string.props_subreddit, comment.getSubreddit().getDecoded(), false));
    // if(comment.getBody() != null && !comment.getBody().getDecoded().isEmpty()) {
    //     items.addView(propView(context, R.string.props_body_markdown, comment.getBody().getDecoded(), false));
    //     if(comment.getBody_html() != null) {
    //         items.addView(propView(context, R.string.props_body_html, comment.getBody_html().getDecoded(), false));
    //     }
    // }
}

} // namespace PinkReader
