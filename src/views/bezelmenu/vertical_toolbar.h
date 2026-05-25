// Origin: org.quantumbadger.redreader.views.bezelmenu.VerticalToolbar.java
// Ported from RedReader (GPLv3)
#pragma once

namespace PinkReader {

class VerticalToolbar {
public:
    // --- Constructor ---
    explicit VerticalToolbar(void* context);

    // --- Public API ---
    void addItem(void* v); // View*

    // --- Original fields exposed for access ---
    void* getButtons() { return m_buttons; } // LinearLayout*

private:
    void* m_buttons = nullptr; // LinearLayout*
};

} // namespace PinkReader
