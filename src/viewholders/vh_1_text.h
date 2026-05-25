// Origin: RedReader/src/main/java/org/quantumbadger/redreader/viewholders/VH1Text.java
#pragma once

#include <string>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.viewholders.VH1Text
 *
 * A view holder for a one line, text only list item.
 * Original: extends RecyclerView.ViewHolder
 * Every field and constructor from the original Java file.
 */
class VH1Text {
public:
	// Port of: public final MaterialTextView text;
	// In C++ this is a string; the actual widget binding is platform-specific.
	std::string m_textContent;

	// Port of: public VH1Text(final View itemView)
	//   super(itemView);
	//   text = itemView.findViewById(R.id.recycler_item_text);
	explicit VH1Text();

	// Accessor matching the public field pattern
	std::string& text() { return m_textContent; }
	const std::string& text() const { return m_textContent; }

private:
	// Port of: View itemView (implicit in RecyclerView.ViewHolder)
	// Stub in C++ port
};

} // namespace PinkReader
