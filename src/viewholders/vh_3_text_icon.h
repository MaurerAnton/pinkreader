// Origin: RedReader/src/main/java/org/quantumbadger/redreader/viewholders/VH3TextIcon.java
#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace PinkReader {

// Forward declarations
class UriString;

/**
 * @brief Port of org.quantumbadger.redreader.viewholders.VH3TextIcon
 *
 * A view holder for a three line, text and icon list item, which can have link buttons.
 * Original: extends RecyclerView.ViewHolder
 * Every field and method from the original Java file.
 */
class VH3TextIcon {
public:
	// Port of: public final LinearLayout textHoldingLayout;
	// Port of: public final TextView text;
	std::string m_text;
	// Port of: public final TextView text2;
	std::string m_text2;
	// Port of: public final TextView text3;
	std::string m_text3;
	// Port of: public final ImageView icon;
	std::string m_iconResource;
	// Port of: public final LinearLayoutCompat extra;
	// In C++, "extra" holds link button data
	std::vector<std::string> m_extraLinks;

	// Port of: public long bindingId = 0;
	int64_t bindingId;

	// Port of: public VH3TextIcon(final View itemView)
	//   super(itemView);
	//   textHoldingLayout = itemView.findViewById(R.id.recycler_text_layout);
	//   text = itemView.findViewById(R.id.recycler_item_text);
	//   text2 = itemView.findViewById(R.id.recycler_item_2_text);
	//   text3 = itemView.findViewById(R.id.recycler_item_3_text);
	//   icon = itemView.findViewById(R.id.recycler_item_icon);
	//   extra = itemView.findViewById(R.id.recycler_item_extra);
	VH3TextIcon();

	// Port of: public void removeExtras()
	//   extra.removeAllViews();
	void removeExtras();

	// Port of: public void addLinkButton(final BaseActivity activity, final UriString url)
	//   final BodyElementLinkButton linkButton
	//     = new BodyElementLinkButton(new HtmlRawElement.LinkButtonDetails(url.value, url));
	//   final View linkButtonView = linkButton.generateView(activity,
	//     new RRThemeAttributes(activity.getApplicationContext()).rrCommentBodyCol,
	//     13.0f, true);
	//   extra.addView(linkButtonView);
	void addLinkButton(const UriString& url);
};

} // namespace PinkReader
