// Origin: org/quantumbadger/redreader/fragments/UserProfileDialog.kt
#include "fragments/user_profile_dialog.h"

namespace PinkReader {

void UserProfileDialog::show(AppCompatActivity& activity, const std::string& username) {
    // val builder = MaterialAlertDialogBuilder(activity)
    // builder.setView(R.layout.user_profile_dialog)
    // val dialog = builder.show()
    //
    // val loadingView = dialog.findViewById<LoadingSpinnerView>(R.id.user_profile_loading)!!
    // val scrollView = dialog.findViewById<ScrollView>(R.id.user_profile_scrollview)!!
    // ... many findViewById calls ...
    //
    // val cm = CacheManager.getInstance(activity)
    // val accountManager = RedditAccountManager.getInstance(activity)
    //
    // RedditAPI.getUser(cm, username, object : UserResponseHandler(activity) {
    //     override fun onDownloadStarted() {}
    //     override fun onSuccess(user: RedditUser, timestamp: TimestampUTC) {
    //         AndroidCommon.UI_THREAD_HANDLER.post {
    //             if (!dialog.isShowing) { return@post }
    //             loadingView.visibility = View.GONE
    //             scrollView.visibility = View.VISIBLE
    //             textviewUsername.text = user.name
    //             ... set up account age, chips visibility, karma, click listeners ...
    //         }
    //     }
    //     override fun onCallbackException(t: Throwable) {
    //         BugReportActivity.handleGlobalError(context, t)
    //     }
    //     override fun onFailure(error: RRError) {
    //         AndroidCommon.UI_THREAD_HANDLER.post {
    //             if (!dialog.isShowing) { return@post }
    //             val root = dialog.findViewById<FrameLayout>(R.id.user_profile_root)!!
    //             root.removeAllViews()
    //             root.addView(ErrorView(context, error))
    //         }
    //     }
    // }, accountManager.getDefaultAccount(), DownloadStrategyAlways.INSTANCE, activity)
}

void UserProfileDialog::unblockUser(
        AppCompatActivity& activity,
        const std::string& username,
        void* /* chipBlock */,
        void* /* chipBlocked */,
        void* /* chipUnblock */) {
    // val cm = CacheManager.getInstance(activity)
    // val currentUser = RedditAccountManager.getInstance(activity).defaultAccount
    // RedditAPI.getUser(cm, currentUser.username, object : UserResponseHandler(activity) {
    //     override fun onDownloadStarted() {}
    //     override fun onSuccess(redditUser: RedditUser, timestamp: TimestampUTC) {
    //         val currentUserFullname = redditUser.fullname()
    //         unblockUserApiCall(activity, username, currentUserFullname, chipBlock, chipBlocked, chipUnblock)
    //     }
    //     override fun onCallbackException(t: Throwable) { ... }
    //     override fun onFailure(error: RRError) { ... }
    // }, currentUser, DownloadStrategyAlways.INSTANCE, activity)
}

void UserProfileDialog::unblockUserApiCall(
        AppCompatActivity& /* activity */,
        const std::string& /* usernameToUnblock */,
        const std::string& /* currentUserFullname */,
        void* /* chipBlock */,
        void* /* chipBlocked */,
        void* /* chipUnblock */) {
    // val cm = CacheManager.getInstance(activity)
    // val currentUser = RedditAccountManager.getInstance(activity).defaultAccount
    // RedditAPI.unblockUser(cm, usernameToUnblock, currentUserFullname,
    //     object : ActionResponseHandler(activity) {
    //         override fun onSuccess() { ... }
    //         override fun onFailure(error: RRError) { ... }
    //         override fun onCallbackException(t: Throwable?) { ... }
    //     }, currentUser, activity)
}

void UserProfileDialog::blockUser(
        AppCompatActivity& /* activity */,
        const std::string& /* username */,
        void* /* chipBlock */,
        void* /* chipBlocked */,
        void* /* chipUnblock */,
        AppCompatActivity& /* context */) {
    // val cm = CacheManager.getInstance(activity)
    // val currentUser = RedditAccountManager.getInstance(activity).defaultAccount
    // RedditAPI.blockUser(cm, username,
    //     object : RedditAPI.BlockUserResponseHandler {
    //         override fun onSuccess() { ... }
    //         override fun onBlockUserPermissionDenied() { ... }
    //         override fun onFailure(error: RRError) { ... }
    //     }, currentUser, activity)
}

void UserProfileDialog::launchAndCompleteLogin(
        AppCompatActivity& /* activity */,
        AppCompatActivity& /* context */) {
    // val loginIntent = Intent(context, OAuthLoginActivity::class.java)
    // (activity as BaseActivity).startActivityForResultWithCallback(loginIntent) { resultCode: Int, data: Intent? ->
    //     if (data != null) {
    //         if (resultCode == 123 && data.hasExtra("url")) {
    //             val uri = data.getStringExtra("url")!!.toUri()
    //             completeLogin(activity, uri, RunnableOnce.DO_NOTHING)
    //         }
    //     }
    // }
}

void UserProfileDialog::assignUserAvatar(
        const UriString& /* url */,
        ImageView& /* imageOutput */,
        AppCompatActivity& /* context */) {
    // CacheManager.getInstance(context).makeRequest(CacheRequest(
    //     url, RedditAccountManager.getAnon(), null,
    //     Priority(Constants.Priority.INLINE_IMAGE_PREVIEW),
    //     DownloadStrategyIfNotCached.INSTANCE,
    //     Constants.FileType.INLINE_IMAGE_PREVIEW,
    //     CacheRequest.DownloadQueueType.IMMEDIATE,
    //     context,
    //     object : CacheRequestCallbacks {
    //         override fun onDataStreamComplete(...) {
    //             try {
    //                 streamFactory.create().use { `is` ->
    //                     val data = BitmapFactory.decodeStream(`is`)
    //                         ?: throw IOException("Failed to decode bitmap")
    //                     runOnUiThread {
    //                         imageOutput.setImageBitmap(data)
    //                         imageOutput.setOnClickListener {
    //                             LinkHandler.onLinkClicked(context, url)
    //                         }
    //                     }
    //                 }
    //             } catch (t: Throwable) { onFailure(...) }
    //         }
    //         override fun onFailure(error: RRError) { Log.d(...) }
    //     }))
}

} // namespace PinkReader
