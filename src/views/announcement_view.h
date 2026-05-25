/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: announcement_view.h - Port of AnnouncementView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/AnnouncementView.java
 *
 * Original: class AnnouncementView extends FrameLayout
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <string>

namespace PinkReader {

// Forward declarations for ported types
class Announcement;
class AppCompatActivity;

// ============================================================================
// AnnouncementView — port of Java class (Java lines 31-67)
//
// Port of: org.quantumbadger.redreader.views.AnnouncementView
// Orig extends: FrameLayout
//
// Displays announcements with title, optional message, View and Dismiss buttons.
// Every field, method, constant ported exactly.
// ============================================================================

class AnnouncementView {
public:
    // ========================================================================
    // Constructor (Java lines 33-66)
    //
    // Port of: public AnnouncementView(@NonNull final AppCompatActivity activity,
    //     @NonNull final Announcement announcement)
    // ========================================================================

    AnnouncementView(AppCompatActivity* activity,
                     const Announcement* announcement);

    virtual ~AnnouncementView();

private:
    // ========================================================================
    // Constants (port of: View.GONE = 0x00000008)
    // ========================================================================

    static constexpr int GONE = 8;
};

} // namespace PinkReader
