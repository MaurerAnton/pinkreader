// Origin: RedReader/src/main/java/org/quantumbadger/redreader/viewholders/VH3TextIcon.java
#include "vh_3_text_icon.h"
#include "../common/uri_string.h"

namespace PinkReader {

// Port of: public VH3TextIcon(final View itemView)
VH3TextIcon::VH3TextIcon()
	: bindingId(0) {
	// Port of: super(itemView);
	// Port of: textHoldingLayout = itemView.findViewById(R.id.recycler_text_layout);
	// Port of: text = itemView.findViewById(R.id.recycler_item_text);
	// Port of: text2 = itemView.findViewById(R.id.recycler_item_2_text);
	// Port of: text3 = itemView.findViewById(R.id.recycler_item_3_text);
	// Port of: icon = itemView.findViewById(R.id.recycler_item_icon);
	// Port of: extra = itemView.findViewById(R.id.recycler_item_extra);
}

// Port of: public void removeExtras()
//   extra.removeAllViews();
void VH3TextIcon::removeExtras() {
	// Port of: extra.removeAllViews();
	m_extraLinks.clear();
}

// Port of: public void addLinkButton(final BaseActivity activity, final UriString url)
void VH3TextIcon::addLinkButton(const UriString& url) {
	// Port of: final BodyElementLinkButton linkButton
	//   = new BodyElementLinkButton(new HtmlRawElement.LinkButtonDetails(url.value, url));
	// Port of: final View linkButtonView = linkButton.generateView(activity,
	//   new RRThemeAttributes(activity.getApplicationContext()).rrCommentBodyCol,
	//   13.0f, true);
	// Port of: extra.addView(linkButtonView);
	m_extraLinks.push_back(url.value().toStdString());
}

} // namespace PinkReader
