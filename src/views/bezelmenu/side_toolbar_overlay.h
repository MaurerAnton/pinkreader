// Origin: org.quantumbadger.redreader.views.bezelmenu.SideToolbarOverlay.java
// Ported from RedReader (GPLv3)
#pragma once

namespace PinkReader {

class SideToolbarOverlay {
public:
    // --- SideToolbarPosition enum ---
    enum class SideToolbarPosition {
        LEFT,
        RIGHT
    };

    // --- Constructor ---
    explicit SideToolbarOverlay(void* context);

    // --- Public API ---
    void setContents(void* contents);         // View*
    void show(SideToolbarPosition pos);
    void hide();
    bool isShown() const;

private:
    void* m_contents = nullptr;               // View*
    SideToolbarPosition* m_shownPosition = nullptr; // null means hidden
};

} // namespace PinkReader
