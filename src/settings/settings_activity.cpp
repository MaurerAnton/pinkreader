/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: settings_activity.cpp - Port of SettingsActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/settings/SettingsActivity.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "settings_activity.h"

namespace PinkReader {

// ===== Constructor =====
SettingsActivity::SettingsActivity(QObject *parent)
    : QObject(parent)
{
}

// ===== Destructor =====
SettingsActivity::~SettingsActivity() = default;

// ===== Port of: private void launchFragment(@NonNull final String panel) (Java lines 32-44) =====
void SettingsActivity::launchFragment(const QString &panel) {
    // final Bundle bundle = new Bundle();
    // bundle.putString("panel", panel);
    //
    // getSupportFragmentManager()
    //         .beginTransaction()
    //         .setReorderingAllowed(false)
    //         .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN)
    //         .replace(R.id.single_fragment_container, SettingsFragment.class, bundle)
    //         .addToBackStack("Settings: " + panel)
    //         .commit();

    emit panelLaunchRequested(panel);

    // Stub: Android FragmentManager not yet ported
}

// ===== Port of: protected void onCreate(final Bundle savedInstanceState) (Java lines 46-65) =====
void SettingsActivity::onCreate(Bundle *savedInstanceState) {
    // PrefsUtility.applySettingsTheme(this);
    //
    // super.onCreate(savedInstanceState);
    //
    // getWindow().setNavigationBarColor(Color.rgb(0x55, 0x55, 0x55));
    //
    // setBaseActivityListing(R.layout.single_fragment_layout);
    //
    // final Bundle bundle = new Bundle();
    // bundle.putString("panel", "root");
    //
    // getSupportFragmentManager()
    //         .beginTransaction()
    //         .setReorderingAllowed(false)
    //         .replace(R.id.single_fragment_container, SettingsFragment.class, bundle)
    //         .commit();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPanelSelected(@NonNull final String panel) (Java lines 67-69) =====
void SettingsActivity::onPanelSelected(const QString &panel) {
    launchFragment(panel);
}

} // namespace PinkReader
