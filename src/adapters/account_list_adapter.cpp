// Origin: org/quantumbadger/redreader/adapters/AccountListAdapter.kt
#include "adapters/account_list_adapter.h"

namespace PinkReader {

AccountListAdapter::AccountListAdapter(AppCompatActivity& context, Fragment& fragment)
    : mContext(context)
    , mFragment(fragment) {
    // private val accounts = RedditAccountManager.getInstance(context).accounts
    // mAccounts = RedditAccountManager::getInstance(context)->getAccounts();

    // init {
    //     @SuppressLint("UseKtx")
    //     val attr = context.obtainStyledAttributes(intArrayOf(R.attr.rrIconAdd, R.attr.rrIconPerson))
    //     rrIconAdd = AppCompatResources.getDrawable(context, attr.getResourceId(0, 0))
    //     //noinspection ResourceType: bug in Lint
    //     rrIconUser = AppCompatResources.getDrawable(context, attr.getResourceId(1, 0))
    //     attr.recycle()
    // }
}

void* AccountListAdapter::onCreateHeaderItemViewHolder(void* /* parent */) {
    // val v = LayoutInflater.from(parent.context)
    //     .inflate(R.layout.list_item_1_text, parent, false)
    // return VH1Text(v)
    return nullptr;
}

void* AccountListAdapter::onCreateContentItemViewHolder(void* /* parent */) {
    // val v = LayoutInflater.from(parent.context)
    //     .inflate(R.layout.list_item_1_text, parent, false)
    // return VH1Text(v)
    return nullptr;
}

void AccountListAdapter::onBindHeaderItemViewHolder(void* /* holder */, int /* position */) {
    // val vh = holder as VH1Text
    // vh.text.text = context.getString(R.string.accounts_add)
    // vh.text.setCompoundDrawablesWithIntrinsicBounds(rrIconAdd, null, null, null)
    // holder.itemView.setOnClickListener { showLoginWarningDialog() }
}

void AccountListAdapter::onBindContentItemViewHolder(void* /* holder */, int /* position */) {
    // val accountManager = RedditAccountManager.getInstance(context)
    // val vh = holder as VH1Text
    // val account = accounts[position]
    // val username = BetterSSB()
    // if (account.isAnonymous) {
    //     username.append(context.getString(R.string.accounts_anon), 0)
    // } else {
    //     username.append(account.username, 0)
    // }
    // if (account == accountManager.defaultAccount) {
    //     val attr = context.obtainStyledAttributes(intArrayOf(R.attr.rrListSubtitleCol))
    //     val col = attr.getColor(0, 0)
    //     attr.recycle()
    //     username.append("  (" + context.getString(R.string.accounts_active) + ")",
    //         BetterSSB.FOREGROUND_COLOR or BetterSSB.SIZE, col, 0, 0.8f)
    // }
    // if (needsRelogin(account)) {
    //     username.append("  (" + context.getString(R.string.reddit_relogin_error_title) + ")",
    //         BetterSSB.FOREGROUND_COLOR or BetterSSB.SIZE, Color.rgb(200, 50, 50), 0, 0.8f)
    // }
    // vh.text.text = username.get()
    // vh.text.setCompoundDrawablesWithIntrinsicBounds(rrIconUser, null, null, null)
    // vh.itemView.setOnClickListener {
    //     val actions = ArrayList<AccountAction>()
    //     if (account != accountManager.defaultAccount) {
    //         actions.add(AccountAction(R.string.accounts_setactive) {
    //             accountManager.defaultAccount = account
    //         })
    //     }
    //     if (account.isNotAnonymous) {
    //         actions.add(AccountAction(R.string.accounts_delete) {
    //             MaterialAlertDialogBuilder(context)
    //                 .setTitle(R.string.accounts_delete)
    //                 .setMessage(R.string.accounts_delete_sure)
    //                 .setPositiveButton(R.string.accounts_delete) { _, _ ->
    //                     accountManager.deleteAccount(account)
    //                 }
    //                 .setNegativeButton(R.string.dialog_cancel, null)
    //                 .show()
    //         })
    //     }
    //     if (needsRelogin(account)) {
    //         actions.add(AccountAction(R.string.accounts_reauth) {
    //             showLoginWarningDialog()
    //         })
    //     }
    //     val items = actions.map { context.getString(it.message) }.toTypedArray()
    //     if (items.isNotEmpty()) {
    //         val builder = MaterialAlertDialogBuilder(context)
    //         builder.setItems(items) { _, which -> actions[which].action() }
    //         builder.setNeutralButton(R.string.dialog_cancel, null)
    //         val alert = builder.create()
    //         alert.setTitle(if (account.isAnonymous) context.getString(R.string.accounts_anon) else account.username)
    //         alert.setCanceledOnTouchOutside(true)
    //         alert.show()
    //     }
    // }
}

int AccountListAdapter::getContentItemCount() const {
    // return accounts.size
    return static_cast<int>(mAccounts.size());
}

void AccountListAdapter::showLoginWarningDialog() {
    // val dialog = MaterialAlertDialogBuilder(context)
    //     .setView(R.layout.dialog_prelogin_prompt)
    //     .setCancelable(true)
    //     .setPositiveButton(R.string.dialog_continue) { _: DialogInterface?, _: Int -> launchLogin() }
    //     .setNegativeButton(R.string.dialog_close) { _: DialogInterface?, _: Int -> }
    //     .show()
    // dialog.findViewById<MaterialTextView>(R.id.login_preprompt_help_link)!!.apply {
    //     paintFlags = paintFlags or Paint.UNDERLINE_TEXT_FLAG
    //     setOnClickListener {
    //         LinkHandler.onLinkClicked(this@AccountListAdapter.context,
    //             UriString("https://redreader.org/loginhelp/"))
    //     }
    // }
}

void AccountListAdapter::launchLogin() {
    // val loginIntent = Intent(context, OAuthLoginActivity::class.java)
    // fragment.startActivityForResult(loginIntent, 123)
}

} // namespace PinkReader
