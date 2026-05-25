// Origin: org/quantumbadger/redreader/fragments/ErrorPropertiesDialog.java
#include "fragments/error_properties_dialog.h"
#include <sstream>
#include <string>
#include <vector>

namespace PinkReader {

ErrorPropertiesDialog::ErrorPropertiesDialog(const RRError& error)
    : mError(error) {
}

std::unique_ptr<ErrorPropertiesDialog> ErrorPropertiesDialog::newInstance(const RRError& error) {
    auto dialog = std::make_unique<ErrorPropertiesDialog>(error);

    // final Bundle args = new Bundle();
    // args.putString("title", error.title);
    // args.putString("message", error.message);
    //
    // if(error.t != null) {
    //     final StringBuilder sb = new StringBuilder(1024);
    //     BugReportActivity.appendException(sb, error.t, 10);
    //     args.putString("t", sb.toString());
    // }
    //
    // if(error.httpStatus != null) {
    //     args.putString("httpStatus", error.httpStatus.toString());
    // }
    //
    // if(error.url != null) {
    //     args.putString("url", error.url.value);
    // }
    //
    // if(error.responseString != null) {
    //     args.putString("response", error.responseString);
    // }
    //
    // dialog.setArguments(args);

    return dialog;
}

void ErrorPropertiesDialog::interceptBuilder(MaterialAlertDialogBuilder& builder) {
    // if(!(mError.t instanceof UnknownHostException)
    //     && !(mError.t instanceof SocketTimeoutException)
    //     && mError.reportable) {
    //     builder.setPositiveButton(
    //         R.string.button_error_send_report,
    //         (dialog, which) -> BugReportActivity.sendBugReport(mContext, mError));
    // }
}

std::string ErrorPropertiesDialog::getTitle(Context& context) {
    // return context.getString(R.string.props_error_title);
    return "";
}

void ErrorPropertiesDialog::prepare(BaseActivity& context, LinearLayout& items) {
    mContext = &context;

    // items.addView(propView(context, R.string.props_title, getArguments().getString("title"), true));
    // items.addView(propView(context, "Message", getArguments().getString("message"), false));
    // if(getArguments().containsKey("httpStatus")) {
    //     items.addView(propView(context, "HTTP status", getArguments().getString("httpStatus"), false));
    // }
    // if(getArguments().containsKey("url")) {
    //     items.addView(propView(context, "URL", getArguments().getString("url"), false));
    // }
    // if(getArguments().containsKey("t")) {
    //     items.addView(propView(context, "Exception", getArguments().getString("t"), false));
    // }
    // if(getArguments().containsKey("response")) {
    //     items.addView(propView(context, "Response", getArguments().getString("response"), false));
    // }
}

} // namespace PinkReader
