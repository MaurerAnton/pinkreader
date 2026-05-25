// Origin: org/quantumbadger/redreader/fragments/UserPropertiesDialog.java
#include "fragments/user_properties_dialog.h"

namespace PinkReader {

std::unique_ptr<UserPropertiesDialog> UserPropertiesDialog::newInstance(const RedditUser& user) {
    auto pp = std::make_unique<UserPropertiesDialog>();

    // final Bundle args = new Bundle();
    // args.putParcelable("user", user);
    // pp.setArguments(args);
    pp->mUser = &user;

    return pp;
}

std::string UserPropertiesDialog::getTitle(Context& /* context */) {
    // return Objects.requireNonNull(BundleCompat.getParcelable(
    //     requireArguments(), "user", RedditUser.class)).name;
    return mUser ? mUser->name : "";
}

void UserPropertiesDialog::prepare(BaseActivity& context, LinearLayout& items) {
    // final RedditUser user = Objects.requireNonNull(
    //     BundleCompat.getParcelable(requireArguments(), "user", RedditUser.class));
    const auto& user = *mUser;

    // items.addView(propView(context, R.string.props_id, user.id, true));
    //
    // if (user.created_utc != null) {
    //     items.addView(propView(context, R.string.userprofile_created,
    //         TimestampUTC.fromUtcSecs(user.created_utc).format(), false));
    // }
    //
    // if (user.link_karma != null) {
    //     items.addView(propView(context, R.string.karma_link, String.valueOf(user.link_karma), false));
    // }
    //
    // if (user.comment_karma != null) {
    //     items.addView(propView(context, R.string.karma_comment, String.valueOf(user.comment_karma), false));
    // }
    //
    // if (user.is_friend != null) {
    //     items.addView(propView(context, R.string.userprofile_isfriend,
    //         user.is_friend ? R.string.general_true : R.string.general_false, false));
    // }
    //
    // if (user.is_gold != null) {
    //     items.addView(propView(context, R.string.userprofile_isgold,
    //         user.is_gold ? R.string.general_true : R.string.general_false, false));
    // }
    //
    // if (user.is_mod != null) {
    //     items.addView(propView(context, R.string.userprofile_moderator,
    //         user.is_mod ? R.string.general_true : R.string.general_false, false));
    // }
    //
    // if (user.is_employee != null) {
    //     items.addView(propView(context, R.string.userprofile_tag_admin,
    //         user.is_employee ? R.string.general_true : R.string.general_false, false));
    // }
    //
    // if (user.is_suspended != null) {
    //     items.addView(propView(context, R.string.userprofile_tag_suspended,
    //         user.is_suspended ? R.string.general_true : R.string.general_false, false));
    // }
    //
    // if (user.is_blocked != null) {
    //     items.addView(propView(context, R.string.userprofile_tag_blocked,
    //         user.is_blocked ? R.string.general_true : R.string.general_false, false));
    // }
    //
    // if (user.icon_img != null) {
    //     items.addView(propView(context, R.string.userprofile_avatar, user.icon_img, false));
    // }
}

} // namespace PinkReader
